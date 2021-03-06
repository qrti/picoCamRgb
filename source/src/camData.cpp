// cam_wiz rgb V0.8 220330 qrt@qland.de

#include "camData.h"

uint16_t CamData::line;
PIO CamData::pio;   
uint CamData::sm;    
uint CamData::dma_chan; 
uint32_t* CamData::capture_buf;
dma_channel_config CamData::dcc;
uint16_t CamData::buf_size_words;

pio_sm_config CamData::sm_config;
uint CamData::sm_offset;


void CamData::init(uint16_t xres, uint16_t yres)
{
    gpio_init(RESET_PIN);                       // init pins
    gpio_set_dir(RESET_PIN, GPIO_OUT);          // reset output
    gpio_put(RESET_PIN, 1);                     // reset off

    gpio_init(PCLK_PIN);                        // PCLK input
    gpio_set_dir(PCLK_PIN, GPIO_IN);            //
    gpio_set_pulls(PCLK_PIN, false, false);     // no pullup, no pulldown

    gpio_init(VSYNC_PIN);                       // VSYNC input
    gpio_set_dir(VSYNC_PIN, GPIO_IN);           //
    gpio_set_pulls(VSYNC_PIN, false, false);    // no pullup, no pulldown

    gpio_init(HREF_PIN);                        // HREF input
    gpio_set_dir(HREF_PIN, GPIO_IN);            //
    gpio_set_pulls(HREF_PIN, false, false);     // no pullup, no pulldown

    buf_size_words = (xres * 2) / 4;            // 2 byte per pixel, 4 byte per word

    // start XCLK - - - - - - - - - - - 
    clock_gpio_init(XCLK_PIN, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS, SYS_CLK_DIV);    // clock_stop(clock_index::clk_gpout0);

    // reset camera - - - - - - - - - -
    sleep_ms(1);                                
    gpio_put(RESET_PIN, 0);                   
    sleep_ms(1);                              
    gpio_put(RESET_PIN, 1);                   
    sleep_ms(100);

    // init PIO - - - - - - - - - - - -
    pio = pio0;                                                     
    sm_offset = pio_add_program(pio, &parInput_program);
    sm = pio_claim_unused_sm(pio, true);
    parInput_program_init(pio, sm, sm_offset, PIN_BASE, PIN_COUNT);   

    // pio_sm_set_enabled(pio, sm, false);
    // pio_sm_clear_fifos(pio, sm);
    // pio_sm_restart(pio, sm);
    // pio_sm_set_enabled(pio, sm, true);

    // init DMA  - -  - - - - - - - - -
    capture_buf = (uint32_t*)malloc(buf_size_words * 4);
    hard_assert(capture_buf);

    bus_ctrl_hw->priority = BUSCTRL_BUS_PRIORITY_DMA_W_BITS | BUSCTRL_BUS_PRIORITY_DMA_R_BITS;

    dma_chan = dma_claim_unused_channel(true);
    dcc = dma_channel_get_default_config(dma_chan);
    channel_config_set_read_increment(&dcc, false);
    channel_config_set_write_increment(&dcc, true);
    channel_config_set_dreq(&dcc, pio_get_dreq(pio, sm, false));

    // init IRQs - - - - - - - - - - - -
    gpio_set_irq_enabled_with_callback(HREF_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback); // init HREF IRQ
    gpio_set_irq_enabled(VSYNC_PIN, GPIO_IRQ_EDGE_FALL, true);                              //      VSYNC IRQ    
    // irq_set_priority(IO_IRQ_BANK0, PICO_HIGHEST_IRQ_PRIORITY);
    // irq_set_enabled(IO_IRQ_BANK0, true);
}

void CamData::startDma()
{
    // dma_channel_abort(dma_chan);

    // pio_sm_set_enabled(pio, sm, false);     // stop pio
    // pio_sm_clear_fifos(pio, sm);            // clear fifos
    // pio_sm_restart(pio, sm);                // restart pio
    pio_sm_init(pio, sm, sm_offset, &sm_config);    

    dma_channel_configure(dma_chan, &dcc,
        capture_buf,                        // destination pointer
        &pio->rxf[sm],                      // source pointer
        buf_size_words,                     // number of transfers
        true                                // start immediately
    );

    // pio_sm_exec(pio, sm, pio_encode_wait_gpio(1, HREF_PIN));     // encoded and execute pio, wait for HREF_PIN==1
    pio_sm_set_enabled(pio, sm, true);      // start pio
}

void CamData::gpio_callback(uint gpio, uint32_t events) 
{    
    if(gpio == HREF_PIN){                   // last byte of line 
        startDma();                         // start DMA
        nLine = line;
        line++;
    }
    else if(gpio == VSYNC_PIN){             // line 0, VGA  first row in 22 lines
        line = 0;                           //         QVGA              26
    }
}

void CamData::parInput_program_init(PIO pio, uint sm, uint offset, uint pin_base, uint pin_count)
{    
    sm_config = parInput_program_get_default_config(offset);
    sm_config_set_in_pins(&sm_config, pin_base);         
    pio_sm_set_consecutive_pindirs(pio, sm, pin_base, pin_count, false);    

    for(uint8_t i=0; i<pin_count; i++){
        pio_gpio_init(pio, pin_base + i);    
        gpio_set_pulls(pin_base + i, false, false);
    }

    sm_config_set_in_shift(
        &sm_config,     // config
        false,          // shift to right, no
        true,           // autopush enabled
        32              //          threshold
    );
    
    sm_config_set_fifo_join(&sm_config, PIO_FIFO_JOIN_RX);
    // pio_sm_init(pio, sm, offset, &sm_config);
}

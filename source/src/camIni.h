// cam_wiz rgb V0.8 220330 qrt@qland.de

#pragma once

#define GAIN            0x00
#define BLUE            0x01
#define RED             0x02
#define RED             0x02
#define GREEN           0x03
#define BAVG            0x05
#define GAVG            0x06
#define RAVG            0x07
#define AECH            0x08
#define COM2            0x09
#define PID             0x0a
#define VER             0x0b
#define COM3            0x0c
#define COM4            0x0d
#define COM5            0x0e
#define COM6            0x0f
#define AEC             0x10
#define CLKRC           0x11
#define COM7            0x12
#define COM8            0x13
#define COM9            0x14
#define COM10           0x15
#define REG16           0x16
#define HSTART          0x17
#define HSIZE           0x18
#define VSTRT           0x19
#define VSIZE           0x1a
#define PSHFT           0x1b
#define MIDH            0x1c
#define MIDL            0x1d
#define LAEC            0x1f
#define COM11           0x20
#define BDBase          0x22
#define BDMStep         0x23
#define AEW             0x24
#define AEB             0x25
#define VPT             0x26
#define REG28           0x28
#define HOutSize        0x29
#define EXHCH           0x2a
#define EXHCL           0x2b
#define VOutSize        0x2c
#define ADVFL           0x2d
#define ADVFH           0x2e
#define YAVE            0x2f
#define LumHTh          0x30
#define LumLTh          0x31
#define HREF            0x32
#define DM_LNL          0x33
#define DM_LNH          0x34
#define ADoff_B         0x35
#define ADoff_R         0x36
#define ADoff_Gb        0x37
#define ADoff_Gr        0x38
#define Off_B           0x39
#define Off_R           0x3a
#define Off_Gb          0x3b
#define Off_Gr          0x3c
#define COM12           0x3d
#define COM13           0x3e
#define COM14           0x3f
#define COM16           0x41
#define TGT_B           0x42
#define TGT_R           0x43
#define TGT_Gb          0x44
#define TGT_Gr          0x45
#define LC_CTR          0x46
#define LC_XC           0x47
#define LC_YC           0x48
#define LC_COEF         0x49
#define LC_RADI         0x4a
#define LC_COEFB        0x4b 
#define LC_COEFR        0x4c
#define FixGain         0x4d
#define AREF1           0x4f
#define AREF6           0x54
#define UFix            0x60
#define VFix            0x61
#define AWBb_blk        0x62
#define AWB_Ctrl0       0x63
#define DSP_Ctrl1       0x64
#define DSP_Ctrl2       0x65
#define DSP_Ctrl3       0x66
#define DSP_Ctrl4       0x67
#define AWB_bias        0x68
#define AWBCtrl1        0x69
#define AWBCtrl2        0x6a
#define AWBCtrl3        0x6b
#define AWBCtrl4        0x6c
#define AWBCtrl5        0x6d
#define AWBCtrl6        0x6e
#define AWBCtrl7        0x6f
#define AWBCtrl8        0x70
#define AWBCtrl9        0x71
#define AWBCtrl10       0x72
#define AWBCtrl11       0x73
#define AWBCtrl12       0x74
#define AWBCtrl13       0x75
#define AWBCtrl14       0x76
#define AWBCtrl15       0x77
#define AWBCtrl16       0x78
#define AWBCtrl17       0x79
#define AWBCtrl18       0x7a
#define AWBCtrl19       0x7b
#define AWBCtrl20       0x7c
#define AWBCtrl21       0x7d 
#define GAM1            0x7e
#define GAM2            0x7f
#define GAM3            0x80
#define GAM4            0x81
#define GAM5            0x82
#define GAM6            0x83
#define GAM7            0x84
#define GAM8            0x85
#define GAM9            0x86
#define GAM10           0x87
#define GAM11           0x88
#define GAM12           0x89
#define GAM13           0x8a
#define GAM14           0x8b
#define GAM15           0x8c
#define SLOP            0x8d
#define DNSTh           0x8e
#define EDGE0           0x8f
#define EDGE1           0x90
#define DNSOff          0x91
#define EDGE2           0x92
#define EDGE3           0x93
#define MTX1            0x94
#define MTX2            0x95
#define MTX3            0x96
#define MTX4            0x97
#define MTX5            0x98
#define MTX6            0x99
#define MTX_Ctrl        0x9a
#define BRIGHT          0x9b
#define CNST            0x9c
#define UVADJ0          0x9e
#define UVADJ1          0x9f
#define SCAL0           0xa0
#define SCAL1           0xa1
#define SCAL2           0xa2
#define SDE             0xa6
#define USAT            0xa7
#define VSAT            0xa8
#define HUECOS          0xa9
#define HUESIN          0xaa
#define SIGN            0xab
#define DSPAuto         0xac

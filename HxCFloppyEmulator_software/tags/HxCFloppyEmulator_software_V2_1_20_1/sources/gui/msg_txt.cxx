#include "fl_includes.h"

#include <stdio.h>
#include <stdint.h>

#include "sdhxcfe_cfg.h"
#include "batch_converter_window.h"
#include "filesystem_generator_window.h"
#include "cb_filesystem_generator_window.h"

#include "floppy_dump_window.h"
#include "floppy_infos_window.h"
#include "rawfile_loader_window.h"
#include "sdhxcfecfg_window.h"
#include "usbhxcfecfg_window.h"
#include "edittool_window.h"
#include "log_gui.h"
#include "about_gui.h"

#include "soft_cfg_file.h"
#include "fl_dnd_box.h"

#include "libhxcfe.h"
#include "libhxcadaptor.h"
#include "usb_hxcfloppyemulator.h"

#include "loader.h"
#include "main_gui.h"
#include "main.h"

#include "msg_txt.h"


main_button_list  txt_buttons_main[]=
{
	{0,"Load","Load a floppy file image"},
	{0,"Load Raw image","Load a custom raw floppy image /\ncreate a custom floppy"},
	{0,"Batch converter","Convert multiple floppy files images"},
	{0,"Disk Browser","Create / Browse a DOS floppy disk"},
	{0,"Export","Export/save the loaded file image"},
	{0,"SD HxC Floppy\nEmulator settings","Configure the SD HxC Floppy Emulator"},
	{0,"USB HxC Floppy\nEmulator settings","Configure the USB HxC Floppy Emulator"},
	{0,"Floppy disk dump","Read a real disk"},
	{0,"Track Analyzer","Low level tracks viewer"}
};

Fl_Menu_Item menutable[] = {
  {"&Floppy image",0,0,0,FL_SUBMENU},
    {"&Load",	FL_ALT+'l',load_file_image_pb,0},
    {"Load custom RAW file/Create custom floppy",	FL_ALT+'r',menu_clicked,(void*)1},
    {"&Create DOS/AmigaDOS disk",	FL_ALT+'c',	menu_clicked,(void*)3},
    {"&Export disk/Save As",	FL_ALT+'e', save_file_image, 0},
    {"Batch convert files images",	FL_ALT+'b', menu_clicked,(void*)2},
    {"Dump a Floppy disk",	FL_ALT+'d', menu_clicked,(void*)7},
    {"Track Analyzer",	FL_ALT+'a', menu_clicked,(void*)10},

    {0},
  {"&Settings",FL_F+2,0,0,FL_SUBMENU},
    {"SD HxC FLoppy Emulator settings",	FL_ALT+'l',menu_clicked,(void*)5},
    {"USB HxC Floppy Emulator settings",	FL_ALT+'s',menu_clicked,(void*)6},
    {0},
  {"&Look",FL_F+3,0,0,FL_SUBMENU},
    {"Classic",	FL_ALT+'C',menu_clicked,(void*)11},
    {"Plastic",	FL_ALT+'P',menu_clicked,(void*)12},
    {"Gtk+",	FL_ALT+'G',menu_clicked,(void*)13},
	{"Gleam",	FL_ALT+'E',menu_clicked,(void*)14},
    {0},
  {"&Log",FL_F+4,0,0,FL_SUBMENU},
    {"&Log",	FL_ALT+'l',menu_clicked,(void*)8},
    {0},
  {"&About",0,0,0,FL_SUBMENU},
  {"&HxCFloppyEmulator",	FL_ALT+'h',menu_clicked,(void*)9},
    {0},
  {0}
};

Fl_Menu_Item format_choices[]=
{
	{ "HFE - SD HxC Floppy Emulator file format",0,format_choice_cb,(void*)PLUGIN_HXC_HFE},
	{ "MFM - MFM/FM track file format",0,format_choice_cb,(void*)PLUGIN_HXC_MFM},
	{ "AFI - Advanced file image format",0,format_choice_cb,(void*)PLUGIN_HXC_AFI},
	{ "VTR - VTrucco Floppy Emulator file format",0,format_choice_cb,(void*)PLUGIN_VTR_IMG},
	{ "RAW - RAW sectors file format",0,format_choice_cb,(void*)PLUGIN_RAW_IMG},
	{ "IMD - IMD sectors file format",0,format_choice_cb,(void*)PLUGIN_IMD_IMG},
	{ "ADF - ADF sectors file format",0,format_choice_cb,(void*)PLUGIN_AMIGA_ADF},
	{ "ADZ - ADZ sectors file format",0,format_choice_cb,(void*)PLUGIN_AMIGA_ADZ},
	{ "JV3 - JV3 TRS80 file format",0,format_choice_cb,(void*)PLUGIN_TRS80_JV3},
	{ "DMK - DMK TRS80 file format",0,format_choice_cb,(void*)PLUGIN_TRS80_DMK},
	{ "VDK - VDK Dragon format",0,format_choice_cb,(void*)PLUGIN_DRAGON3264_VDK},
	{ "TRD - TRD Zx Spectrum file format",0,format_choice_cb,(void*)PLUGIN_ZXSPECTRUM_TRD},
	{ "SDD - SpeccyDOS SDD file format",0,format_choice_cb,(void*)PLUGIN_SPECCYSDD},
	{ "V9T9- TI99/4A sectors file format",0,format_choice_cb,(void*)PLUGIN_TI994A_V9T9},
	{ "D88 - PC88 D88 file format",0,format_choice_cb,(void*)PLUGIN_NEC_D88},
	{ "ST - ATARI ST ST file format",0,format_choice_cb,(void*)PLUGIN_ATARIST_ST},
	{ "MSA - ATARI ST MSA file format",0,format_choice_cb,(void*)PLUGIN_ATARIST_MSA},
	{ "DIM - ATARI ST DIM file format",0,format_choice_cb,(void*)PLUGIN_ATARIST_DIM},
	{ "STX - Pasti/STX file format",0,format_choice_cb,(void*)PLUGIN_ATARIST_STX},
	{ "STW - STW file format",0,format_choice_cb,(void*)PLUGIN_ATARIST_STW},
	{ "FD - Thomson FD file format",0,format_choice_cb,(void*)PLUGIN_THOMSON_FD},
	{ "DSK - Amstrad CPC DSK file format",0,format_choice_cb,(void*)PLUGIN_AMSTRADCPC_DSK},
	{ "HFE - SD HxC Floppy Emulator (HDDD A2 Support)",0,format_choice_cb,(void*)PLUGIN_HXC_HDDD_A2},
	{ "HFE - Rev 2 - Experimental",0,format_choice_cb,(void*)PLUGIN_HXC_EXTHFE},
	{ "HFE - Rev 3 - Experimental",0,format_choice_cb,(void*)PLUGIN_HXC_HFEV3},	
	{ "Arburg - Arburg RAW sectors file format",0,format_choice_cb,(void*)PLUGIN_ARBURG},
	{ "Raw - Stream Kryoflux file format",0,format_choice_cb,(void*)PLUGIN_SKF},
	{ "IPF - SPS IPF file format (W.I.P)",0,format_choice_cb,(void*)PLUGIN_IPF},
	{ "SCP - SCP stream file format",0,format_choice_cb,(void*)PLUGIN_SCP},
	{ "BMP - BMP file image",0,format_choice_cb,(void*)PLUGIN_BMP},
	{ "BMP - BMP file image (disk)",0,format_choice_cb,(void*)PLUGIN_DISK_BMP},
	{ "XML - XML file image",0,format_choice_cb,(void*)PLUGIN_GENERIC_XML},
	{ "NSI - Northstar file image",0,format_choice_cb,(void*)PLUGIN_NORTHSTAR},
	{ "H8D - Heathkit file image",0,format_choice_cb,(void*)PLUGIN_HEATHKIT},	
	{0}
};

Fl_Menu_Item fs_choices[]=
{
	{ "5\"25 & 8\" 160KB SSDD 300RPM FAT12",0,fs_choice_cb,(void*)FS_5P25_300RPM_160KB_MSDOS_FAT12},
	{ "5\"25 & 8\" 160KB SSDD 360RPM FAT12",0,fs_choice_cb,(void*)FS_5P25_360RPM_160KB_MSDOS_FAT12},

	{ "5\"25       180KB SSDD 300RPM FAT12",0,fs_choice_cb,(void*)FS_5P25_300RPM_180KB_MSDOS_FAT12},
	{ "5\"25       180KB SSDD 360RPM FAT12",0,fs_choice_cb,(void*)FS_5P25_360RPM_180KB_MSDOS_FAT12},

	{ "5\"25       320KB SSDD 300RPM FAT12",0,fs_choice_cb,(void*)FS_5P25_SS_300RPM_320KB_MSDOS_FAT12},
	{ "5\"25       320KB SSDD 360RPM FAT12",0,fs_choice_cb,(void*)FS_5P25_SS_360RPM_320KB_MSDOS_FAT12},

	{ "5\"25       320KB DSDD 300RPM FAT12",0,fs_choice_cb,(void*)FS_5P25_DS_300RPM_320KB_MSDOS_FAT12},
	{ "5\"25       320KB DSDD 360RPM FAT12",0,fs_choice_cb,(void*)FS_5P25_DS_360RPM_320KB_MSDOS_FAT12},

	{ "5\"25 & 8\" 360KB DSDD 300RPM FAT12",0,fs_choice_cb,(void*)FS_5P25_DS_300RPM_360KB_MSDOS_FAT12},
	{ "5\"25 & 8\" 360KB DSDD 360RPM FAT12",0,fs_choice_cb,(void*)FS_5P25_DS_360RPM_360KB_MSDOS_FAT12},

	{ "3\"5        640KB DSDD FAT12",0,fs_choice_cb,(void*)FS_3P5_DS_300RPM_640KB_MSDOS_FAT12},

	{ "3\"5        720KB DSDD FAT12",0,fs_choice_cb,(void*)FS_720KB_MSDOS_FAT12},
	{ "3\"5        738KB DSDD FAT12",0,fs_choice_cb,(void*)FS_738KB_MSDOS_FAT12},
	{ "3\"5        800KB DSDD FAT12",0,fs_choice_cb,(void*)FS_800KB_MSDOS_FAT12},
	{ "3\"5        820KB DSDD FAT12",0,fs_choice_cb,(void*)FS_820KB_MSDOS_FAT12},

	{ "5\"25       1.2MB DSHD FAT12",0,fs_choice_cb,(void*)FS_5P25_300RPM_1200KB_MSDOS_FAT12},
	{ "5\"25       1.23MB DSHD FAT12",0,fs_choice_cb,(void*)FS_5P25_300RPM_1230KB_MSDOS_FAT12},

	{ "3\"5        1.44MB DSHD FAT12",0,fs_choice_cb,(void*)FS_1_44MB_MSDOS_FAT12},
	{ "3\"5        1.476MB DSHD FAT12",0,fs_choice_cb,(void*)FS_1_476MB_MSDOS_FAT12},

	{ "3\"5        1.6MB DSHD FAT12",0,fs_choice_cb,(void*)FS_1_600MB_MSDOS_FAT12},
	{ "3\"5        1.64MB DSHD FAT12",0,fs_choice_cb,(void*)FS_1_640MB_MSDOS_FAT12},
	{ "3\"5        1.68MB DSHD FAT12",0,fs_choice_cb,(void*)FS_1_68MB_MSDOS_FAT12},

	{ "3\"5        1.722MB DSHD FAT12",0,fs_choice_cb,(void*)FS_1_722MB_MSDOS_FAT12},
	{ "3\"5        1.743MB DSHD FAT12",0,fs_choice_cb,(void*)FS_1_743MB_MSDOS_FAT12},
	{ "3\"5        1.764MB DSHD FAT12",0,fs_choice_cb,(void*)FS_1_764MB_MSDOS_FAT12},
	{ "3\"5        1.785MB DSHD FAT12",0,fs_choice_cb,(void*)FS_1_785MB_MSDOS_FAT12},

	{ "3\"5        2.50MB DSDD FAT12",0,fs_choice_cb,(void*)FS_2_50MB_MSDOS_FAT12},

	{ "3\"5        2.88MB DSED FAT12",0,fs_choice_cb,(void*)FS_2_88MB_MSDOS_FAT12},
	{ "3\"5        3.38MB DSHD FAT12",0,fs_choice_cb,(void*)FS_3_38MB_MSDOS_FAT12},
	{ "3\"5        4.50MB DSHD FAT12",0,fs_choice_cb,(void*)FS_4_50MB_MSDOS_FAT12},

	{ "3\"5        5.35MB DSHD FAT12",0,fs_choice_cb,(void*)FS_5_35MB_MSDOS_FAT12},
	{ "3\"5        5.35MB DSHD FAT12 (B)",0,fs_choice_cb,(void*)FS_5_35MB_B_MSDOS_FAT12},

	{ "3\"5        6.78MB DSHD FAT12",0,fs_choice_cb,(void*)FS_6_78MB_MSDOS_FAT12},

	{ "3\"5        360KB SSDD Atari FAT12",0,fs_choice_cb,(void*)FS_360KB_ATARI_FAT12},
	{ "3\"5        720KB DSDD Atari FAT12",0,fs_choice_cb,(void*)FS_720KB_ATARI_FAT12},
	{ "3\"5        902KB DSDD Atari FAT12",0,fs_choice_cb,(void*)FS_902KB_ATARI_FAT12},
	{ "3\"5        3.42MB DSDD Atari FAT12",0,fs_choice_cb,(void*)FS_3_42MB_ATARI_FAT12},

	{ "3\"5        880KB DSDD AmigaDOS",0,fs_choice_cb,(void*)FS_880KB_AMIGADOS},

	{0}
};

Fl_Menu_Item sectorsize_choices[]=
{
	{ "128 Bytes",0,raw_loader_window_datachanged,(void*)128},
	{ "256 Bytes",0,raw_loader_window_datachanged,(void*)256},
	{ "512 Bytes",0,raw_loader_window_datachanged,(void*)512},
	{ "1024 Bytes",0,raw_loader_window_datachanged,(void*)1024},
	{ "2048 Bytes",0,raw_loader_window_datachanged,(void*)2048},
	{ "4096 Bytes",0,raw_loader_window_datachanged,(void*)4096},
	{ "8192 Bytes",0,raw_loader_window_datachanged,(void*)8192},
	{ "16384 Bytes",0,raw_loader_window_datachanged,(void*)16384},
	{0}
};


Fl_Menu_Item if_choices[]=
{
	{ "Amiga",0,format_choice_cb,(void*)AMIGA_DD_FLOPPYMODE},
	{ "Amiga HD",0,format_choice_cb,(void*)AMIGA_HD_FLOPPYMODE},
	{ "Atari ST",0,format_choice_cb,(void*)ATARIST_DD_FLOPPYMODE},
	{ "Atari ST HD",0,format_choice_cb,(void*)ATARIST_HD_FLOPPYMODE},
	{ "IBM PC 720kB",0,format_choice_cb,(void*)IBMPC_DD_FLOPPYMODE},
	{ "IBM PC 1.44MB",0,format_choice_cb,(void*)IBMPC_HD_FLOPPYMODE},
	{ "IBM PC 2.88MB",0,format_choice_cb,(void*)IBMPC_ED_FLOPPYMODE},
	{ "AKAI S900/S950 DD",0,format_choice_cb,(void*)S950_DD_FLOPPYMODE},
	{ "AKAI S950 HD",0,format_choice_cb,(void*)S950_HD_FLOPPYMODE},
	{ "Amstrad CPC",0,format_choice_cb,(void*)CPC_DD_FLOPPYMODE},
	{ "MSX 2",0,format_choice_cb,(void*)MSX2_DD_FLOPPYMODE},
	{ "Generic Shugart",0,format_choice_cb,(void*)GENERIC_SHUGART_DD_FLOPPYMODE},
	{ "Emu Shugart",0,format_choice_cb,(void*)EMU_SHUGART_FLOPPYMODE},
	{ "C64 1541",0,format_choice_cb,(void*)C64_DD_FLOPPYMODE},
	{ 0,0,0,(void*)0},
	{0}
};

Fl_Menu_Item pincfg_choices[]=
{
	{ "Auto",0,NULL,(void*)PIN_CFG_AUTO},
	{ "Low",0,NULL,(void*)PIN_CFG_LOW},
	{ "High",0,NULL,(void*)PIN_CFG_HIGH},
	{ "Ready",0,NULL,(void*)PIN_CFG_READY},
	{ "nReady",0,NULL,(void*)PIN_CFG_NOTREADY},
	{ "Density",0,NULL,(void*)PIN_CFG_DENSITY},
	{ "nDensity",0,NULL,(void*)PIN_CFG_NOTDENSITY},
	{ "DiskChange 1",0,NULL,(void*)PIN_CFG_DC1},
	{ "nDiskChange 1",0,NULL,(void*)PIN_CFG_NOTDC1},
	{ "DiskChange 2",0,NULL,(void*)PIN_CFG_DC2},
	{ "nDiskChange 2",0,NULL,(void*)PIN_CFG_NOTDC2},
	{ "DiskChange 3",0,NULL,(void*)PIN_CFG_DC3},
	{ "nDiskChange 3",0,NULL,(void*)PIN_CFG_NOTDC3},
	{ 0,0,0,(void*)0},
	{0}
};

Fl_Menu_Item feifcfg_choices[]=
{
	{ "From HFE",0,NULL,(void*)-1},
	{ "Generic Shugart",0,NULL,(void*)GENERIC_SHUGART_DD_FLOPPYMODE},
	{ "IBM PC DD",0,NULL,(void*)IBMPC_DD_FLOPPYMODE},
	{ "IBM PC HD",0,NULL,(void*)IBMPC_HD_FLOPPYMODE},
	{ "AKAI S900/S950 DD",0,NULL,(void*)S950_DD_FLOPPYMODE},
	{ "AKAI S950 HD",0,NULL,(void*)S950_HD_FLOPPYMODE},
	{ "Emu Shugart",0,NULL,(void*)EMU_SHUGART_FLOPPYMODE},
	{ "ATARI ST",0,NULL,(void*)ATARIST_DD_FLOPPYMODE},
	{ "ATARI ST HD",0,NULL,(void*)ATARIST_HD_FLOPPYMODE},
	{ "Amiga",0,NULL,(void*)AMIGA_DD_FLOPPYMODE},
	{ "Amiga HD",0,NULL,(void*)AMIGA_HD_FLOPPYMODE},
	{ "Amstrad CPC",0,NULL,(void*)CPC_DD_FLOPPYMODE},
	{ "MSX 2",0,NULL,(void*)MSX2_DD_FLOPPYMODE},
	{ "Custom",0,NULL,(void*)0x80},
	{ 0,0,0,(void*)0},
	{0}
};


Fl_Menu_Item track_type_choices[]=
{
	{ "FM",0,raw_loader_window_datachanged,(void*)ISOFORMAT_SD},
	{ "IBM FM",0,raw_loader_window_datachanged,(void*)IBMFORMAT_SD},
	{ "MFM",0,raw_loader_window_datachanged,(void*)ISOFORMAT_DD},
	{ "IBM MFM",0,raw_loader_window_datachanged,(void*)IBMFORMAT_DD},
	{ "TYCOM FM",0,raw_loader_window_datachanged,(void*)TYCOMFORMAT_SD},
	{ "MEMBRAIN MFM",0,raw_loader_window_datachanged,(void*)MEMBRAINFORMAT_DD},
	{ "UKNC MFM",0,raw_loader_window_datachanged,(void*)UKNCFORMAT_DD},
	{0}
};

Fl_Menu_Item nbside_choices[]=
{
	{ "1 Side",0,raw_loader_window_datachanged,(void*)1},
	{ "2 Sides",0,raw_loader_window_datachanged,(void*)2},
	{0}
};

Fl_Menu_Item disklayout_choices[512]=
{
	{ "Custom Disk Layout",0,raw_loader_window_datachanged,(void*)255},
	{ "XML file Disk Layout",0,raw_loader_window_datachanged,(void*)255},
	{0}
};

/*
const platform platformlist[]=
{
	{ AMIGA_DD_FLOPPYMODE,"Amiga","DS0","DS1","DS2","MTRON"},
	{ AMIGA_HD_FLOPPYMODE,"Amiga HD","DS0","DS1","DS2","MTRON"},
	{ ATARIST_DD_FLOPPYMODE,"Atari ST","D0SEL","D1SEL","-","MTRON"},
	{ ATARIST_HD_FLOPPYMODE,"Atari ST HD","D0SEL","D1SEL","-","MTRON"},
	{ IBMPC_DD_FLOPPYMODE,"IBM PC 720kB","MOTEA","DRVSB","DRVSA","MOTEB"},
	{ IBMPC_HD_FLOPPYMODE,"IBM PC 1.44MB","MOTEA","DRVSB","DRVSA","MOTEB"},
	{ IBMPC_ED_FLOPPYMODE,"IBM PC 2.88MB","MOTEA","DRVSB","DRVSA","MOTEB"},
	{ CPC_DD_FLOPPYMODE,"Amstrad CPC","Drive Select 0","Drive Select 1","-","MOTOR ON"},
	{ MSX2_DD_FLOPPYMODE,"MSX 2","DS0","DS1","DS2","MTRON"},
	{ GENERIC_SHUGART_DD_FLOPPYMODE,"Generic Shugart","DS0","DS1","DS2","MTRON"},
	{ EMU_SHUGART_FLOPPYMODE,"Emu Shugart","DS0","DS1","DS2","MTRON"},
	{ C64_DD_FLOPPYMODE,"C64 1541","NA","NA","NA","NA"},
	{ -1,"?","DS0","DS1","DS2","MTRON"}
};

*/

const track_type track_type_list[]=
{
	{ FM_TRACK_TYPE,"FM",ISOFORMAT_SD},
	{ FMIBM_TRACK_TYPE,"IBM FM",IBMFORMAT_SD},
	{ MFM_TRACK_TYPE,"MFM",ISOFORMAT_DD},
	{ MFMIBM_TRACK_TYPE,"IBM MFM",IBMFORMAT_DD},
	{ TYCOM_TRACK_TYPE,"TYCOM FM",TYCOMFORMAT_SD},
	{ MEMBRAIN_TRACK_TYPE,"MEMBRAIN MFM",MEMBRAINFORMAT_DD},
	{ UKNC_TRACK_TYPE,"UKNC MFM",UKNCFORMAT_DD},
	{ ARBURGDATA_TRACK_TYPE,"ARBURG DATA",ARBURG_DAT},
	{ ARBURGSYST_TRACK_TYPE,"ARBURG SYSTEM",ARBURG_SYS},
	//{ GCR_TRACK_TYPE,"GCR"},
	{ -1,"",0},
	{ 0,0,0}
};

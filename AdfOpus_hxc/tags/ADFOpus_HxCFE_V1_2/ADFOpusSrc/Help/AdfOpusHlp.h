/* AdfOpusHlp.h
** Header file containing identifiers for help file items for use in context-
** sensitive help.
**
** Gary Harris
** 25/9/98
** Updated for V0.9 December 2001.
*/

/*
** Help file topics.
*/

#define	IDH_INTRODUCTION				7000		
#define	IDH_MAIN						7001
#define	IDH_CREATE_DIALOGUE				7002
#define	IDH_INFORMATION_DIALOGUE_VOL	7003
#define	IDH_INFORMATION_DIALOGUE_DEV	7004
#define	IDH_INSTALLATION				7005
#define	IDH_USING_THE_PROGRAM			7006
#define	IDH_FILE_TYPES					7007
#define	IDH_UAE							7008
#define	IDH_FILESYSTEM					7009
#define	IDH_BUGS						7010
#define	IDH_COPYRIGHT					7011
#define	IDH_FEEDBACK					7012
#define	IDH_BATCH_CONVERTER				7013
#define	IDH_OPTIONS						7014
#define	IDH_VOLUME_SELECT				7015
#define	IDH_LICENCE						7016
#define	IDH_HISTORY						7017
#define	IDH_PROPERTIES					7018
#define	IDH_FILE_REGISTERING			7019
#define	IDH_DISK2FDI					7020
#define	IDH_BOOTBLOCK					7021
#define	IDH_HARDDRIVES					7022
#define	IDH_TEXTVIEWER					7023

/*
** Main window items.
*/

#define	IDH_ADF_TITLE				7100
#define	IDH_FILE_MENU				7101
#define	IDH_EDIT_MENU				7102
#define	IDH_VIEW_MENU				7103
#define	IDH_ACTION_MENU				7104
#define	IDH_TOOLS_MENU				7105
#define	IDH_WINDOW_MENU				7106
#define	IDH_HELP_MENU				7107
#define	IDH_CONTEXT_MENU		7108
#define	IDH_NEW_BUTTON				7109
#define	IDH_OPEN_BUTTON				7110
#define	IDH_CLOSE_BUTTON			7111
#define	IDH_INFO_BUTTON				7112
#define	IDH_RENAME_BUTTON			7113
#define	IDH_DELETE_BUTTON			7114
#define	IDH_NEW_DIR_BUTTON			7115
#define	IDH_PROPERTIES_BUTTON		7116
#define	IDH_SHOW_UNDELETABLE_BUTTON		7117
#define	IDH_UNDELETE_BUTTON		7118
#define	IDH_TEXTVIEWER_BUTTON		7119
#define	IDH_SYSTEM_CONTENTS_WINDOW	7120
#define	IDH_ADF_CONTENTS_WINDOW		7121
#define	IDH_UP_ONE_LEVEL			7122
#define	IDH_SYSTEM_DIR_WINDOW		7123	
#define	IDH_ADF_DIR_WINDOW			7124
#define	IDH_STATUS_BAR				7125
#define	IDH_FILENAME				7126
#define	IDH_FILESIZE				7127
#define	IDH_FILEFLAGS_PC			7128
#define	IDH_FILEFLAGS_AMIGA			7129
#define	IDH_FILE_COMMENT			7130

/*
** These are the references for the menu items.
*/

#define	IDH_FILE_NEW					7200
#define	IDH_FILE_OPEN					7201
#define	IDH_FILE_OPEN_DEVICE			7202
#define	IDH_FILE_CLOSE					7203
#define	IDH_FILE_INFORMATION			7204
#define	IDH_FILE_EXIT					7205
#define	IDH_FILE_PROPERTIES				7206

#define	IDH_EDIT_SELECT_ALL				7210
#define	IDH_EDIT_SELECT_NONE			7211
#define	IDH_EDIT_INVERT_SELECTION		7212

#define	IDH_VIEW_TOOLBAR				7220
#define	IDH_VIEW_STATUS_BAR				7221
#define	IDH_VIEW_REFRESH				7222
#define	IDH_VIEW_WINLIST				7223

#define	IDH_ACTION_UP_ONE_LEVEL			7230
#define	IDH_ACTION_NEW_DIRECTORY		7231
#define	IDH_ACTION_DELETE				7232
#define	IDH_ACTION_RENAME				7233
#define	IDH_ACTION_PROPERTIES			7234	

#define	IDH_TOOLS_BATCH_CONVERTER		7240
#define	IDH_TOOLS_INSTALL				7241
#define	IDH_TOOLS_DISK2FDI				7242
#define	IDH_TOOLS_OPTIONS				7243
#define	IDH_TOOLS_BOOTBLOCK				7244
#define	IDH_TOOLS_TEXTVIEWER				7245

#define	IDH_VOLUME_NEW					7250
#define	IDH_VOLUME_OPEN					7251
#define	IDH_VOLUME_CLOSE				7252
#define	IDH_VOLUME_INFORMATION			7253

#define	IDH_WINDOW_CASCADE				7260
#define	IDH_WINDOW_TILE_HORIZONTALLY	7261
#define	IDH_WINDOW_TILE_VERTICALLY		7262
#define	IDH_WINDOW_ARRANGE_ICONS		7263
#define	IDH_WINDOW_CLOSE_ALL			7264
#define	IDH_WINDOW_LIST					7265

#define	IDH_HELP_TOPICS					7270
#define	IDH_HELP_OVERVIEW				7271
#define	IDH_HELP_ABOUT					7272

/*
** List of items from the Volume Information dialogue.
*/

#define	IDH_INFO_DEV_PATH			7300
#define	IDH_INFO_DEV_SIZE			7301
#define	IDH_INFO_DEV_TYPE			7302
#define	IDH_INFO_DEV_GEOMETRY		7303
#define	IDH_INFO_DEV_VOLUMES		7304
#define	IDH_INFO_VOL_LABEL			7305
#define	IDH_INFO_VOL_USED			7306
#define	IDH_INFO_VOL_FREE			7307
#define	IDH_INFO_VOL_TOTAL			7308
#define	IDH_INFO_VOL_FULL			7309
#define	IDH_INFO_VOL_FFS			7310
#define	IDH_INFO_VOL_DC				7311
#define	IDH_INFO_VOL_INTL			7312
#define	IDH_INFO_HELP_BUTTON		7313
#define	IDH_INFO_CLOSE_BUTTON		7314

/*
** These are the items for the Create new Volume dialogue.
*/

#define	IDH_CREATE_CLOSE						7400
#define	IDH_CREATE_PATH_EDIT					7401
#define	IDH_CREATE_PATH_BROWSE					7402
#define	IDH_CREATE_TYPE_ADF						7403
#define	IDH_CREATE_TYPE_HD						7404
#define	IDH_CREATE_TYPE_HARDFILE				7405
#define	IDH_CREATE_TYPE_HARDFILE_PRESET			7406
#define	IDH_CREATE_TYPE_HARDFILE_PRESET_SIZES	7407
#define	IDH_CREATE_TYPE_HARDFILE_CUSTOM			7408
#define	IDH_CREATE_TYPE_HARDFILE_CUSTOM_SIZE	7409
#define	IDH_CREATE_FILESYSTEM_FFS				7410
#define	IDH_CREATE_FILESYSTEM_DC				7411
#define	IDH_CREATE_FILESYSTEM_IFS				7412
#define	IDH_CREATE_FILESYSTEM_BOOT				7413
#define	IDH_CREATE_LABEL						7414
#define	IDH_CREATE_OPEN							7415
#define	IDH_CREATE_CREATE_BUTTON				7416
#define	IDH_CREATE_HELP_BUTTON					7417
#define	IDH_CREATE_CANCEL_BUTTON				7418
#define	IDH_CREATE_TYPE_HFE						7419
/*
** These are the items for the Batch Converter dialogue.
*/

#define	IDH_BATCH_SOURCE_LIST					7500
#define	IDH_BATCH_ADD_BUTTON					7501
#define	IDH_BATCH_REMOVE_BUTTON					7502
#define	IDH_BATCH_REMOVE_ALL_BUTTON				7503
#define	IDH_BATCH_DELETE_ORIGINAL				7504
#define	IDH_BATCH_ADF_BUTTON					7505
#define	IDH_BATCH_ADZ_BUTTON					7506
#define	IDH_BATCH_STATUS						7507
#define	IDH_BATCH_START_BUTTON					7508
#define	IDH_BATCH_HELP_BUTTON					7509
#define	IDH_BATCH_CLOSE_BUTTON					7510


/*
** These are the items for the Options dialogue.
*/

#define	IDH_OPTIONS_WINLIST_DRIVE				7600
#define	IDH_OPTIONS_WINLIST_DIR					7601
#define	IDH_OPTIONS_DIR							7602
#define	IDH_OPTIONS_LABEL						7603
#define	IDH_OPTIONS_CONFIRM_COMDEL				7604
#define	IDH_OPTIONS_CONFIRM_DELDIR				7605
#define	IDH_OPTIONS_MISC_DIRCACHE				7606
#define	IDH_OPTIONS_REGISTER_BUTTON				7607
#define	IDH_OPTIONS_OK_BUTTON					7608
#define	IDH_OPTIONS_APPLY_BUTTON				7609
#define	IDH_OPTIONS_HELP_BUTTON					7610
#define	IDH_OPTIONS_CANCEL_BUTTON				7611


/*
** These are the items for the Select Volume dialogue.
*/

#define	IDH_VOLSEL_LIST							7700
#define	IDH_VOLSEL_OPEN_BUTTON					7701
#define	IDH_VOLSEL_HELP_BUTTON					7702
#define	IDH_VOLSEL_CANCEL_BUTTON				7703


/*
** These are the items for the Windows property dialogue.
*/

#define	IDH_PROPERTIES_FILENAME_WIN				7800
#define	IDH_PROPERTIES_WIN_READONLY				7801
#define	IDH_PROPERTIES_WIN_ARCHIVE				7802
#define	IDH_PROPERTIES_WIN_HIDDEN				7803
#define	IDH_PROPERTIES_WIN_SYSTEM				7804
#define	IDH_PROPERTIES_WIN_OK_BUTTON			7805
#define	IDH_PROPERTIES_WIN_HELP_BUTTON			7806
#define	IDH_PROPERTIES_WIN_CANCEL_BUTTON		7807


/*
** These are the items for the Amiga property dialogue.
*/

#define	IDH_PROPERTIES_FILENAME_AMI				7820
#define	IDH_PROPERTIES_AMI_READABLE				7821
#define	IDH_PROPERTIES_AMI_WRITABLE				7822
#define	IDH_PROPERTIES_AMI_EXECUTABLE			7823
#define	IDH_PROPERTIES_AMI_DELETABLE			7824
#define	IDH_PROPERTIES_AMI_SCRIPT				7825
#define	IDH_PROPERTIES_AMI_ARCHIVE				7826
#define	IDH_PROPERTIES_AMI_PURE					7827
#define	IDH_PROPERTIES_AMI_HOLDBIT				7828
#define	IDH_PROPERTIES_AMI_COMMENT				7829
#define	IDH_PROPERTIES_AMI_OK_BUTTON			7830
#define	IDH_PROPERTIES_AMI_HELP_BUTTON			7831
#define	IDH_PROPERTIES_AMI_CANCEL_BUTTON		7832


/*
** These are the items for the Disk2FDI dialogue.
*/

#define	IDH_DISK2FDI_FILENAME					7840
#define	IDH_DISK2FDI_CHECK_OPEN					7841
#define	IDH_DISK2FDI_TYPE_FDI					7842
#define	IDH_DISK2FDI_TYPE_ADF					7843
#define	IDH_DISK2FDI_TYPE_ST					7844
#define	IDH_DISK2FDI_TYPE_IMG					7845
#define	IDH_DISK2FDI_USE_B						7846
#define	IDH_DISK2FDI_1SIDED						7847
#define	IDH_DISK2FDI_2SIDED						7848
#define	IDH_DISK2FDI_NUM_TRACKS					7849
#define	IDH_DISK2FDI_NUM_SECTORS				7850
#define	IDH_DISK2FDI_START_BUTTON				7851
#define	IDH_DISK2FDI_HELP_BUTTON				7852
#define	IDH_DISK2FDI_CANCEL_BUTTON				7853

/*
** These are the items for the Display Bootblock dialogue.
*/

#define	IDH_BOOTBLOCK_DISPLAY					7860
#define	IDH_BOOTBLOCK_HELP_BUTTON				7861
#define	IDH_BOOTBLOCK_OK_BUTTON					7862


/*
** These are the items for the Text Viewer dialogue.
*/

#define	IDH_TEXTVIEWER_BUTTON_HELP				7870
#define	IDH_TEXTVIEWER_BUTTON_OK				7871
#define	IDH_TEXTVIEWER_EDIT						7872
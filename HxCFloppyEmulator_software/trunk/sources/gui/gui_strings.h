
typedef struct string_entry_
{
	unsigned int string_id;
	const char * string;
}string_entry;

enum
{
	STR_COMMON_OK = 1,
	STR_COMMON_CLOSE,
	STR_SDSETTINGSWINDOW_0001,
	STR_SDSETTINGSWINDOW_0002,
	STR_SDSETTINGSWINDOW_0003,
	STR_SDSETTINGSWINDOW_0004,
	STR_SDSETTINGSWINDOW_0005,
	STR_SDSETTINGSWINDOW_0006,
	STR_SDSETTINGSWINDOW_0007,
	STR_SDSETTINGSWINDOW_0008,
	STR_SDSETTINGSWINDOW_0009,
	STR_SDSETTINGSWINDOW_0010,
	STR_SDSETTINGSWINDOW_0011,
	STR_SDSETTINGSWINDOW_0012,
	STR_SDSETTINGSWINDOW_0013,
	STR_SDSETTINGSWINDOW_0014,
	STR_SDSETTINGSWINDOW_0015,
	STR_SDSETTINGSWINDOW_0016,
	STR_SDSETTINGSWINDOW_0017,
	STR_SDSETTINGSWINDOW_0018,
	STR_SDSETTINGSWINDOW_0019,
	STR_SDSETTINGSWINDOW_0020,
	STR_SDSETTINGSWINDOW_0021,
	STR_SDSETTINGSWINDOW_0022,
	STR_SDSETTINGSWINDOW_0023,
	STR_SDSETTINGSWINDOW_0024,
	STR_SDSETTINGSWINDOW_0025,
	STR_SDSETTINGSWINDOW_0026,
	STR_SDSETTINGSWINDOW_0027,
	STR_SDSETTINGSWINDOW_0028,
	STR_SDSETTINGSWINDOW_0029,
	STR_SDSETTINGSWINDOW_0030,
	STR_SDSETTINGSWINDOW_0031,
	STR_SDSETTINGSWINDOW_0032,
	STR_SDSETTINGSWINDOW_0033,
	STR_SDSETTINGSWINDOW_0034,
	STR_SDSETTINGSWINDOW_0035,
	STR_SDSETTINGSWINDOW_0036,
	STR_SDSETTINGSWINDOW_0038,
	STR_SDSETTINGSWINDOW_0039,

	STR_BATCHCONVERTWINDOW_0001,
	STR_BATCHCONVERTWINDOW_0002,
	STR_BATCHCONVERTWINDOW_0003,
	STR_BATCHCONVERTWINDOW_0004,
	STR_BATCHCONVERTWINDOW_0005,
	STR_BATCHCONVERTWINDOW_0006,
	STR_BATCHCONVERTWINDOW_0007,
	STR_BATCHCONVERTWINDOW_0008,
	STR_BATCHCONVERTWINDOW_0009,
	STR_BATCHCONVERTWINDOW_0010,
	STR_BATCHCONVERTWINDOW_0011,
	STR_BATCHCONVERTWINDOW_0012,
	STR_BATCHCONVERTWINDOW_0013,
	STR_BATCHCONVERTWINDOW_0014,
	STR_BATCHCONVERTWINDOW_0015,
	STR_BATCHCONVERTWINDOW_0016,

	STR_FSGENERATORWINDOW_0001,
	STR_FSGENERATORWINDOW_0002,
	STR_FSGENERATORWINDOW_0004,
	STR_FSGENERATORWINDOW_0005,
	STR_FSGENERATORWINDOW_0006,
	STR_FSGENERATORWINDOW_0007,
	STR_FSGENERATORWINDOW_0008,
	STR_FSGENERATORWINDOW_0009,
	STR_FSGENERATORWINDOW_0010,

	STR_FLOPPYDUMPWINDOW_0001,
	STR_FLOPPYDUMPWINDOW_0002,
	STR_FLOPPYDUMPWINDOW_0003,
	STR_FLOPPYDUMPWINDOW_0004,
	STR_FLOPPYDUMPWINDOW_0005,
	STR_FLOPPYDUMPWINDOW_0006,
	STR_FLOPPYDUMPWINDOW_0007,
	STR_FLOPPYDUMPWINDOW_0008,
	STR_FLOPPYDUMPWINDOW_0009,
	STR_FLOPPYDUMPWINDOW_0011,
	STR_FLOPPYDUMPWINDOW_0012,

	STR_EDITTOOLWINDOW_0001,
	STR_EDITTOOLWINDOW_0002,
	STR_EDITTOOLWINDOW_0003,
	STR_EDITTOOLWINDOW_0004,
	STR_EDITTOOLWINDOW_0005,
	STR_EDITTOOLWINDOW_0006,
	STR_EDITTOOLWINDOW_0007,
	STR_EDITTOOLWINDOW_0008,
	STR_EDITTOOLWINDOW_0009,
	STR_EDITTOOLWINDOW_0010,
	STR_EDITTOOLWINDOW_0011,
	STR_EDITTOOLWINDOW_0012,
	STR_EDITTOOLWINDOW_0013,
	STR_EDITTOOLWINDOW_0014,
	STR_EDITTOOLWINDOW_0015,
	STR_EDITTOOLWINDOW_0016,
	STR_EDITTOOLWINDOW_0017,
	STR_EDITTOOLWINDOW_0018,
	STR_EDITTOOLWINDOW_0019,
	STR_EDITTOOLWINDOW_0020,
	STR_EDITTOOLWINDOW_0021,
	STR_EDITTOOLWINDOW_0022,
	STR_EDITTOOLWINDOW_0023,
	STR_EDITTOOLWINDOW_0024,

	STR_USBSETTINGSWINDOW_0001,
	STR_USBSETTINGSWINDOW_0002,
	STR_USBSETTINGSWINDOW_0003,
	STR_USBSETTINGSWINDOW_0004,
	STR_USBSETTINGSWINDOW_0005,
	STR_USBSETTINGSWINDOW_0006,
	STR_USBSETTINGSWINDOW_0007,
	STR_USBSETTINGSWINDOW_0008,
	STR_USBSETTINGSWINDOW_0009,
	STR_USBSETTINGSWINDOW_0010,
	STR_USBSETTINGSWINDOW_0011,
	STR_USBSETTINGSWINDOW_0012,
	STR_USBSETTINGSWINDOW_0013,
	STR_USBSETTINGSWINDOW_0014,
	STR_USBSETTINGSWINDOW_0015,
	STR_USBSETTINGSWINDOW_0016,
	STR_USBSETTINGSWINDOW_0017,
	STR_USBSETTINGSWINDOW_0018,
	STR_USBSETTINGSWINDOW_0019,
	STR_USBSETTINGSWINDOW_0020,
	STR_USBSETTINGSWINDOW_0021,
	STR_USBSETTINGSWINDOW_0022,

	STR_FLOPPYVIEWERWINDOW_0001,
	STR_FLOPPYVIEWERWINDOW_0002,
	STR_FLOPPYVIEWERWINDOW_0003,
	STR_FLOPPYVIEWERWINDOW_0004,
	STR_FLOPPYVIEWERWINDOW_0005,
	STR_FLOPPYVIEWERWINDOW_0007,
	STR_FLOPPYVIEWERWINDOW_0008,
	STR_FLOPPYVIEWERWINDOW_0009,
	STR_FLOPPYVIEWERWINDOW_0010,
	STR_FLOPPYVIEWERWINDOW_0011,
	STR_FLOPPYVIEWERWINDOW_0012,
	STR_FLOPPYVIEWERWINDOW_0013,
	STR_FLOPPYVIEWERWINDOW_0014,
	STR_FLOPPYVIEWERWINDOW_0015,
	STR_FLOPPYVIEWERWINDOW_0016,
	STR_FLOPPYVIEWERWINDOW_0017,
	STR_FLOPPYVIEWERWINDOW_0018,
	STR_FLOPPYVIEWERWINDOW_0019,
	STR_FLOPPYVIEWERWINDOW_0020,
	STR_FLOPPYVIEWERWINDOW_0021,
	STR_FLOPPYVIEWERWINDOW_0022,
	STR_FLOPPYVIEWERWINDOW_0023,
	STR_FLOPPYVIEWERWINDOW_0024,
	STR_FLOPPYVIEWERWINDOW_0025,
	STR_FLOPPYVIEWERWINDOW_0026,
	STR_FLOPPYVIEWERWINDOW_0027,

	STR_RAWLOADERWINDOW_0001,
	STR_RAWLOADERWINDOW_0002,
	STR_RAWLOADERWINDOW_0003,
	STR_RAWLOADERWINDOW_0004,
	STR_RAWLOADERWINDOW_0005,
	STR_RAWLOADERWINDOW_0006,
	STR_RAWLOADERWINDOW_0007,
	STR_RAWLOADERWINDOW_0008,
	STR_RAWLOADERWINDOW_0009,
	STR_RAWLOADERWINDOW_0010,
	STR_RAWLOADERWINDOW_0011,
	STR_RAWLOADERWINDOW_0012,
	STR_RAWLOADERWINDOW_0013,
	STR_RAWLOADERWINDOW_0014,
	STR_RAWLOADERWINDOW_0015,
	STR_RAWLOADERWINDOW_0016,
	STR_RAWLOADERWINDOW_0017,
	STR_RAWLOADERWINDOW_0018,
	STR_RAWLOADERWINDOW_0019,
	STR_RAWLOADERWINDOW_0020,
	STR_RAWLOADERWINDOW_0021,
	STR_RAWLOADERWINDOW_0023,
	STR_RAWLOADERWINDOW_0024,
	STR_RAWLOADERWINDOW_0025,
	STR_RAWLOADERWINDOW_0026,
	STR_RAWLOADERWINDOW_0027,

	STR_MAINWINDOW_0001,
	STR_MAINWINDOW_0002,
	STR_MAINWINDOW_0003,
	STR_MAINWINDOW_0004,
	STR_MAINWINDOW_0005,
	STR_MAINWINDOW_0006,
	STR_MAINWINDOW_0007,
	STR_MAINWINDOW_0008,
	STR_MAINWINDOW_0009,
	STR_MAINWINDOW_0010,
	STR_MAINWINDOW_0011,
	STR_MAINWINDOW_0012,
	STR_MAINWINDOW_0013,
	STR_MAINWINDOW_0014,
	STR_MAINWINDOW_0015,
	STR_MAINWINDOW_0016,
	STR_MAINWINDOW_0017,
	STR_MAINWINDOW_0018,
	STR_MAINWINDOW_0019,
	STR_MAINWINDOW_0020

};

const char * getString(unsigned int str_id);

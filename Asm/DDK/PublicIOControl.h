#ifndef __PUBLIC_IO_CONTROL_H__
#define __PUBLIC_IO_CONTROL_H__

#include <ntddk.h>

#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#define MAX_PATH		260


#define MYDEVICE_TYPE 40000
#define IOCTL_MYDEVICE_GET_CR0 \
    CTL_CODE( MYDEVICE_TYPE, 0x900, METHOD_BUFFERED, FILE_READ_ACCESS )
	
#define IOCTL_ENUM_DIRECTORY \
    CTL_CODE( MYDEVICE_TYPE, 0x901, METHOD_BUFFERED, FILE_READ_ACCESS )
	
/*
#define IOCTL_GPD_READ_PORT_USHORT \
    CTL_CODE( GPD_TYPE, 0x901, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_GPD_READ_PORT_ULONG \
    CTL_CODE( GPD_TYPE, 0x902, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_GPD_WRITE_PORT_UCHAR \
    CTL_CODE(GPD_TYPE,  0x910, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_GPD_WRITE_PORT_USHORT \
    CTL_CODE(GPD_TYPE,  0x911, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_GPD_WRITE_PORT_ULONG \
    CTL_CODE(GPD_TYPE,  0x912, METHOD_BUFFERED, FILE_WRITE_ACCESS)
*/

typedef enum//command types:
{	list,
	searchForFiltr
};
typedef struct TDrvInp
{
	UCHAR	cmnd;
	UCHAR	maxOutptItmCnt;
	WCHAR	path [MAX_PATH];
	WCHAR	filtr[MAX_PATH];
	UNICODE_STRING pathStr;
	UNICODE_STRING filtrStr;
	PULONG	pIOBuffer;            // Pointer to transfer buffer
    ULONG   InBufferSize;         // Amount of data avail. from caller.
    ULONG   OutBufferSize;        // Max data that caller can accept.
}TDrvInp,*PTDrvInp;

typedef struct _FILE_BOTH_DIR_INFORMATION
{
	ULONG NextEntryOffset;
	ULONG FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG FileAttributes;
	ULONG FileNameLength;
	ULONG EaSize;
	CCHAR ShortNameLength;
	WCHAR ShortName[12];
	WCHAR FileName[1];
} FILE_BOTH_DIR_INFORMATION, *PFILE_BOTH_DIR_INFORMATION;

#endif	
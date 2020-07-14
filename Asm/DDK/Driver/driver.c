#include "..\PublicIOControl.h"

#define DEBUG

#ifdef DEBUG
  #define DPRINT DbgPrint
#else
  #define DPRINT
#endif

UNICODE_STRING	DeviceName;
UNICODE_STRING	SymbolicLinkName;
PDEVICE_OBJECT	deviceObject = NULL;
TDrvInp			Inp;

NTSTATUS DispatchInpCmnd();
NTSTATUS ListDirectoryFiles();

#pragma pack (push, 1)


void CopyInp(TDrvInp* k, TDrvInp* i)
{
	k->cmnd = i->cmnd;
	k->maxOutptItmCnt = i->maxOutptItmCnt;
	RtlInitUnicodeString(&k->pathStr,i->path);
	RtlCopyMemory(k->path,i->path,k->pathStr.Length);
}

VOID DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
	DPRINT("Driver unloaded");

	IoDeleteSymbolicLink(&SymbolicLinkName); // удал€ем символическую ссылку
    IoDeleteDevice(deviceObject);            // удал€ем устройство

	return;
}

NTSTATUS DriverDispatcher(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp)
{
	PIO_STACK_LOCATION pisl;
	NTSTATUS ns = STATUS_SUCCESS;
	
	
    pisl = IoGetCurrentIrpStackLocation(Irp);
    Irp->IoStatus.Information = 0;
   
	__try
	{	switch (pisl->MajorFunction)
		{	
			case IRP_MJ_CREATE:
			case IRP_MJ_CLOSE:
				// We don't need any special processing on open/close so we'll
				// just return success.
				ns = STATUS_SUCCESS;
            break;
			case IRP_MJ_DEVICE_CONTROL:
              //  Dispatch on IOCTL
              switch (pisl->Parameters.DeviceIoControl.IoControlCode)
              {   case IOCTL_ENUM_DIRECTORY:
					
					// Size of buffer containing data from application
					Inp.InBufferSize  = pisl->Parameters.DeviceIoControl.InputBufferLength;
									    
					// Size of buffer for data to be sent to application
					Inp.OutBufferSize = pisl->Parameters.DeviceIoControl.OutputBufferLength;
					
					// Check to ensure input buffer is big enough to hold a port number and
					// the output buffer is at least as big as the port data width.
					//
					//DataBufferSize = sizeof(ULONG);
					//if ( InBufferSize != sizeof(ULONG) || inp.OutBufferSize < DataBufferSize )
					//{
					//	STATUS_INVALID_PARAMETER;
					//}
					
					Inp.pIOBuffer = (PULONG)Irp->AssociatedIrp.SystemBuffer;
					CopyInp(&Inp, (TDrvInp*)Inp.pIOBuffer);

					DispatchInpCmnd();
					//*inp.pIOBuffer = ++testGlVar;

					Irp->IoStatus.Information = sizeof(ULONG);
					ns = STATUS_SUCCESS;
	              break;
	              default:      
					ns = STATUS_INVALID_PARAMETER;
				  break;
              }
            break;
			default: 
				ns = STATUS_NOT_IMPLEMENTED;
            break;
	}	}

	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		Irp->IoStatus.Information = 0;
		ns = STATUS_IN_PAGE_ERROR;
    }
    Irp->IoStatus.Status = ns;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return ns;
}

NTSTATUS
DriverCreateClose(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
    PIO_STACK_LOCATION   irpStack;
    NTSTATUS             status = STATUS_SUCCESS;

    PAGED_CODE ();

    DPRINT(("DriverCreateClose Enter\n"));

    //
    // Get a pointer to the current location in the Irp.
    //

    irpStack = IoGetCurrentIrpStackLocation(Irp);

    switch(irpStack->MajorFunction)
    {
        case IRP_MJ_CREATE:

            DPRINT(("IRP_MJ_CREATE\n"));
            Irp->IoStatus.Information = 0xffff;
            break;

        case IRP_MJ_CLOSE:

            DPRINT(("IRP_MJ_CLOSE\n"));
            Irp->IoStatus.Information = 0;
            break;

        default:
            DPRINT((" Invalid CreateClose Parameter\n"));
            status = STATUS_INVALID_PARAMETER;
            break;
    }

    Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    DPRINT((" CsampCreateClose Exit."));

    return status;
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject,
                     IN PUNICODE_STRING RegistryPath)
{
	NTSTATUS st;
	PCWSTR dDeviceName       = L"\\Device\\SinoKrnl";
    PCWSTR dSymbolicLinkName = L"\\DosDevices\\SinoKrnl";
	PDRIVER_DISPATCH *ppdd;

	DPRINT("Driver loaded");
    
    RtlInitUnicodeString(&DeviceName,       dDeviceName);
    RtlInitUnicodeString(&SymbolicLinkName, dSymbolicLinkName);
    
    st = IoCreateDevice(DriverObject,    // указатель на DriverObject 
		                256,             // максимальный размер принимаемых данных  
		                &DeviceName,     // им€ создаваемого устройства
                        MYDEVICE_TYPE,   // тип создаваемого устройства
						METHOD_NEITHER,  // метод буфферизации
                        FALSE,           // "эксклюзивное" устройство
						&deviceObject);  // указатель на обьект устройства
    
    
    if (st == STATUS_SUCCESS)
      st = IoCreateSymbolicLink(&SymbolicLinkName, // им€ создаваемой символической ссылки
	                            &DeviceName);      // им€ устройства 
   
	ppdd = DriverObject->MajorFunction;
            
	// обь€вл€ю процедуры обработки ввода-вывода
    ppdd [IRP_MJ_CREATE] =
    ppdd [IRP_MJ_CLOSE ] = DriverCreateClose;
	ppdd [IRP_MJ_DEVICE_CONTROL] = DriverDispatcher;

	DriverObject->DriverUnload = DriverUnload;

    return st;
}

NTSTATUS DispatchInpCmnd()
{
	switch(Inp.cmnd)
	{	default: case list:
			return ListDirectoryFiles();
		break;
		case searchForFiltr:
		break;
}	}

NTSTATUS ListDirectoryFiles()
{
UNICODE_STRING RootDirectoryName;
ANSI_STRING as;
OBJECT_ATTRIBUTES RootDirectoryAttributes;
NTSTATUS Status;
HANDLE RootDirectoryHandle;
IO_STATUS_BLOCK Iosb;
HANDLE Event;
PUCHAR Buffer[65536];
PFILE_BOTH_DIR_INFORMATION DirInformation;

	InitializeObjectAttributes(&RootDirectoryAttributes, &RootDirectoryName, OBJ_CASE_INSENSITIVE, 0, 0);
	if(ZwCreateFile(&RootDirectoryHandle,
					GENERIC_READ,
					&RootDirectoryAttributes,
					&Iosb,
					0,
					FILE_ATTRIBUTE_DIRECTORY,
					FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
					FILE_OPEN,
					FILE_DIRECTORY_FILE,
					0, 0) != STATUS_SUCCESS)
	{
		//printf("Unable to open %s, error = 0x%x\n", &RootDirectoryName, Status);
		return Status;
	}

	//if(NtCreateEvent(&Event, GENERIC_ALL, 0, NotificationEvent, FALSE))!= STATUS_SUCCESS)
	{
		//printf("Event creation failed with error 0x%x\n", Status);
	//	return Status;
	}

	if(ZwQueryInformationFile(RootDirectoryHandle,
							&Iosb,
							FileBothDirectoryInformation,
							FALSE,
							NULL,
							FALSE)) == STATUS_PENDING)
	{
		Status = NtWaitForSingleObject(Event, TRUE, 0);
	}

	if (!NT_SUCCESS(Status))
	{
		//printf("Unable to query directory contents, error 0x%x\n", Status);
		return Status;
	}

	DirInformation = (PFILE_BOTH_DIR_INFORMATION)Buffer;

	while (1)
	{
		UNICODE_STRING EntryName;
		EntryName.MaximumLength = EntryName.Length = (USHORT) DirInformation -> FileNameLength;
		EntryName.Buffer = &DirInformation -> FileName[0];
		
		RtlUnicodeStringToAnsiString(&as, &EntryName, TRUE);
		//printf("%s\n", as.Buffer);
		
		if (0 == DirInformation -> NextEntryOffset)
		{
			break;
		}
		else
		{
			DirInformation = (PFILE_BOTH_DIR_INFORMATION) (((PUCHAR)DirInformation) + DirInformation -> NextEntryOffset);
		}
		
	}
	ZwClose(RootDirectoryHandle);
	return STATUS_SUCCESS;
}




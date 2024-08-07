#include <efi.h>
#include <efilib.h>

extern EFI_GUID GraphicsOutputProtocol;

static void
print_modes(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop)
{
	int i, imax;
	EFI_STATUS rc;

	if (gop->Mode) {
		imax = gop->Mode->MaxMode;
		Print(u"GOP reports MaxMode %d\n", imax);
	} else {
		Print(u"gop->Mode is NULL\n");
		imax = 1;
	}

	for (i = 0; i < imax; i++) {
		EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
		UINTN SizeOfInfo;
		rc = uefi_call_wrapper(gop->QueryMode, 4, gop, i, &SizeOfInfo,
					&info);
		if (rc == EFI_NOT_STARTED) {
			Print(u"gop->QueryMode() returned %r\n", rc);
			Print(u"Trying to start GOP with SetMode().\n");
			rc = uefi_call_wrapper(gop->SetMode, 2, gop,
				gop->Mode ? gop->Mode->Mode : 0);
			rc = uefi_call_wrapper(gop->QueryMode, 4, gop, i,
				&SizeOfInfo, &info);
		}

		if (EFI_ERROR(rc)) {
			Print(u"%d: Bad response from QueryMode: %r (%d)\n",
			      i, rc, rc);
			continue;
		}
		Print(u"%c%d: %dx%d ",
		      (gop->Mode &&
		       CompareMem(info,gop->Mode->Info,sizeof(*info)) == 0
		       ) ? '*' : ' ',
		      i, info->HorizontalResolution, info->VerticalResolution);
		switch(info->PixelFormat) {
			case PixelRedGreenBlueReserved8BitPerColor:
				Print(u"RGBR");
				break;
			case PixelBlueGreenRedReserved8BitPerColor:
				Print(u"BGRR");
				break;
			case PixelBitMask:
				Print(u"R:%08x G:%08x B:%08x X:%08x",
					info->PixelInformation.RedMask,
					info->PixelInformation.GreenMask,
					info->PixelInformation.BlueMask,
					info->PixelInformation.ReservedMask);
				break;
			case PixelBltOnly:
				Print(u"(blt only)");
				break;
			default:
				Print(u"(Invalid pixel format)");
				break;
		}
		Print(u" pitch %d\n", info->PixelsPerScanLine);
	}
}

static EFI_STATUS
SetWatchdog(UINTN seconds)
{
	EFI_STATUS rc;
	rc = uefi_call_wrapper(BS->SetWatchdogTimer, 4, seconds, 0x1ffff,
				0, NULL);
	if (EFI_ERROR(rc)) {
		CHAR16 Buffer[64];
		StatusToString(Buffer, rc);
		Print(u"Bad response from QueryMode: %s (%d)\n", Buffer, rc);
	}
	return rc;
}

EFI_STATUS
efi_main (EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *systab)
{
	EFI_STATUS rc;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

	InitializeLib(image_handle, systab);

	SetWatchdog(10);

	rc = LibLocateProtocol(&GraphicsOutputProtocol, (void **)&gop);
	if (EFI_ERROR(rc)) {
		Print(u"Could not locate GOP: %r\n", rc);
		return rc;
	}

	if (!gop) {
		Print(u"LocateProtocol(GOP, &gop) returned %r but GOP is NULL\n", rc);
		return EFI_UNSUPPORTED;
	}

	print_modes(gop);

	SetWatchdog(0);
	return EFI_SUCCESS;
}

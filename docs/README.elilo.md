
# IMPORTANT information related to the gnu-efi package
----------------------------------------------------
June 2001

As of version 3.0, the gnu-efi package is now split in two different packages:

- gnu-efi-X.y: contains the EFI library, include files and crt0.

- elilo-X.y  : contains the ELILO bootloader. 
	
Note that X.y don't need to match for both packages. However elilo-3.x
requires at least gnu-efi-3.0. EFI support for x86_64 is provided in
gnu-efi-3.0d.

Both packages can be downloaded from:

- [gnu-efi](https://github.com/ncroxon/gnu-efi)
- [elilo](https://sourceforge.net/projects/elilo/)

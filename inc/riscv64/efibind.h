/* SPDX-License-Identifier: GPL-2.0-or-later OR BSD-2-Clause */
/*
 * Copyright (C) 2014 - 2015 Linaro Ltd.
 * Author: Ard Biesheuvel <ard.biesheuvel@linaro.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice and this list of conditions, without modification.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 */

#include <stdint.h>
#include <stddef.h>

//
// Basic EFI types of various widths
//

typedef uint64_t                UINT64;
typedef int64_t                 INT64;
typedef uint32_t                UINT32;
typedef int32_t                 INT32;
typedef uint16_t                UINT16;
typedef int16_t                 INT16;
typedef uint8_t                 UINT8;
typedef int8_t                  INT8;
typedef unsigned char                    CHAR8;
typedef uint16_t                 CHAR16;
#define WCHAR                   CHAR16
#undef VOID
typedef void                    VOID;
typedef int64_t                 INTN;
typedef uint64_t                UINTN;

#define EFI_ERROR_MASK          0x8000000000000000
#define EFIERR(a)               (EFI_ERROR_MASK | a)
#define EFIERR_OEM(a)           (0xc000000000000000 | a)

#define BAD_POINTER             0xFBFBFBFBFBFBFBFB
#define MAX_ADDRESS             0xFFFFFFFFFFFFFFFF

#define BREAKPOINT()            while(1);

//
// Pointers must be aligned to these address to function
//
#define MIN_ALIGNMENT_SIZE      8

#define ALIGN_VARIABLE(Value, Adjustment) \
   (UINTN)Adjustment = 0; \
   if((UINTN)Value % MIN_ALIGNMENT_SIZE) \
       (UINTN)Adjustment = MIN_ALIGNMENT_SIZE - ((UINTN)Value % MIN_ALIGNMENT_SIZE); \
   Value = (UINTN)Value + (UINTN)Adjustment

//
// Define macros to build data structure signatures from characters.
//
#define EFI_SIGNATURE_16(A,B)             ((A) | (B<<8))
#define EFI_SIGNATURE_32(A,B,C,D)         (EFI_SIGNATURE_16(A,B)     | (EFI_SIGNATURE_16(C,D)     << 16))
#define EFI_SIGNATURE_64(A,B,C,D,E,F,G,H) (EFI_SIGNATURE_32(A,B,C,D) | ((UINT64)(EFI_SIGNATURE_32(E,F,G,H)) << 32))

//
// EFIAPI - prototype calling convention for EFI function pointers
// BOOTSERVICE - prototype for implementation of a boot service interface
// RUNTIMESERVICE - prototype for implementation of a runtime service interface
// RUNTIMEFUNCTION - prototype for implementation of a runtime function that is not a service
// RUNTIME_CODE - pragma macro for declaring runtime code
//
#ifndef EFIAPI                  // Forces EFI calling conventions reguardless of compiler options
#define EFIAPI                  // Substitute expresion to force C calling convention
#endif
#define BOOTSERVICE
#define RUNTIMESERVICE
#define RUNTIMEFUNCTION
#define RUNTIME_CODE(a)         alloc_text("rtcode", a)
#define BEGIN_RUNTIME_DATA()    data_seg("rtdata")
#define END_RUNTIME_DATA()      data_seg("")

#define VOLATILE                volatile
#define MEMORY_FENCE            __sync_synchronize

//
// When build similiar to FW, then link everything together as
// one big module. For the MSVC toolchain, we simply tell the
// linker what our driver init function is using /ENTRY.
//
#if defined(_MSC_EXTENSIONS)
#define EFI_DRIVER_ENTRY_POINT(InitFunction) \
    __pragma(comment(linker, "/ENTRY:" # InitFunction))
#else
#define EFI_DRIVER_ENTRY_POINT(InitFunction)    \
    UINTN                                       \
    InitializeDriver (                          \
        VOID    *ImageHandle,                   \
        VOID    *SystemTable                    \
        )                                       \
    {                                           \
        return InitFunction(ImageHandle,        \
                SystemTable);                   \
    }                                           \
                                                \
    EFI_STATUS efi_main(                        \
        EFI_HANDLE image,                       \
        EFI_SYSTEM_TABLE *systab                \
        ) __attribute__((weak,                  \
                alias ("InitializeDriver")));
#endif

#define LOAD_INTERNAL_DRIVER(_if, type, name, entry) \
   (_if)->LoadInternal(type, name, entry)

//
// Some compilers don't support the forward reference construct:
//  typedef struct XXXXX
//
// The following macro provide a workaround for such cases.
#define INTERFACE_DECL(x)       struct x

#define uefi_call_wrapper(func, va_num, ...) (func)(__VA_ARGS__)
#define EFI_FUNCTION

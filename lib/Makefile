#
#  Copyright (C) 1999-2001 Hewlett-Packard Co.
#	Contributed by David Mosberger <davidm@hpl.hp.com>
#	Contributed by Stephane Eranian <eranian@hpl.hp.com>
#
#    All rights reserved.
#
#    Redistribution and use in source and binary forms, with or without
#    modification, are permitted provided that the following conditions
#    are met:
#
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following
#      disclaimer in the documentation and/or other materials
#      provided with the distribution.
#    * Neither the name of Hewlett-Packard Co. nor the names of its
#      contributors may be used to endorse or promote products derived
#      from this software without specific prior written permission.
#
#    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
#    CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
#    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
#    BE LIABLE FOR ANYDIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
#    OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
#    TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
#    THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
#    SUCH DAMAGE.
#

VPATH ?= .
SRCDIR = $(VPATH)


include $(SRCDIR)/../Make.defaults

FILES = boxdraw smbios console crc data debug dpath \
        entry error event exit guid hand hw init lock \
        misc pause sread str cmdline runtime/rtlock \
        runtime/efirtlib runtime/rtstr runtime/vm runtime/rtdata \
        $(ARCH)/initplat $(ARCH)/math $(ARCH)/setjmp \
        ctors print ms_va_print va_print

ifeq ($(ARCH),ia64)
FILES += $(ARCH)/salpal $(ARCH)/palproc
endif

ifeq ($(ARCH),x86_64)
FILES += $(ARCH)/callwrap $(ARCH)/efi_stub
endif

ifeq ($(ARCH),arm)
FILES += $(ARCH)/uldiv $(ARCH)/ldivmod $(ARCH)/div $(ARCH)/llsl $(ARCH)/llsr \
	 $(ARCH)/mullu
endif

OBJS  = $(FILES:%=%.o)

SUBDIRS = ia32 x86_64 ia64 aarch64 arm mips64el riscv64 loongarch64 runtime

all: libsubdirs libefi.a

.PHONY: libsubdirs
libsubdirs:
	@set -e ; for sdir in $(SUBDIRS); do mkdir -p $$sdir; done

$(OBJS): libsubdirs

libefi.a: $(OBJS)
	@$(ECHO) "  AR       $(notdir $@)"
	$(HIDE)$(AR) $(ARFLAGS) $@ $^

$(SRCDIR)/ms_va_print.c:
	@$(ECHO) "  CP       $(notdir ms_va_print.c)"
	$(HIDE)cp $(SRCDIR)/va_print.c $(SRCDIR)/ms_va_print.c

ms_va_print.o: EFI_CFLAGS += -Wno-error=unused-function -D__GNU_EFI_INTERNAL_USE_MS_VA_ABI

va_print.o: EFI_CFLAGS += -Wno-error=unused-function

print.o: EFI_CFLAGS += -Wno-error=unused-function

clean:
	@rm -vf libefi.a *~ $(OBJS) */*.o $(SRCDIR)/ms_va_print.c

install:
	mkdir -p $(INSTALLROOT)$(LIBDIR)
	$(INSTALL) -m 644 libefi.a $(INSTALLROOT)$(LIBDIR)

include $(SRCDIR)/../Make.rules

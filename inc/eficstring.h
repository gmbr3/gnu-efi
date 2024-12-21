#ifndef _EFI_CSTRING


UINTN
AsciiStrLen (
    IN CONST CHAR8    *s1
    );

UINTN
AsciiStrCmp (
    IN CONST CHAR8    *s1,
    IN CONST CHAR8    *s2
    );

UINTN
AsciiStrnCmp (
    IN CONST CHAR8    *s1,
    IN CONST CHAR8    *s2,
    IN UINTN          len
    );

//
// For compatibility with previous gnu-efi versions
//
#define strlena AsciiStrLen
#define strcmpa AsciiStrCmp
#define strncmpa AsciiStrnCmp

/* C standard functions */

STATIC inline size_t efi_c_strlen(const char *s) {
    return (size_t)(AsciiStrLen((CONST CHAR8*)s));
}

STATIC inline int efi_c_strcmp(const char *s1, const char *s2) {
    return (int)AsciiStrCmp((CONST CHAR8*)s1, (CONST CHAR8*)s2);
}


STATIC inline int efi_c_strncmp(const char *s1, const char *s2, size_t n) {
    return (int)(AsciiStrnCmp((CONST CHAR8*)s1, (CONST CHAR8*)s2, (UINTN)n));
}

#endif

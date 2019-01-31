
#pragma once


#include <cstdio>
#include <windows.h>
#include <set>

#ifdef _WIN64
#define ADDRESSFORMATW   L"0x%.16X" // Format string for 64-bit addresses
#define ADDRESSFORMATA   "0x%.16X" // Format string for 64-bit addresses
#else
#define ADDRESSFORMATW   L"0x%.8X"  // Format string for 32-bit addresses
#define ADDRESSFORMATA   "0x%.8X"  // Format string for 32-bit addresses
#endif // _WIN64
#define MAXREPORTLENGTH 511        // Maximum length, in characters, of "report" messages.

// Architecture-specific definitions for x86 and x64
#if defined(_M_IX86)
#define SIZEOFPTR 4
#define X86X64ARCHITECTURE IMAGE_FILE_MACHINE_I386
#define AXREG Eax
#define BPREG Ebp
#define IPREG Eip
#define SPREG Esp
#elif defined(_M_X64)
#define SIZEOFPTR 8
#define X86X64ARCHITECTURE IMAGE_FILE_MACHINE_AMD64
#define AXREG Rax
#define BPREG Rbp
#define IPREG Rip
#define SPREG Rsp
#endif // _M_IX86

#if defined(_M_IX86) || defined (_M_X64)
#define FRAMEPOINTER(fp) __asm mov fp, BPREG // Copies the current frame pointer to the supplied variable.
#else
// If you want to retarget Visual Leak Detector to another processor
// architecture then you'll need to provide an architecture-specific macro to
// obtain the frame pointer (or other address) which can be used to obtain the
// return address and stack pointer of the calling frame.
#error "Visual Leak Detector is not supported on this architecture."
#endif // _M_IX86 || _M_X64


// This structure stores information, primarily the virtual address range, about
// a given module and can be used with the Set template because it supports the
// '<' operator (sorts by virtual address range).
typedef struct moduleinfo_s {
    BOOL operator < (const struct moduleinfo_s &other) const
    {
        if (addrhigh < other.addrlow) {
            return TRUE;
        }
        else {
            return FALSE;
        }
    }

    SIZE_T addrhigh;                 // Highest address within the module's virtual address space (i.e. base + size).
    SIZE_T addrlow;                  // Lowest address within the module's virtual address space (i.e. base address).
    UINT32 flags;                    // Module flags:
#define VLD_MODULE_EXCLUDED      0x1 //   If set, this module is excluded from leak detection.
#define VLD_MODULE_SYMBOLSLOADED 0x2 //   If set, this module's debug symbols have been loaded.
    LPCSTR name;                     // The module's name (e.g. "kernel32.dll").
    LPCSTR path;                     // The fully qualified path from where the module was loaded.
} moduleinfo_t;

// ModuleSets store information about modules loaded in the process.
typedef std::set<moduleinfo_t> ModuleSet;


// This structure allows us to build a table of APIs which should be patched
// through to replacement functions provided by VLD.
typedef struct patchentry_s
{
    LPCSTR  exportmodulename; // The name of the module exporting the patched API.
    LPCSTR  importname;       // The name (or ordinal) of the imported API being patched.
    SIZE_T  modulebase;       // The base address of the exporting module (filled in at runtime when the modules are loaded).
    LPCVOID replacement;      // Pointer to the function to which the imported API should be patched through to.
} patchentry_t;



VOID report (LPCWSTR format, ...);
VOID report (LPCSTR format, ...);
VOID strapp (LPWSTR *dest, LPCWSTR source);
VOID strapp (LPSTR *dest, LPCSTR source);

BOOL CALLBACK addloadedmodule ( PSTR modulepath, DWORD64 modulebase, ULONG modulesize, PVOID context );
VOID CALLBACK attachtoloadedmodules ( ModuleSet *newmodules );

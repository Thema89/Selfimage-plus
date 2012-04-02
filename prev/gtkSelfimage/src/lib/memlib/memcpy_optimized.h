//------------------------------ $Keywords ----------------------------------
// Fast Memory Operations Library
// memcpy.h - Optimized memory copy, set, and zero functions header
//---------------------------------------------------------------------------
#ifndef memcpy_optimizedH
#define memcpy_optimizedH
//---------------------------------------------------------------------------
void * __stdcall memcpy_optimized(void *dest, const void *src, size_t n);
void * __stdcall memset_optimized(void *dest, int c, size_t n);
void __stdcall   memzero_optimized(void *dest, size_t n);
unsigned long get_cpu_type();
//---------------------------------------------------------------------------
// Borland TASM doesn't support a lot of new instructions, so we have some
// macros that emulate them
__asm INCLUDE memlib/sse3.asi
//---------------------------------------------------------------------------
// Because we don't have a good alignment method, we have this macro.
// Limitations in TASM require that the current location counter only be
// referenced as a relative value ($ - SomeOtherPoint).  To use this macro,
// CodeAlignStart must refer to an address that is already aligned with the
// same or greater granularity than you will ever call the macro for. The
// actual built-in ALIGN statement only aligns relative to the entry point of
// a function. This means that each function's start must be hand aligned with
// NOPs. Once this is done, though, the rest of the CALIGNs will work.
__asm {
        CALIGN macro c
          while (($ - @@CodeAlignStart) and (&&c-1))
            nop
          endm
        endm
}  // __asm
#endif

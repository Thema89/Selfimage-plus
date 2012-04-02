#ifndef STDCALL_H_
#define STDCALL_H_

// Define __stdcall and __fastcall etc

#ifdef __i386__
# if defined(__GNUC__) && ((__GNUC__ > 2) || ((__GNUC__ == 2) && (__GNUC_MINOR__ >= 7)))
#  ifndef _EGCS_ 
#define __stdcall __attribute__((__stdcall__))
#define __cdecl   __attribute__((__cdecl__))
#  define __RESTORE_ES  __asm__ __volatile__("pushl %ds\n\tpopl %es")
#  endif
# else
// #  error You need gcc >= 2.7 to build Wine on a 386
# endif
#else 
# define __stdcall
# define __cdecl
# define __RESTORE_ES
#endif

#if !defined(__CYGWIN__) && !defined(__MINGW32__)
#define _stdcall    __stdcall
#define _fastcall   __stdcall
#define __fastcall  __stdcall
#endif

#endif //STDCALL_H_


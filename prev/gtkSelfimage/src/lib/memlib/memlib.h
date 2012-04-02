//------------------------------ $Keywords ----------------------------------
// Fast Memory Operations Library
// memlib.h - Library header
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// VCS: $Version: 0 $ $Revision: 1 $
/*
$History: **** Latest ** V 0.1 by kfitzner ** 2005-12-04 10:50:40 AM ****
$History: * Initial check-in
*/
//----------------------------  $NoKeywords ---------------------------------
#ifndef memlibH
#define memlibH
//---------------------------------------------------------------------------
//#ifndef NO_ASM_MEMCPY
//void * __stdcall memcpy_optimized(void *dest, const void *src, size_t n);
//void * __stdcall memset_optimized(void *dest, int c, size_t n);
//void __stdcall   memzero_optimized(void *dest, size_t n);
//unsigned long get_cpu_type();
//#else
#define memcpy_optimized(d,s,n) memcpy(d,s,n)
#define memset_optimized(d,c,n) memset(d,c,n)
#define memzero_optimized(d,n) memset(d,0,n)
//#endif
//---------------------------------------------------------------------------
#endif

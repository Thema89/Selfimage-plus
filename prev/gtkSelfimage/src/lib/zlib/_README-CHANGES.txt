zlib Version 1.2.3
------------------

The files here represent Jean-loup Gailly and Mark Adler's zlib library.  Only
those files needed for SelfImage are included.

The files included are functionally unchanged.  They would be completely
unchanged except for three modifications needed in order to get them to compile
and link correctly in C++ Builder 6.0.  The changes are marked in the affected
source files, and noted below.

zconf.h
-------
Line 270 originally read:
   typedef Byte  FAR Bytef;
and was changed to:
   typedef ::Byte  FAR Bytef;

This circumvents the compiler error: "[C++ Error] zconf.h(270): E2015
Ambiguity between 'Byte' and 'System::Byte'".


deflate.c
---------
Lines 88-89 originally read:
      void match_init OF((void)); /* asm code initialization */
      uInt longest_match  OF((deflate_state *s, IPos cur_match));
and were changed to:
      void __cdecl match_init OF((void)); /* asm code initialization */
      uInt __cdecl longest_match  OF((deflate_state *s, IPos cur_match));

zlib has some assembly-language optimisations that ship with it.  Because the
"official" calling convention for zlib is the 'C' calling convention (cdecl),
and because SelfImage uses fastcall calling conventions, the declarations for
the assembly routines needed to be changed.


inffas86.c
-----------
This file is included with zlib in the "contrib" tree.  It's an assembly
language optimized inflate.  It required two small changes to enable it to
compile:

Line 28 added (was a blank line so line numbers below aren't changed)
  #pragma inline

Line 797 originally read:
  #elif defined( _MSC_VER ) && ! defined( _M_AMD64 )
and was changed to read:
  #elif ( defined( __BORLANDC__ ) || defined( _MSC_VER ) ) && ! defined( _M_AMD64 )
  
The first change, the added #pragma, is to tell Borland C++ Builder to use the
external Turbo Assembler instead of its internal built-in assembler.  This is
necesary to support the ALIGN directive.

The second change was because the original author's conditional compiliation
directive only allowed Microsoft compilers to use the code.
SelfImage Source Tree Copyright Information
-------------------------------------------
SelfImage is copyright ©2005, Kurt Fitzner <kfitzner@excelcia.org>.  It is
licensed and released under under the terms of version 2 of the GNU General
Public License.  The full text of this license should have accompanied this
software.


Change in Copyright Licensing Terms
-----------------------------------
Please note the version of the GPL that SelfImage is released under.  Most GPL-
licensed programs allow you the discretion to choose to abide by the current
terms of the GPL or any later version (if/when later versions are published)
of the GPL.  This was the case for the initial release (0.1.0 build 2).  After
this, however, the terms were changed to limit the GPL license to version 2.
In other words, you may NOT choose the terms of a later version of the GPL
(if/when one is published) instead of version 2 for SelfImage.  If/when a
later version is published, the decision will be made at that time as to
whether or not to release of SelfImage under the new license.


Copyright Terms for SelfImage's Components
------------------------------------------
While SelfImage is, as a whole, copyright and licensed as noted above, there
are some files in it which are copyright by other authors.  The following
lists the different components that make up SelfImage and their copyright:


Code written by the author:
---------------------------
Those source files that have code written by the author are marked
with a copyright notice as shown in the below example:

 //---------------------------------------------------------------------------
 // SelfImage - Disk image manipulation program
 // SelfImage.cpp - Project source - framework for the program
 // Copyright ©2005, Kurt Fitzner <kfitzner@excelcia.org>
 //---------------------------------------------------------------------------
 // This file is part of SelfImage.
 //
 // SelfImage is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License (Version 2) as
 // published by the Free Software Foundation.
 //
 // SelfImage is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 // GNU General Public License for more details.
 //
 // You should have received a copy of the GNU General Public License
 // along with this program; if not, write to the Free Software
 // Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 //---------------------------------------------------------------------------


Bitstream Vera Sans Mono Fonts
------------------------------
Bitstream kindly has donated their "Vera" fonts to be used under an open
source license.  These fonts are included in both the source and binary
versions of SelfImage (they are used to ensure a common look to SelfImage across
multiple versions of Windows, or when run through Wine or other emulators).

These fonts are freely redistributable and are licensed under a BSD-ish style
license.  The license is included inside each font with their license field.
In case you have difficulty viewing this, the license is reproduced in the
install directory (in the source code tree) in the file "VeraFontLicense.txt".


ZLIB Library
------------
SelfImage makes use of the zlib compression library, which is copyright (C)
1995-2005 Jean-loup Gailly and Mark Adler.  The full licensing terms are
in the file "zlib.h" which is located in the lib/zlib/ directory in SelfImage's
source tree.

The library is functionally unchanged, but required some very minor adjustments
in order for it to compile properly.  These changes are marked in the source
files, and in the file _README-CHANGES.txt that is also located in the lib/zlib/
directory in the source tree.


LIBBZIP2 Library
----------------
SelfImage makes use of the libbzip2 library found in the bzip2 version 1.0.3
distribution.  The copyright for it is as follows:

  This program, "bzip2", the associated library "libbzip2", and all
  documentation, are copyright (C) 1996-2005 Julian R Seward.  All
  rights reserved.
  
The full licensing terms for it are available in the file "LICENSE.txt" which
is located in the lib/libbz2/ directory in the SelfImage source tree.


FastMM MEMORY MANAGER
---------------------
SelfImage includes the FastMM (Fast Memory Manager) library, which is
Copyright (C) 2004-2005 Professional Software Development / Pierre le Riche.
This library was originally released under the Mozilla Public License, which
is incompatible with the GPL that SelfImage is released under.  When asked,
Mr. le Riche very kindly agreed to release FastMM under a dual MPL/LGPL
license that would allow SelfImage to use it.

As SelfImage links the FastMM code in statically, the LGPL's GPL conversion
clause (paragraph 3 of the LGPL) is invoked, and FastMM is distributed with
SelfImage under the terms of the GNU GPL.  Therefore, the FastMM that is
distributed with SelfImage is licensed solely under the terms of the GNU
General Public License.

If you wish to obtain a copy under the original MPL/LGPL terms, please visit
http://fastmm.sourceforge.net


MEMLIB
------
Memlib is a compilation of code from several sources:

 memcpy_optimized.cpp (originally memcpy_amd.cpp) is copyright (C) 2001,
 Advanced Micro Devices, Inc.  It has been heavily modified by "katsyonak",
 a developer with the eMule Plus project.  Presumably the modifications are
 at least released under the GPL as that is the license for eMule Plus.  It
 has also been modified for use as a small library by Kurt Fitzner
 <kfitzner@excelcia.org> who's contributions to that file are public domain.
 
 sse3.asi is copyright (C) 2005, Agner Fog, and is released under the terms
 of the GPL.
 

All Other Files
---------------
Unless otherwise noted above or within the individual file, all other files
included in the SelfImage source distribution are copyright ©2005, Kurt Fitzner.


Licensing Requests
------------------
If you want to use code from SelfImage in a project with a different FOSS
license, please feel free to contact the author (kfitzner@excelcia.org).
In general, if the license has similar terms to the GPL where modifications
are requried to be made available as FOSS, permission will most likely be
given.  If, however, the license is one that allows use by proprietary
software projects, then permission will not likely be forthcoming.

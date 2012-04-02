//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_TypesDefs.h - SelfImage internal types and useful defines
// Copyright ©2005-2007, Kurt Fitzner <kfitzner@excelcia.org>
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

//---------------------------------------------------------------------------
// VCS: $Version: 1 $ $Revision: 3 $
/*
$History: **** V 1.0 by kfitzner ****
$History: * selfimage_typesdefs.h - 2005-11-26 7:01:36 PM - 2196 Bytes
$History: * Initial check-in
$History: **** V 1.1 by kfitzner ****
$History: * selfimage_typesdefs.h - 2005-12-08 9:39:09 AM - 2805 Bytes
$History: * Add externs for SelfImage's globals
$History: **** V 1.2 by kfitzner ****
$History: * selfimage_typesdefs.h - 2005-12-09 2:20:54 PM - 3012 Bytes
$History: * Add TImageStoreType
$History: **** Latest ** V 1.3 by kfitzner ** 2007-03-14 1:48:36 PM ****
$History: * Rename "block" to "chunk" for clarity
*/
//----------------------------  $NoKeywords ---------------------------------
#ifndef SelfImage_TypesDefsH
#define SelfImage_TypesDefsH
//---------------------------------------------------------------------------
#include "TConfiguration.h"
#include "TProgramLog.h"
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Globals
//
extern TConfiguration *SelfImageConfig;
extern TProgramLog    *Log;
extern AnsiString      sBinDir;
extern AnsiString      sVersionString;
extern int             nBuildNumber;
extern bool            HaveNTCalls;


//---------------------------------------------------------------------------
// Types that are needed globally (or at least often)
//
enum TCompressionFormat { cfNone, cfGzipStream, cfGzipChunk, cfBzip2Stream, cfBzip2Chunk };
enum TImageStoreType { isUndefined, isFile, isDrive, isNBD };

//---------------------------------------------------------------------------
// Access the individual DWORDS inside a 64-bit integer
//
#ifndef LOW_DWORD
#define LOW_DWORD(x)  ((*(LARGE_INTEGER *)&x).LowPart)
#endif
#ifndef HIGH_DWORD
#define HIGH_DWORD(x) ((*(LARGE_INTEGER *)&x).HighPart)
#endif

//---------------------------------------------------------------------------
#endif

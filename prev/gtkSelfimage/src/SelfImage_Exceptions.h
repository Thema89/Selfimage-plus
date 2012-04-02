//------------------------------ $Keywords ----------------------------------
// SelfImage - Windows disk image backup
// SelfImage_Exceptions.h - SelfImage-specific exceptions
// Copyright ©2005, Kurt Fitzner <kfitzner@excelcia.org>
//---------------------------------------------------------------------------
// This file is part of SelfImage.
//
// SelfImageis free software; you can redistribute it and/or modify
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
// VCS: $Version: 1 $ $Revision: 2 $
/*
$History: **** V 0.1 by kfitzner ****
$History: * selfimage_exceptions.h - 2005-07-31 8:30:11 AM - 3240 Bytes
$History: * Initial check-in
$History: **** V 0.2 by kfitzner ****
$History: * selfimage_exceptions.h - 2005-07-31 8:33:59 AM - 5285 Bytes
$History: * Add ESelfImageAllocError exception.
$History: **** V 0.3 by kfitzner ****
$History: * selfimage_exceptions.h - 2005-07-31 3:16:29 PM - 7363 Bytes
$History: * Add ESelfImageFileError
$History: **** V 0.4 by kfitzner ****
$History: * selfimage_exceptions.h - 2005-08-17 7:51:34 AM - 7487 Bytes
$History: * Change licensing - only version 2 of the GPL, no later versions
$History: **** V 0.5 by kfitzner ****
$History: * selfimage_exceptions.h - 2005-11-10 9:42:16 AM - 9611 Bytes
$History: * Add ESelfImageSystemError
$History: **** V 1.0 by kfitzner ****
$History: * selfimage_exceptions.h - 2005-11-23 3:16:32 AM - 11696 Bytes
$History: * Add ESelfImageCompressionError
$History: **** V 1.1 by kfitzner ****
$History: * selfimage_exceptions.h - 2005-12-05 2:45:52 PM - 13790 Bytes
$History: * Add ESelfImageNetworkError
$History: **** Latest ** V 1.2 by kfitzner ** 2005-12-05 7:00:11 PM ****
$History: * Forgot to ensure the header can't be included twice
*/
using namespace std;

//----------------------------  $NoKeywords ---------------------------------
#ifndef SelfImage_ExceptionsH
#define SelfImage_ExceptionsH
//---------------------------------------------------------------------------
#include <exception>
#include "global.h"

#ifdef _WIN32
// General SelfImage error - anything that doesn't fit into the below classes
//
class ESelfImageError : public exception {
public:
#if 0
  __fastcall ESelfImageError(const string Msg);
  __fastcall ESelfImageError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size): Exception(Msg, Args, Args_Size) {};
  __fastcall ESelfImageError(int Ident): Exception(Ident) {};
  __fastcall ESelfImageError(System::PResStringRec ResStringRec): Exception(ResStringRec) {};
  __fastcall ESelfImageError(int Ident, const System::TVarRec * Args, const int Args_Size): Exception(Ident, Args, Args_Size) {};
  __fastcall ESelfImageError(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size): Exception(ResStringRec, Args, Args_Size) {};
  __fastcall ESelfImageError(const AnsiString Msg, int AHelpContext): Exception(Msg, AHelpContext) {};
  __fastcall ESelfImageError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(Msg, Args, Args_Size, AHelpContext) {};
  __fastcall ESelfImageError(int Ident, int AHelpContext): Exception(Ident, AHelpContext) {};
  __fastcall ESelfImageError(System::PResStringRec ResStringRec, int AHelpContext): Exception(ResStringRec, AHelpContext) {};
  __fastcall ESelfImageError(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(ResStringRec, Args, Args_Size) {};
  __fastcall ESelfImageError(int Ident, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(Ident, Args, Args_Size, AHelpContext) {};
  #endif
}; // class ESelfImageError
//---------------------------------------------------------------------------
#if 0
//---------------------------------------------------------------------------
// General SelfImage system error - problems with a system call that is not
// related to memory allocation (which is ESelfImageAllocError)
//
class ESelfImageSystemError : public Exception {
public:
  __fastcall ESelfImageSystemError(const AnsiString Msg): Exception(Msg) {};
  __fastcall ESelfImageSystemError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size): Exception(Msg, Args, Args_Size) {};
  __fastcall ESelfImageSystemError(int Ident): Exception(Ident) {};
  __fastcall ESelfImageSystemError(System::PResStringRec ResStringRec): Exception(ResStringRec) {};
  __fastcall ESelfImageSystemError(int Ident, const System::TVarRec * Args, const int Args_Size): Exception(Ident, Args, Args_Size) {};
  __fastcall ESelfImageSystemError(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size): Exception(ResStringRec, Args, Args_Size) {};
  __fastcall ESelfImageSystemError(const AnsiString Msg, int AHelpContext): Exception(Msg, AHelpContext) {};
  __fastcall ESelfImageSystemError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(Msg, Args, Args_Size, AHelpContext) {};
  __fastcall ESelfImageSystemError(int Ident, int AHelpContext): Exception(Ident, AHelpContext) {};
  __fastcall ESelfImageSystemError(System::PResStringRec ResStringRec, int AHelpContext): Exception(ResStringRec, AHelpContext) {};
  __fastcall ESelfImageSystemError(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(ResStringRec, Args, Args_Size) {};
  __fastcall ESelfImageSystemError(int Ident, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(Ident, Args, Args_Size, AHelpContext) {};
}; // class ESelfImageSystemError
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// SelfImage allocation error - problems allocating memory or resources
//
class ESelfImageAllocError : public Exception {
public:
  __fastcall ESelfImageAllocError(const AnsiString Msg): Exception(Msg) {};
  __fastcall ESelfImageAllocError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size): Exception(Msg, Args, Args_Size) {};
  __fastcall ESelfImageAllocError(int Ident): Exception(Ident) {};
  __fastcall ESelfImageAllocError(System::PResStringRec ResStringRec): Exception(ResStringRec) {};
  __fastcall ESelfImageAllocError(int Ident, const System::TVarRec * Args, const int Args_Size): Exception(Ident, Args, Args_Size) {};
  __fastcall ESelfImageAllocError(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size): Exception(ResStringRec, Args, Args_Size) {};
  __fastcall ESelfImageAllocError(const AnsiString Msg, int AHelpContext): Exception(Msg, AHelpContext) {};
  __fastcall ESelfImageAllocError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(Msg, Args, Args_Size, AHelpContext) {};
  __fastcall ESelfImageAllocError(int Ident, int AHelpContext): Exception(Ident, AHelpContext) {};
  __fastcall ESelfImageAllocError(System::PResStringRec ResStringRec, int AHelpContext): Exception(ResStringRec, AHelpContext) {};
  __fastcall ESelfImageAllocError(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(ResStringRec, Args, Args_Size) {};
  __fastcall ESelfImageAllocError(int Ident, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(Ident, Args, Args_Size, AHelpContext) {};
}; // class ESelfImageAllocError
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// SelfImage file error - problems opening, reading from, or writing to a
// file
//
class ESelfImageFileError : public Exception {
public:
  __fastcall ESelfImageFileError(const AnsiString Msg): Exception(Msg) {};
  __fastcall ESelfImageFileError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size): Exception(Msg, Args, Args_Size) {};
  __fastcall ESelfImageFileError(int Ident): Exception(Ident) {};
  __fastcall ESelfImageFileError(System::PResStringRec ResStringRec): Exception(ResStringRec) {};
  __fastcall ESelfImageFileError(int Ident, const System::TVarRec * Args, const int Args_Size): Exception(Ident, Args, Args_Size) {};
  __fastcall ESelfImageFileError(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size): Exception(ResStringRec, Args, Args_Size) {};
  __fastcall ESelfImageFileError(const AnsiString Msg, int AHelpContext): Exception(Msg, AHelpContext) {};
  __fastcall ESelfImageFileError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(Msg, Args, Args_Size, AHelpContext) {};
  __fastcall ESelfImageFileError(int Ident, int AHelpContext): Exception(Ident, AHelpContext) {};
  __fastcall ESelfImageFileError(System::PResStringRec ResStringRec, int AHelpContext): Exception(ResStringRec, AHelpContext) {};
  __fastcall ESelfImageFileError(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(ResStringRec, Args, Args_Size) {};
  __fastcall ESelfImageFileError(int Ident, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(Ident, Args, Args_Size, AHelpContext) {};
}; // class ESelfImageFileError
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// SelfImage compression error
//
class ESelfImageCompressionError : public Exception {
public:
  __fastcall ESelfImageCompressionError(const AnsiString Msg): Exception(Msg) {};
  __fastcall ESelfImageCompressionError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size): Exception(Msg, Args, Args_Size) {};
  __fastcall ESelfImageCompressionError(int Ident): Exception(Ident) {};
  __fastcall ESelfImageCompressionError(System::PResStringRec ResStringRec): Exception(ResStringRec) {};
  __fastcall ESelfImageCompressionError(int Ident, const System::TVarRec * Args, const int Args_Size): Exception(Ident, Args, Args_Size) {};
  __fastcall ESelfImageCompressionError(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size): Exception(ResStringRec, Args, Args_Size) {};
  __fastcall ESelfImageCompressionError(const AnsiString Msg, int AHelpContext): Exception(Msg, AHelpContext) {};
  __fastcall ESelfImageCompressionError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(Msg, Args, Args_Size, AHelpContext) {};
  __fastcall ESelfImageCompressionError(int Ident, int AHelpContext): Exception(Ident, AHelpContext) {};
  __fastcall ESelfImageCompressionError(System::PResStringRec ResStringRec, int AHelpContext): Exception(ResStringRec, AHelpContext) {};
  __fastcall ESelfImageCompressionError(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(ResStringRec, Args, Args_Size) {};
  __fastcall ESelfImageCompressionError(int Ident, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(Ident, Args, Args_Size, AHelpContext) {};
}; // class ESelfImageCompressionError
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// SelfImage network (NBD) error
//
class ESelfImageNetworkError : public Exception {
public:
  __fastcall ESelfImageNetworkError(const AnsiString Msg): Exception(Msg) {};
  __fastcall ESelfImageNetworkError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size): Exception(Msg, Args, Args_Size) {};
  __fastcall ESelfImageNetworkError(int Ident): Exception(Ident) {};
  __fastcall ESelfImageNetworkError(System::PResStringRec ResStringRec): Exception(ResStringRec) {};
  __fastcall ESelfImageNetworkError(int Ident, const System::TVarRec * Args, const int Args_Size): Exception(Ident, Args, Args_Size) {};
  __fastcall ESelfImageNetworkError(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size): Exception(ResStringRec, Args, Args_Size) {};
  __fastcall ESelfImageNetworkError(const AnsiString Msg, int AHelpContext): Exception(Msg, AHelpContext) {};
  __fastcall ESelfImageNetworkError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(Msg, Args, Args_Size, AHelpContext) {};
  __fastcall ESelfImageNetworkError(int Ident, int AHelpContext): Exception(Ident, AHelpContext) {};
  __fastcall ESelfImageNetworkError(System::PResStringRec ResStringRec, int AHelpContext): Exception(ResStringRec, AHelpContext) {};
  __fastcall ESelfImageNetworkError(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(ResStringRec, Args, Args_Size) {};
  __fastcall ESelfImageNetworkError(int Ident, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(Ident, Args, Args_Size, AHelpContext) {};
}; // class ESelfImageNetworkError
//---------------------------------------------------------------------------
#endif
#else
// General SelfImage error - anything that doesn't fit into the below classes
//
class ESelfImageError : public std::exception
{
std::string _what;

	public:
		ESelfImageError(std::string what) : _what(what) {}
		~ESelfImageError() throw() {}
		virtual const char* what() const throw() { return _what.c_str(); }
}; // class ESelfImageError



// General SelfImage error - anything that doesn't fit into the below classes
//
//class ESelfImageError : public exception {
//public:
//  __fastcall ESelfImageError(const string Msg);
//}; // class ESelfImageError

#endif
#endif

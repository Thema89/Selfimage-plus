//------------------------------ $Keywords ----------------------------------
// TProgramLog.cpp - TProgramLog class general purpose debug logger
// Copyright ©2003-2005, Kurt Fitzner <kfitzner@excelcia.org>
//---------------------------------------------------------------------------
// This file is part of the TProgramLog general purpose debug logging
// class.
//
// The TProgramLog class is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License (Version 2)
// as published by the Free Software Foundation.
//
// The TProgramLog class is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// VCS: $Version: 0 $ $Revision: 4 $
/*
$History: **** V 0.0 by kfitzner ****
$History: * tprogramlog.cpp - 2005-05-13 11:14:17 PM - 8166 Bytes
$History: * tprogramlog.h - 2005-05-13 3:12:18 PM - 4907 Bytes
$History: * Initial check-in
$History: **** V 0.1 by kfitzner ****
$History: * tprogramlog.cpp - 2005-07-31 2:10:37 AM - 8496 Bytes
$History: * tprogramlog.h - 2005-07-31 2:10:08 AM - 4956 Bytes
$History: * Alter header to reflect that it's a unit shared by 
$History: * several projects, not just GPGee.
$History: **** V 0.2 by kfitzner ****
$History: * tprogramlog.cpp - 2005-08-08 6:40:41 AM - 8671 Bytes
$History: * tprogramlog.h - 2005-08-08 6:40:32 AM - 4892 Bytes
$History: * License change - remove option for later versions of GPL
$History: **** V 0.3 by kfitzner ****
$History: * tprogramlog.cpp - 2005-09-05 3:58:12 PM - 9088 Bytes
$History: * tprogramlog.h - 2005-09-05 3:53:30 PM - 4959 Bytes
$History: * Add __IFLOGGING__ and __ENDIFLOGGING__ macros  Added 
$History: * __LOGGINGENABLED__ and __LOGGINGDISABLED__ instead 
$History: * - C doesn't support #ifdef dirrectives within macros 
$History: * - sheesh, I should have known that.
$History: **** Latest ** V 0.4 by kfitzner ** 2005-12-02 2:03:01 AM ****
$History: * Add DELETELOG macro
*/
//----------------------------  $NoKeywords ---------------------------------


//---------------------------------------------------------------------------
// File Notes:
//---------------------------------------------------------------------------
// 6 May 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// A fairly generic logging class.  It is designed to be as fast as possible.
// When in buffered-writing mode (when synchronous is false in the
// constructor), a logging statement will incur approximately 20 microseconds
// of overhead on most gigahertz-range CPUs. In syncronous (unbuffered
// writes) mode, this increases to about 90 microseconds.
//
// It is also designed such that logging statements expand to nothing when
// the program is not compiled in debug mode - Compilation of most of the
// features is dependent on having either _DEBUG or ENABLE_LOGGING defined.
// Thus for production software there is zero overhead.
//---------------------------------------------------------------------------
#include <vcl.h>
#include <windows.h>
#pragma hdrstop

#include <math.h>
#include <Math.hpp>
#include <stdio.h>
#include <StrUtils.hpp>

#include "TProgramLog.h"


//---------------------------------------------------------------------------
// TProgramLog() - Class constructor
//
// Takes:   AnsiString filename
// Throws:  Nothing
//---------------------------------------------------------------------------
__fastcall TProgramLog::TProgramLog(AnsiString FileName, bool Synchronous, bool OverWrite, TLogLevels LogLevel)
{
  FunctionStack = new TStack;
  if (OverWrite)
    DeleteFile(FileName);
  if (!FileExists(FileName)) {
    FileHandle = FileCreate(FileName);
    FileClose(FileHandle);
  }  // if (!FileExists(FileName))
  if (!Synchronous) {
    FileHandle = FileOpen(FileName, fmOpenReadWrite);
    FileSeek(FileHandle, 0, 2);
  } else
    FileHandle = -1;
  this->FileName = ExpandFileName(FileName);
  this->Synchronous = Synchronous;
  this->LogLevel = LogLevel;
}  // _fastcall TProgramLog::TProgramLog(AnsiString FileName, bool Synchronous, bool OverWrite)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ~TProgramLog() - C'est une destructor
//
// Throws: Nothing
//---------------------------------------------------------------------------
__fastcall TProgramLog::~TProgramLog()
{
  if (FileHandle != -1)
    FileClose(FileHandle);
  delete FunctionStack;
}  // __fastcall TProgramLog::~TProgramLog()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// _LogEntry() - Write an entry into the log.
//
// Takes:   TLogLevels enum with the log level, AnsiString with the filename,
//          AnsiString with the line numnber, a message with optional
//          "printf" style format specifiers. and optional arguments
// Returns: Nothing
// Throws:  Nothing
//
void __fastcall TProgramLog::_LogEntry(TLogLevels LogLevel, AnsiString File, AnsiString Line, AnsiString Message, va_list ArgList)
{
  if (LogLevel >= this->LogLevel) {
    if (FileHandle == -1) {
      FileHandle = FileOpen(FileName, fmOpenReadWrite);
      FileSeek(FileHandle, 0, 2);
    }  // if (FileHandle == -1)

    if (FileHandle != -1) {
      static bool    UsePerformanceTimer, ConfiguredTimer = false;
      static __int64 PerformanceFreq = 0;
      static __int64 PerformanceOffset;
      LARGE_INTEGER  perftimer;
      static char    Buffer[256];
      int            Count;
      SYSTEMTIME     SystemTime;
      AnsiString sTimeStamp = "";

      // Configure the high-performance counter - a one-time process.
      if (!ConfiguredTimer) {
        QueryPerformanceFrequency(&perftimer);                  // How many times the counter increments per second
        PerformanceFreq = perftimer.QuadPart / 1000000;         // We want how many times the counter increments per microsecond.
        if (PerformanceFreq) {                                  // If it's zero, then there is no performance counter on this PC, if not then we're go...
          QueryPerformanceCounter(&perftimer);                  // Get the counter value
          // This offset lets us turn the counter value into the number of microseconds since 12/30/1899 00:00
          // which is what the TDateTime uses as its baseline.
          PerformanceOffset = (__int64)((long double)Now().Val * 86400000000.0l) - perftimer.QuadPart / PerformanceFreq;
          UsePerformanceTimer = true;
        }  // if (pFreq)
        else
          UsePerformanceTimer = false;
        ConfiguredTimer = true;
      }  // if (pFreq != -1)

      if (UsePerformanceTimer) {
        __int64 MicroTimeStamp;
        TDateTime CalcedNow;
        int Microseconds;

        QueryPerformanceCounter(&perftimer);
        MicroTimeStamp = perftimer.QuadPart / PerformanceFreq + PerformanceOffset;
        CalcedNow = ((double)MicroTimeStamp) / 86400000000.0;
        Microseconds = MicroTimeStamp % 1000000;
        sTimeStamp = CalcedNow.FormatString("ddmmmyyyy hh:nn:ss.") + String().sprintf("%6.6u", Microseconds);
      } else {
        GetSystemTime(&SystemTime);
        sTimeStamp = TDateTime().CurrentDateTime().FormatString("ddmmmyyyy hh:nn:ss.") + String().sprintf("%3.3u", SystemTime.wMilliseconds);
      }  // if (UsePerformanceTimer)

      Count = vsnprintf(Buffer, sizeof(Buffer), (String("[") + sTimeStamp + String("]-[") + LeftStr(ExtractFileName(File)+"                      ",22) + String(":") + RightStr("000" + Line, 4) + "] " + Message + String("\r\n")).c_str(), ArgList);
      Buffer[Count] = '\000';
      FileWrite(FileHandle, Buffer, Count);
      if (Synchronous) {
        FileClose(FileHandle);
        FileHandle = -1;
      }  // if (Synchronous)
    }  //   if (FIleHandle != -1)
  }  // if (LogLevel >= this->LogLevel)
}  // void _cdecl LogEntry(AnsiString File, AnsiString Line, AnsiString Message, ...)
//---------------------------------------------------------------------------


/*
    if (Log) {
      AnsiString Trace = "Unhandled exception occured in function " + *(AnsiString *)Log->FunctionStack->Peek();
      delete (AnsiString *)Log->FunctionStack->Pop();
      while (Log->FunctionStack->AtLeast(1)) {
        Trace += " called by " + *(AnsiString *)Log->FunctionStack->Peek();
        delete (AnsiString *)Log->FunctionStack->Pop();
      }  // while (Log->FunctionStack->AtLeast(1))
      MessageDlg(WrapText(Trace), mtError, TMsgDlgButtons() << mbOK, 0);
    }  // if (Log)
*/




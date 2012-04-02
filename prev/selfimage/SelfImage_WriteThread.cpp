//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_WriteThread.cpp - Data write thread class
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
// VCS: $Version: 1 $ $Revision: 2 $
/*
$History: **** V 0.1 by kfitzner ****
$History: * selfimage_writethread.cpp - 2005-11-07 2:39:39 AM - 5545 Bytes
$History: * selfimage_writethread.h - 2005-11-07 1:48:32 AM - 2392 Bytes
$History: * Initial check-in
$History: **** V 0.2 by kfitzner ****
$History: * selfimage_writethread.cpp - 2005-11-12 5:22:12 PM - 5808 Bytes
$History: * selfimage_writethread.h - 2005-11-12 5:19:02 PM - 2396 Bytes
$History: * Typo in copyright area + change in program description
$History: **** V 0.3 by kfitzner ****
$History: * selfimage_writethread.cpp - 2005-11-18 3:30:57 PM - 6129 Bytes
$History: * selfimage_writethread.h - 2005-11-12 5:19:02 PM - 2396 Bytes
$History: * Make sure zero length buffers are handled properly
$History: **** V 0.4 by kfitzner ****
$History: * selfimage_writethread.cpp - 2005-11-22 8:12:00 PM - 6391 Bytes
$History: * selfimage_writethread.h - 2005-11-12 5:19:02 PM - 2396 Bytes
$History: * Fix error flag not being initialized
$History: **** V 1.0 by kfitzner ****
$History: * selfimage_writethread.cpp - 2005-11-23 2:00:59 PM - 6594 Bytes
$History: * selfimage_writethread.h - 2005-11-23 2:25:58 AM - 2200 Bytes
$History: * Add intermediary TSelfImage thread class
$History: **** V 1.1 by kfitzner ****
$History: * selfimage_writethread.cpp - 2005-12-09 2:21:10 PM - 7030 Bytes
$History: * selfimage_writethread.h - 2005-11-23 2:25:58 AM - 2200 Bytes
$History: * Add support for zero flag - if the write thread sees 
$History: * the flag, it zeros the block before writing to disk
$History: **** Latest ** V 1.2 by kfitzner ** 2007-03-18 1:37:00 PM ****
$History: * Change references to BytesWritten to FBytesWritten 
$History: * as BytesWritten is now a read-only property.  Remove 
$History: * declarations - in base class.  Raise priority to tpHighest.
*/
//----------------------------  $NoKeywords ---------------------------------

//---------------------------------------------------------------------------
// File Notes:
//---------------------------------------------------------------------------
// 16 Sep 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// See the notes for SelfImage_ReadThread.cpp
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelfImage_WriteThread.h"
#include "SelfImage_Utility.h"
#include "SelfImage_Exceptions.h"
#include "memlib.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Write thread constructor
//
__fastcall TSelfImageWriteThread::TSelfImageWriteThread(TImageStore *WriteStore, TImageBuffer *ImageBuffer): TSelfImageThread()
{
  this->WriteStore  = WriteStore;
  this->ImageBuffer = ImageBuffer;
  FBytesWritten = 0;
  Priority = tpHighest;
}  // __fastcall TSelfImageWriteThread::TSelfImageWriteThread(TImageStore *WriteStore, TImageBuffer *ImageBuffer): TThread(true)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Function to set the thread's name for debugging purposes.  This is done
// by filling in a special structure and firing off an exception.  A
// compatible debugger knows to look in exceptions of this type and look for
// the thread's name.  A little bit of an odd way of doing things, but it
// seems to work.
//
void TSelfImageWriteThread::SetName()
{
  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = "WriteThread";
  info.dwThreadID = -1;
  info.dwFlags = 0;

  __try {
    RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD),(DWORD*)&info );
  }
  __except (EXCEPTION_CONTINUE_EXECUTION) { }
}  // void TSelfImageWriteThread::SetName()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Main execute method for the thread.
//
void __fastcall TSelfImageWriteThread::Execute()
{
  bool bEOF = false;
  bool bWriteSuccess;
  bool bCheckSize;
  unsigned nWriteCount;
  unsigned nBytesWritten;

  SetName();

  try {
    // Set a flag to tell the write loop whether we need to worry about a fixed amount of space (writing to a smaller partition)
    // Easier (on 32-bit systems) to do with a flag than by constantly checking a 64-bit value.
    bCheckSize = WriteStore->Geometry.Bytes > 0;
    while (!bEOF && !Terminated) {
      TBufferChunk *ReadChunk = ImageBuffer->GetChunk(csAllocRead, &Terminated);
      if (!ReadChunk)
        continue;
      if (ReadChunk->ZeroFlag)
        memzero_optimized(ReadChunk->Data, ReadChunk->Size);
      if (bCheckSize && FBytesWritten + ReadChunk->Size > WriteStore->Geometry.Bytes) {
          nWriteCount = WriteStore->Geometry.Bytes - FBytesWritten;
          bEOF = true;
      } else {
          nWriteCount = ReadChunk->Size;
          bEOF = ReadChunk->EOF;
      }  // if (bCheckSize && FBytesWritten + ReadChunk->Size > WriteStore->Geometry.Bytes)
      if (nWriteCount) {
        WriteStore->Write(ReadChunk->Data, nWriteCount, &nBytesWritten);
        if (nBytesWritten != nWriteCount) {
          // An error occured while writing - handle this
          throw ESelfImageFileError("Error while writing to " + WriteStore->FileName + ": " + GetLastErrorMessage());
        }  // else if (!nBytesRead)
        FBytesWritten += nBytesWritten;
      }  // if (nWriteCount)
      ImageBuffer->ReleaseChunk(ReadChunk);
    }  // while (!EOF)
    FFinished = true;
  }  // try
  // C++ Builder doesn't support passing exceptions from a thread.  So, we have to catch the exception here and
  // manually pass word that an error has occured.  This is done with the ErrorFlag property.
  catch (Exception &e) {
    FErrorFlag = true;
    FErrorMessage = "Write thread encountered " + e.ClassName() + " exception: \"" + e.Message + "\"";
    FFinished = true;
  }  // catch
}  // void __fastcall TSelfImageWriteThread::Execute()
//---------------------------------------------------------------------------

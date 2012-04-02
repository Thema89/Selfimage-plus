//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_CompressionThread.cpp - Data compression thread class
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

//---------------------------------------------------------------------------
// VCS: $Version: 1 $ $Revision: 5 $
/*
$History: **** V 0.1 by kfitzner ****
$History: * selfimage_compressionthread.cpp - 2005-11-23 12:03:13 AM - 8580 Bytes
$History: * selfimage_compressionthread.h - 2005-11-22 11:43:58 PM - 2846 Bytes
$History: * Initial check-in
$History: **** V 0.2 by kfitzner ****
$History: * selfimage_compressionthread.cpp - 2005-11-23 1:14:52 AM - 8815 Bytes
$History: * selfimage_compressionthread.h - 2005-11-22 11:43:58 PM - 2846 Bytes
$History: * Fix indirection typo
$History: **** V 0.3 by kfitzner ****
$History: * selfimage_compressionthread.cpp - 2005-11-23 1:22:44 AM - 9359 Bytes
$History: * selfimage_compressionthread.h - 2005-11-22 11:43:58 PM - 2846 Bytes
$History: * Forgot to test for the EOF - whoops
$History: **** V 0.4 by kfitzner ****
$History: * selfimage_compressionthread.cpp - 2005-11-26 5:51:43 PM - 10657 Bytes
$History: * selfimage_compressionthread.h - 2005-11-26 5:51:32 PM - 2636 Bytes
$History: * Add intermediary TSelfImage thread class
$History: **** V 0.5 by kfitzner ****
$History: * selfimage_compressionthread.cpp - 2005-11-26 6:08:06 PM - 10916 Bytes
$History: * selfimage_compressionthread.h - 2005-11-26 6:07:40 PM - 2537 Bytes
$History: * Move TCompressionMethod enum to TypesDefs.h
$History: **** V 0.6 by kfitzner ****
$History: * selfimage_compressionthread.cpp - 2005-11-26 6:59:42 PM - 11196 Bytes
$History: * selfimage_compressionthread.h - 2005-11-26 5:51:32 PM - 2636 Bytes
$History: * Revert last change (Move TCompressionMethod enum to TypesDefs.h)
$History: **** V 0.7 by kfitzner ****
$History: * selfimage_compressionthread.cpp - 2005-11-27 6:09:03 AM - 11463 Bytes
$History: * selfimage_compressionthread.h - 2005-11-27 1:29:02 AM - 2583 Bytes
$History: * Remove BytesRead & BytesWritten - now in base class
$History: **** V 1.0 by kfitzner ****
$History: * selfimage_compressionthread.cpp - 2005-11-28 1:39:51 PM - 12025 Bytes
$History: * selfimage_compressionthread.h - 2005-11-27 1:29:02 AM - 2583 Bytes
$History: * Add support for gzip compression level configuration 
$History: * and add set FFinished to be true at the end of the loop
$History: **** V 1.1 by kfitzner ****
$History: * selfimage_compressionthread.cpp - 2005-12-02 5:57:52 AM - 12064 Bytes
$History: * selfimage_compressionthread.h - 2005-11-27 1:29:02 AM - 2583 Bytes
$History: * Reduce memory allocation/freeing per compression block 
$History: * as much as possible, and fix a memory leak - memory 
$History: * leak not a SelfImage bug but a problem with Borland's 
$History: * memory manager not being thread-safe
$History: **** V 1.2 by kfitzner ****
$History: * selfimage_compressionthread.cpp - 2005-12-04 12:36:50 PM - 13294 Bytes
$History: * selfimage_compressionthread.h - 2005-12-03 9:58:30 PM - 2655 Bytes
$History: * Add support for zero flagged blocks (blocks that are 
$History: * marked as being all zeros)
$History: **** V 1.3 by kfitzner ****
$History: * selfimage_compressionthread.cpp - 2005-12-08 9:47:11 AM - 13436 Bytes
$History: * selfimage_compressionthread.h - 2005-12-03 9:58:30 PM - 2655 Bytes
$History: * Remove externs, add #include of TypesDefs.h
$History: **** V 1.4 by kfitzner ****
$History: * selfimage_compressionthread.cpp - 2007-03-18 2:28:39 PM - 13765 Bytes
$History: * selfimage_compressionthread.h - 2005-12-03 9:58:30 PM - 2655 Bytes
$History: * BytesRead & BytesWritten now read-only properties 
$History: * - FBytesRead & FBytesWritten used internally.
$History: **** Latest ** V 1.5 by kfitzner ** 2007-03-19 2:39:43 PM ****
$History: * Add support for different gzip chunk sizes, remove 
$History: * TConfiguration access during each gzip chunk's compression
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// File Notes:
//---------------------------------------------------------------------------
// 22 Nov 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// A "filter" style thread - this thread sits in between the input and
// output threads and compresses the data.  The whole point for making
// SelfImage multithreaded was so that we could have more than one
// of these threads acting together - parallel compression.  Because the
// input and output threads are detached from one another and only
// the TImageBuffer links them, all that's needed to add this thread in
// the middle is add a second buffer to hold the compressed data.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "zlib.h"
#include "bzlib.h"
#include "SelfImage_TypesDefs.h"
#include "SelfImage_CompressionThread.h"
#include "SelfImage_Exceptions.h"
#include "SelfImage_Utility.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructor
//
__fastcall TSelfImageCompressionThread::TSelfImageCompressionThread(TCompressionMethod CompressionMethod, int ChunkSize, TImageBuffer *InputBuffer, TImageBuffer *OutputBuffer): TSelfImageThread()
{
  HRSRC PrecompressedResource;
  HGLOBAL PrecompressedGlobal;
  AnsiString ResourceName;

  this->CompressionMethod = CompressionMethod;
  this->InputBuffer  = InputBuffer;
  this->OutputBuffer = OutputBuffer;
  InputBuffer->AllocationSister = OutputBuffer;
  FBytesRead = 0;
  FBytesWritten = 0;

  // Find out what resource we will be using for pre-compressed "zero" chunks
  if (CompressionMethod == cmBzip2)
    ResourceName = "PRECOMPRESSED_BZIP2";
  else {
    ResourceName ="PRECOMPRESSED_GZIP_" + String(ChunkSize/1024);
    // Take the opportunity of also looking up our gzip compression level too
    if (CompressionMethod == cmGzip1)
      CompressionLevel = SelfImageConfig->Values["gzipFastLevel"];
    else
      CompressionLevel = SelfImageConfig->Values["gzipBestLevel"];
  }  // else if (CompressionMethod != cmBzip2)
  PrecompressedResource = FindResource(HInstance, ResourceName.c_str(), RT_RCDATA);
  if (!PrecompressedResource)
    throw EResNotFound("There was an error while attempting to locate the " + ResourceName + " resource. This is probably a programming error and should be reported to " + Application->Name + "'s maintainers.");
  PrecompressedSize = SizeofResource(HInstance, PrecompressedResource);
  PrecompressedGlobal = LoadResource(HInstance, PrecompressedResource);
  Precompressed = (unsigned char *)LockResource(PrecompressedGlobal);
}  // __fastcall TSelfImageCompressionThread::TSelfImageCompressionThread(TCompressionMethod CompressionMethod, int ChunkSize, TImageBuffer *InputBuffer, TImageBuffer *OutputBuffer): TSelfImageThread()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Function to set the thread's name for debugging purposes.  This is done
// by filling in a special structure and firing off an exception.  A
// compatible debugger knows to look in exceptions of this type and look for
// the thread's name.  A little bit of an odd way of doing things, but it
// seems to work.
//
void TSelfImageCompressionThread::SetName()
{
  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = "CompressionThread";
  info.dwThreadID = -1;
  info.dwFlags = 0;

  __try {
    RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD),(DWORD*)&info );
  }
  __except (EXCEPTION_CONTINUE_EXECUTION) {}
}  // void TSelfImageCompressionThread::SetName()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// The thread's main execution loop - keep this as simple as possible
//
void __fastcall TSelfImageCompressionThread::Execute()
{
  bool bEOF = false;

  SetName();

  try {
    while (!bEOF && !Terminated) {
      TBufferChunk *ReadChunk, *CompressChunk;
      ReadChunk = InputBuffer->GetChunk(csAllocRead, &Terminated);
      if (!ReadChunk)
        continue;
      CompressChunk = ReadChunk->Sister;
      // If we didn't get a sister chunk and the thread isn't terminating, then there is a problem.
      if (!CompressChunk)
        if (!Terminated)
          throw ESelfImageSystemError("Buffer allocation failed to return both a read and write buffer chunk.  This is probably a programming error - please report it to the maintainers.");
        else
          continue;
      bEOF = ReadChunk->EOF;
      CompressChunk->EOF = bEOF;
      // Only compress if we actually have data - otherwise just set the output chunk's size to zero.
      if (ReadChunk->Size) {
        if (ReadChunk->ZeroFlag) {
          memcpy(CompressChunk->Data, Precompressed, PrecompressedSize);
          CompressChunk->Size = PrecompressedSize;
        } else
          Compress(ReadChunk, CompressChunk);
        FBytesRead += ReadChunk->Size;
        FBytesWritten += CompressChunk->Size;
      } else {
        CompressChunk->Size = 0;
      }  // else if (!ReadChunk->Size)
      InputBuffer->ReleaseChunk(ReadChunk);
    }  // while (!bEOF && !Terminated)
    FFinished = true;
  }  // try
  // C++ Builder doesn't support passing exceptions from a thread.  So, we have to catch the exception here and
  // manually pass word that an error has occured.  This is done with the ErrorFlag property.
  catch (Exception &e) {
    FErrorFlag = true;
    FErrorMessage = "Compression thread encountered " + e.ClassName() + " exception: \"" + e.Message + "\"";
    FFinished = true;
  }  // catch
}  // void __fastcall TSelfImageCompressionThread::Execute()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// The actual work of compressing the data is done here (well, and also in
// the compression libraries that we link to) :)
//
void TSelfImageCompressionThread::Compress(TBufferChunk *Source, TBufferChunk *Target)
{
  unsigned char *InData = (unsigned char *)Source->Data;
  unsigned char *OutData = (unsigned char *)Target->Data;

  switch (CompressionMethod) {
    // gzip compression
    case cmGzip1:
    case cmGzip9: {
      z_stream  zsStream;
      static gz_header gzHeader = {false, 0, 0, 255, XTRA_BLOCKSIZE, 8, 0, NULL, 0, NULL, 0, false, false};

      int ret;
      unsigned int *BlockSize;

      // Configure the z_stream - windowbits > 15 means to write a gzip stream out, not just a zlib stream
      memset(&zsStream, 0, sizeof(zsStream));
      //zsStream.zalloc = zlib_allocmem;
      //zsStream.zfree = zlib_freemem;
      zsStream.next_in = InData;
      zsStream.next_out = OutData;
      ret = deflateInit2(&zsStream, CompressionLevel, Z_DEFLATED, 31, 9, Z_DEFAULT_STRATEGY);
      if (ret != Z_OK)
        throw ESelfImageCompressionError("Initialization of a gzip compression block failed.");

      // Configure the gzip header for our stream.  Nothing special, except an extra field.  This field will
      // store the compressed size of the block.  An extra field consists of one or more subfields.  We only
      // have one subfield with the id bytes "SI" (for SelfImage of course), followed by the subfield length
      // of 8, and then followed by the subfield itself which is a 32 bit integer holding the block size.
      ret = deflateSetHeader(&zsStream, &gzHeader);
      if (ret != Z_OK)
        throw ESelfImageCompressionError("Initialization of a gzip compression header failed.");

      // Keep compressing until we get something other than Z_OK - hopefully we get Z_STREAM_END
      zsStream.avail_in = Source->Size;
      zsStream.avail_out = Target->MaxSize;
      do ret = deflate(&zsStream, Z_FINISH); while (ret == Z_OK);
      if (ret != Z_STREAM_END)
        throw ESelfImageCompressionError("Failed to compress a block - " + String(zsStream.msg));

      ret = deflateEnd(&zsStream);
      if (ret != Z_OK)
        throw ESelfImageCompressionError("Failed to clean up after compressing a block.");

      // Now that we know the compressed size, go back and set it in the gzip header.  It's probably not a good idea
      // to do this by a fixed byte offset, but the gzip file format probably isn't changing any time soon, so this
      // is ok for now.
      BlockSize = (unsigned int *)(OutData + 16);
      *BlockSize = zsStream.total_out;
      Target->Size = zsStream.total_out;
    }  // case cmGzip
    break;

    // bzip2 compression - libbz2 is much simpler to use this way than zlib
    case cmBzip2: {
      int ret;
      unsigned int BlockSize;

      BlockSize = Target->MaxSize;
      BZ2_bzBuffToBuffCompress(OutData, &BlockSize, InData, Source->Size, 9, 0, 0);
      Target->Size = BlockSize;
    }  // case cmBzip2:
    break;

    default:
      return;
  }  // switch (CompressionMethod)
}  // void Compress(TBufferChunk *Source, TBufferChunk *Target)
//---------------------------------------------------------------------------

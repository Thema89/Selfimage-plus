//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_DecompressionThread.cpp - Data decompression thread class
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
// VCS: $Version: 1 $ $Revision: 5 $
/*
$History: **** V 0.1 by kfitzner ****
$History: * selfimage_decompressionthread.cpp - 2005-11-27 6:08:37 AM - 14280 Bytes
$History: * selfimage_decompressionthread.h - 2005-11-27 4:56:56 AM - 2539 Bytes
$History: * Initial Check-in
$History: **** V 1.0 by kfitzner ****
$History: * selfimage_decompressionthread.cpp - 2005-11-28 1:39:55 PM - 14583 Bytes
$History: * selfimage_decompressionthread.h - 2005-11-27 4:56:56 AM - 2539 Bytes
$History: * Set FFinished to true at end of loops
$History: **** V 1.1 by kfitzner ****
$History: * selfimage_decompressionthread.cpp - 2005-12-02 4:47:56 AM - 14968 Bytes
$History: * selfimage_decompressionthread.h - 2005-11-27 4:56:56 AM - 2539 Bytes
$History: * Remove as much memory management as possible from 
$History: * block decompression, and set zlib's external memory manager.
$History: **** V 1.2 by kfitzner ****
$History: * selfimage_decompressionthread.cpp - 2005-12-02 5:58:01 AM - 15290 Bytes
$History: * selfimage_decompressionthread.h - 2005-11-27 4:56:56 AM - 2539 Bytes
$History: * Comment out setting custom zlib memory management 
$History: * functions - not needed with FastMM
$History: **** V 1.3 by kfitzner ****
$History: * selfimage_decompressionthread.cpp - 2005-12-21 12:08:09 AM - 15681 Bytes
$History: * selfimage_decompressionthread.h - 2005-12-20 10:55:48 PM - 2567 Bytes
$History: * Forgot a break in a case statment - gzip stream decompression 
$History: * was falling through to bzip2
$History: **** V 1.4 by kfitzner ****
$History: * selfimage_decompressionthread.cpp - 2007-03-14 1:50:19 PM - 15955 Bytes
$History: * selfimage_decompressionthread.h - 2007-03-14 1:33:14 PM - 2572 Bytes
$History: * Rename "block" to "chunk" for clarity
$History: **** Latest ** V 1.5 by kfitzner ** 2007-03-18 2:28:32 PM ****
$History: * BytesRead & BytesWritten now read-only properties 
$History: * - FBytesRead & FBytesWritten used internally.
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// File Notes:
//---------------------------------------------------------------------------
// 26 Nov 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// Decompression thread.  For each type of compression supported, this
// thread has to support decompressing in two ways.  The chunk method, where
// the input is a compressed file that's divided up into chunks of
// discreetly compressed data, and the stream method, where the input file
// is one big compressed stream.  Detection of the type is done way back in
// the TImageStore class, and that detection is used to determine how many
// copies of this thread are started.  Chunk decompression can be done in
// parallel - stream can only be done linearly.  This thread also doesn't
// have to worry about dividing up the chunks - that is also done in
// TImageStore.  But, we do need to handle the decompression very differently
// for each type.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelfImage_DecompressionThread.h"
#include "zlib.h"
#include "bzlib.h"
#include "SelfImage_Exceptions.h"
#include "SelfImage_Utility.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructor
//
__fastcall TSelfImageDecompressionThread::TSelfImageDecompressionThread(TCompressionFormat CompressionFormat, TImageBuffer *InputBuffer, TImageBuffer *OutputBuffer): TSelfImageThread()
{
  this->InputBuffer  = InputBuffer;
  this->OutputBuffer = OutputBuffer;
  this->CompressionFormat = CompressionFormat;
  if (CompressionFormat == cfGzipChunk || CompressionFormat == cfBzip2Chunk) {
    ChunkDecompress = true;
    InputBuffer->AllocationSister = OutputBuffer;
  } else
    ChunkDecompress = false;
  FBytesRead = 0;
  FBytesWritten = 0;
  InitializeStream = true;
}  // __fastcall TSelfImageCompressionThread::TSelfImageCompressionThread(TCompressionMethod CompressionMethod, TImageBuffer *InputBuffer, TImageBuffer *OutputBuffer): TSelfImageThread()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Function to set the thread's name for debugging purposes.  This is done
// by filling in a special structure and firing off an exception.  A
// compatible debugger knows to look in exceptions of this type and look for
// the thread's name.  A little bit of an odd way of doing things, but it
// seems to work.
//
void TSelfImageDecompressionThread::SetName()
{
  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = "DecompressionThread";
  info.dwThreadID = -1;
  info.dwFlags = 0;

  __try {
    RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD),(DWORD*)&info );
  }
  __except (EXCEPTION_CONTINUE_EXECUTION) {}
}  // void TSelfImageCompressionThread::SetName()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// We have two different main loops - one for chunkk decompression, the
// other two for stream decompression.  All we do here is decide which loop
// to use and pass control.
//
void __fastcall TSelfImageDecompressionThread::Execute()
{
  SetName();

  if (ChunkDecompress)
    ChunkDecompressLoop();
  else
    StreamDecompressLoop();
    
}  // void __fastcall TSelfImageDecompressionThread::Execute()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Chunk decompress loop - decompress files that were compressed in chunks
//
void __fastcall TSelfImageDecompressionThread::ChunkDecompressLoop(void)
{
  bool bEOF = false;

  try {
    while (!bEOF && !Terminated) {
      TBufferChunk *ReadChunk, *DecompressedChunk;
      ReadChunk = InputBuffer->GetChunk(csAllocRead, &Terminated);
      if (!ReadChunk)
        continue;
      DecompressedChunk = ReadChunk->Sister;
      // If we didn't get a sister chunk and the thread isn't terminating, then there is a problem.
      if (!DecompressedChunk)
        if (!Terminated)
          throw ESelfImageSystemError("Buffer allocation failed to return both a read and write buffer chunk.  This is probably a programming error - please report it to the maintainers.");
        else
          continue;
      bEOF = ReadChunk->EOF;
      DecompressedChunk->EOF = bEOF;
      // Only compress if we actually have data - otherwise just set the output chunk's size to zero.
      if (ReadChunk->Size) {
        DecompressChunk(ReadChunk, DecompressedChunk);
        FBytesRead += ReadChunk->Size;
        FBytesWritten += DecompressedChunk->Size;
      } else {
        DecompressedChunk->Size = 0;
      }  // else if (!ReadChunk->Size)
      InputBuffer->ReleaseChunk(ReadChunk);
    }  // while (!bEOF && !Terminated)
    FFinished = true;
  }  // try
  // C++ Builder doesn't support passing exceptions from a thread.  So, we have to catch the exception here and
  // manually pass word that an error has occured.  This is done with the ErrorFlag property.
  catch (Exception &e) {
    FErrorFlag = true;
    FErrorMessage = "Decompression thread encountered " + e.ClassName() + " exception: \"" + e.Message + "\"";
    FFinished = true;
  }  // catch
}  // void __fastcall TSelfImageDecompressionThread::ChunkDecompressLoop(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Decompress one chunk
//
void __fastcall TSelfImageDecompressionThread::DecompressChunk(TBufferChunk *Source, TBufferChunk *Target)
{
  switch (CompressionFormat) {
    // Gzip-compressed file that is divided up into individual chunks.
    // Each chunk is a complete mini-gzip stream in and of itself.
    case cfGzipChunk: {
      z_stream zsStream;

      int ret;
      unsigned char *InData = (unsigned char *)Source->Data;
      unsigned char *OutData = (unsigned char *)Target->Data;

      // Configure the z_stream - windowbits > 15 means to accept a gzip stream, not just a zlib stream
      memset(&zsStream, 0, sizeof(z_stream));
      //zsStream.zalloc = zlib_allocmem;
      //zsStream.zfree = zlib_freemem;
      zsStream.next_in  = InData;
      zsStream.avail_in = Source->Size;
      ret = inflateInit2(&zsStream, 31);
      if (ret != Z_OK)
        throw ESelfImageCompressionError("Initialization of a gzip decompression block failed.");

      zsStream.avail_out = Target->MaxSize;
      zsStream.next_out  = OutData;
      do ret = inflate(&zsStream, Z_FINISH); while (ret == Z_OK);
      if (ret != Z_STREAM_END)
        throw ESelfImageCompressionError("Failed to decompress a chunk - " + String(zsStream.msg));

      ret = inflateEnd(&zsStream);
      if (ret != Z_OK)
        throw ESelfImageCompressionError("Failed to clean up after decompressing a chunk.");

      Target->Size = zsStream.total_out;
    }  // case cfGzipChunk:
    break;

    case cfBzip2Chunk: {
      int ret;
      unsigned char *InData = (unsigned char *)Source->Data;
      unsigned char *OutData = (unsigned char *)Target->Data;
      unsigned int ChunkSize;

      ChunkSize = Target->MaxSize;
      BZ2_bzBuffToBuffDecompress(OutData, &ChunkSize, InData, Source->Size, false, 0);
      Target->Size = ChunkSize;
    }  // case cgBzip2Chunk:
    break;
  }  // switch (CompressionFormat)
}  // void TSelfImageDecompressionThread::Decompress(TBufferChunk *Source, TBufferChunk *Target)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// gzip stream decompress loop - decompress files that were compressed
// normally - ie: aren't divided up into little "streamlet" chunks.
//
void __fastcall TSelfImageDecompressionThread::StreamDecompressLoop(void)
{
  bool bEOF = false;

  try {
    while (!bEOF && !Terminated) {
      TBufferChunk *ReadChunk;
      ReadChunk = InputBuffer->GetChunk(csAllocRead, &Terminated);
      if (!ReadChunk)
        continue;
      // Only compress if we actually have data - otherwise just set the output chunk's size to zero.
      if (ReadChunk->Size) {
        DecompressStream(ReadChunk);
        FBytesRead += ReadChunk->Size;
      }  // else if (!ReadChunk->Size)
      bEOF = ReadChunk->EOF;
      InputBuffer->ReleaseChunk(ReadChunk);
    }  // while (!bEOF && !Terminated)
    FFinished = true;
  }  // try
  // C++ Builder doesn't support passing exceptions from a thread.  So, we have to catch the exception here and
  // manually pass word that an error has occured.  This is done with the ErrorFlag property.
  catch (Exception &e) {
    FErrorFlag = true;
    FErrorMessage = "Decompression thread encountered " + e.ClassName() + " exception: \"" + e.Message + "\"";
    FFinished = true;
  }  // catch

}  // void __fastcall TSelfImageDecompressionThread::StreamDecompressLoop(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Decompress part of a stream - one input chunk goes out to one or more
// output chunks.  When using the bzip2 "low level interface", it is almost
// identical to using zlib.
//
// This will only ever be called from one thread - when stream decompression
// occurs, only one decompression thread is started.
//
void __fastcall TSelfImageDecompressionThread::DecompressStream(TBufferChunk *Source)
{
  int ret;
  unsigned char *InData = (unsigned char *)Source->Data;
  unsigned char *OutData;
  TBufferChunk *Target;

  switch (CompressionFormat) {
    case cfGzipStream: {
      static z_stream zsStream;

      if (InitializeStream) {
        InitializeStream = false;
        ret = inflateInit2(&zsStream, 31);
        if (ret != Z_OK)
          throw ESelfImageCompressionError("Initialization of gzip decompression failed.");
      }  // if (bFirst)

      zsStream.next_in = InData;
      zsStream.avail_in = Source->Size;
      do {
        Target = OutputBuffer->GetChunk(csAllocWrite, &Terminated);
        if (!Target)
          return;
        zsStream.next_out = (unsigned char *)Target->Data;
        zsStream.avail_out = Target->MaxSize;
        ret = inflate(&zsStream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
          ret = inflateEnd(&zsStream);
          if (ret != Z_OK)
            throw ESelfImageCompressionError("Failed to clean up after decompressing.");
          Target->EOF = true;
        }  // if (ret == Z_STREAM_END)
        else if (ret != Z_OK)
          throw ESelfImageCompressionError("Error while decompressing - " + String(zsStream.msg));
        Target->Size = Target->MaxSize - zsStream.avail_out;
        FBytesWritten += Target->Size;
        OutputBuffer->ReleaseChunk(Target);
      } while (zsStream.avail_in && ret != Z_STREAM_END);
      break;
    }  // case cfGzipStream:

    case cfBzip2Stream: {
      static bz_stream bzsStream;

      if (InitializeStream) {
        InitializeStream = false;
        ret = BZ2_bzDecompressInit(&bzsStream, 0, false);
        if (ret != BZ_OK)
          throw ESelfImageCompressionError("Initialization of bzip2 decompression failed.");
      }  // if (bFirst)

      bzsStream.next_in = InData;
      bzsStream.avail_in = Source->Size;
      do {
        Target = OutputBuffer->GetChunk(csAllocWrite, &Terminated);
        if (!Target)
          return;
        bzsStream.next_out = (unsigned char *)Target->Data;
        bzsStream.avail_out = Target->MaxSize;
        ret = BZ2_bzDecompress(&bzsStream);
        if (ret == BZ_STREAM_END) {
          ret = BZ2_bzDecompressEnd(&bzsStream);
          if (ret != BZ_OK)
            throw ESelfImageCompressionError("Failed to clean up after decompressing.");
          Target->EOF = true;
        }  // if (ret == Z_STREAM_END)
        else if (ret != BZ_OK)
          throw ESelfImageCompressionError("Bzip2 error while decompressing.");
        Target->Size = Target->MaxSize - bzsStream.avail_out;
        FBytesWritten += Target->Size;
        OutputBuffer->ReleaseChunk(Target);
      } while (bzsStream.avail_in && ret != BZ_STREAM_END);
      break;
    }  // case cfBzip2Stream:

  }  // switch (CompressionFormat)
}  // void __fastcall TSelfImageDecompressionThread::DecompressStream(TBufferChunk *Source, TImageBuffer OutputBuffer)
//---------------------------------------------------------------------------


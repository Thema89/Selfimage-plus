//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_ReadThread.cpp - Data read thread class
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
// VCS: $Version: 1 $ $Revision: 4 $
/*
$History: **** V 0.1 by kfitzner ****
$History: * selfimage_readthread.cpp - 2005-11-07 2:39:38 AM - 6668 Bytes
$History: * selfimage_readthread.h - 2005-11-07 1:48:52 AM - 2377 Bytes
$History: * Initial check-in
$History: **** V 0.2 by kfitzner ****
$History: * selfimage_readthread.cpp - 2005-11-12 5:22:11 PM - 6929 Bytes
$History: * selfimage_readthread.h - 2005-11-12 5:18:28 PM - 2381 Bytes
$History: * Typo in copyright area + change in program description
$History: **** V 0.3 by kfitzner ****
$History: * selfimage_readthread.cpp - 2005-11-18 3:30:51 PM - 7240 Bytes
$History: * selfimage_readthread.h - 2005-11-12 5:18:28 PM - 2381 Bytes
$History: * Fix bug where exception thrown when read input ends 
$History: * on a buffer-size boundary
$History: **** V 0.4 by kfitzner ****
$History: * selfimage_readthread.cpp - 2005-11-22 8:11:28 PM - 7500 Bytes
$History: * selfimage_readthread.h - 2005-11-12 5:18:28 PM - 2381 Bytes
$History: * Fix error flag not being initialized
$History: **** V 0.5 by kfitzner ****
$History: * selfimage_readthread.cpp - 2005-11-23 2:00:59 PM - 7830 Bytes
$History: * selfimage_readthread.h - 2005-11-23 2:23:02 AM - 2185 Bytes
$History: * Add intermediary TSelfImage thread class
$History: **** V 0.6 by kfitzner ****
$History: * selfimage_readthread.cpp - 2005-11-26 7:07:27 AM - 17526 Bytes
$History: * selfimage_readthread.h - 2005-11-26 3:11:42 AM - 2356 Bytes
$History: * Add decompression support (block-decompression-aware read loops)
$History: **** V 1.0 by kfitzner ****
$History: * selfimage_readthread.cpp - 2005-11-29 7:16:27 AM - 17841 Bytes
$History: * selfimage_readthread.h - 2005-11-26 3:11:42 AM - 2356 Bytes
$History: * Fix bug where blocks aren't divided correctly when 
$History: * a block header is split in between two reads
$History: **** V 1.1 by kfitzner ****
$History: * selfimage_readthread.cpp - 2005-12-08 9:45:16 AM - 18656 Bytes
$History: * selfimage_readthread.h - 2005-11-26 3:11:42 AM - 2356 Bytes
$History: * Add support for skipping unallocated blocks, remove 
$History: * externs and replace with #include of TypesDefs.h
$History: **** V 1.2 by kfitzner ****
$History: * selfimage_readthread.cpp - 2005-12-09 2:20:47 PM - 18784 Bytes
$History: * selfimage_readthread.h - 2005-11-26 3:11:42 AM - 2356 Bytes
$History: * Remove externs, add #include of TypesDefs.h
$History: **** V 1.3 by kfitzner ****
$History: * selfimage_readthread.cpp - 2007-03-14 1:51:20 PM - 19128 Bytes
$History: * selfimage_readthread.h - 2007-03-14 1:49:00 PM - 2361 Bytes
$History: * Rename IsNextBlockEmpty() to IsNextChunkEmpty(), and 
$History: * generally change references of the word "block" to "chunk" for clarity
$History: **** Latest ** V 1.4 by kfitzner ** 2007-03-18 1:26:16 PM ****
$History: * BytesRead now read-only property, add BytesSkipped, 
$History: * raise thread priority to tpHigher
*/
//----------------------------  $NoKeywords ---------------------------------

//---------------------------------------------------------------------------
// File Notes:
//---------------------------------------------------------------------------
// 8 Sep 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// About to embark on a whole new structure for SelfImage.  Reading and
// writing are to be separated in their own threads and will be abstracted
// through the TImageBuffer class.  This read thread (read from disk/file)
// is given a buffer to write its data to.  The write thread will be given a
// buffer to read from.  This will eventually allow sticking threads in the
// middle to process the data too.  The intention with that is to allow
// compression threads (compresseion thread reads from one buffer,
// compresses, writes to another).  This will allow chaining of the threads
// together.
//
// Borland's thread object wizard dumps the following note in thread object
// units that it creates:
//
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TSelfImageReadThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//
// Only one thread at a time can use VCL - ug, what a horrible design. :p
// Ok, that means for piping data from the read/write threads back to the
// user, we want to update memory variables only and let the main thread
// actually do the UI updating.  This way we won't impact performance by
// strangling the read or write threads with linearized VCL calls.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "memlib.h"
#include "SelfImage_TypesDefs.h"
#include "SelfImage_ReadThread.h"
#include "SelfImage_Utility.h"
#include "SelfImage_Exceptions.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Construction zone ahead - this is a hardhat area.
//
__fastcall TSelfImageReadThread::TSelfImageReadThread(TImageStore *ReadStore, TImageBuffer *ImageBuffer, bool BlockRead): TSelfImageThread()
{
  this->ReadStore   = ReadStore;
  this->ImageBuffer = ImageBuffer;
  this->BlockRead   = BlockRead;
  FBytesRead = 0;
  FBytesSkipped = 0;
  Priority = tpHigher;
}  // __fastcall TSelfImageReadThread::TSelfImageReadThread(TImageStore *ReadStore, TImageBuffer *ImageBuffer): TSelfImageThread()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Function to set the thread's name for debugging purposes.  This is done
// by filling in a special structure and firing off an exception.  A
// compatible debugger knows to look in exceptions of this type and look for
// the thread's name.  A little bit of an odd way of doing things, but it
// seems to work.
//
void TSelfImageReadThread::SetName()
{
  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = "ReadThread";
  info.dwThreadID = -1;
  info.dwFlags = 0;

  __try {
    RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD),(DWORD*)&info );
  }
  __except (EXCEPTION_CONTINUE_EXECUTION) {}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Off with the thread's head!  Execute time - read the image.
// We support three different input types now - normal input for files and
// partitions, gzip chunk input, and bzip2 chunk input.  Since we want to
// keep each read loop as tight as possible, we have different read loops
// for each type.  We decide here which one to use and send control there.
//
void __fastcall TSelfImageReadThread::Execute(void)
{
  SetName();
  if (BlockRead && ReadStore->CompressionFormat == cfGzipChunk)
    GzipChunkReadLoop();
  else if (BlockRead && ReadStore->CompressionFormat == cfBzip2Chunk)
    Bzip2ChunkReadLoop();
  else
    ReadLoop();
}  // void __fastcall TSelfImageReadThread::Execute()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// The standard read loop - this is a thread designed for parallel execution,
// so we want to keep things as simple as possible here.  Allocate a buffer
// chunk, read into it, release the buffer.  Simple as that
//
void __fastcall TSelfImageReadThread::ReadLoop(void)
{
  bool bEOF = false;
  bool bReadSuccess;
  bool bSkipUnallocated = SelfImageConfig->Values["SkipUnallocated"];
  unsigned nBytesRead;


  try {
    while (!bEOF && !Terminated) {
      TBufferChunk *WriteChunk = ImageBuffer->GetChunk(csAllocWrite, &Terminated);
      if (!WriteChunk)
        continue;
      if (bSkipUnallocated && ReadStore->IsNextChunkEmpty()) {
        bReadSuccess = true;
        WriteChunk->ZeroFlag = true;
        nBytesRead = WriteChunk->Size;
        ReadStore->Seek(nBytesRead, FILE_CURRENT);
        FBytesSkipped += nBytesRead;
      } else
        bReadSuccess = ReadStore->Read(WriteChunk->Data, WriteChunk->Size, &nBytesRead);
      // If we didn't get as much data as we expected, then we're at the end of the file.  Set the EOF marker
      // in the buffer chunk so the write thread knows this is the last.
      if (bReadSuccess && WriteChunk->Size != nBytesRead) {
        WriteChunk->Size = nBytesRead;
        WriteChunk->EOF = true;
        bEOF = true;
      }  // if (nBytesRead && WriteChunk->Size != nBytesRead)
      else if (!bReadSuccess) {
        // An error occured while reading - handle this.
        throw ESelfImageFileError("Error while reading from " + ReadStore->FileName + ": " + GetLastErrorMessage());
      }  // else if (!nBytesRead)
      FBytesRead += nBytesRead;
      ImageBuffer->ReleaseChunk(WriteChunk);
    }  // while (!bEOF && !Terminated)
    FFinished = true;
  }  // try
  // C++ Builder doesn't support passing exceptions from a thread.  So, we have to catch the exception here and
  // manually pass word that an error has occured.  This is done with the ErrorFlag property.
  catch (Exception &e) {
    FErrorFlag = true;
    FErrorMessage = "Read thread encountered " + e.ClassName() + " exception: \"" + e.Message + "\"";
    FFinished = true;
  }  // catch
}  // void __fastcall TSelfImageReadThread::ReadLoop(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// The gzip block read loop.  Of the two block compression formats we
// support, this is the easier of the two because we get a size for each
// block.  Although simplicity and speed are both goals, we also want to
// make sure to be careful and check all data coming in.
//
void __fastcall TSelfImageReadThread::GzipChunkReadLoop(void)
{
  bool bReadSuccess;
  unsigned nBytesRead;
  unsigned nBlockSize;
  unsigned char *Block;

  try {
    while (!Terminated) {
      TBufferChunk *WriteChunk = ImageBuffer->GetChunk(csAllocWrite, &Terminated);
      if (!WriteChunk)
        continue;
      Block = (unsigned char *)WriteChunk->Data;
      // Read in the first 20 bytes - this is the gzip "block" header and contains the block size
      bReadSuccess = ReadStore->Read(Block, 20, &nBytesRead);
      // This is the only way we should break out of the loop normally - getting EOF with no bytes read
      if (bReadSuccess && !nBytesRead) {
        WriteChunk->Size = 0;
        WriteChunk->EOF = true;
        ImageBuffer->ReleaseChunk(WriteChunk);
        break;
      }  // if (bReadSuccess && !nBytesRead)
      // Read failure? - throw an exception
      if (!bReadSuccess)
        throw ESelfImageFileError("Error while reading a gzip block header from " + ReadStore->FileName + ": " + GetLastErrorMessage());
      // If we got some data, but not the full 20 bytes, then something is wrong.
      if (nBytesRead < 20)
        throw ESelfImageFileError("Unexpected end of data while reading a gzip block header from " + ReadStore->FileName + ".");
      // Something is also wrong if we don't see the "SI" labelled gzip extra field in the right spot in the header
      if (Block[12] != 'S' || Block[13] != 'I')
        throw ESelfImageFileError("Error while reading a gzip block header from " + ReadStore->FileName + ": Header is missing the block size field.");
      // Extract our block size from the header
      nBlockSize = *(unsigned int *)&Block[16];
      // Last check before we read the block - make sure the block size isn't bigger than our buffer chunk size
      if (nBlockSize + 20 > WriteChunk->MaxSize)
        throw ESelfImageFileError("Error while reading a gzip block from " + ReadStore->FileName + ": Compressed block is larger than the buffer chunk size setting.");
      bReadSuccess = ReadStore->Read(Block + 20, nBlockSize - 20, &nBytesRead);
      // Now we check to make sure we read the block ok
      if (!bReadSuccess)
        throw ESelfImageFileError("Error while reading a gzip block from " + ReadStore->FileName + ": " + GetLastErrorMessage());
      if (nBytesRead < nBlockSize - 20)
        throw ESelfImageFileError("Unexpected end of data while reading a gzip block from " + ReadStore->FileName + ".");
      FBytesRead += nBlockSize;
      WriteChunk->Size = nBlockSize;
      ImageBuffer->ReleaseChunk(WriteChunk);
    }  // while (!Terminated)
    FFinished = true;
  }  // try
  // C++ Builder doesn't support passing exceptions from a thread.  So, we have to catch the exception here and
  // manually pass word that an error has occured.  This is done with the ErrorFlag property.
  catch (Exception &e) {
    FErrorFlag = true;
    FErrorMessage = "Read thread encountered " + e.ClassName() + " exception: \"" + e.Message + "\"";
    FFinished = true;
  }  // catch
}  // void __fastcall TSelfImageReadThread::GzipBlockReadLoop(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// The bzip2 block read loop.  This one is a pain because there is no way
// we can embed the block sizes into each bzip2 block and still keep the
// result compatible with the actual bzip2 program.  So, we have to scan
// the data for block headers, and chunkify it that way.
//
void __fastcall TSelfImageReadThread::Bzip2ChunkReadLoop(void)
{
  bool bReadSuccess;
  unsigned nBytesRead, nSearchMarker;
  unsigned char *Block, *NextBlock, BlockHeader[10];
  int nSearchAreaSize;

  try {
    TBufferChunk *WriteChunk, *NextWriteChunk = NULL;

    // Read in the bzip2 header - all of them will be the same.
    ReadStore->Read(BlockHeader, 10, &nBytesRead);
    if (nBytesRead != 10)
      throw ESelfImageFileError("Error while reading the initizl bzip2 block header from " + ReadStore->FileName + ".");
    ReadStore->Seek(0, FILE_BEGIN);

    while (!Terminated) {

      if (NextWriteChunk)
        WriteChunk = NextWriteChunk;
      else {
        WriteChunk = ImageBuffer->GetChunk(csAllocWrite, &Terminated);
        if (!WriteChunk)
          continue;
        WriteChunk->Size = 0;
      }  // else if (!NextWriteChunk)
      Block = (unsigned char *)WriteChunk->Data;
      NextBlock = NULL;
      nSearchMarker = 10;

      // Below is the block read loop.  With each iteration we grab 64k of data at a time and search it for a bzip2 header.
      // We then keep doing this until a header is found, we run out of data, or we are about to exceed the buffer size.
      do {
        nSearchAreaSize = WriteChunk->Size - nSearchMarker;
        // If there is already enough data to search (leftover from the block before), then don't
        // grab any more just yet.  If there isn't any more data, then grab some more.
        if (nSearchAreaSize < 10) {
          bReadSuccess = ReadStore->Read(Block + WriteChunk->Size, 64 * 1024, &nBytesRead);
          if (!bReadSuccess)
            throw ESelfImageFileError("Error while reading a bzip2 block from " + ReadStore->FileName + ": " + GetLastErrorMessage());
          // If we just received zero bytes, it means we've run out of data without finding the next block header.
          // In that case, we're done.  Set the EOF marker in this chunk, release it, and break out of the block read loop.
          if (!nBytesRead) {
            WriteChunk->EOF = true;
            FBytesRead += WriteChunk->Size;
            ImageBuffer->ReleaseChunk(WriteChunk);
            break;
          }  // if (nBytesRead < 64 * 1024)
          WriteChunk->Size += nBytesRead;
          // If we've reached the limit of what we can store and haven't found the header yet, then we've got a problem
          if (WriteChunk->Size > WriteChunk->MaxSize - 64 * 1024)
            throw ESelfImageFileError("Error while reading a bzip2 block from " + ReadStore->FileName + ": Block exceeds maximum bzip2 compressed block size.");
        }  // if (nSearchAreaSize < 10)
        // Look for the block header.  We have to do this a lot, so FindString() uses a modified Boyer-Moore algorithm
        NextBlock = FindString(Block + nSearchMarker, WriteChunk->Size - nSearchMarker, BlockHeader);
        nSearchMarker = WriteChunk->Size - 9;
      } while (!NextBlock);

      // If we've found the next block header, then NextBlock is a pointer to it.  Copy anything from NextBlock
      // on to a new buffer chunk, resize the current one to exclude the next block's data, and release this block's
      // buffer chunk.
      if (NextBlock) {
        NextWriteChunk = ImageBuffer->GetChunk(csAllocWrite, &Terminated);
        if (!NextWriteChunk)
          break;
        NextWriteChunk->Size = WriteChunk->Size - (NextBlock - Block);
        memcpy_optimized(NextWriteChunk->Data, NextBlock, NextWriteChunk->Size);
        WriteChunk->Size -= NextWriteChunk->Size;
        FBytesRead += WriteChunk->Size;
        ImageBuffer->ReleaseChunk(WriteChunk);
      }  // if (NextBlock)
      // If we didn't find the next block header, it means we're at the end.  The buffer chunk will have already
      // been released above, so just break out of the loop.
      else
        break;
    }  // while (!Terminated)
    FFinished = true;
  }  // try
  // C++ Builder doesn't support passing exceptions from a thread.  So, we have to catch the exception here and
  // manually pass word that an error has occured.  This is done with the ErrorFlag property.
  catch (Exception &e) {
    FErrorFlag = true;
    FErrorMessage = "Read thread encountered " + e.ClassName() + " exception: \"" + e.Message + "\"";
    FFinished = true;
  }  // catch
}  // void __fastcall TSelfImageReadThread::Bzip2BlockReadLoop(void)
//---------------------------------------------------------------------------


//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_Buffer.cpp - TImageBuffer class
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
// VCS: $Version: 1 $ $Revision: 4 $
/*
$History: **** V 0.1 by kfitzner ****
$History: * selfimage_timagebuffer.cpp - 2005-11-07 2:39:38 AM - 11089 Bytes
$History: * selfimage_timagebuffer.h - 2005-11-07 2:39:38 AM - 3741 Bytes
$History: * Initial check-in
$History: **** V 0.2 by kfitzner ****
$History: * selfimage_timagebuffer.cpp - 2005-11-12 5:22:11 PM - 11355 Bytes
$History: * selfimage_timagebuffer.h - 2005-11-12 5:22:11 PM - 4005 Bytes
$History: * Typo in copyright area + change in program description
$History: **** V 0.3 by kfitzner ****
$History: * selfimage_timagebuffer.cpp - 2005-11-22 10:08:29 PM - 15798 Bytes
$History: * selfimage_timagebuffer.h - 2005-11-22 10:08:29 PM - 4507 Bytes
$History: * Add support for sister buffers
$History: **** V 0.4 by kfitzner ****
$History: * selfimage_timagebuffer.cpp - 2005-11-23 2:03:12 PM - 17083 Bytes
$History: * selfimage_timagebuffer.h - 2005-11-23 2:03:13 PM - 4833 Bytes
$History: * Fix allocation failure when multiple TImageBuffers 
$History: * are used - was due to identically named event objects.
$History: **** V 0.5 by kfitzner ****
$History: * selfimage_timagebuffer.cpp - 2005-11-25 8:49:16 PM - 17778 Bytes
$History: * selfimage_timagebuffer.h - 2005-11-25 8:49:16 PM - 5131 Bytes
$History: * Didn't make a destructor to deallocate the buffers - whoops
$History: **** V 0.6 by kfitzner ****
$History: * selfimage_timagebuffer.cpp - 2005-11-25 9:02:21 PM - 18087 Bytes
$History: * selfimage_timagebuffer.h - 2005-11-25 9:02:21 PM - 5687 Bytes
$History: * Fix bug with input files being a multiple of the buffer 
$History: * chunk size causing the output file being one buffer chunk too large.
$History: **** V 1.0 by kfitzner ****
$History: * selfimage_timagebuffer.cpp - 2005-11-25 11:28:36 PM - 18376 Bytes
$History: * selfimage_timagebuffer.h - 2005-11-25 11:28:36 PM - 5926 Bytes
$History: * Fix some incorrect code comments
$History: **** V 1.1 by kfitzner ****
$History: * selfimage_timagebuffer.cpp - 2005-12-02 2:04:00 AM - 18689 Bytes
$History: * selfimage_timagebuffer.h - 2005-12-02 2:04:00 AM - 6192 Bytes
$History: * Fix memory leak (Chunks and Chunkstates arrays not deleted)
$History: **** V 1.2 by kfitzner ****
$History: * selfimage_timagebuffer.cpp - 2005-12-02 5:56:47 AM - 18918 Bytes
$History: * selfimage_timagebuffer.h - 2005-12-02 5:56:47 AM - 6422 Bytes
$History: * Reduce maximum wait time
$History: **** V 1.3 by kfitzner ****
$History: * selfimage_timagebuffer.cpp - 2005-12-04 10:36:29 AM - 19174 Bytes
$History: * selfimage_timagebuffer.h - 2005-12-04 10:36:29 AM - 6769 Bytes
$History: * Add empty block "zero" flag
$History: **** Latest ** V 1.4 by kfitzner ** 2005-12-21 12:10:07 AM ****
$History: * Buffer chunks that contain highly compressed areas 
$History: * can take a long time to write out with NBD - in debug 
$History: * mode, GetChunk() was reaching its maximal wait/retry 
$History: * count - upped from 1000 to 10000
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// File Notes:
//---------------------------------------------------------------------------
// 13 Sep 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// The TImageBuffer class is used to hold and arbitrate allocation of a group
// memory of "chunks" used for buffering.  A circular buffer is used to hold
// pointers to the buffer chunks.  These chunks are allocated off the bottom
// for writing and off the top for reading.
//
// Reading and writing, as referred to in this unit, are relative to THIS
// class.  Writing means writing TO a buffer chunk - reading is reading from
// it.  Thus, a thread that is reading data will allocate a write chunk as a
// place to write its data to.
//
// Care needs to be taken here, as this class is a memory buffer arbitration
// class for multiple threads.  Thread safety is important.  An instance of
// TMultiReadExclusiveWriteSynchronizer (that's a mouthfull!) is used instead
// of critical sections.  Most of the time there will be no buffers free to
// allocate and the allocation method will block.  In this case, since we're
// not writing, there is no need to completely linearize the method with a
// critical section.  The multi-read synchronizer lets us get away with this.
//
// Care also needs to be taken about the order that chunks are allocated out.
// Once multi-threaded compression is implemented, this becomes possible:
//   - Compression thread #1 allocates chunk 1 for writing to
//   - Compression thread #2 allocates chunk 2 for writing to
//   - Compression thread #2 completes chunk 2 and releases it
//   - Compression thread #1 completes chunk 1 and releases it
// We could put serial numbers inside the chunks, but then we would have to
// deal with reordering them.  Instead, we guarantee that chunks will be
// allocated out in the same order every time.  The same order they are
// allocated out for writing will be the same order they are allocated out
// for reading.  Now it doesn't matter if a 'later' block is processed
// before an 'earlier' block - they will be served out in the correct order.
//---------------------------------------------------------------------------
// 22 Nov 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// I was implementing compression threads and I found out I hadn't thought
// quite as far enough ahead as I thought I had.  The idea with using the
// TMultiReadExclisiveWriteSynchronizer was to avoid having to use a
// critical section.  Problem is, compression needs two buffere - one to
// read the raw data from and one to read the compressed data to.  At first
// I thought I'd have to use a critical section to allocate both at the same
// time, but I didn't like that idea because I'd gone to a lot of trouble
// before to avoid it.  What I've done is to implement the idea of allocation
// sisters.  When one buffer is being used by a thread as input and another
// buffer for output, you can tell the input buffer that it has an allocation
// sister.  Every time a buffer chunk is allocated for reading from the
// buffer with the sister set, once it has the write lock it will call the
// allocate method in the second buffer to allocate a buffer chunk to write
// to.  Since it's inside the write lock for the first buffer's allocation,
// there's no need for a critical section.
//
// To be clear, here's how an allocation works:
// - A thread asks the image buffer for a chunk to read from.
// - Image buffer gets a "read lock" on its internal stuctures and checks
//   to see if there are any free chunks.
// - If there are no free chunks ready, then the image buffer waits for
//   one.
// - Once a chunk is free, the image buffer gets a "write lock" on its
//   internal structures to prepare to mark the chunk as allocated.
// - Once the chunk is allocated, and before the write lock is terminated,
//   the image buffer calls its sister image buffer to allocate a chunk
//   to write to.
// - A pointer to the chunk that the sister image buffer returns as the
//   allocated write chunk is stored in the original allocated chunk.
//
// Thus, the calling thread gets a twofor - two chunks for the price of one.
// The system is set up like this:
//
//   TImageBuffer *InputBuffer = new TImageBuffer(blahblah);
//   TImageBuffer *CompressionBuffer = new TImageBuffer(blahblah);
//   TBufferChunk *InputChunk, *CompressionChunk;
//
//   InputBuffer->AllocationSister = CompressionBuffer;
//   InputChunk = InputBuffer->GetChunk(csAllocRead, &Terminated);
//   OutputChunk = InputChunk->Sister;
//
// Right now, it's set up so that it only chains the allocation when the
// first buffer is called to request a chunk for reading.  There is no reason
// a write allocation - if you need two chunks at once, it's because you need
// to read from one in order to write to the other.
//
// Calling ReleaseChunk() to release a chunk that was allocated together with
// a sister will cause the sister to be released as well.  Once this is done,
// the chunk's sister setting is cleared.
//
// This isn't quite as good as having a double buffer where both the input
// and output side are allocated together, but it's better than using a
// critical section.
//---------------------------------------------------------------------------
// 22 Nov 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// Learned something new today - all TEvent objects must be created with
// unique names.  If you have two with the same name, the system will only
// find the first one.  Borland's help file for TEvent suggests that the
// call to initialize a new TEvent will fail to set the event's handle if
// it's name matches another event, but this isn't true either.  Each image
// buffer's events are now have the IntToHex() hex string of the buffer
// object in their name.  String matching to find an event - you'd think it
// would use the handle. :p
//---------------------------------------------------------------------------
#include <vcl.h>
#include <Math.hpp>
#pragma hdrstop

#include "SelfImage_Exceptions.h"
#include "SelfImage_Utility.h"
#include "SelfImage_TImageBuffer.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TBufferChunk class
//

//---------------------------------------------------------------------------
// TBufferChunk constructor
//
TBufferChunk::TBufferChunk(int nSize, int nIndex) {

  FData = VirtualAlloc(NULL, nSize, MEM_COMMIT, PAGE_READWRITE);
  if (!FData)
    throw ESelfImageAllocError("SelfImage failed to allocate " + String(nSize) + " bytes of memory: " + GetLastErrorMessage());

  FMaxSize = nSize;
  FUsedSize = SIZE_NOT_SET;
  FEOF = false;
  FIndex = nIndex;
  FZeroFlag = false;
  FSister = NULL;
}  // TBufferChunk::TBufferChunk(int nSize, int nIndex)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TBufferChunk destructor
//
TBufferChunk::~TBufferChunk() {
  VirtualFree(FData, 0, MEM_RELEASE);
}  // TBufferChunk::~TBufferChunk()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TImageBuffer class
//

//---------------------------------------------------------------------------
// TImageBuffer constructor
//
__fastcall TImageBuffer::TImageBuffer(int Size, int Count) {

  FAllocSister = NULL;

  Sync = new TMultiReadExclusiveWriteSynchronizer();

  ChunkSize = Size;
  ChunkCount = 1 << (int)Ceil(Log2(Count));
  IndexMask = ChunkCount - 1;
  Chunks = new TBufferChunk *[ChunkCount];
  ChunkStates = new TBufferChunkState [ChunkCount];

  NextReadChunkIndex = NextWriteChunkIndex = 0;

  // Create all the buffer chunks
  for (unsigned n = 0; n < ChunkCount; n++) {
    Chunks[n] = new TBufferChunk(ChunkSize, n);
    ChunkStates[n] = csFree;
  }  // for (unsigned n = 0; n < nChunkCount; n++)

  // Initialize our event objects
  ReadChunkReady  = new TEvent(NULL, true, false, IntToHex((int)this, 8) + ":Read");
  if (!ReadChunkReady->Handle)
    throw ESelfImageSystemError("Failed to create image buffer read event object.");
  WriteChunkReady = new TEvent(NULL, true, false, IntToHex((int)this, 8) + ":Write");
  if (!WriteChunkReady->Handle)
    throw ESelfImageSystemError("Failed to create image buffer write event object.");

}  // TImageBuffer::TImageBuffer(int nChunkSize, int nChunkCount)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Destructor - free up all the resources please
//
__fastcall TImageBuffer::~TImageBuffer()
{
  delete ReadChunkReady;
  delete WriteChunkReady;
  for (unsigned n = 0; n < ChunkCount; n++)
    delete Chunks[n];
  delete [] Chunks;
  delete [] ChunkStates;
}  // __fastcall TImageBuffer::~TImageBuffer()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Allocate a chunk.  This has to be thread safe!
//
TBufferChunk * __fastcall TImageBuffer::GetChunk(TBufferChunkState Purpose, bool *TerminationFlag) {
  unsigned *pnIndexMarker;
  TBufferChunkState DesiredInitialChunkState, NewChunkState, ChainPurpose;
  TEvent *WaitObject;
  bool bChunkObtained = false;
  TImageBuffer *SisterImageBuffer;
  TBufferChunk *retval = NULL;
  #ifdef _DEBUG
  int nRaceCounter = 1000;
  #endif

  // Set up the function for either read or write chunk allocation
  if (Purpose == csAllocRead) {
    pnIndexMarker = &NextReadChunkIndex;
    DesiredInitialChunkState = csFinishedWrite;
    ChainPurpose = csAllocWrite;
    SisterImageBuffer = FAllocSister;
    NewChunkState = csAllocRead;
    WaitObject = ReadChunkReady;
  } else if (Purpose == csAllocWrite) {
    pnIndexMarker = &NextWriteChunkIndex;
    DesiredInitialChunkState = csFree;
    SisterImageBuffer = NULL;
    NewChunkState = csAllocWrite;
    WaitObject = WriteChunkReady;
  }  // if (Purpose == ...)

  // Loop until we get an allocation
  while (!bChunkObtained) {
    Sync->BeginRead();
    if (ChunkStates[*pnIndexMarker] != DesiredInitialChunkState) {
      // No free chunks - wait
      Sync->EndRead();
      WaitObject->WaitFor(1000);
      // Check to see if the calling thread is being told to terminate.  If so, then return back.
      if (*TerminationFlag)
        return(NULL);
    }  // if (ChunkStates[*pnIndexMarker] != DesiredInitialChunkState)
    else {  // if (ChunkStatus[*IndexMarker] == DesiredInitialChunkState)
      // There IS a free chunk, so get a write lock
      Sync->EndRead();
      Sync->BeginWrite();
      // Another thread could have been waiting (eventually with compression there will be multiple reader/writer threads)
      // so even though we checked, now we have the write lock we have to check again to ensure that some other thread didn't
      // snatch the chunk before we got here.
      if (ChunkStates[*pnIndexMarker] == DesiredInitialChunkState) {
        // Free chunk is still there and we have a write lock, so we're good to go - allocate it
        retval = Chunks[*pnIndexMarker];
        ChunkStates[*pnIndexMarker] = NewChunkState;
        *pnIndexMarker = (*pnIndexMarker + 1) & IndexMask;
        // If the next chunk in the ring isn't free then it means we're full, so make sure the wait event is reset
        if (ChunkStates[*pnIndexMarker] != DesiredInitialChunkState)
          WaitObject->ResetEvent();
        // Check if we are chaining this allocation to a sister TImageBuffer object.  If we are, then we set this
        // buffer chunk's sister chunk to be a newly allocated buffer chunk in the sister TImageBuffer.  This
        // lets us allocate two buffer chunks together for use with filter threads (compression)
        if (SisterImageBuffer)
          retval->Sister = SisterImageBuffer->GetChunk(ChainPurpose, TerminationFlag);
        // Set the flag stating we have our chunk
        bChunkObtained = true;
      }  // if (ChunkStates[*pnIndexMarker] == DesiredInitialChunkState)
      Sync->EndWrite();
    }  // if (ChunkStatus[*IndexMarker] == DesiredInitialChunkState)

    #ifdef _DEBUG
    // Check for a race or some other bug - if we've looped a thousand times with no result, there's a problem
    if (!nRaceCounter--)
      throw ESelfImageAllocError("Buffer chunk allocation failed - possible race condition.");
    #endif

    // If we are looping, it's probably because some other thread has allocated the chunk we wanted out from
    // under us, but hasn't reset WaitObject (ReadChunkReady or WriteChunkReady) yet - so let's give up the
    // rest of this thread's timeslice before we loop so that the other thread can finish up.
    if (!bChunkObtained)
      Sleep(0);
  }  // while (!bChunkObtained)

  return retval;
}  // TBufferChunk *TImageBuffer::GetChunk(TBufferChunkAlloc Purpose)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Release a chunk
//
bool __fastcall TImageBuffer::ReleaseChunk(TBufferChunk *Chunk) {
  unsigned nChunkIndex;
  unsigned nMarkerIndex;
  TBufferChunkState StateAfterRelease;
  TEvent *WaitObject;

  nChunkIndex = Chunk->Index;
  // We're going to have to perform an update, so get a write lock right from the beginning - saves relocking
  Sync->BeginWrite();

  // Free up this chunk's sister, if it has one
  if (Chunk->Sister) {
    if (FAllocSister)
      FAllocSister->ReleaseChunk(Chunk->Sister);
    else
      throw ESelfImageAllocError("Buffer chunk has an allocation sister set, but the ImageBuffer has no sister.  This is probably a programming error - please report it to the maintainers.");
    Chunk->Sister = NULL;
  }  // if (Chunk->Sister)

  // Configure the function for releasing a chunk previously allocated for either reading or writing
  if (ChunkStates[nChunkIndex] == csAllocRead) {
    StateAfterRelease = csFree;
    nMarkerIndex = NextWriteChunkIndex;
    WaitObject = WriteChunkReady;
    // This chunk is now free for reuse, so reset it
    Chunk->Reset();
  } else if (ChunkStates[nChunkIndex] == csAllocWrite) {
    StateAfterRelease = csFinishedWrite;
    nMarkerIndex = NextReadChunkIndex;
    WaitObject = ReadChunkReady;
  }  // if (ChunkStates[nIndex] = ...)

  ChunkStates[nChunkIndex] = StateAfterRelease;
  // If we've just released the chunk that is pointed to by the marker (the marker that is relevant to the state that
  // the chunk is now), then we signal correct event in case some other thread is waiting for it.
  if (nChunkIndex == nMarkerIndex)
    WaitObject->SetEvent();
  Sync->EndWrite();

  return true;
}  // bool __fastcall ReleaseChunk(TBufferChunk *Chunk)
//---------------------------------------------------------------------------


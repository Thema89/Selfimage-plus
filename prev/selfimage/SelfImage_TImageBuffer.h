//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_Buffer.h - TImageBuffer class header
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
//----------------------------  $NoKeywords ---------------------------------
#ifndef SelfImage_TImageBufferH
#define SelfImage_TImageBufferH
//---------------------------------------------------------------------------
#include <SyncObjs.hpp>
//---------------------------------------------------------------------------
#define SIZE_NOT_SET 0xFFFFFFFF
//---------------------------------------------------------------------------
// TBufferChunk class - each instance of this class is one of a group of
// buffer chunks.  These chunks hold data as they pass from the read to the
// write threads.  Allocating chunks is done through the main TImageBuffer
// class.
// Memory used in these chunks is guaranteed to be alligned on a page
// boundary - thus on x86 it will be a multiple of any known disk sector
// size (as required when unbuffered file I/O is performed).
//
class TBufferChunk {
  public:
    TBufferChunk(int nSize, int nIndex);
    ~TBufferChunk();
    void inline __fastcall Reset(void) { FEOF = false; FUsedSize = SIZE_NOT_SET; FZeroFlag = false; }
    __property unsigned MaxSize = {read = FMaxSize};
    __property unsigned Size = {read = GetSize, write = FUsedSize};
    __property bool     EOF = {read = FEOF, write = FEOF};
    __property void    *Data  = {read = FData};
    __property int      Index = {read = FIndex};
    __property bool     ZeroFlag = {read = FZeroFlag, write=FZeroFlag};
    __property TBufferChunk *Sister = {read = FSister, write = FSister};

  private:
    unsigned inline __fastcall GetSize() { return (FUsedSize!=SIZE_NOT_SET)?FUsedSize:FMaxSize; };
    unsigned FMaxSize;
    unsigned FUsedSize;
    bool FEOF;
    void *FData;
    int FIndex;
    bool FZeroFlag;
    TBufferChunk *FSister;
};  // class TBufferChunk
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TImageBuffer class - this class acts as a buffer in between two threads
//
enum TBufferChunkState {csFree, csAllocRead, csAllocWrite, csFinishedWrite};

class TImageBuffer {
  public:
    __property TImageBuffer *AllocationSister = {read=FAllocSister, write=FAllocSister};

    __fastcall TImageBuffer(int ChunkSize, int ChunkCount);
    __fastcall ~TImageBuffer();
    TBufferChunk * __fastcall GetChunk(TBufferChunkState Purpose, bool *TerminationFlag);
    bool _fastcall ReleaseChunk(TBufferChunk *Chunk);

  private:
    TImageBuffer *FAllocSister;
    TBufferChunk **Chunks;
    TBufferChunkState *ChunkStates;
    unsigned ChunkCount;         // Must be power of two
    unsigned ChunkSize;
    unsigned IndexMask;          // Mask for chunk counters - used to wrap the ring buffer
    unsigned NextReadChunkIndex, NextWriteChunkIndex;
    TMultiReadExclusiveWriteSynchronizer *Sync;
    TEvent *ReadChunkReady, *WriteChunkReady;
};  // class TImageBuffer
//---------------------------------------------------------------------------
#endif

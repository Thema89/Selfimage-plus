//---------------------------------------------------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_CompressionThread.cpp - Data compression thread class header
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
#ifndef SelfImage_CompressionThreadH
#define SelfImage_CompressionThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "SelfImage_TImageBuffer.h"
#include "SelfImage_TSelfImageThread.h"
//---------------------------------------------------------------------------
#define XTRA_BLOCKSIZE "SI\x08\x00\x01\x02\x03\x04"
//---------------------------------------------------------------------------
enum TCompressionMethod {cmGzip1, cmGzip9, cmBzip2};
//---------------------------------------------------------------------------
class TSelfImageCompressionThread : public TSelfImageThread
{
  typedef struct tagTHREADNAME_INFO {
    DWORD dwType;     // must be 0x1000
    LPCSTR szName;    // pointer to name (in user addr space)
    DWORD dwThreadID; // thread ID (-1=caller thread)
    DWORD dwFlags;    // reserved for future use, must be zero
  } THREADNAME_INFO;

  public:
    __fastcall TSelfImageCompressionThread(TCompressionMethod CompressionMethod, int ChunkSize, TImageBuffer *InputBuffer, TImageBuffer *OutputBuffer);

  protected:
    TCompressionMethod CompressionMethod;
    int CompressionLevel;
    TImageBuffer *InputBuffer, *OutputBuffer;

    void __fastcall Execute();

  private:
    unsigned char *Precompressed;
    unsigned int PrecompressedSize;
    void SetName();
    void Compress(TBufferChunk *Source, TBufferChunk *Target);
};  // class TSelfImageCompressionThread
//---------------------------------------------------------------------------
#endif
 
//---------------------------------------------------------------------------
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
#ifndef SelfImage_DecompressionThreadH
#define SelfImage_DecompressionThreadH
//---------------------------------------------------------------------------
#include "SelfImage_TSelfImageThread.h"
#include "SelfImage_TypesDefs.h"
#include "SelfImage_TImageBuffer.h"
//---------------------------------------------------------------------------
class TSelfImageDecompressionThread : public TSelfImageThread
{
  typedef struct tagTHREADNAME_INFO {
    DWORD dwType;     // must be 0x1000
    LPCSTR szName;    // pointer to name (in user addr space)
    DWORD dwThreadID; // thread ID (-1=caller thread)
    DWORD dwFlags;    // reserved for future use, must be zero
  } THREADNAME_INFO;

  public:
    __fastcall TSelfImageDecompressionThread(TCompressionFormat CompressionFormat, TImageBuffer *InputBuffer, TImageBuffer *OutputBuffer);

  protected:
    TImageBuffer *InputBuffer, *OutputBuffer;
    TCompressionFormat CompressionFormat;

    void __fastcall Execute(void);
    void __fastcall ChunkDecompressLoop(void);
    void __fastcall StreamDecompressLoop(void);

  private:
    bool ChunkDecompress;
    void SetName();
    bool InitializeStream;
    void __fastcall DecompressChunk(TBufferChunk *Source, TBufferChunk *Target);
    void __fastcall DecompressStream(TBufferChunk *Source);
};  // class TSelfImageCompressionThread
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_ReadThread.h - Data read thread class header
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
#ifndef SelfImage_ReadThreadH
#define SelfImage_ReadThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "SelfImage_TImageStore.h"
#include "SelfImage_TImageBuffer.h"
#include "SelfImage_TSelfImageThread.h"
//---------------------------------------------------------------------------
class TSelfImageReadThread : public TSelfImageThread
{
  typedef struct tagTHREADNAME_INFO {
    DWORD dwType;     // must be 0x1000
    LPCSTR szName;    // pointer to name (in user addr space)
    DWORD dwThreadID; // thread ID (-1=caller thread)
    DWORD dwFlags;    // reserved for future use, must be zero
  } THREADNAME_INFO;

  public:
    __property __int64 BytesSkipped = {read = FBytesSkipped};
    __fastcall TSelfImageReadThread(TImageStore *ReadStore, TImageBuffer *Buffer, bool BlockRead);

  protected:
    __int64 FBytesSkipped;
    TImageBuffer *ImageBuffer;
    TImageStore  *ReadStore;
    bool BlockRead;
    void __fastcall Execute(void);
    void __fastcall ReadLoop(void);
    void __fastcall GzipChunkReadLoop(void);
    void __fastcall Bzip2ChunkReadLoop(void);

  private:
    void SetName();
};  // class TSelfImageReadThread
//---------------------------------------------------------------------------
#endif

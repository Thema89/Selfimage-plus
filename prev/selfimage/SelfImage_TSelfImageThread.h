//---------------------------------------------------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_CompressionThread.cpp - SelfImage base thread class
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
#ifndef SelfImage_TSelfImageThreadH
#define SelfImage_TSelfImageThreadH
//---------------------------------------------------------------------------
class TSelfImageThread : public TThread
{
  public:
    __property __int64 BytesRead = {read = FBytesRead};
    __property __int64 BytesWritten = {read = FBytesWritten};
    __property bool Finished = {read = FFinished};
    __property bool ErrorFlag = {read = FErrorFlag};
    __property AnsiString ErrorMessage = {read = FErrorMessage};

    __fastcall TSelfImageThread() : TThread(true) { FFinished = FErrorFlag = false; }


  protected:
    __int64 FBytesRead;
    __int64 FBytesWritten;
    bool FFinished;
    bool FErrorFlag;
    AnsiString FErrorMessage;
};  // class TSelfImageThread
//---------------------------------------------------------------------------
#endif


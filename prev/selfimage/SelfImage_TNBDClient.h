//---------------------------------------------------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_TNBDClient.cpp - TNBDClient class header
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
#ifndef SelfImage_TNBDClientH
#define SelfImage_TNBDClientH
//---------------------------------------------------------------------------
#include <ScktComp.hpp>
//---------------------------------------------------------------------------
// First of all, a basic TInetClient, which is a less stupid decendant of
// TClientSocket.  Borland offers no basic internet components with truly
// blocking behavior.  Really, it's true.  Even if you tell a TClientSocket
// that you want it to block, the reads won't block.  TPowersock only works
// with string data.  Could use Indy, but the Indy code ends up copying every
// byte of data at least twice.  And it's way too bulky.  So, here we are...
//
class TInetClient : public TClientSocket
{
  public:
    __property int Timeout = {read = FTimeout, write = FTimeout};

    __fastcall TInetClient(TComponent *Owner);
    __fastcall ~TInetClient();
    void __fastcall Open(void);
    void __fastcall Close(void);
    int __fastcall Read(void *Buffer, int Count);
    void __fastcall Write(void *Buffer, int Count) { SockStream->WriteBuffer(Buffer, Count); };
    __int32 __fastcall ReadInt32(bool Convert = true);
    __int64 __fastcall ReadInt64(bool Convert = true);
    void __fastcall WriteInteger(__int32 Value, bool Convert = true);
    void __fastcall WriteInteger(__int64 Value, bool Convert = true);

  private:
    int FTimeout;
    bool bOpened;
    TWinSocketStream *SockStream;
};  // class TTimeoutTCPConnection : public TIdTCPConnection
//---------------------------------------------------------------------------
// Some NBD defines
//
#define NBD_MAGIC_SIGNATURE   "NBDMAGIC"
#define NBD_MAGIC_CONNECT     0x0000420281861253ui64
#define NBD_MAGIC_REQUEST     0x25609513
#define NBD_MAGIC_REPLY       0x67446698
//---------------------------------------------------------------------------
class TNBDClient : public TInetClient
{
  // First, some structures that are needed for communicating with an NBD server
  #pragma pack(push,1)
  struct TNBDConnectionHeader {
    char Signature[8];
    unsigned __int64 Magic;
    unsigned __int64 Size;
    char Zeros[128];
  };
  #pragma pack(pop)
  enum TNBDCommand {nbdRead = 0, nbdWrite = 1, nbdDisconnect = 2};

  public:
    __property __int64 Size = {read = FSize};
    __property bool Connected = {read = FConnected};
    __property int TransactionSize = {read = FMaxTransaction, write = FMaxTransaction};

    __fastcall TNBDClient(TComponent *Owner);
    __fastcall ~TNBDClient();
    void __fastcall Open(void);
    void __fastcall Close(void);
    int __fastcall Read(__int64 Position, void *Buffer, int Size);
    int __fastcall Write(__int64 Position, void *Buffer, int Count);

  protected:
    void __fastcall DoTransaction(TNBDCommand Type, __int64 Position, int Length, void *Buffer);

  private:
    __int64 FSize;
    bool FConnected;
    int FMaxTransaction;
    __int64 RequestHandle;


};  // class TNBCClient : public TInetClient
//---------------------------------------------------------------------------
// Network read error
//
class EInetReadError : public Exception { public:  __fastcall EInetReadError(const AnsiString Msg): Exception(Msg) {}; };
//---------------------------------------------------------------------------
// NBD protocol errors
//
class ENBDConnectionError : public Exception { public:  __fastcall ENBDConnectionError(const AnsiString Msg): Exception(Msg) {}; };
class ENBDReadError       : public Exception { public:  __fastcall ENBDReadError(const AnsiString Msg): Exception(Msg) {}; };
class ENBDWriteError      : public Exception { public:  __fastcall ENBDWriteError(const AnsiString Msg): Exception(Msg) {}; };
//---------------------------------------------------------------------------
#ifndef ntohll
#define ntohll(x) (((__int64)(ntohl((int)((x << 32) >> 32))) << 32) | (unsigned int)ntohl(((int)(x >> 32))))
#endif
#ifndef htonll
#define htonll(x) ntohll(x)
#endif
//---------------------------------------------------------------------------
#endif

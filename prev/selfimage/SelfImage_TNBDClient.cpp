//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_TNBDClient.cpp - TNBDClient (Network Block Device client) class
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
// VCS: $Version: 1 $ $Revision: 2 $
/*
$History: **** V 1.0 by kfitzner ****
$History: * selfimage_tnbdclient.cpp - 2005-12-08 4:11:56 PM - 13138 Bytes
$History: * selfimage_tnbdclient.h - 2005-12-08 3:48:02 PM - 5114 Bytes
$History: * Initial check-in
$History: **** V 1.1 by kfitzner ****
$History: * selfimage_tnbdclient.cpp - 2005-12-09 2:20:19 PM - 13400 Bytes
$History: * selfimage_tnbdclient.h - 2005-12-09 10:25:26 AM - 5189 Bytes
$History: * Add support for configurable transaction size
$History: **** Latest ** V 1.2 by kfitzner ** 2005-12-10 4:03:00 PM ****
$History: * Fix problem where some reads won't time out (windows 
$History: * select on a closed socket doesn't return an error)
*/
//----------------------------  $NoKeywords ---------------------------------

//---------------------------------------------------------------------------
// File Notes:
//---------------------------------------------------------------------------
// 6 Dec 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// NBD is a very simple network protocol aimed at allowing one computer
// to share a raw block device with another computer.  Data is shared at a
// very low level, aimed at allowing the client to mount the shared device.
// We are interested in using this as a way to make a disk image of a disk
// on a remote system.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelfImage_Exceptions.h"
#include "SelfImage_TNBDClient.h"
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TInetClient
//

//---------------------------------------------------------------------------
// Constructor
//
__fastcall TInetClient::TInetClient(TComponent *Owner) : TClientSocket(Owner)
{
  ClientType = ctBlocking;
  SockStream = NULL;
  FTimeout = 5000;
  bOpened = false;
}  // __fastcall TNBDClient::TNBDClient(TComponent *Owner) : TClientSocket(Owner)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Destructor
//
__fastcall TInetClient::~TInetClient()
{
  if (SockStream)
    delete SockStream;
}  // __fastcall TNBDClient::~TNBDClient()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Open a connection
//
void __fastcall TInetClient::Open(void)
{
  TClientSocket::Open();
  if (SockStream)
    delete SockStream;
  SockStream = new TWinSocketStream(Socket, Timeout);
  bOpened = true;
}  // void __fastcall TNBDClient::Open(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Close a connection
//
void __fastcall TInetClient::Close(void)
{
  if (SockStream)
    delete SockStream;
  SockStream = NULL;
  TClientSocket::Close();
  bOpened = false;
}  // void __fastcall TInetClient::Close(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// The help for TClientSocket/TWinSocketStream claim that they can be used
// in a blocking manner.  They will block on connect, but they offer no
// blocking reads.  This corrects that, blocking until a timeout occurs.
//
int __fastcall TInetClient::Read(void *Buffer, int Count)
{
  unsigned char *Target = (unsigned char *)Buffer;
  int BytesRead = 0, ret;

  if (!bOpened)
    throw EInetReadError("Network read error - an attempt was made to read from a socket that wasn't opened.");

  while (BytesRead < Count && SockStream->WaitForData(Timeout)) {
    ret = SockStream->Read(Target + BytesRead, Count - BytesRead);
    // If we didn't time out (WaitForData() returned true) and we didn't read any data, then the connection has been terminated
    if (!ret)
      throw EInetReadError("Network read error - the remote host closed the connection.");
    BytesRead += ret;
  }  // while (BytesRead < Count && SockStream->WaitForData(Timeout))
  return BytesRead;
}  // int __fastcall TNBDClient::Read(void *Buffer, int Count)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Read a 32-bit integer
//
__int32 __fastcall TInetClient::ReadInt32(bool Convert)
{
  __int32 retval;
  if (Read(&retval, sizeof(retval)) != sizeof(retval))
    throw EInetReadError("Timeout while attempting to read " + String(sizeof(retval)) + " bytes from " + Host + ".");
  if (Convert)
    return ntohl(retval);
  else
    return retval;
}  // __int32 __fastcall TInetClient::ReadInt32(bool Convert)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Read a 64-bit integer
//
__int64 __fastcall TInetClient::ReadInt64(bool Convert)
{
  __int64 retval;
  if (Read(&retval, sizeof(retval)) != sizeof(retval))
    throw EInetReadError("Timeout while attempting to read " + String(sizeof(retval)) + " bytes from " + Host + ".");
  if (Convert)
    return ntohll(retval);
  else
    return retval;
}  // __int32 __fastcall TInetClient::ReadInt32(bool Convert)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Write a 32-bit integer
//
void __fastcall TInetClient::WriteInteger(__int32 Value, bool Convert)
{
  if (Convert)
    Value = htonl(Value);
  Write(&Value, sizeof(Value));
}  // void __fastcall TInetClient::WriteInt(__int32 Value, bool Convert)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Write a 64-bit integer
//
void __fastcall TInetClient::WriteInteger(__int64 Value, bool Convert)
{
  if (Convert)
    Value = htonll(Value);
  Write(&Value, sizeof(Value));
}  // void __fastcall TInetClient::WriteInt(__int32 Value, bool Convert)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TNBDClient
//

//---------------------------------------------------------------------------
// Constructor
//
__fastcall TNBDClient::TNBDClient(TComponent *Owner) : TInetClient(Owner)
{
  RequestHandle = 0;
  FMaxTransaction = 0x100000;
}  // void __fastcall TNBDClient::TNBDClient(TComponent *Owner) : TInetClient(Owner)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Destructor
//
__fastcall TNBDClient::~TNBDClient()
{
  if (FConnected)
    Close();
}  // __fastcall TNBDClient::~TNBDClient()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Send out an NBD request, and wait for a reply.
//
void __fastcall TNBDClient::DoTransaction(TNBDCommand Type, __int64 Position, int Length, void *Buffer)
{
  __int32 ReplyMagic, ReplyError;
  __int64 ReplyHandle;

  // Send out the request
  TInetClient::WriteInteger(NBD_MAGIC_REQUEST);
  TInetClient::WriteInteger(Type);
  TInetClient::WriteInteger(++RequestHandle);
  TInetClient::WriteInteger(Position);
  TInetClient::WriteInteger(Length);

  // What happens next depends on the type of the request
  switch (Type) {
    case nbdRead: {
      int ReplyLength;

      ReplyMagic = TInetClient::ReadInt32();
      if (ReplyMagic != NBD_MAGIC_REPLY)
        throw ENBDReadError("Bad signature on read reply from \"" + Host + "\".");
      ReplyError = TInetClient::ReadInt32();
      if (ReplyError == -1)
        throw ENBDReadError("Remote server \"" + Host + "\" returned a read error.");
      ReplyHandle = TInetClient::ReadInt64();
      if (ReplyHandle != RequestHandle)
        throw ENBDReadError("Reply serial number from \"" + Host + "\" does not match the read request's serial number.");
      ReplyLength = TInetClient::Read(Buffer, Length);
      if (ReplyLength != Length)
        throw ENBDReadError("Timeout while attempting to read " + String(Length) + " bytes from \"" + Host + "\".  Only " + String(ReplyLength) + " bytes received.");
      break;
    }  // case nbdRead

    case nbdWrite: {
      TInetClient::Write(Buffer, Length);
      ReplyMagic = TInetClient::ReadInt32();
      if (ReplyMagic != NBD_MAGIC_REPLY)
        throw ENBDWriteError("Bad signature on write reply from \"" + Host + "\".");
      ReplyError = TInetClient::ReadInt32();
      if (ReplyError == -1)
        throw ENBDWriteError("Remote server \"" + Host + "\" returned a write error.");
      ReplyHandle = TInetClient::ReadInt64();
      if (ReplyHandle != RequestHandle)
        throw ENBDWriteError("Reply serial number from \"" + Host + "\" does not match the write request's serial number.");
      break;
    }  // case nbdWrite:

    case nbdDisconnect:
      break;
  }  // switch (Type)
}  // bool __fastcall TNBDClient::SendRequestHeader(TNBDRequestHeader *Header)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Open a connection to a Network Block Device server
//
void __fastcall TNBDClient::Open(void)
{
  TNBDConnectionHeader *ConnectionHeader = NULL;
  int ret;
  try {
    TInetClient::Open();
    ConnectionHeader = new TNBDConnectionHeader;
    ret = TInetClient::Read(ConnectionHeader, sizeof(TNBDConnectionHeader));
    if (ret != sizeof(TNBDConnectionHeader))
      throw ENBDConnectionError("Incorrectly sized header.");
    else if (memcmp(ConnectionHeader->Signature, NBD_MAGIC_SIGNATURE, 8))
      throw ENBDConnectionError("Wrong signature.");
    else if (ntohll(ConnectionHeader->Magic) != NBD_MAGIC_CONNECT)
      throw ENBDConnectionError("The server is using a protocol that isn't supported.");
    FSize = ntohll(ConnectionHeader->Size);
    FConnected = true;
  }  // try
  __finally {
    if (ConnectionHeader)
      delete ConnectionHeader;
  }  // __finally
}  // void __fastcall TNBDClient::Open(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Close connection
//
void __fastcall TNBDClient::Close(void)
{
  DoTransaction(nbdDisconnect, 0, 0, NULL);
  TInetClient::Close();
  FConnected = false;
}  // void __fastcall TNBDClient::Close(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Read from the NBD server using one or more transactions
//
int __fastcall TNBDClient::Read(__int64 Position, void *Buffer, int Count)
{
  unsigned char *Data = (unsigned char *)Buffer;
  int BytesRead = 0;

  if (Position + Count > Size)
    Count = Size - Position;

  while (BytesRead < Count) {
    int TransactionSize = Count - BytesRead;
    TransactionSize = TransactionSize > FMaxTransaction?FMaxTransaction:TransactionSize;
    DoTransaction(nbdRead, Position + BytesRead, TransactionSize, Data + BytesRead);
    BytesRead += TransactionSize;
  }  // while (BytesWritten < Size)
  return BytesRead;
}  // void __fastcall TNBDClient::Read(__int64 Position, void *Buffer, int Size)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Write to the NBD server using one or more transactions.  Should probably
// have made one function for both reading and writing, but I'm too lazy
// to do it now.
//
int __fastcall TNBDClient::Write(__int64 Position, void *Buffer, int Count)
{
  unsigned char *Data = (unsigned char *)Buffer;
  int BytesWritten = 0;

  if (Position + Count > Size)
    Count = Size - Position;

  while (BytesWritten < Count) {
    int TransactionSize = Count - BytesWritten;
    TransactionSize = TransactionSize > FMaxTransaction?FMaxTransaction:TransactionSize;
    DoTransaction(nbdWrite, Position + BytesWritten, TransactionSize, Data + BytesWritten);
    BytesWritten += TransactionSize;
  }  // while (BytesWritten < Count)
  return BytesWritten;
}  // int __fastcall TNBDClient::Write(__int64 Position, void *Buffer, int Count)
//---------------------------------------------------------------------------


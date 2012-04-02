//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_TImageStore.cpp - TImageStore class
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
// VCS: $Version: 1 $ $Revision: 14 $
/*
$History: **** V 0.1 by kfitzner ****
$History: * selfimage_timagestore.cpp - 2005-11-07 2:39:39 AM - 8573 Bytes
$History: * selfimage_timagestore.h - 2005-10-21 10:56:46 AM - 2239 Bytes
$History: * Initial check-in
$History: **** V 0.2 by kfitzner ****
$History: * selfimage_timagestore.cpp - 2005-11-12 5:22:11 PM - 8837 Bytes
$History: * selfimage_timagestore.h - 2005-11-12 5:19:26 PM - 2243 Bytes
$History: * Typo in copyright area + change in program description
$History: **** V 1.0 by kfitzner ****
$History: * selfimage_timagestore.cpp - 2005-11-27 6:09:23 AM - 12796 Bytes
$History: * selfimage_timagestore.h - 2005-11-26 6:11:50 PM - 3073 Bytes
$History: * Add decompression support
$History: **** V 1.1 by kfitzner ****
$History: * selfimage_timagestore.cpp - 2005-12-04 10:52:30 AM - 16143 Bytes
$History: * selfimage_timagestore.h - 2005-12-03 10:33:20 PM - 4433 Bytes
$History: * Add support for determining if a particular area is 
$History: * marked by NTFS as being unused
$History: **** V 1.2 by kfitzner ****
$History: * selfimage_timagestore.cpp - 2005-12-04 11:03:21 AM - 17164 Bytes
$History: * selfimage_timagestore.h - 2005-12-04 10:54:50 AM - 4447 Bytes
$History: * Add better checking for non NTFS/FAT32 partitions, 
$History: * and document a little better
$History: **** V 1.3 by kfitzner ****
$History: * selfimage_timagestore.cpp - 2005-12-08 4:12:45 PM - 18840 Bytes
$History: * selfimage_timagestore.h - 2005-12-06 10:25:20 PM - 5075 Bytes
$History: * Network Block Device support
$History: **** V 1.4 by kfitzner ****
$History: * selfimage_timagestore.cpp - 2005-12-08 4:56:41 PM - 19050 Bytes
$History: * selfimage_timagestore.h - 2005-12-06 10:25:20 PM - 5075 Bytes
$History: * Fix NBD EOF condition
$History: **** V 1.5 by kfitzner ****
$History: * selfimage_timagestore.cpp - 2005-12-09 2:20:38 PM - 19598 Bytes
$History: * selfimage_timagestore.h - 2005-12-08 7:24:02 PM - 4946 Bytes
$History: * Fix FPosition not being set on writes, add configurable 
$History: * timeout and transaction size support
$History: **** V 1.6 by kfitzner ****
$History: * selfimage_timagestore.cpp - 2005-12-09 5:01:55 PM - 19865 Bytes
$History: * selfimage_timagestore.h - 2005-12-08 7:24:02 PM - 4946 Bytes
$History: * Forgot to multiply the transaction size by 1024 - whoops
$History: **** V 1.7 by kfitzner ****
$History: * selfimage_timagestore.cpp - 2006-01-11 6:27:25 AM - 20400 Bytes
$History: * selfimage_timagestore.h - 2005-12-08 7:24:02 PM - 4946 Bytes
$History: * Only read up to the stated end of device - no more. 
$History: *  ReadFile() returns a device not ready error instead 
$History: * of an EOF if you read pas the end of a removable device 
$History: * (USB CF card reader, etc).  Stupid OS.
$History: **** V 1.8 by kfitzner ****
$History: * selfimage_timagestore.cpp - 2007-02-12 10:28:21 AM - 20740 Bytes
$History: * selfimage_timagestore.h - 2005-12-08 7:24:02 PM - 4946 Bytes
$History: * Fixed FPosition not being zeroed early enough in constructor 
$History: * - it caused the initial read  in CheckCompression() to fail.
$History: **** V 1.9 by kfitzner ****
$History: * selfimage_timagestore.cpp - 2007-03-18 1:41:20 PM - 31605 Bytes
$History: * selfimage_timagestore.h - 2007-03-15 6:02:36 AM - 8090 Bytes
$History: * Fix potential inaccuracy in free chunk detection, 
$History: * add support for skipping unallocated chunks on Linux 
$History: * ext2/ext3 partitions, rename "block" to "chunk" in 
$History: * most cases to disambiguate between a Linux block and 
$History: * a SelfImage processing chunk.
$History: **** V 1.10 by kfitzner ****
$History: * selfimage_timagestore.cpp - 2007-03-18 7:11:34 PM - 32820 Bytes
$History: * selfimage_timagestore.h - 2007-03-18 5:34:24 PM - 8094 Bytes
$History: * Alter LoadEmptyChunkMap to return a count of empty 
$History: * chunks, check on read if size is a multiple of 512 and step down if so
$History: **** V 1.11 by kfitzner ****
$History: * selfimage_timagestore.cpp - 2007-03-19 2:44:34 PM - 33008 Bytes
$History: * selfimage_timagestore.h - 2007-03-18 5:34:24 PM - 8094 Bytes
$History: * Fix nGroupDescriptorsSize
$History: **** V 1.12 by kfitzner ****
$History: * selfimage_timagestore.cpp - 2007-03-27 2:54:39 PM - 33864 Bytes
$History: * selfimage_timagestore.h - 2007-03-24 4:14:32 PM - 8085 Bytes
$History: * Deferred read error from Read() causing exception 
$History: * in Seek() when reading files.  Read() cleaned up.
$History: **** V 1.13 by kfitzner ****
$History: * selfimage_timagestore.cpp - 2007-03-30 7:18:27 PM - 34507 Bytes
$History: * selfimage_timagestore.h - 2007-03-24 4:14:32 PM - 8085 Bytes
$History: * Add progress dialog when loading the empty chunk map
$History: **** Latest ** V 1.14 by kfitzner ** 2007-04-01 2:09:14 PM ****
$History: * Fix seek error again.  Stupid Windows bug.
*/
//----------------------------  $NoKeywords ---------------------------------

//---------------------------------------------------------------------------
// File Notes:
//---------------------------------------------------------------------------
// 12 Sep 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// An image "store" represtents any place where, no surprise, an image can
// be stored or retrieved from.  A file, a partition, even a network source.
// This class abstracts opens, closes, reads, etc so that the other classes
// don't need to know (or care) where their data is coming from or going.
//
// This class doesn't need to be particularly thread-safe, since it's a
// little unlikely that, say, the read thread is going to want to try and
// access the write store.
//---------------------------------------------------------------------------
// 16 Feb 2007 - Kurt Fitzner <kfitzner@excelcia.org>
//
// Adding support for determining what areas on a Linux (EXT2) partition are
// unused.  Support was already added for this for Windows (FAT32 and NTFS)
// partitions.  I say EXT2, but really I mean either EXT2 or EXT3.  EXT3
// isn't really any different, structure-wise.  EXT4 is out now, but I don't
// have any information on its structure yet, so I'm putting in as many tests
// as I can to ensure SelfImage doesn't wrongly interpret a newer filesystem
// than it properly understands.
//
// One thing to note about file comments - in Windows, a group of disk
// sectors that are read and allocated together are called a "cluster". In
// Linux, though, they are called a "block".  Variable names will generally
// use the word "cluster" for the sake of consistency, as will comments
// anywhere where the code isn't referring to Linux-only disk structures.
// When it is Linux-only code or disk structures referred to in comments,
// then the word "block" will be used.  Hopefully it won't get too confusing.
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <math.h>
#pragma hdrstop

#include "memlib.h"
#include "SelfImage_ProgressDialog.h"
#include "SelfImage_Utility.h"
#include "SelfImage_TImageStore.h"
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TImageStore constructor that opens the target at the same time
//
__fastcall TImageStore::TImageStore(TImageStoreType isType, AnsiString Target, bool WriteAccess) {
  FType = isType;
  FFileName = Target;
  FCompressionFormat = cfNone;
  FPosition = 0;
  bWriteAccess = WriteAccess;
  PartitionType = ptUnknown;
  BootSector = NULL;
  NBDConnection = NULL;
  if (!Open())
    throw ESelfImageError("Could not open store target \"" + Target + "\": " + GetLastErrorMessage());
  EmptyChunkMap = NULL;
}  // constructor
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TImageStore::~TImageStore()
//
__fastcall TImageStore::~TImageStore() {
  if (EmptyChunkMap)
    delete EmptyChunkMap;
  Close();
}  // __fastcall TImageStore::~TImageStore()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Open an image store - this determines the type of the store and whether
// or not NT open functions are available and then decides the best way
// to open the target.
//
bool __fastcall TImageStore::Open(void) {

  switch (Type) {
    case isFile: {
      LARGE_INTEGER nnFileSize;

      DWORD Access     = GENERIC_READ | (bWriteAccess?GENERIC_WRITE:0);
      DWORD ShareMode  = (bWriteAccess?0:FILE_SHARE_READ);
      DWORD CreateMode = (bWriteAccess?CREATE_ALWAYS:OPEN_EXISTING);
      hHandle = CreateFile(FileName.c_str(), Access, ShareMode, NULL, CreateMode, FILE_ATTRIBUTE_NORMAL, NULL);
      if (hHandle != INVALID_HANDLE_VALUE) {
        if (!bWriteAccess) {
          nnFileSize.LowPart = GetFileSize(hHandle, (unsigned long *)&nnFileSize.HighPart);
          FGeometry.Bytes = nnFileSize.QuadPart;
          CheckCompression();
        } else
          FGeometry.Bytes = -1;
        FGeometry.BytesPerSector = 0;
        FGeometry.BytesPerCluster = 0;
      }  // if (hHandle != INVALID_HANDLE_VALUE)
      break;
    }  // case isFile:

    case isDrive: {
      PARTITION_INFORMATION PartitionInfo;
      DISK_GEOMETRY DiskGeometry;
      DWORD Dummy;

      DWORD Access     = GENERIC_READ | (bWriteAccess?GENERIC_WRITE:0);
      DWORD ShareMode  = (bWriteAccess?0:FILE_SHARE_READ|FILE_SHARE_WRITE);
      if (HaveNTCalls)
        hHandle = NTOpen(FileName, SYNCHRONIZE | Access, FILE_ATTRIBUTE_NORMAL, ShareMode, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT | FILE_SEQUENTIAL_ONLY);
      else
        hHandle = CreateFile(FileName.c_str(), GENERIC_READ, ShareMode, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_BACKUP_SEMANTICS, NULL);
      if (hHandle != INVALID_HANDLE_VALUE) {
        DeviceIoControl(hHandle, IOCTL_DISK_GET_PARTITION_INFO, NULL, 0, &PartitionInfo, sizeof(PartitionInfo), &Dummy, NULL);
        DeviceIoControl(hHandle, IOCTL_DISK_GET_DRIVE_GEOMETRY	, NULL, 0, &DiskGeometry, sizeof(DiskGeometry), &Dummy, NULL);
        FGeometry.Bytes = PartitionInfo.PartitionLength.QuadPart;
        FGeometry.BytesPerSector = DiskGeometry.BytesPerSector;
        // Check to see if we're dealing with a floppy.  Does anyone have one any more?
        if (FileName.Pos("Floppy") || FileName.Pos("A:") || FileName.Pos("B:"))
          FGeometry.Bytes = DiskGeometry.Cylinders.QuadPart * DiskGeometry.TracksPerCylinder * DiskGeometry.SectorsPerTrack * DiskGeometry.BytesPerSector;
      }  // if (hHandle != INVALID_HANDLE_VALUE)
      CheckFileSystem();
      break;
    }  // case isDrive:

    case isNBD: {
      AnsiString Host;
      int nPort = 0;

      SplitConnectionString(FileName, Host, nPort);

      if (NBDConnection)
        delete NBDConnection;
      NBDConnection = new TNBDClient(NULL);
      NBDConnection->Host = Host;
      NBDConnection->Port = nPort;
      NBDConnection->Timeout = SelfImageConfig->Values["NBDTimeout"];
      NBDConnection->TransactionSize = SelfImageConfig->Values["NBDTransactionSize"] * 1024;
      NBDConnection->Open();
      FGeometry.Bytes = NBDConnection->Size;
      CheckFileSystem();
    }  // case isNBD:
  }  // switch (Type)

  return (hHandle != INVALID_HANDLE_VALUE);
}  // bool __fastcall TImageStore::Open(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Close() - Close an image store
//
bool __fastcall TImageStore::Close(void) {
  if (BootSector) {
    delete BootSector;
    BootSector = NULL;
  }  // if (BootSector)
  if (FType == isNBD && NBDConnection) {
    NBDConnection->Close();
    delete NBDConnection;
    return true;
  }  // if (FType == isNBD && NBDConnection)
  else return CloseHandle(hHandle);
}  // bool __fastcall Close(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Read() - As the name suggests, read from the file/device.  Special care
// has to be taken when reading from handles created with "NtCreateFile()".
// Windows versions based on NT have problems with partial reads of disk
// devices opened with NtCreateFile().  When there isn't enough data in the
// file to satisfy the entire length of the read, ReadFile() is supposed to
// always read as many as there are.  Disk devices of the type
// "\Device\Harddisk0\Partition1" opened with NtCreateFile don't do this.
// When there isn't enough data left to satisfy the entire read, ReadFile()
// will fail entirely and GetLastError() will return ERROR_INVALID_PARAMETER.
// To compensate for this, whenever we encounter ERROR_INVALID_PARAMETER, we
// step down the size of the read to the device's sector size and then read
// until we get an error again.
//
// The same ERROR_INVALID_PARAMETER error will be returned in cases where
// the read length is not a multiple of the disk's sector size.  The same
// step down code gets around this by using a temporary buffer to read
// into a sector at a time, and them copies the exact number of bytes
// requested out after reading the sector that puts it past the requested
// length.  In this case, nBytesRead will be set to the requested length,
// but the read position will be at the next sector boundary.
//
bool __fastcall TImageStore::Read(void *buffer, unsigned Length, unsigned *BytesRead)
{
  if (FPosition + Length > FGeometry.Bytes)
    Length = FGeometry.Bytes - FPosition;

  switch (FType) {
    case isNBD: {
      int ret = NBDConnection->Read(FPosition, buffer, Length);
      FPosition += ret;
      *BytesRead = ret;
      return true;
    }  // case isNBD:

    case isFile: {
      bool bSuccess = ReadFile(hHandle, buffer, Length, (DWORD *)BytesRead, NULL);
      if (bSuccess)
        FPosition += *BytesRead;
      return bSuccess;
    }  // case isFile:

    case isDrive: {
      DWORD nBytesReadTemp;
      unsigned pos;
      bool bSuccess;
      if (Length & 511ui64) {
        bSuccess = false;
        ::SetLastError(ERROR_INVALID_PARAMETER);
      } else
        bSuccess = ReadFile(hHandle, buffer, Length, &nBytesReadTemp, NULL);
      if (!bSuccess && GetLastError() == ERROR_INVALID_PARAMETER) {
        // ERROR_INVALID_PARAMETER - step down read size as discussed above
        char *TempBuffer = (char *)SysGetMem((Length / 4096 + 1) * 4096);
        pos = 0;
        while (pos < Length && ReadFile(hHandle, TempBuffer + pos, Geometry.BytesPerSector?Geometry.BytesPerSector:512, (unsigned long *)&nBytesReadTemp, NULL))
          pos += nBytesReadTemp;
        nBytesReadTemp = pos < Length?pos:Length;
        memcpy_optimized(buffer, TempBuffer, nBytesReadTemp);
        SysFreeMem(TempBuffer);
        FPosition += pos;
        bSuccess = true;
      }  // if (!bSuccess)
      else
         FPosition += nBytesReadTemp;
      *BytesRead = nBytesReadTemp;
      return bSuccess;
    }  // case isDrive:

    default:
      return false;
  }  // switch (FType)
}  // bool __fastcall TImageStore::Read(void *buffer, int nLength, unsigned *nBytesRead)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Write() - similar to Read() but different.
//
bool __fastcall TImageStore::Write(void *buffer, unsigned nLength, unsigned *nBytesWritten) {
  bool retval;
  unsigned nWrote;

  if (FType == isNBD)
    retval = (nWrote = NBDConnection->Write(FPosition, buffer, nLength)) != 0;
  else
    retval = WriteFile(hHandle, buffer, nLength, (unsigned long *)&nWrote, NULL);
  FPosition += nWrote;
  *nBytesWritten = nWrote;
  return retval;
}  // bool __fastcall TImageStore::Write(void *buffer, int nLength, unsigned *nBytesWritten)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  Set the file position pointer
//
void __fastcall TImageStore::Seek(__int64 Offset, DWORD MoveMethod)
{
  if (FType == isNBD) {
    switch (MoveMethod) {
      case FILE_BEGIN: FPosition = Offset; break;
      case FILE_CURRENT: FPosition += Offset; break;
      case FILE_END: FPosition = FGeometry.Bytes + Offset; break;
    }  // switch (MoveMethod)
  }  // if (FType == isNBD)
  else {
    DWORD nError;
    LOW_DWORD(Offset) = SetFilePointer(hHandle, LOW_DWORD(Offset), &HIGH_DWORD(Offset), MoveMethod);
    if (LOW_DWORD(Offset) == INVALID_SET_FILE_POINTER && (nError = GetLastError()) != NO_ERROR)
      throw ESelfImageFileError("Seek error with " + FFileName + ": '" + GetErrorMessage(nError) + "'");
    else
      FPosition = Offset;
  }  // else if (FType != NBD)
}  // bool __fastcall TImageStore::Seek(__int64)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Check to see if the file is compressed.  This is a full check that looks
// at the contents of the file to see if it contains the correct signature
// and also looks to see if it's in SelfImage's chunk format for parallel
// decompression.
//
void __fastcall TImageStore::CheckCompression(void)
{
  unsigned char *Buffer = NULL;
  AnsiString Extension;

  if (FType != isFile)
    return;

  try {
    Extension = ExtractFileExt(FileName);
    if (Extension == ".gz") {
      unsigned ReadCount;
      Buffer = new unsigned char[20];
      // Read in the gzip header - 20 bytes is all we need
      Read(Buffer, 20, &ReadCount);
      if (ReadCount == 20) {
        // If the file is actually a gzip file then the first two bytes will be 0x1F 0x8B.  The next byte identifies the
        // gzip compression method - since deflate is all we support, we check to make sure it's a deflate gzip file.
        // Deflate is all zlib supports right now - probably all it will ever support.
        if (Buffer[0] == 0x1F && Buffer[1] == 0x8B && Buffer[2] == 0x08)
          // By this point we know for sure it's a gzip file.  Now check to see if it's a gzip that's in SelfImage chunk format.
          // If it is, it will have a gzip Extra field identified by 'SI' followed by the field length, which must be little-endian 0x0008
          if (Buffer[12] == 'S' && Buffer[13] == 'I' && Buffer[14] == 0x08 && Buffer[15] == 0x00)
            FCompressionFormat = cfGzipChunk;
          else
            FCompressionFormat = cfGzipStream;
      }  // if (ReadCount == 20)
    }  // if (Extension == ".gz")
    else if (Extension == ".bz2") {
      unsigned ReadCount;
      Buffer = new unsigned char[1024 * 1024];
      // Need to read in the entire first bzip2 chunk, plus the header of the next one.
      // Techinically that's 909610 bytes - make it one meg for good measure
      if (Read(Buffer, 1024 * 1024, &ReadCount)) {
        // If it's a bz2 file, it will start with "BZh" and then have a number from 1 to 9
        if (ReadCount > 20 && Buffer[0] == 'B' && Buffer[1] == 'Z' && Buffer[2] == 'h' && Buffer[3] >= '1' && Buffer[3] <= '9') {
           char *Header = "BZh_1AY&SY";
           Header[3] = Buffer[3];
           // Check to see if there is another full bzip2 header after the first bzip2 chunk.  If there is,
           // then we have a SelfImage format file - otherwise it's a standard bzip2 file with chunks that
           // we can't tell the size of.
           if (FindString(Buffer+10, ReadCount, Header, true))
             FCompressionFormat = cfBzip2Chunk;
           else
             FCompressionFormat = cfBzip2Stream;
        }  // if (...is a bzip2 file)
      }  // if (Read(Buffer, 20, &ReadCount))
      // Read in the first compression chunk
    }  // else if (Extension == ".bz2")
  }  // try
  __finally {
    if (Buffer)
      delete [] Buffer;
    Seek(0, FILE_BEGIN);
  }  // __finally
}  // void __fastcall TImageStore::CheckCompression(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Check to see if the partition we are reading from has a filesystem on it
// that is a type we understand.  This can be Windows (NTFS or FAT32) or
// Linux (EXT2).  If it is, then we flag it so we can check what areas on it
// are unallocated and skip them automatically.
//
void __fastcall TImageStore::CheckFileSystem(void)
{
  unsigned int BytesRead;

  if (bWriteAccess || Type == isFile)
    return;

  if (BootSector) {
    delete BootSector;
    BootSector = NULL;
  }  // if (BootSector)
  BootSector = new TBootSector;

  Read(BootSector, sizeof(TBootSector), &BytesRead);
  Seek(0, FILE_BEGIN);
  if (BytesRead == sizeof(TBootSector)) {
    // Check to see if it's a Windows partition - we can only tell if space is unallocated on local Windows partitions
    // so NBD is excluded here
    if (Type != isNBD && (!memcmp(BootSector->Win.OEMID, "NTFS    ", 8) || !memcmp(BootSector->Win.OEMID, "MSDOS5.0", 8))) {
      FGeometry.BytesPerCluster = BootSector->Win.SectorsPerCluster * FGeometry.BytesPerSector;
      PartitionType = ptWindows;
    }  // if (is NTFS/FAT32)
    // Check to see if it's a Linux (EXT2) partition.  We do a lot of checking here to make sure not only
    // it is EXT2, but to make sure it's a variant we understand.  Don't want to stumble onto a version of
    // EXT2 that is newer than SelfImage is and screw up the image.
    if (BootSector->EXT2.MagicSignature == EXT2_MAGIC && BootSector->EXT2.RevisionLevel <= 1 &&
        BootSector->EXT2.Log2FragSize == (UINT16)BootSector->EXT2.Log2BlockSize) {
      FGeometry.BytesPerCluster = 0x400u << BootSector->EXT2.Log2BlockSize;
      PartitionType = ptEXT2;
      // Some Linux kernels have problems reading past the last filesystem block on mounted EXT2/3 partitions.
      // NBD servers on those kernels will return an error if you try to read to the physical end of the device.
      // This data is unused by the filesystem anyway, so we just ignore it
      FGeometry.Bytes = (__int64)FGeometry.BytesPerCluster * (__int64)BootSector->EXT2.BlocksCount;
    }  // if (... is ext2 partition)
  }  // if (BytesRead == sizeof(TBootSector))
  // If we didn't end up with a filesystem we know about, then delete the boot sector.  Otherwise we keep it.
  if (PartitionType == ptUnknown && BootSector) {
    delete BootSector;
    BootSector = NULL;
  }  // if (PartitionType == ptUnknown && BootSector)

  return;
}  // void __fastcall CheckCompression(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Configure the chunk map for this device.  The empty chunk map is an array
// of bool that maps which chunks consist entirely of unallocated clusters.
//
unsigned __fastcall TImageStore::LoadEmptyChunkMap(unsigned int ChunkSize)
{
  unsigned nChunkCount, nClustersPerChunk, nBitmapBytes;
  VOLUME_BITMAP_BUFFER *VolumeBitmap;
  TBitArray *Bitmap = new TBitArray;
  int nExtraClusterBits;
  bool bChunkEmpty;
  unsigned nEmptyChunkCount = 0;

  // IF this isn't a Linux or local Windows partition then we have no business here
  if (PartitionType == ptUnknown || PartitionType == ptWindows && FType != isDrive )
    return 0;

  // In cases where the input size is an exact multiple of the chunk size, we actually want nChunkCount to be one
  // higher than the actual number of chunks.  In that case, the last real chunk can potentially be empty, and the
  // next read will return zero bytes, telling the read thread that it's at EOF.
  nChunkCount = (Geometry.Bytes + ChunkSize) / ChunkSize;

  if (EmptyChunkMap)
    delete EmptyChunkMap;
  EmptyChunkMap = new TBitArray();
  EmptyChunkMap->Size = nChunkCount;
  nChunkSize = ChunkSize;

  // Determine the number of clusters in a chunk, and the number of cluster bitmap bytes that are taken by a chunk.
  nClustersPerChunk = (ChunkSize + Geometry.BytesPerCluster - 1) / Geometry.BytesPerCluster;
  nBitmapBytes = (nClustersPerChunk + 7) / 8;
  // Once we know all that, we can determine the size of the volume bitmap buffer that we need for each chunk
  // nBitmanBytes is actually one larger than we need because there is one bitmap byte already in the VOLUME_BITMAP_BUFFER
  // struct.  This is good, though, because FSCTL_GET_VOLUME_BITMAP rounds the start cluster down to a multiple of 8, so we
  // may need this extra space.
  VolumeBitmap = (VOLUME_BITMAP_BUFFER *)SysGetMem(sizeof(VOLUME_BITMAP_BUFFER) + nBitmapBytes);

  // Create the progress bar dialog
  Application->CreateForm(__classid(TProgressDialog), &ProgressDialog);
  ProgressDialog->Display("Mapping unallocated space...", 0, nChunkCount);

  // Iterate through all the chunks and determine if the chunk resides entirely on clusters that are unallocated
  for (unsigned n = 0; n < nChunkCount - 1; n++) {
    LONGLONG nnStartCluster = (LONGLONG)n * (LONGLONG)ChunkSize / (LONGLONG)Geometry.BytesPerCluster;
    DWORD nBytesReturned;

    // Ask for a cluster bitmap starting with the cluster this chunk starts on
    if (PartitionType == ptWindows) {
      int ret = DeviceIoControl(hHandle, FSCTL_GET_VOLUME_BITMAP, &nnStartCluster, sizeof(nnStartCluster), VolumeBitmap, sizeof(VOLUME_BITMAP_BUFFER) + nBitmapBytes, &nBytesReturned, NULL);
      if (!ret && GetLastError() != ERROR_MORE_DATA) {
        delete EmptyChunkMap;
        EmptyChunkMap = NULL;
        nEmptyChunkCount = 0;
        break;
      }  // if (!ret && GetLastError() != ERROR_MORE_DATA)
    }  // if (PartitionType == ptWindows)
    else if (PartitionType == ptEXT2) {
      bool ret;
      VolumeBitmap->StartingLcn.QuadPart = nnStartCluster;
      ret = GetEXT2VolumeBitmap(&VolumeBitmap->StartingLcn.QuadPart, VolumeBitmap->Buffer, nBitmapBytes + 1);
      if (!ret) {
        delete EmptyChunkMap;
        EmptyChunkMap = NULL;
        nEmptyChunkCount = 0;
        break;
      }  // if (!ret)
    }  // if (PartitionType == ptEXT2)

    nExtraClusterBits = nnStartCluster - VolumeBitmap->StartingLcn.QuadPart;
    VolumeBitmap->Buffer[0] &= 0xFF>>nExtraClusterBits;
    Bitmap->LoadBuffer(VolumeBitmap->Buffer, nClustersPerChunk + nExtraClusterBits);
    bChunkEmpty = !Bitmap->HasSetBit();
    if (bChunkEmpty)
      nEmptyChunkCount++;
    EmptyChunkMap->Bits[n] = bChunkEmpty;
    if (!(n & 7))
      ProgressDialog->Position = n;
  }  // for (unsigned n = 0; n < nnChunkCount; n++)
  ProgressDialog->Position = nChunkCount;

  SysFreeMem(VolumeBitmap);
  delete Bitmap;
  if (PartitionType == ptEXT2)
    GetEXT2VolumeBitmap(NULL, NULL, 0, true);

  Seek(0, FILE_BEGIN);

  ProgressDialog->Close();
  delete ProgressDialog;

  // The last chunk must always be read normally so that the resulting image is sized properly
  if (EmptyChunkMap)
    EmptyChunkMap->Bits[nChunkCount - 1] = false;

  return nEmptyChunkCount;
}  // void __fastcall TImageStore:LoadChunkMap(unsigned int ChunkSize)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Get a portion of an EXT2 partition's cluster allocation bitmap
//
bool __fastcall TImageStore::GetEXT2VolumeBitmap(__int64 *StartCluster, BYTE *Bitmap, int BitmapSize, bool Cleanup)
{
  static TEXT2GroupDescriptor *GroupDescriptors = NULL;
  unsigned nGroupDescriptorsSize;
  static unsigned nGroupCount;
  int nNeededGroup;
  static int nLoadedGroup = -1;
  static BYTE *GroupBitmap = NULL;
  static unsigned nGroupBitmapSize;
  unsigned nBytesRead;
  bool bReadSuccess;
  __int64 nnRoundedStartCluster, nnGroupStartCluster;
  int nGroupBitmapIndex, nGroupBitmapRemaining;

  // If we've been called to clean up, then do so and return
  if (Cleanup) {
    if (GroupBitmap) {
      SysFreeMem(GroupBitmap);
      GroupBitmap = NULL;
    }  // if (GroupBitmap)
    if (GroupDescriptors) {
      SysFreeMem(GroupDescriptors);
      GroupDescriptors = NULL;
    }  // if (GroupDescriptors)
    nLoadedGroup = -1;
    return true;
  }  // if (Cleanup)

  // If the group descriptors haven't been read yet, then grab them
  if (!GroupDescriptors) {
    nGroupCount = (BootSector->EXT2.BlocksCount + BootSector->EXT2.BlocksPerGroup - 1) / BootSector->EXT2.BlocksPerGroup;
    nGroupDescriptorsSize = (nGroupCount * sizeof(TEXT2GroupDescriptor) + FGeometry.BytesPerCluster - 1) / FGeometry.BytesPerCluster * FGeometry.BytesPerCluster;
    GroupDescriptors = (TEXT2GroupDescriptor *)SysGetMem(nGroupDescriptorsSize);
    Seek((__int64)(BootSector->EXT2.FirstDataBlock + 1) * (__int64)FGeometry.BytesPerCluster, FILE_BEGIN);
    bReadSuccess = Read(GroupDescriptors, nGroupDescriptorsSize, &nBytesRead);
    if (!bReadSuccess || nBytesRead != nGroupDescriptorsSize)
      throw ESelfImageFileError("Could not read group descriptors for linux EXT2 partition.");
  }  // if (!GroupDescriptors)

  // Round StartCluster down to a multiple of 8
  nnRoundedStartCluster = *StartCluster & ~7ui64;

  // Calculate what EXT2 block group StartCluster is in, and load it if it's not loaded already
  nNeededGroup = nnRoundedStartCluster / BootSector->EXT2.BlocksPerGroup;
  if (nNeededGroup != nLoadedGroup) {
    // Have we allocated a block group bitmap cache yet?
    if (!GroupBitmap) {
      nGroupBitmapSize = (BootSector->EXT2.BlocksPerGroup / 8 + FGeometry.BytesPerCluster - 1) / FGeometry.BytesPerCluster * FGeometry.BytesPerCluster;
      GroupBitmap = (BYTE *)SysGetMem(nGroupBitmapSize);
    }  // if (!GroupBitmap)
    Seek((__int64)GroupDescriptors[nNeededGroup].BlockBitmap * (__int64)FGeometry.BytesPerCluster, FILE_BEGIN);
    bReadSuccess = Read(GroupBitmap, nGroupBitmapSize, &nBytesRead);
    if (!bReadSuccess || nBytesRead != nGroupBitmapSize)
      throw ESelfImageFileError("Could not read EXT2 block group allocation bitmap.");
    nLoadedGroup = nNeededGroup;
  }  // if (nNeededGroup != nLoadedGroup)

  // Figure out what byte in the group block bitmap we'll be starting with, how much data we have
  // in the group block bitmap, then copy it over
  nnGroupStartCluster = (__int64)nNeededGroup * (__int64)BootSector->EXT2.BlocksPerGroup;
  nGroupBitmapIndex = (nnRoundedStartCluster - nnGroupStartCluster) / 8;
  nGroupBitmapRemaining = BootSector->EXT2.BlocksPerGroup / 8 - nGroupBitmapIndex;
  memcpy_optimized(Bitmap, GroupBitmap + nGroupBitmapIndex, MIN(BitmapSize, nGroupBitmapRemaining));
  // If we have satisfied the request, set return values and return
  if (BitmapSize <= nGroupBitmapRemaining) {
    *StartCluster = nnRoundedStartCluster;
    return true;
  }  // if (BitmapSize <= nGroupBitmapRemaining)
  // Otherwise, it's because we didn't have enough bitmap bytes in this block group's bitmap to satisfy
  // the request, so recursively call the function again to get the rest
  else {
    __int64 nnExtraStartCluster = nnRoundedStartCluster + nGroupBitmapRemaining * 8;
    if (GetEXT2VolumeBitmap(&nnExtraStartCluster, Bitmap + nGroupBitmapRemaining, BitmapSize - nGroupBitmapRemaining)) {
      *StartCluster = nnRoundedStartCluster;
      return true;
    } else
      // The funny thing about this, is that the way this is set up with the recursion, is that the only way this function
      // can ever return false is if the function returns false.  So, if this next return ever executes, something is
      // fundamentally wrong with the universe, and I would suggest you immediately take cover
      return false;
  }  // else if (BitmapSize > nGroupBitmapRemaining)
}  // bool __fastcall TImageStore::GetEXT2VolumeBitmap(__int64 *StartCluster,  BYTE *Bitmap, int BitmapSize, bool Cleanup = false)
//---------------------------------------------------------------------------


#ifdef _DEBUG
//---------------------------------------------------------------------------
// Non-inline test version of IsNextChunkEmpty() for debugging. This is more
// for peace of mind than anything - useful for verifying part of the empty
// chunk detection system
//
bool __fastcall TImageStore::IsNextChunkEmpty(void)
{
  if (!EmptyChunkMap)
    return false;

  if (FPosition > FGeometry.Bytes)
    throw ESelfImageError("Internal error: FPosition > FGeometry.Bytes");
  else if (FPosition % nChunkSize)
    throw ESelfImageError("Internal error: FPosition not a multiple of chunk size");

  return EmptyChunkMap->Bits[(unsigned int)(FPosition / nChunkSize)];
}  // bool __fastcall TImageStore::IsNextChunkEmpty(void)
//---------------------------------------------------------------------------
#endif


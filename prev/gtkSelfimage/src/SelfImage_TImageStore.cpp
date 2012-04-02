//---------------------------------------------------------------------------
// GtkSelfImage - Disk image manipulation program
//---------------------------------------------------------------------------
// This file is part of SelfImage.
//
// GtkSelfImage is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License (Version 2) as
// published by the Free Software Foundation.
//
// GtkSelfImage is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//---------------------------------------------------------------------------

#include <stdio.h>	// fopen, fclose, getchar
#include <fcntl.h>	// open(), close()
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#include <winioctl.h>
#else
#include <linux/hdreg.h>
#include <sys/ioctl.h>             /* for _IO */
#endif
#include <errno.h>

#include "alert/alert.h"
#include "./lib/memlib/memlib.h"
#include "SelfImage_Utility.h"
#include "SelfImage_TypesDefs.h"
#include "SelfImage_TImageStore.h"
//---------------------------------------------------------------------------

//Defines for Symbolic Links...
#ifdef _WIN32
#define DriveC "\\??\\C:"
#define DeviceC "\\Device\\Harddisk0\\Partition1"
#define DriveD "\\??\\D:"
#define CdRom "\\Device\\Cdrom0"
#define DriveE "\\??\\E:"
#define DeviceE "\\Device\\Harddisk0\\Partition1"
#define DriveF "\\??\\F:"
#define DeviceF "\\Device\\Harddisk0\\Partition6"
#endif

//#define DEBUG_PARTSIZE

//---------------------------------------------------------------------------
// TImageStore constructor that opens the target at the same time
//
TImageStore::TImageStore(TImageStoreType isType, AnsiString Target, bool WriteAccess) {
	FType = isType;
	FFileName = Target;
	FCompressionFormat = cfNone;
	FPosition = 0;
	bWriteAccess = WriteAccess;
	PartitionType = ptUnknown;
	BootSector = NULL;
// WARNING FIX FOLLOWING
#if 0
  NBDConnection = NULL;
#endif
	if (!Open()){
		printf ("Failed to open file/drive: %s\n", FFileName.c_str());
		//throw (1);
	}
//      throw ESelfImageError("Could not open store target");
//    throw ESelfImageError("Could not open store target \"" + Target + "\": ");// + GetLastErrorMessage());
//  EmptyChunkMap = NULL;
}  // constructor
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TImageStore::~TImageStore()
//
TImageStore::~TImageStore() {
//  if (EmptyChunkMap)
//    delete EmptyChunkMap;
  Close();
}  // __fastcall TImageStore::~TImageStore()
//---------------------------------------------------------------------------

bool TImageStore::IsOpen() {
#ifdef _WIN32     
     if (hHandle != INVALID_HANDLE_VALUE) {
         return true;
     }
#else
     if (hHandle > 0) {
        return true;
     }
#endif
      return false;
}


#ifdef WIN32
//---------------------------------------------------------------------------
// Open an image store - this determines the type of the store and whether
// or not NT open functions are available and then decides the best way
// to open the target.
//
bool __fastcall TImageStore::Open(void) {
     FGeometry.Bytes = (unsigned long long)0;
     FGeometry.BytesPerSector = (unsigned long long)0;
     bool bSuccess;

	switch (FType) {
		case isFile:{
			LARGE_INTEGER nnFileSize;

			DWORD ShareMode  = FILE_SHARE_READ;//(bWriteAccess?0:FILE_SHARE_READ);
			DWORD CreateMode = (bWriteAccess?CREATE_ALWAYS:OPEN_EXISTING);
			DWORD Access     = GENERIC_READ | (bWriteAccess?GENERIC_WRITE:0);

			hHandle = CreateFile(FFileName.c_str(), Access, ShareMode, NULL, CreateMode, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hHandle != INVALID_HANDLE_VALUE) {
				if (!bWriteAccess) {
					nnFileSize.LowPart = GetFileSize(hHandle, (unsigned long *)&nnFileSize.HighPart);
					FGeometry.Bytes = nnFileSize.QuadPart;
					CheckCompression();
				} else
					FGeometry.Bytes = 0; //-1;
				FGeometry.BytesPerSector = 0;
				FGeometry.BytesPerCluster = 0;
			}else
				return false;
			break;
		}  // case isFile:

		case isDrive: {
			PARTITION_INFORMATION PartitionInfo;
			DISK_GEOMETRY DiskGeometry;
			DWORD Dummy;
			

			
			// A example for a valid file name is: \\.\C

			DWORD Access     = GENERIC_READ | (bWriteAccess?GENERIC_WRITE:0);
			DWORD ShareMode  = (bWriteAccess?0:FILE_SHARE_READ|FILE_SHARE_WRITE);

			if (HaveNTCalls ){
				wchar_t wFileName[255];
				int pos = 0;
				for (pos = 0; pos < FFileName.size(); pos++){
					wFileName[pos] = FFileName[pos];
					wFileName[pos+1] = L'\0';
				}

				hHandle = NTOpen(wFileName, SYNCHRONIZE | Access, FILE_ATTRIBUTE_NORMAL, ShareMode, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT | FILE_SEQUENTIAL_ONLY);
			}else{
                 if (strncmp ("\\Device\\Floppy", FFileName.c_str(), 14) == 0){
                        printf ("Make open floppy fail\n");
                        return false;
                }
			    printf ("%s line %i: Try to open %s\n", __FILE__, __LINE__, FFileName.c_str());
				hHandle = CreateFile(FFileName.c_str(), GENERIC_READ, ShareMode, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_BACKUP_SEMANTICS, NULL);
            }

			if (hHandle != INVALID_HANDLE_VALUE) {
				bSuccess = DeviceIoControl(hHandle, IOCTL_DISK_GET_PARTITION_INFO, NULL, 0, &PartitionInfo, sizeof(PartitionInfo), &Dummy, NULL);
				if (!bSuccess){
                   Close();
                   return false;
                }
				bSuccess = DeviceIoControl(hHandle, IOCTL_DISK_GET_DRIVE_GEOMETRY	, NULL, 0, &DiskGeometry, sizeof(DiskGeometry), &Dummy, NULL);
				if (!bSuccess){
                   Close();
                   return false;
                }
				FGeometry.Bytes = (unsigned long long)PartitionInfo.PartitionLength.QuadPart;
				FGeometry.BytesPerSector = DiskGeometry.BytesPerSector;
#ifdef DEBUG_PARTSIZE
				printf("Bytes: %llu\n", FGeometry.Bytes);
				if ( FGeometry.Bytes == (__uint64)0)
				   printf ("Hallo\n");
#endif

#if 0
				// Check to see if we're dealing with a floppy.  Does anyone have one any more?
				if (FFileName.Pos("Floppy") || FileName.Pos("A:") || FileName.Pos("B:"))
					FGeometry.Bytes = DiskGeometry.Cylinders.QuadPart * DiskGeometry.TracksPerCylinder * DiskGeometry.SectorsPerTrack * DiskGeometry.BytesPerSector;
#endif
			}else{
   printf ("invalid file handle\n");
				return false;
            }
#if 0
			CheckFileSystem();
#endif

			break;
		}  // case isDrive:

    case isNBD: {
    	printf ("%s line %i: Here\n", __FILE__, __LINE__);
	return false;
#ifdef _WIN32
#if 0
      AnsiString Host;
      int nPort = 0;

      SplitConnectionString(FFileName, Host, nPort);

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
#endif
#endif
    }  // case isNBD:

    default:
	// ERROR
	return false;
	break;
  }  // switch (Type)

  return (hHandle != INVALID_HANDLE_VALUE);
}  // bool __fastcall TImageStore::Open(void)

#else	// LINUX

//---------------------------------------------------------------------------
// Open an image store - this determines the type of the store and whether
// or not NT open functions are available and then decides the best way
// to open the target.
//
bool __fastcall TImageStore::Open(void)
{
FGeometry.Bytes = (unsigned long long)0;
FGeometry.BytesPerSector = (unsigned long long)0;

	switch (FType) {
		case isFile:{
			/// TODO handle Access var, createmode, shareMode
			int flags = O_NOCTTY |O_LARGEFILE;
			if (bWriteAccess){
				flags |= O_WRONLY|O_CREAT|O_TRUNC;
			}else{
				flags |= O_RDONLY;
			}
			hHandle = open(FFileName.c_str(), flags);
			if (hHandle > 0) {
				if (!bWriteAccess) {
					//LONGLONG Pos;
					//Pos = lseek (hHandle, 0, SEEK_CUR);
					FGeometry.Bytes = lseek (hHandle, 0, SEEK_END);
					lseek (hHandle, 0, SEEK_SET);
					CheckCompression();
				} else
					FGeometry.Bytes = 0; //-1;
				FGeometry.BytesPerSector = 0;
				FGeometry.BytesPerCluster = 0;
			}else
				return false;
			break;
		}  // case isFile:

		case isDrive: {
			struct hd_geometry	geom;
			double			MBsize = 0;

			/// TODO handle Access var, createmode, shareMode
			int flags = O_NOCTTY | O_SYNC | O_LARGEFILE;
			if (bWriteAccess){
				flags |= O_WRONLY;
			}else{
				flags |= O_RDONLY;
			}
			hHandle = open(FFileName.c_str(), flags);
			if (hHandle > 0) {

				//DeviceIoControl(hHandle, IOCTL_DISK_GET_PARTITION_INFO, NULL, 0, &PartitionInfo, sizeof(PartitionInfo), &Dummy, NULL);
				//DeviceIoControl(hHandle, IOCTL_DISK_GET_DRIVE_GEOMETRY	, NULL, 0, &DiskGeometry, sizeof(DiskGeometry), &Dummy, NULL);

				/* get C/H/S geometry and size of partition */
				if(ioctl(hHandle, HDIO_GETGEO, &geom) < 0)
					printf("HDIO_GETGEO ioctl failed");

				/* get size of block  */
				if(ioctl(hHandle, BLKSSZGET, &FGeometry.BytesPerSector) < 0)
					printf("BLKSSZGET ioctl failed");

				/* get size of the partition, chould match C*H*S */
				if(ioctl(hHandle, BLKGETSIZE, &FGeometry.NumbSectors) < 0)
					printf("BLKGETSIZE ioctl failed");

				// TODO
				FGeometry.Bytes = (unsigned long long)FGeometry.NumbSectors * (unsigned long long)FGeometry.BytesPerSector;

				MBsize = ( FGeometry.NumbSectors)/(1024 * 1024 / FGeometry.BytesPerSector);
#ifdef DEBUG_PARTSIZE
				printf("\n");
				if (MBsize > 1024)
					printf(" Geometry for %s, size = %.2f Gb\n", FFileName.c_str(), (float) (MBsize/1024));
				else
					printf(" Geometry for %s, size = %.2f Mb\n", FFileName.c_str(), (float) MBsize);
				printf("\t        Cylinders: %d\n", geom.cylinders);
				printf("\t            Heads: %d\n", geom.heads);
				printf("\t          Sectors: %d\n", geom.sectors);
				printf("\t    Total sectors: %lu\n", (long unsigned) FGeometry.NumbSectors);
				printf("\t     First sector: %lu\n", geom.start);
				printf("\t      Last sector: %lu\n", (long unsigned) (geom.start + FGeometry.NumbSectors - 1));
				printf("\n\n\n");
				printf("\t   BytesPerSector: %i\n",  FGeometry.BytesPerSector);
				printf("\t  PartitionLength: %i\n", (int)FGeometry.NumbSectors);
				printf("\t  Bytes: %llu\n", FGeometry.Bytes);

				printf("\n");
#endif

			}else
				return false;
			/// TODO enable check compression
			//CheckFileSystem();
			break;
		}  // case isDrive:

    default:
	// ERROR
	return false;
	break;
  }  // switch (Type)

	return true;
}  // bool __fastcall TImageStore::Open(void)

#endif


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Close() - Close an image store
//
bool __fastcall TImageStore::Close(void) {
	if (BootSector) {
		delete BootSector;
		BootSector = NULL;
	}  // if (BootSector)
#ifdef _WIN32
	///WINDOWS
#if 0
	if (FType == isNBD && NBDConnection) {
		NBDConnection->Close();
		delete NBDConnection;
		return true;
	}else
#endif
	if (CloseHandle(hHandle)){
		hHandle = NULL;
		return true;
	}
	return false;
#else
	///LINUX
	close (hHandle);
	return true;
#endif

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
bool __fastcall TImageStore::Read(void *buffer, unsigned Length, unsigned long *BytesRead)
{
  if (FPosition + Length > (unsigned) FGeometry.Bytes)
    Length = FGeometry.Bytes - FPosition;

  switch (FType) {
    case isFile: {
	bool bSuccess = true;
#ifdef _WIN32
	bSuccess = ReadFile(hHandle, buffer, Length, (DWORD *)BytesRead, NULL);
#else
//	printf ("Pos = %i\n", lseek(hHandle, 0, SEEK_CUR));
	long int BytesRead_t = read(hHandle, buffer, Length);

	if (BytesRead_t > 0)
		*BytesRead = BytesRead_t;
	else{
		bSuccess = false;
		*BytesRead = 0;
	}
#endif
      if (bSuccess)
        FPosition += *BytesRead;
      return bSuccess;
    }  // case isFile:

    case isDrive: {
#ifdef _WIN32
	DWORD nBytesReadTemp;
	unsigned pos;
	bool bSuccess;
	if (Length & 511 /*ui64*/) {
		bSuccess = false;
		::SetLastError(ERROR_INVALID_PARAMETER);
	} else
	bSuccess = ReadFile(hHandle, buffer, Length, &nBytesReadTemp, NULL);
	if (!bSuccess && GetLastError() == ERROR_INVALID_PARAMETER) {
		// ERROR_INVALID_PARAMETER - step down read size as discussed above
		char *TempBuffer;
// WARNING FIX FOLLOWING
#if 0
		TempBuffer = (char *)SysGetMem((Length / 4096 + 1) * 4096);
#endif
		pos = 0;
		while (pos < Length && ReadFile(hHandle, TempBuffer + pos, FGeometry.BytesPerSector?FGeometry.BytesPerSector:512, (unsigned long *)&nBytesReadTemp, NULL))
			pos += nBytesReadTemp;
		nBytesReadTemp = pos < Length?pos:Length;
		memcpy_optimized(buffer, TempBuffer, nBytesReadTemp);
// WARNING FIX FOLLOWING
#if 0
		SysFreeMem(TempBuffer);
#endif
		FPosition += pos;
		bSuccess = true;
	}  // if (!bSuccess)
	else
		FPosition += nBytesReadTemp;
	*BytesRead = nBytesReadTemp;
	return bSuccess;
#else
	bool bSuccess = true;
	long int BytesRead_t = read(hHandle, buffer, Length);
	if (BytesRead_t > 0)
		*BytesRead = BytesRead_t;
	else{
		bSuccess = false;
		*BytesRead = 0;
	}
	if (bSuccess)
		FPosition += *BytesRead;
	return bSuccess;
#endif
    }  // case isDrive:

#ifdef _WIN32
#if 0
    case isNBD: {
      int ret = NBDConnection->Read(FPosition, buffer, Length);
      FPosition += ret;
      *BytesRead = ret;
      return true;
    }  // case isNBD:
#endif
#endif

    default:
      return false;
  }  // switch (FType)
}  // bool __fastcall TImageStore::Read(void *buffer, int nLength, unsigned *nBytesRead)
//---------------------------------------------------------------------------

#ifdef _WIN32
//---------------------------------------------------------------------------
// Write() - similar to Read() but different.
//
bool __fastcall TImageStore::Write(void *buffer, unsigned nLength, unsigned long *nBytesWritten) {
  bool retval = true;
#if 0
	if (FType == isNBD)
		retval = (nWrote = NBDConnection->Write(FPosition, buffer, nLength)) != 0;
	else
#endif
		retval = WriteFile(hHandle, buffer, nLength, (unsigned long *)nBytesWritten, NULL);
//    	printf ("Write %i bytes\n", nWrote);

  FPosition += *nBytesWritten;

  
  if (!retval){
#if 1

    const DWORD size = 100+1;
    WCHAR buffer[size];
    DWORD dw = GetLastError(); 
    buffer[size-2] = L'\n';

    FormatMessage(/*FORMAT_MESSAGE_ALLOCATE_BUFFER |*/
            FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            dw,
            0,
            (LPTSTR)&buffer,
            size,
            NULL);

wprintf(L"ERROR: %S", buffer);
#endif
      //ShowMessage(SysErrorMessage(GetLastError()));
//      printf ("Failed to write with error code: %i\n",  GetLastError());
  }
  
  return retval;
}  // bool __fastcall TImageStore::Write(void *buffer, int nLength, unsigned *nBytesWritten)
#else
//---------------------------------------------------------------------------
// Write() - similar to Read() but different.
//
bool __fastcall TImageStore::Write(void *buffer, unsigned nLength, unsigned long *nBytesWritten) 
{
bool retval = true;
unsigned nWrote;

	long int nWrote_t = write(hHandle, buffer, nLength);
	if (nWrote_t >= 0){
		nWrote = nWrote_t;
	}else{
		retval = false;
		nWrote = 0;
	}
	FPosition += nWrote;
	*nBytesWritten = nWrote;

	return retval;
}  // bool __fastcall TImageStore::Write(void *buffer, int nLength, unsigned *nBytesWritten)

#endif

//---------------------------------------------------------------------------
//  Set the file position pointer
//
__int64 TImageStore::Seek(__int64 Offset, DWORD MoveMethod)
{
__int64 NewPosition;
#ifdef _WIN32
#if 0
	if (FType == isNBD) {

		switch (MoveMethod) {
			case FILE_BEGIN: FPosition = Offset; break;
			case FILE_CURRENT: FPosition += Offset; break;
			case FILE_END: FPosition = FGeometry.Bytes + Offset; break;
		}  // switch (MoveMethod)
	}else {
#endif

		DWORD nError;
		LOW_DWORD(Offset) = SetFilePointer(hHandle, LOW_DWORD(Offset), &HIGH_DWORD(Offset), MoveMethod);
		if (LOW_DWORD(Offset) == INVALID_SET_FILE_POINTER && (nError = GetLastError()) != NO_ERROR){
			printf ("Seek error with \n");
			//throw ESelfImageFileError("Seek error with " + FFileName + ": '" + GetErrorMessage(nError) + "'");
		}else
			FPosition = Offset;
#else
		NewPosition = lseek (hHandle,Offset, MoveMethod);
		if (NewPosition<0){
			// ERROR
			printf ("Seek error with \n");
		}else
			FPosition = NewPosition;
#endif
#if 0
	}  // else if (FType != NBD)
#endif

	return FPosition;
}  // bool __fastcall TImageStore::Seek(__int64)

//---------------------------------------------------------------------------
// end of file check
bool TImageStore::eof(void)
{
__uint64 curpos;

	curpos = Seek(0, FILE_CURRENT);

	if (curpos >= FGeometry.Bytes)
		return true;

	return false;
}


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
    Extension = ExtractFileExt(FFileName);
    if (Extension == ".gz") {
      unsigned long ReadCount;
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
          if (Buffer[12] == 'S' && Buffer[13] == 'I' && Buffer[14] == 0x08 && Buffer[15] == 0x00){
            FCompressionFormat = cfGzipChunk;
          }else{
            FCompressionFormat = cfGzipStream;
	  }
      }  // if (ReadCount == 20)
    }  // if (Extension == ".gz")
    else if (Extension == ".bz2") {
      unsigned long ReadCount;
      Buffer = new unsigned char[1024 * 1024];
      // Need to read in the entire first bzip2 chunk, plus the header of the next one.
      // Techinically that's 909610 bytes - make it one meg for good measure
      if (Read(Buffer, 1024 * 1024, &ReadCount)) {
        // If it's a bz2 file, it will start with "BZh" and then have a number from 1 to 9
        if (ReadCount > 20 && Buffer[0] == 'B' && Buffer[1] == 'Z' && Buffer[2] == 'h' && Buffer[3] >= '1' && Buffer[3] <= '9') {
           char Header[] = "BZh_1AY&SY";
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
  }catch (int){ //try 
#if 0
	__finally {
#endif

    if (Buffer)
      delete [] Buffer;
    Seek(0, FILE_BEGIN);
    return;

    }
    if (Buffer)
      delete [] Buffer;
    Seek(0, FILE_BEGIN);
#if 0
  }  // __finally
#endif
}  // void __fastcall TImageStore::CheckCompression(void)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if 0







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
#endif


//---------------------------------------------------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_TImageStore.h - TImageStore class header
// Copyright �2005-2007, Kurt Fitzner <kfitzner@excelcia.org>
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
#ifndef SelfImage_TImageStoreH
#define SelfImage_TImageStoreH
//---------------------------------------------------------------------------
#ifdef _WIN32
#include <windows.h>
#include <winioctl.h>
#endif

#include "global.h"
#include "SelfImage_Exceptions.h"
#include "SelfImage_TypesDefs.h"
//#include "SelfImage_TNBDClient.h"
//#include "SelfImage_Utility.h"
//---------------------------------------------------------------------------

#define BLKGETSIZE	_IO(0x12,96)		/* get partition/device size */
#define BLKGETSIZE64	_IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#define BLKSSZGET	_IO(0x12,104)		/* get sector size */
#define BLKRRPART	_IO(0x12,95)		/* re-read partition table */

//---------------------------------------------------------------------------
// Boot sector/block definitions for each of the partition types that we
// support a check for unused space on
//
// A Windows boot sector is much more compact.  It contains an instruction to
// jump over the configuration data, then it packs all the data in before the
// actual bootstrap code.  In Linux, there is (up to) a full kilobyte of
// bootstrap code, then the filesystem configuration data.
//
#pragma pack(push,1)
struct TWinBootSector {
  unsigned char Jump[3];
  char          OEMID[8];
  WORD          BytesPerSector;
  BYTE          SectorsPerCluster;
  WORD          ReservedSectors;
  BYTE          NumberOfFATs;
  WORD          RootEntries;
  WORD          FAT_TotalSectors;
  BYTE          MediaDescriptor;
  WORD          SectorsPerFAT;
  WORD          SectorsPerTrack;
  WORD          Heads;
  DWORD         HiddenSectors;
  DWORD         FAT_BigTotalSectors;
  DWORD         BigSectorsPerFAT;
  LONGLONG      TotalSectors;
  LONGLONG      MFTCluster;
  LONGLONG      MFTMirrorCluster;
  DWORD         ClustersPerFileRecord;
  DWORD         ClustersPerIndexBlock;
  LONGLONG      VolumeSerial;
  DWORD         Checksum;
  unsigned char Bootstrap[426];
  WORD          EndOfSector;
  unsigned char Cruft[1536];
};

struct TEXT2SuperBlock {
  unsigned char Bootstrap[1024];
  UINT32        InodesCount;
  UINT32        BlocksCount;
  UINT32        ReservedBlocksCount;
  UINT32        FreeBlocksCount;
  UINT32        FreeInodesCount;
  UINT32        FirstDataBlock;
  UINT32        Log2BlockSize;
  INT32         Log2FragSize;
  UINT32        BlocksPerGroup;
  UINT32        FragmentsPerGroup;
  UINT32        InodesPerGroup;
  UINT32        MTime;
  UINT32        WTime;
  UINT16        MountCount;
  INT16         MaxMountCount;
  UINT16        MagicSignature;
  UINT16        FileSystemState;
  UINT16        ErrorBehaviour;
  UINT16        MinorRevisionLevel;
  UINT32        LastCheck;
  UINT32        CheckInterval;
  UINT32        CreatorOS;
  UINT32        RevisionLevel;
  UINT32        DefaultReservedUID;
  UINT32        DefaultReservedGID;
  UINT32        FirstInode;
  UINT16        InodeSize;
  UINT16        BlockGroupNumber;
  UINT32        CompatibleFeatures;
  UINT32        IncompatibleFeatures;
  UINT32        ROCompatibleFeatures;
  UINT8         UUID[8];
  char          VolumeName[16];
  char          LastMountPoint[64];
  UINT32        CompressionAlgorithms;
  unsigned char Cruft[824];
};

union TBootSector {
  struct TWinBootSector Win;
  struct TEXT2SuperBlock EXT2;
};
#pragma pack(pop)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


struct TTargetGeometry {
	__uint64 Bytes;
	uint32_t NumbSectors;
	int BytesPerSector;
	int BytesPerCluster;

};

enum TPartitionType {ptUnknown, ptWindows, ptEXT2};



//enum TImageStoreType { isUndefined, isFile, isDrive, isNBD };

class TImageStore {
  public:
  	TCompressionFormat getCompressionFormat (void){
		return FCompressionFormat;
	}

	TImageStoreType getType (void){
		return FType;
	}

	TImageStore(TImageStoreType isType, AnsiString Target, bool Writable = false);
	~TImageStore();
	bool __fastcall Read(void *buffer, unsigned nLength, unsigned long *nBytesRead);
	bool __fastcall Write(void *buffer, unsigned nLength, unsigned long *nBytesWritten);
	__uint64 GetTotalSizeInBytes (void){
		return (FGeometry.Bytes);
	}
	unsigned long GetNumbSectors (void){
		return (FGeometry.NumbSectors);
	}
	unsigned long GetBytesPerSector (void){
		return (FGeometry.BytesPerSector);
	}
    __int64 Seek(__int64 Offset, DWORD MoveMethod);
    bool IsOpen();
	bool eof(void);
//    unsigned __fastcall LoadEmptyChunkMap(unsigned int ChunkSize);
//    #ifndef _DEBUG
//    bool __fastcall inline IsNextChunkEmpty(void) {  if (EmptyChunkMap) return EmptyChunkMap->Bits[(unsigned int)(FPosition / nChunkSize)]; else return false; }
//    #else
//    bool __fastcall IsNextChunkEmpty(void);
//    #endif

	protected:

	private:
		TImageStoreType		FType;
		AnsiString		FFileName;
		TTargetGeometry		FGeometry;
		TCompressionFormat	FCompressionFormat;
		__uint64		FPosition;

		TPartitionType		PartitionType;
		TBootSector		*BootSector;
		unsigned int		nChunkSize;
		bool			bWriteAccess;
#ifdef _WIN32
		HANDLE			hHandle;
#else
		int			hHandle;
#endif
//		TNBDClient        *NBDConnection;
//		TBitArray         *EmptyChunkMap;

		bool __fastcall Open(void);
		bool __fastcall Close(void);
		void __fastcall CheckCompression(void);
//		void __fastcall CheckFileSystem(void);
//		bool inline __fastcall GetCompressed(void) { return (FCompressionFormat != cfNone); };
//		bool inline __fastcall GetChunkCompressed(void) { return (FCompressionFormat == cfGzipChunk || FCompressionFormat == cfBzip2Chunk); };
//		bool __fastcall GetEXT2VolumeBitmap(__int64 *StartCluster,  BYTE *Bitmap, int BitmapSize, bool Cleanup = false);

};  // class TImageStore
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Some other EXT2 structures - we need these because we're all on our own
// when it comes to Linux formats
struct TEXT2GroupDescriptor {
  UINT32        BlockBitmap;
  UINT32        InodeBitmap;
  UINT32        InodeTable;
  UINT16        FreeBlocksCount;
  UINT16        FreeInodesCount;
  UINT16        UsedDirsCount;
  UINT16        Padding;
  unsigned char Reserved[12];
};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// NBD Defines
#define NBD_MAGIC_CONNECT1 "NBDMAGIC"
#define NBD_MAGIC_CONNECT2 0x0000420281861253ui64
#define NBD_MAGIC_REQUEST 0x25609513
#define NBD_MAGIC_REPLY   0x67446698
//---------------------------------------------------------------------------
// NBD convenience macros
#ifndef ntohll
#define ntohll(x) (((__int64)(ntohl((int)((x << 32) >> 32))) << 32) | (unsigned int)ntohl(((int)(x >> 32))))
#endif
#ifndef htonll
#define htonll(x) ntohll(x)
#endif
//---------------------------------------------------------------------------
// EXT2 Defines
#define EXT2_MAGIC 0xEF53u
//---------------------------------------------------------------------------
#endif

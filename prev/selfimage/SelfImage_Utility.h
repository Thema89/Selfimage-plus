//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_Utility.h - Utility and miscelaneous functions header
// Copyright ©2005-2007, Kurt Fitzner <kfitzner@excelcia.org>
//---------------------------------------------------------------------------
// This file is part of SelfImage.
//
// SelfImageis free software; you can redistribute it and/or modify
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
#ifndef SelfImage_UtilityH
#define SelfImage_UtilityH
//---------------------------------------------------------------------------
#include <WinIOCTL.h>
#include "SelfImage_TDriveList.h"
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Prototypes
//
void __fastcall InitializeUtilityFunctions(void);
AnsiString __fastcall GetErrorMessage(DWORD Error);
AnsiString __fastcall MakeByteLabel(long double ddByteCount);
TStringList * __fastcall GetNTDirectoryObjectContents(WideString Directory);
AnsiString __fastcall GetNTLinkDestination(WideString Source);
HANDLE NTOpen(WideString FileName, ACCESS_MASK DesiredAccess, ULONG FileAttributes, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions);
bool GetVolumeNameForMountPoint(LPCSTR lpszVolumeMountPoint, LPSTR lpszVolumeName, DWORD cchBufferLength);
void CheckVersion(void);
AnsiString GetFileSystemType(AnsiString FileName);
__int64 GetFreeDiskSpace(AnsiString FileName);
__int64 GetFileSizeByName(AnsiString FileName);
unsigned GetCPUCount(void);
bool IsFileCompressed(AnsiString FileName);
unsigned char * __fastcall FindString(unsigned char *SearchArea, unsigned int AreaSize, unsigned char *LookFor, bool PreProcess = false);
bool __fastcall SplitConnectionString(AnsiString ConnectionString, AnsiString &HostPart, int &PortPart);
extern "C" void __fastcall bz_internal_error(int errcode);
extern "C" void * zlib_allocmem(void *dummy, unsigned Count, unsigned Size);
extern "C" void zlib_freemem(void *dummy, void *Address);
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Defines needed for above
#define LAST_ERROR NO_ERROR
#define GetLastErrorMessage() GetErrorMessage(LAST_ERROR)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// For SetThreadName()
//
typedef struct tagTHREADNAME_INFO {
  DWORD dwType;     // must be 0x1000
  LPCSTR szName;    // pointer to name (in user addr space)
  DWORD dwThreadID; // thread ID (-1=caller thread)
  DWORD dwFlags;    // reserved for future use, must be zero
} THREADNAME_INFO;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TBitArray class
//
class TBitArray {
  protected:
    unsigned  FArraySize;
    void __fastcall Error(void);
    void __fastcall SetSize(unsigned NewSize);
    void __fastcall SetBit(unsigned Index, bool Value);
    bool __fastcall GetBit(unsigned Index);

  public:
    unsigned *FArray;
    __fastcall TBitArray(void);
    __fastcall ~TBitArray();
    int __fastcall OpenBit(void);
    void __fastcall LoadBuffer(void *Bits, unsigned BitCount);
    unsigned __fastcall FindSetBit(void);
    bool inline __fastcall HasSetBit(void) { return (FindSetBit() < FArraySize); }

    __property bool Bits[unsigned Index] = {read = GetBit, write = SetBit};
    __property unsigned Size = {read = FArraySize, write = SetSize};
};  // class TBitArray

#define BITSPERUINT (sizeof(unsigned int) * 8)
#define DWORDENDIANSWAP(n) (((n)>>24) | (((n)<<8) & 0x00FF0000) | (((n)>>8) & 0x0000FF00) | ((n)<<24))
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Some ntdll.dll stuff - we use some undocumented NT system calls to read
// the /device directory and get all the disk devices
//
typedef long NTSTATUS;

struct UNICODE_STRING {
    WORD Length;
    WORD MaximumLength;
    WCHAR *Buffer;
};

struct OBJECT_ATTRIBUTES {
  DWORD           Length;
  HANDLE          RootDirectory;
  UNICODE_STRING *ObjectName;
  DWORD           Attributes;
  void           *SecurityDescriptor;
  void           *SecurityQualityOfService;
};

struct OBJDIR_INFORMATION {
  UNICODE_STRING  ObjectName;
  UNICODE_STRING  ObjectTypeName;
  char           *Data;
};

struct IO_STATUS_BLOCK {
  union {
    NTSTATUS Status;
    PVOID Pointer;
  };
  ULONG_PTR Information;
};

typedef NTSTATUS __stdcall (*NtOpenDirectoryObject_t)(HANDLE *DirObjectHandle, ACCESS_MASK DesiredAccess,
                             OBJECT_ATTRIBUTES *ObjectAttributes);
typedef NTSTATUS __stdcall (*NtQueryDirectoryObject_t)(HANDLE DirObjectHandle, OBJDIR_INFORMATION *DirObjInformation,
                             DWORD BufferLength, DWORD GetNextIndex, DWORD IgnoreInputIndex, DWORD *ObjectIndex, DWORD *DataWritten);
typedef NTSTATUS __stdcall (*NtOpenSymbolicLinkObject_t)(HANDLE *SymLinkObjHandle, ACCESS_MASK DesiredAccess,
                             OBJECT_ATTRIBUTES *ObjectAttributes);
typedef NTSTATUS __stdcall (*NtQuerySymbolicLinkObject_t)(HANDLE SymLinkObjHandle, UNICODE_STRING *LinkName, DWORD *DataWritten);
typedef NTSTATUS __stdcall (*NtCreateFile_t)(HANDLE *FileHandle, ACCESS_MASK DesiredAccess, OBJECT_ATTRIBUTES *ObjectAttributes,
                            IO_STATUS_BLOCK *IoStatusBlock, LARGE_INTEGER *AllocationSize, ULONG FileAttributes, ULONG ShareAccess,
                            ULONG CreateDisposition, ULONG CreateOptions, PVOID EaBuffer, ULONG EaLength);
typedef NTSTATUS __stdcall (*RtlInitUnicodeString_t)(UNICODE_STRING *DestinationString, WCHAR *SourceString);

typedef WINBASEAPI BOOL __stdcall (*GetVolumeNameForVolumeMountPointA_t)(LPCSTR lpszVolumeMountPoint, LPSTR lpszVolumeName, DWORD cchBufferLength);


#define OBJ_INHERIT          0x00000002
#define OBJ_PERMANENT        0x00000010
#define OBJ_EXCLUSIVE        0x00000020
#define OBJ_CASE_INSENSITIVE 0x00000040
#define OBJ_OPENIF           0x00000080
#define OBJ_OPENLINK         0x00000100
#define OBJ_VALID_ATTRIBUTES 0x000001F2

#define DIRECTORY_QUERY      0x0001
#define SYMBOLIC_LINK_QUERY  0x0001

#define NT_SUCCESS(x) (x>=0)

#define FILE_SUPERSEDE                          0x00000000
#define FILE_OPEN                               0x00000001
#define FILE_CREATE                             0x00000002
#define FILE_OPEN_IF                            0x00000003
#define FILE_OVERWRITE                          0x00000004
#define FILE_OVERWRITE_IF                       0x00000005
#define FILE_MAXIMUM_DISPOSITION                0x00000005

#define FILE_DIRECTORY_FILE                     0x00000001
#define FILE_WRITE_THROUGH                      0x00000002
#define FILE_SEQUENTIAL_ONLY                    0x00000004
#define FILE_NO_INTERMEDIATE_BUFFERING          0x00000008
#define FILE_SYNCHRONOUS_IO_ALERT               0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT            0x00000020
#define FILE_NON_DIRECTORY_FILE                 0x00000040
#define FILE_CREATE_TREE_CONNECTION             0x00000080
#define FILE_COMPLETE_IF_OPLOCKED               0x00000100
#define FILE_NO_EA_KNOWLEDGE                    0x00000200
#define FILE_OPEN_FOR_RECOVERY                  0x00000400
#define FILE_RANDOM_ACCESS                      0x00000800
#define FILE_DELETE_ON_CLOSE                    0x00001000
#define FILE_OPEN_BY_FILE_ID                    0x00002000
#define FILE_OPEN_FOR_BACKUP_INTENT             0x00004000
#define FILE_NO_COMPRESSION                     0x00008000
#define FILE_RESERVE_OPFILTER                   0x00100000
#define FILE_OPEN_REPARSE_POINT                 0x00200000
#define FILE_OPEN_NO_RECALL                     0x00400000
#define FILE_OPEN_FOR_FREE_SPACE_QUERY          0x00800000
#define FILE_COPY_STRUCTURED_STORAGE            0x00000041
#define FILE_STRUCTURED_STORAGE                 0x00000441
#define FILE_VALID_OPTION_FLAGS                 0x00ffffff
#define FILE_VALID_PIPE_OPTION_FLAGS            0x00000032
#define FILE_VALID_MAILSLOT_OPTION_FLAGS        0x00000032
#define FILE_VALID_SET_FLAGS                    0x00000036
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Some miscelaneous useful #defines
// MIN and MAX because the stdlib ones don't work
//
#define MIN(x,y) ((x<y)?x:y)
#define MAX(x,y) ((x>y)?x:y)
//---------------------------------------------------------------------------

#endif

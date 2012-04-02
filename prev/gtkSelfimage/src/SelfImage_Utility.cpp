#include <string.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <sys/stat.h>

//#include "memlib.h"
#include "global.h"
#include "SelfImage_TypesDefs.h"
#include "SelfImage_Exceptions.h"
#include "SelfImage_Utility.h"

#ifdef _WIN32
bool HaveNTCalls = true;
//---------------------------------------------------------------------------
static NtCreateFile_t                                        NtCreateFile              = NULL;
static NtOpenDirectoryObject_t                               NtOpenDirectoryObject     = NULL;
static NtQueryDirectoryObject_t                              NtQueryDirectoryObject    = NULL;
static NtOpenSymbolicLinkObject_t                            NtOpenSymbolicLinkObject  = NULL;
static NtQuerySymbolicLinkObject_t                           NtQuerySymbolicLinkObject = NULL;
static RtlInitUnicodeString_t                                RtlInitUnicodeString      = NULL;
static GetVolumeNameForVolumeMountPointA_t                   GetVolumeNameForMountPointA = NULL;
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
// Convert a number into a normalized string with a KB/MB/GB/TB suffix
// appropriate for a byte capacity label
//
std::string MakeByteLabel(uint64_t ddByteCount, unsigned char Precision)
{
std::ostringstream tempretval;
#if 0
unsigned long temp;

         temp = (unsigned long) ddByteCount;
printf("MakeByteLabel  Bytes: %lu\n", temp);
                       if (temp == 0)
                          printf ("Hallo\n");
#endif		
    tempretval.setf(ios::fixed);
    tempretval.str("");

	if (ddByteCount > 1099511627776.0)
		tempretval<< setprecision(Precision) << (long double) (ddByteCount / 1099511627776.0)<<"TB";
	else if (ddByteCount > 1073741824.0){
		tempretval<< setprecision(Precision) << (long double) (ddByteCount / 1073741824.0)<<"GB";
	}else if (ddByteCount > 1048576.0){
		tempretval<< setprecision(Precision) << (long double) (ddByteCount / 1048576.0)<<"MB";
	}else if (ddByteCount > 1024.0){
		tempretval<< setprecision(Precision) << (long double) (ddByteCount / 1024.0)<<"KB";
	}else{
		tempretval<< setprecision(Precision) << (long double) (ddByteCount)<<" Bytes";
	}
	
	std::string retval(tempretval.str());

	return(retval);
}

//---------------------------------------------------------------------------


std::string ExtractFileExt(std::string FileName)
{
std::string Extension;

	if (FileName.size() > 3 && FileName[FileName.size()-3] == '.'){
		Extension += FileName[FileName.size()-3];
		Extension += FileName[FileName.size()-2];
		Extension += FileName[FileName.size()-1];
	}else if (FileName.size() > 4 &&  FileName[FileName.size()-4] == '.'){
		Extension += FileName[FileName.size()-4];
		Extension += FileName[FileName.size()-3];
		Extension += FileName[FileName.size()-2];
		Extension += FileName[FileName.size()-1];
	}
	
	return Extension;
}

const char* WideStringToString (wchar_t *wString)
{
std::string result;

		int Length = wcslen (wString);
		for (int j = 0; j < Length; j++){
			result += (char)wString[j]&0xff;
		}
        return result.c_str();         
}

const wchar_t* StringToWideString (const std::string String)
{
wchar_t *wString;

        wString = new wchar_t[String.size()+4];
		for (unsigned int j = 0; j < String.size(); j++){
			wString[j]    = String[j];
			wString[j+1]  = L'\0';
		}
        return wString;
}

#ifdef _WIN32
//---------------------------------------------------------------------------
void __fastcall InitializeUtilityFunctions(void) {

  HMODULE ntdll;
  HMODULE kernel32dll;
  OSVERSIONINFO Version;

  // We used to just depend on the pressence of ntdll.dll to tell if this is running on an NT-based system
  // but things like SysInternal's NTFS for Windows 98 adds this DLL, so now we actually do the official version
  // test and go from there.  This will also filter out Windows NT 4.0, which probably wouldn't work either.
  Version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&Version);

  if (Version.dwMajorVersion >= 5 && Version.dwPlatformId == VER_PLATFORM_WIN32_NT) {
    ntdll = GetModuleHandle("ntdll.dll");
    kernel32dll = GetModuleHandle("kernel32.dll");
    if (ntdll && kernel32dll) {
      NtOpenDirectoryObject     = (NtOpenDirectoryObject_t)    GetProcAddress(ntdll, "NtOpenDirectoryObject");
      NtQueryDirectoryObject    = (NtQueryDirectoryObject_t)   GetProcAddress(ntdll, "NtQueryDirectoryObject");
      NtOpenSymbolicLinkObject  = (NtOpenSymbolicLinkObject_t) GetProcAddress(ntdll, "NtOpenSymbolicLinkObject");
      NtQuerySymbolicLinkObject = (NtQuerySymbolicLinkObject_t)GetProcAddress(ntdll, "NtQuerySymbolicLinkObject");
      NtCreateFile              = (NtCreateFile_t)             GetProcAddress(ntdll, "NtCreateFile");
      RtlInitUnicodeString      = (RtlInitUnicodeString_t)     GetProcAddress(ntdll, "RtlInitUnicodeString");
      GetVolumeNameForMountPointA= (GetVolumeNameForVolumeMountPointA_t)GetProcAddress(kernel32dll, "GetVolumeNameForVolumeMountPointA");
      if (NtOpenDirectoryObject && NtQueryDirectoryObject && NtOpenSymbolicLinkObject && NtCreateFile && RtlInitUnicodeString && GetVolumeNameForMountPointA){
        HaveNTCalls = true;
        }
    }  // if (ntdll)
  }  // if (Version.dwMajorVersion >= 5)
}  // void __fastcall InitializeUtilityFunctions(void)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// This is a wrapper function around the native NtCreateFile() function.
// Officially, NtCreateFile() is undocumented, but it is widely used.
//
HANDLE NTOpen(WideString FileName, ACCESS_MASK DesiredAccess, ULONG FileAttributes, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions)
{
  UNICODE_STRING usFileName;
  OBJECT_ATTRIBUTES oa;
  HANDLE hFile;
  IO_STATUS_BLOCK ios;
  NTSTATUS nStatus;

  if (!HaveNTCalls)
    return INVALID_HANDLE_VALUE;
  RtlInitUnicodeString(&usFileName, FileName);
  oa.Length = sizeof(oa);
  oa.RootDirectory = 0;
  oa.ObjectName = &usFileName;
  oa.Attributes = OBJ_CASE_INSENSITIVE;
  oa.SecurityDescriptor = NULL;
  oa.SecurityQualityOfService = NULL;
  nStatus = NtCreateFile(&hFile, DesiredAccess, &oa, &ios, NULL, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, NULL, 0);
  if (NT_SUCCESS(nStatus))
    return hFile;
  else
    return INVALID_HANDLE_VALUE;
}  // HANDLE XpCreateFile()

//---------------------------------------------------------------------------
AnsiString __fastcall GetNTLinkDestination(WideString Source)
{
  UNICODE_STRING usName;
  OBJECT_ATTRIBUTES oa;
  NTSTATUS nStatus;
  HANDLE hLink;
  wchar_t *buffer;
  AnsiString retval;

  RtlInitUnicodeString(&usName, Source);
  oa.Length = sizeof(OBJECT_ATTRIBUTES);
  oa.RootDirectory = 0;
  oa.ObjectName = &usName;
  oa.Attributes = OBJ_CASE_INSENSITIVE;
  oa.SecurityDescriptor = NULL;
  oa.SecurityQualityOfService = NULL;
  nStatus = NtOpenSymbolicLinkObject(&hLink, SYMBOLIC_LINK_QUERY, &oa);
  if (NT_SUCCESS(nStatus)) {
    buffer = (wchar_t *)malloc(2048);
    usName.Length = 0;
    usName.MaximumLength = 1024;
    usName.Buffer = buffer;
    nStatus = NtQuerySymbolicLinkObject(hLink, &usName, NULL);
    usName.Buffer[usName.Length/2] = 0;
    if (NT_SUCCESS(nStatus)){
      //retval = String(usName.Buffer);
      int Length = wcslen (usName.Buffer);
      //wprintf (L"%s, Length = %i\n", DList[i], Length);
      for (int j = 0; j < Length; j++){
			retval += usName.Buffer[j];
      }
    }
    free(buffer);
  }  // if (NTSUCCESS(nStatus))
  return retval;
}  // AnsiString __fastcall GetNTLinkDestination(AnsiString Source)

//---------------------------------------------------------------------------
// Get the contents of an NT directory object - this uses some undocumented
// calls from ntdll.dll
//

std::vector<wchar_t*> __fastcall GetNTDirectoryObjectContents(WideString Directory)
{
  std::vector<wchar_t*> Entries;
  std::string Entry;
  UNICODE_STRING usDir;
  OBJECT_ATTRIBUTES oa;
  HANDLE hDeviceDir;
  NTSTATUS nStatus;
  OBJDIR_INFORMATION *DirInfo;
  DWORD index;
  AnsiString Error;
  wchar_t *Temp;

  if (!HaveNTCalls)
    return Entries;

  RtlInitUnicodeString(&usDir, Directory);
  oa.Length = sizeof(OBJECT_ATTRIBUTES);
  oa.ObjectName = &usDir;
  oa.Attributes = OBJ_CASE_INSENSITIVE;
  oa.SecurityDescriptor = NULL;
  oa.SecurityQualityOfService = NULL;
  oa.RootDirectory = 0;

  // Fail when trying to open a floppy
  if (wmemcmp (L"\\Device\\Floppy", Directory, 14) == 0 ){
     return Entries;
  }

  nStatus = NtOpenDirectoryObject(&hDeviceDir, STANDARD_RIGHTS_READ | DIRECTORY_QUERY, &oa);
  if (!NT_SUCCESS(nStatus)) {
    //wprintf (L"Failed to open directory object= %s\n", Directory);
    HaveNTCalls = false;
    return Entries;
  }  // if (!NT_SUCCESS(nStatus))
  DirInfo = (OBJDIR_INFORMATION *)malloc(2048);
  index = 0;

  while (NT_SUCCESS(NtQueryDirectoryObject(hDeviceDir, DirInfo, 1024, true, false, &index, NULL))){
        Temp = new wchar_t[255];
        swprintf (Temp, L"%s\\%s", Directory, DirInfo->ObjectName.Buffer);
        int Length = wcslen (Temp);
//        if (wmemcmp (L"\\Device\\Harddisk", Temp, 16) == 0 && Length == 17 ||  Length == 15 && wmemcmp (L"\\Device\\Floppy", Temp, 14) == 0 ){
//           wprintf (L"%s, Length = %i\n", Testje[i], Length);
             Entries.push_back( Temp );
//        }


  }
  CloseHandle(hDeviceDir);
  free(DirInfo);
//  if (Entries.size())
    return Entries;
//  else {  // if (!Entries->Count)
//    delete Entries;
//    return NULL;
//  }  // if (!Entries->Count)
}   // TStringList * __fastcall GetNTDirectoryObjectContents(WideString Directory)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Wrapper for the Windows NT-based GetVolumeNameForVolumeMountPointA()
// function.  It is wrapped here to keep all dynamic function calls here in
// one unit.  We dynamically get the function because it doesn't exist in
// Windows 98.  It would be nice if delayed loading could keep this from
// throwing an error when run on Windows 98, but you can't specify a DLL
// entry point to delay load, only an entire DLL.  Because this function is
// in kernel32.dll, there is no way to prevent it from tripping the delayed
// loading.
//
bool GetVolumeNameForMountPoint(LPCSTR lpszVolumeMountPoint, LPSTR lpszVolumeName, DWORD cchBufferLength)
{
  return GetVolumeNameForMountPointA(lpszVolumeMountPoint, lpszVolumeName, cchBufferLength);
}  // bool GetVolumeNameForMountPoint(LPCSTR lpszVolumeMountPoint, LPSTR lpszVolumeName, DWORD cchBufferLength)

//---------------------------------------------------------------------------
// Format a Windows error into an AnsiString.  If Error isn't provided, then
// perform a GetLastError() to grab it.
//
std::string GetErrorMessage(DWORD Error = LAST_ERROR)
{
  char *buffer;
  std::string retval;

  if (Error == NO_ERROR)
    Error = GetLastError();

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, Error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR) &buffer, 0, NULL );

    //retval = String(buffer).Trim();
    retval = buffer;

  LocalFree(buffer);

  return(retval);
}  // inline AnsiString GetLastErrorMessage(void)

//---------------------------------------------------------------------------

#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Find a string in memory, if it's there.  A simplified form of the
// Boyer-Moore algorithm that is very similar to Horspool's adaptation.
// Call this function with PreProcess=true to configure it for a new search
// string.  It is not thread safe - it can only be used by one thread at a
// time.
//
unsigned char * __fastcall FindString(unsigned char *SearchArea, unsigned int AreaSize, char *LookFor, bool PreProcess)
{
  static unsigned char ShiftTable[256];
  static unsigned int nLookForSize;
//  unsigned int nPos;

  // Create the shift table.  Tells how many characters we can shift per iteration.
  if (PreProcess) {
    nLookForSize = strlen((char*)LookFor);
    for (int n = 0; n < 256; n++)
      ShiftTable[n] = nLookForSize + 1;
    for (unsigned int n = 0; n < nLookForSize; n++)
      ShiftTable[(int)LookFor[n]] = nLookForSize - n;
  }  // if (PreProcess)

  for (register unsigned int n=0; n < AreaSize - nLookForSize; n+= ShiftTable[SearchArea[n + nLookForSize]])
    if (!memcmp(SearchArea + n, LookFor, nLookForSize))
      return SearchArea + n;

  return NULL;
}  // unsigned char * FindString(unsigned char *SearchArea, unsigned int Size, unsigned char *What)
//---------------------------------------------------------------------------

__int64 GetFileSizeByName(std::string FileName)
{
#ifdef _WIN32
struct _stati64 buf;

    if (_stati64( FileName.c_str(),&buf) != 0)
        return 0;

    return buf.st_size;
#else
struct stat64 buf;

    if (stat64( FileName.c_str(),&buf) != 0)
        return 0;

    return buf.st_size;

#endif
}

//---------------------------------------------------------------------------
// Returns true if the file is compressed with a method we know how to
// decompress
//
bool IsFileCompressed(std::string FileName)
{
  std::string Extension = ExtractFileExt(FileName);
  // At some point there may be more checking done here than just an extension check
  if (Extension == ".gz" || Extension == ".bz2")
    return true;
  else
    return false;
}  // bool IsCompressed(AnsiString FileName)

//---------------------------------------------------------------------------

#if 0

//---------------------------------------------------------------------------
void SetThreadName(AnsiString Name)
{
  THREADNAME_INFO ThreadInfo;
  ThreadInfo.dwType = 0x1000;
  ThreadInfo.szName = Name.c_str();
  ThreadInfo.dwThreadID = -1;
  ThreadInfo.dwFlags = 0;

  __try {
    RaiseException( 0x406D1388, 0, sizeof(ThreadInfo)/sizeof(DWORD),(DWORD*)&ThreadInfo );
  }
  __except (EXCEPTION_CONTINUE_EXECUTION) {}
}  // void SetThreadName(AnsiString Name)
//---------------------------------------------------------------------------














//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Check to see if SelfImage is up-to-date - notify the user if it isn't
//
void CheckVersion(void) {

  TDateTime *dtLastCheck = NULL;
  char *filename = NULL;
  char *buffer = NULL;
  char *verbuff = NULL;
  unsigned nVersionInfoSize;
  int nBuildPos;
  static AnsiString sMyVersion = "";
  static int nMyBuild = 0;
  AnsiString sCurrentVersion, sURL;
  int nCurrentBuild = 0;
  TIdHTTP *HTTP;
  DWORD nDummy;

  __ENTERFUNCTION__;

  try {
    try {
      formSelfImageNewVersionNotify = NULL;
      if (!SelfImageConfig->Values["Update Notify"]) {
        LOG(LOG_DEBUG, "Update Notify is turned off - not checking for new version.");
        goto END;  // Ugly - C++ Builder silently ignores a __finally block if a return is process from a nested try.
      }  // if (!SelfImageConfig->Values["Update Notify"])

      if (String(SelfImageConfig->Values["Last Version Check"]).IsEmpty())
        SelfImageConfig->Values["Last Version Check"] = (Now()-1).DateTimeString();
      dtLastCheck = new TDateTime(String(SelfImageConfig->Values["Last Version Check"]));
      if (!DaysBetween(Now(), *dtLastCheck)) {
        LOG1(LOG_DEBUG, "Last new version check was performed %s - not checking again.", dtLastCheck->DateTimeString().c_str());
        goto END;  // Ugly - C++ Builder silently ignores a __finally block if a return is process from a nested try.
      }  // if (!DaysBetween(Now(), *dtLastCheck))

      SelfImageConfig->Values["Last Version Check"] = Now().DateTimeString();

      // Get this executable's version - this is a pain, so let's just do it once for each time SelfImage is instantiated
      if (!nMyBuild) {
        LOG(LOG_DEBUG, "Retrieving internal version number.");
        filename = (char *)malloc(MAXPATH);
        GetModuleFileName(HInstance, filename, MAXPATH);
        nVersionInfoSize = GetFileVersionInfoSize(filename, &nDummy);
        // These following three exceptions should never make it to the user except through the logs in a debug build.
        // Since they are only for logging, they are allowed to have text that doesn't come from the string table.
        if (!nVersionInfoSize)
          throw ESelfImageSystemError("GetFileVersionInfoSize() failed: " + GetLastErrorMessage());
        buffer = (char *)malloc(nVersionInfoSize);
        if (!GetFileVersionInfo(filename, NULL, nVersionInfoSize, buffer))
          throw ESelfImageSystemError("GetFileVersionInfo() failed:" + GetLastErrorMessage());
        if (!VerQueryValue(buffer, "\\StringFileInfo\\100904E4\\FileVersion", (void **)&verbuff, NULL))
          throw ESelfImageSystemError("VerQueryValue() failed:" + GetLastErrorMessage());
        sMyVersion = verbuff;
        nBuildPos = sMyVersion.LastDelimiter(".");
        nMyBuild = sMyVersion.SubString(nBuildPos + 1, sMyVersion.Length() - nBuildPos).ToInt();
        sMyVersion = sMyVersion.SubString(1,nBuildPos-1) + " (Build " + String(nMyBuild) + ")";
        LOG1(LOG_MESSAGE, "Internal version string retrieved from resources: \"%s\".", sMyVersion.c_str());
      }  // if (sMyVersion.IsEmpty())

      // Get SelfImage's current version from the interweb - woo, this is cool
      HTTP = new TIdHTTP(NULL);
      sURL = SelfImageConfig->Values["Version URL"];
      LOG1(LOG_DEBUG, "Attempting to retrieve version string from URL \"%s\".", sURL.c_str());
      try {
        sCurrentVersion = HTTP->Get(SelfImageConfig->Values["Version URL"]);
        nBuildPos = sCurrentVersion.Pos("(Build ");
        nCurrentBuild = sCurrentVersion.SubString(nBuildPos + 7, sCurrentVersion.Length() - nBuildPos - 7).ToInt();
      }
      catch (...) { sCurrentVersion = ""; }
      delete HTTP;
      #ifdef __LOGGINGENABLED__
      if (!sCurrentVersion.IsEmpty())
        LOG2(LOG_MESSAGE, "Version string \"%s\" retrieved from %s.", sCurrentVersion.c_str(), sURL.c_str())
      else
        LOG2(LOG_WARNING, "Unable to retrieve version string from URL \"%s\": %s", sURL.c_str(), HTTP->ResponseText.c_str());
      #endif

      // Compare the two...
      if (nCurrentBuild && nMyBuild < nCurrentBuild && sCurrentVersion != String(SelfImageConfig->Values["Notify Squelch"])) {
        LOG(LOG_DEBUG, "Notifying user of new version.");
        formSelfImageNewVersionNotify = new TformSelfImageNewVersionNotify(NULL, sMyVersion, sCurrentVersion);
        if (formSelfImageNewVersionNotify->ShowModal() == mrCancel)
          SelfImageConfig->Values["Notify Squelch"] = sCurrentVersion;
      }  // if (different versions)
    }  // inner try
    catch (Exception &e) {
      LOG2(LOG_ERROR, "Exception of type \"%s\" raised during new version check with message \"%s\"", String(e.ClassName()).c_str(), e.Message.c_str());
    }  // catch
  }  // outer try
  __finally {
    if (dtLastCheck)
      delete dtLastCheck;
    if (filename)
      free(filename);
    if (buffer)
      free(buffer);
    if (formSelfImageNewVersionNotify) {
      delete formSelfImageNewVersionNotify;
      formSelfImageNewVersionNotify = NULL;
    }  // if (formSelfImageNewVersionNotify)
  }
  END:
  __RETURNFUNCTION__;
}  // void CheckVersion(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Returns the type of filesystem that a file resides on.
//
AnsiString GetFileSystemType(AnsiString FileName)
{
  AnsiString retval;
  char *VolumeName, *FileSystemName;
  DWORD VolumeSerialNumber, MaxComponentLength, FileSystemFlags;
  bool success;

  VolumeName = new char[MAX_PATH];
  FileSystemName = new char[MAX_PATH];
  success = GetVolumeInformation((ExtractFileDrive(FileName) + "\\").c_str(), VolumeName, MAX_PATH, &VolumeSerialNumber, &MaxComponentLength, &FileSystemFlags, FileSystemName, 64);
  if (success)
    retval = String(FileSystemName);
  delete VolumeName;
  delete FileSystemName;

  return String(FileSystemName);
}  // AnsiString GetFileSystemType(AnsiString FileName)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Return the amount of free space on the volume that a file resides on.
//
__int64 GetFreeDiskSpace(AnsiString FileName)
{
  unsigned __int64 FreeBytesAvailable, TotalBytes, TotalFreeBytes;
  bool success;

  success = ::GetDiskFreeSpaceEx((ExtractFileDrive(FileName) + "\\").c_str(), (ULARGE_INTEGER *)&FreeBytesAvailable, (ULARGE_INTEGER *)&TotalBytes, (ULARGE_INTEGER *)&TotalFreeBytes);
  if (success)
    return FreeBytesAvailable;
  else
    return 0;
}  // __int64 GetFreeSpace(AnsiString FileName)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Returns the number of virtual processors.  The GetSystemInfo() system call
// that this uses counts a CPU with hyperthreading as two CPUs.  In our case,
// this is what we want.
//
unsigned GetCPUCount(void)
{
  SYSTEM_INFO SysInfo;
  GetSystemInfo(&SysInfo);
  return SysInfo.dwNumberOfProcessors;
}  // unsigned GetCPUCount(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// Split a NBD connection string (host:port) into the host and port parts.
//
bool __fastcall SplitConnectionString(AnsiString ConnectionString, AnsiString &HostPart, int &PortPart)
{
  AnsiString sHost;
  int nPort = 0, nPortPos;

  nPortPos = ConnectionString.Pos(":") + 1;
  if (nPortPos) {
    sHost = ConnectionString.SubString(1,nPortPos - 2);
    if (TryStrToInt(ConnectionString.SubString(nPortPos, ConnectionString.Length() - nPortPos + 1), nPort) && nPort > 0 && nPort < 65536) {
      HostPart = sHost;
      PortPart = nPort;
      return true;
    }  // if (port number is valid)
  }  // if (nPortPos)
  return false;
}  // bool __fastcall SplitConnectionString(AnsiString ConnectionString, AnsiString &HostPart, int &PortPart)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Called by the bzip2 library when it runs into an error.
//
extern "C" void __fastcall bz_internal_error(int errcode)
{
  throw ESelfImageCompressionError("bzip2 library reports error code " + String(errcode));
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Zlib memory allocation callback - needed because zlib's memory allocation
// isn't thread-safe.  This may be a bug somewhere in Borland's SysGetMem,
// which is what malloc() ends up calling.  It appears to enter a critical
// section, but when it is used, SelfImage acts like it has a memory leak.
//
// This function isn't needed if FastMM is used with SelfImage.  FastMM
// is thread safe and now used by default.
//
extern "C" void * zlib_allocmem(void *dummy, unsigned Count, unsigned Size)
{
  return GlobalAlloc(GMEM_FIXED, Count * Size);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Zlib memory free callback.  See above.
//
extern "C" void zlib_freemem(void *dummy, void *Address)
{
  GlobalFree(Address);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TBitArray class
//

//---------------------------------------------------------------------------
// TBitArray::TBitArray(void)
__fastcall TBitArray::TBitArray(void)
{
  FArray = NULL;
  FArraySize = 0;
}  // TBitArray::TBitArray(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TBitArray::~TBitArray()
//
TBitArray::~TBitArray()
{
  SetSize(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// void TBitArray::Error(void)
//
// Throw an EBitsError exception
//
// This is a separate function because of the way the compiler works.  Any
// function that has the potential to throw an exception has a function call
// silently added to exception initialization code.  I want the bit setting
// and testing functions to be as fast as possible, so since they won't often
// throw exceptions, no need to waste time with the exception init call.
//
void TBitArray::Error(void)
{
  throw EBitsError("Bit array index out of range");
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// void __fastcall TBitArray::SetSize(unsigned Value)
//
// Resize the array.  Any new space added is zeroed out.  Old values are
// copied over.  I could have used SysReallocMem() instead of allocating
// a new buffer and copying the old contents every time, but even realloc
// will do a whole new alloc and copy contents over a lot of the time (it
// just does it behind the scenes), and my memcpy is way faster than its is.
//
void __fastcall TBitArray::SetSize(unsigned NewSize)
{
  unsigned *NewMem = NULL;
  unsigned nNewMemSize, nOldMemSize;

  nNewMemSize = (NewSize + BITSPERUINT - 1) / BITSPERUINT * sizeof(unsigned);
  nOldMemSize = (FArraySize + BITSPERUINT - 1) / BITSPERUINT * sizeof(unsigned);
  if (nNewMemSize != nOldMemSize) {
    if (nNewMemSize) {
      NewMem = (unsigned *)SysGetMem(nNewMemSize);
      if (nNewMemSize > nOldMemSize)
        memzero_optimized(NewMem + nOldMemSize, nNewMemSize - nOldMemSize);
    }  // if (nNewMemSize)
    if (nOldMemSize) {
      if (nNewMemSize)
        memcpy_optimized(NewMem, FArray, MIN(nNewMemSize, nOldMemSize));
      SysFreeMem(FArray);
    }  // if (nOldMemSize)
    FArray = NewMem;
  }  // if (nNewMemSize != nOldMemSize)
  FArraySize = NewSize;
}  // void __fastcall TBitArray::SetSize(unsigned NewSize)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// void __fastcall TBitArray::LoadBuffer(void *Buffer, unsigned BitCount)
//
// Load the bit array from an external buffer.
//
void __fastcall TBitArray::LoadBuffer(void *Bits, unsigned BitCount)
{
  unsigned nBufferBytes = (BitCount + 7) / 8;
  unsigned nNewMemSize = (BitCount + BITSPERUINT - 1) / BITSPERUINT * sizeof(unsigned);
  unsigned nMask;

  if (FArray)
    SysFreeMem(FArray);
  FArray = (unsigned *)SysGetMem(nNewMemSize);
  memcpy_optimized(FArray, Bits, nBufferBytes);
  // Mask out all the bits in the final storage array value that are in excess of our bit count
  nMask = UINT_MAX << (BITSPERUINT - BitCount % BITSPERUINT);
  nMask = DWORDENDIANSWAP(nMask);  // Have I mentioned how stupid little endian is?
  FArray[nNewMemSize / sizeof(unsigned) - 1] &= nMask;
  FArraySize = BitCount;
}  // void __fastcall TBitArray::LoadBuffer(void *Buffer, unsigned BitCount)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// void __fastcall TBitArray::SetBit(int Index, bool Value)
//
// Set a bit in the array
//
// For a __fastcall method, "this" starts out in EAX, and the first two
// regular parameters are in EDX and ECX respectively.  This simplifies
// our assembly somewhat.  "Value" is actually in the low 8 bits of ECX
// (or CL) because bools are stored in one byte in Borland compilers.
//
void __fastcall TBitArray::SetBit(unsigned Index, bool Value)
{
  __asm {
    // If Index > FArraySize then resize up the array.
              cmp   edx,   [eax].FArraySize; // EDX = Index, EAX = this
              jae   @@RESIZE
    @@BEGIN:  mov   eax,   [eax].FArray      // EAX is now the FArray pointer
    // Intel bit routines all work counting from the least significant bit in each byte.
    // This isn't useful for here, because with the ability to load a bitmap, we will be
    // dealing with bitmap data from other sources.  We need to treat bits left to right,
    // which is counting from the most significant bit.  So here we manipulate our index
    // to effectively do a: Index = Index & 0xfffffff8 + 7 - (Index mod 8)
              push  edx                      // Because we're going to hammer EDX
              and   edx,   7                 // Index mod 8
              mov   ebx,   7
              sub   ebx,   edx               // (7 - (Index mod 8)) is now in ebx
              pop   edx                      // Grab Index back off the stack because we hammered it
              and   edx,   0xfffffff8        // Index & 0xfffffff8 (which is Index - Index mod 8)
              add   edx,   ebx               // ... plus ebx
    // Check to see if we are setting or resetting the bit...
              or    cl,    cl                // CL = Value
              jz    @@RESET                  // If we're setting then...
              bts   [eax], edx               // set the bit
              jmp   @@EXIT
    @@RESET:  btr   [eax], edx               // otherwise reset it
              jmp   @@EXIT
    @@RESIZE:
              push  eax
              push  edx
              push  ecx
  }  // __asm
  // C++ inline assemblers are brain dead in that they won't allow you to call a member function... even a non-virtual
  // completely normal plain jane member function.  So, drop out of the __asm block...
  SetSize(Index+1);
  // And now because this hammered all our registers, copy them off the stack where this function's entry code put them
  __asm {
              pop   ecx
              pop   edx
              pop   eax
              jmp   @@BEGIN
    // Let the function's own cleanup/return code handle the return
    @@EXIT:
  }  // __asm
}  // void __fastcall TBitArray::SetBit(int Index, bool Value)
//---------------------------------------------------------------------------


#pragma warn -8070  // Turn off warning of no return value
//---------------------------------------------------------------------------
// bool __fastcall TBitArray::GetBit(unsigned Index)
//
// Retrieve a bit in the array.  If the index is out of range,
//
// EAX = this, EDX = Index.  Return result is in EAX.
//
bool __fastcall TBitArray::GetBit(unsigned Index)
{
  __asm {
              cmp   edx,   [eax].FArraySize
              jae   @@RANGE
              mov   eax,   [eax].FArray      // EAX now is the FArray pointer
              push  edx
              and   edx,   7                 // Index mod 8
              mov   ebx,   7
              sub   ebx,   edx               // (7 - (Index mod 8)) is now in ebx
              pop   edx                      // Grab Index back off the stack because we hammered it
              and   edx,   0xfffffff8        // Index & 0xfffffff8 (which is Index - Index mod 8)
              add   edx,   ebx               // ... plus ebx
              bt    [eax], edx               // Set the carry flag if the bit is set
              sbb   eax,   eax               // If the carry flag is set, the result here will be -1 (0xFFFFFFFF)
              and   eax,   1
     @@EXIT:
              #ifdef _DEBUG
              mov   esp, ebp                 // Clean up and return
              pop   ebp
              ret
              #else
              pop   ebx
              ret
              #endif
     @@RANGE:
  }  // __asm
  Error();
} // bool __fastcall TBitArray::GetBit(unsigned Index)
//---------------------------------------------------------------------------
#pragma warn .8070


//---------------------------------------------------------------------------
// unsigned __fastcall TBitArray::FindSetBit(void)
//
// Find the first set bit in the bit array.  If there are none, then it
// returns the size of the array.
//
unsigned __fastcall TBitArray::FindSetBit(void)
{
  unsigned nArraySize = (FArraySize + BITSPERUINT - 1) / BITSPERUINT;
  for (unsigned n = 0; n < nArraySize; n++)
    if (FArray[n]) {
      int nBit = n * sizeof(unsigned);
      while (!GetBit(nBit))
        nBit++;
      return nBit;
    }  // if (FArray[n])
  return FArraySize;
}  // unsigned __fastcall TBitArray::FindSetBit(void)
//---------------------------------------------------------------------------
#endif

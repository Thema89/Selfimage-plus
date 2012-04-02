//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_TDriveList.cpp - TDriveList and TDriveInfo classes
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
$History: **** V 0.1 by kfitzner ****
$History: * selfimage_tdrivelist.cpp - 2005-11-07 2:39:38 AM - 11582 Bytes
$History: * selfimage_tdrivelist.h - 2005-11-07 12:31:56 AM - 3049 Bytes
$History: * Initial check-in
$History: **** V 0.2 by kfitzner ****
$History: * selfimage_tdrivelist.cpp - 2005-11-12 5:22:11 PM - 11845 Bytes
$History: * selfimage_tdrivelist.h - 2005-11-12 5:20:24 PM - 3053 Bytes
$History: * Typo in copyright area + change in program description
$History: **** V 1.0 by kfitzner ****
$History: * selfimage_tdrivelist.cpp - 2005-11-15 11:28:46 AM - 12112 Bytes
$History: * selfimage_tdrivelist.h - 2005-11-12 5:20:24 PM - 3053 Bytes
$History: * Rename "\device\harddisk0 (Full Drive)" to "... (Entire Disk)" 
$History: **** V 1.1 by kfitzner ****
$History: * selfimage_tdrivelist.cpp - 2005-12-02 2:04:40 AM - 12698 Bytes
$History: * selfimage_tdrivelist.h - 2005-12-02 1:26:42 AM - 3084 Bytes
$History: * Clean up memory leak (not destructed)
$History: **** Latest ** V 1.2 by kfitzner ** 2007-02-28 3:30:05 PM ****
$History: * Replace GetVolumeNameForVolumeMountPoint() call with 
$History: * a call to a wrapper that calls the same function but 
$History: * dynamically loaded to avoid errors in Win 98
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// File Notes:
//---------------------------------------------------------------------------
// 25 Oct 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// The TDriveList and TDriveInfo classes used to be mostly c-style utility
// functions located in SelfImage_Utility.cpp that operated on struct TDrive.
//---------------------------------------------------------------------------
#include <vcl.h>
#include <winioctl.h>
#pragma hdrstop

#include "SelfImage_Utility.h"
#include "SelfImage_TDriveList.h"
#include "SelfImage_ProgressDialog.h"
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TDriveInfo class implementation
//
// An instantiation of this class represents a single drive/partition.


//---------------------------------------------------------------------------
// Constructor
//
__fastcall TDriveInfo::TDriveInfo(AnsiString DisplayName, AnsiString DeviceName, AnsiString MountPoint, TDeviceType Type)
{
  FDisplayName = DisplayName;
  FDeviceName  = DeviceName;
  FMountPoint  = MountPoint;
  FDriveType   = Type;
  // Refresh();
}


//---------------------------------------------------------------------------
// Refresh all the drive information
//
void __fastcall TDriveInfo::Refresh(void)
{
  HANDLE hDrive;
  PARTITION_INFORMATION Partition;
  DISK_GEOMETRY Geometry;
  DWORD nCount;
  bool bSuccess;

  if (!HaveNTCalls)
    hDrive = CreateFile(FDeviceName.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_BACKUP_SEMANTICS, NULL);
  else
    hDrive = NTOpen(FDeviceName, GENERIC_READ, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN, FILE_SEQUENTIAL_ONLY);
  if (hDrive != INVALID_HANDLE_VALUE) {
      bSuccess = DeviceIoControl(hDrive, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &Geometry, sizeof(Geometry), &nCount, NULL);
    if (bSuccess) {
      DeviceIoControl(hDrive, IOCTL_DISK_GET_PARTITION_INFO, NULL, 0, &Partition, sizeof(Partition), &nCount, NULL);
      CloseHandle(hDrive);
      if (DriveType == driveFloppy)
        FBytes = Geometry.Cylinders.QuadPart * Geometry.TracksPerCylinder * Geometry.SectorsPerTrack * Geometry.BytesPerSector;
      else
        FBytes = Partition.PartitionLength.QuadPart;
      FBytesPerSector = Geometry.BytesPerSector;
      FSectors = FBytes / FBytesPerSector;
      FSectorsPerTrack = Geometry.SectorsPerTrack;
      FReadable = true;
    } else {  // if (!bSuccess)
      FBytes = 0; FSectors = 0; FBytesPerSector = 0; FSectorsPerTrack = 0;
      FReadable = false;
    }  // else if (!bSuccess)
  }  // if (hDrive != INVALID_HANDLE_VALUE)
  // Is this partition writable?  In Windows, only unmounted partitions can be written to.
  if (FDriveType == driveFloppy)
    FWritable = true;
  else { // if (Drive->DriveType != driveFloppy)
    if (!HaveNTCalls)
      hDrive = CreateFile(FDeviceName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    else
      hDrive = NTOpen(FDeviceName, GENERIC_WRITE, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN, FILE_SEQUENTIAL_ONLY);
    if (hDrive != INVALID_HANDLE_VALUE) {
      FWritable = true;
      CloseHandle(hDrive);
    } else
      FWritable = false;
  }  // if (Drive->DriveType != driveFloppy)
  // UpToDate = true;
}  // void __fastcall TDriveInfo::Refresh(void)


//---------------------------------------------------------------------------
// TDriveList class implementation
//
// A list of drives/partitions based on (but not derived from) TList

//---------------------------------------------------------------------------
// Constructor
//
__fastcall TDriveList::TDriveList(bool ShowProgress)
{
  TStringList *Devices;
  TStringList *Volumes;


  FList = new TList;

  // Our first choice is to use the Windows 2000/XP device listing, as it will allow access to unmounted partitions.
  // To get a listing of drives, we get a listing of the virtual \Device directory.  There are a lot more entries in
  // the \Device virtual directory than drives, so we need to also check each result to see if it's what we want.
  Devices = GetNTDirectoryObjectContents("\\Device");
  if (Devices) {
    // Before enumerating the devices to search for drives, cross reference all the volume links to drive letters
    // we need this later to determine which drive letters each harddisk device is mounted as.
    char *buffer = (char *)malloc(1024);
    HANDLE hVolume;
    Volumes = new TStringList;
    for (char d = 'C'; d <= 'Z'; d++) {
      AnsiString Drive = String(d) + ":\\";
      if (GetVolumeNameForMountPoint(Drive.c_str(), buffer, 1000)) {
        AnsiString Volume = String(buffer);
        AnsiString VolumeLink = GetNTLinkDestination("\\??\\" + Volume.SubString(5,Volume.Length()-5));
        Volumes->Add(VolumeLink + "=" + Drive);
      }  // if (GetVolumeNameForMountPoint(Drive.c_str(), buffer, 1000))
    }  // for (char d = 'C'; d <= 'Z'; c++)
    free(buffer);

    // Now we enumerate all the devices in \Device
    for (int n = 0; n < Devices->Count; n++) {
      AnsiString DeviceName = Devices->Strings[n], DisplayName, MountPoint;
      TDeviceType DeviceType;

      int Length = DeviceName.Length();
      if (/* Length == 14 && DeviceName.Pos("CdRom") == 9 || */ Length == 15 && DeviceName.Pos("Floppy") == 9 || Length == 17 && DeviceName.Pos("Harddisk") == 9) {
        DisplayName = DeviceName;
        MountPoint = Volumes->Values[DeviceName];
        if (!MountPoint.IsEmpty())
          DisplayName += " (" + MountPoint + ")";
        if (DeviceName.Pos("Harddisk")) {
          TStringList *Partitions = GetNTDirectoryObjectContents(DeviceName);
          DeviceName += "\\Partition0";
          DisplayName += " (entire disk)";
          DeviceType = driveFixed;
          // If this is a hard disk, check for and add any partitions
          if (Partitions) {
            for (int p = 0; p < Partitions->Count; p++) {
              AnsiString PartitionDeviceName = Partitions->Strings[p], PartitionDisplayName, PartitionMountPoint;
              if (PartitionDeviceName.Pos("Partition") == 19 && PartitionDeviceName.Length() == 28 && !PartitionDeviceName.Pos("Partition0")) {
                TDriveInfo *Partition;
                AnsiString VolumeLink;
                PartitionDisplayName = PartitionDeviceName;
                VolumeLink = GetNTLinkDestination(PartitionDeviceName);
                PartitionMountPoint = Volumes->Values[VolumeLink];
                if (!PartitionMountPoint.IsEmpty())
                  PartitionDisplayName += " (" + PartitionMountPoint + ")";
                Partition = new TDriveInfo(PartitionDisplayName, PartitionDeviceName, PartitionMountPoint, driveFixed);
                FList->Add(Partition);
              }  // if (PartitionName.Pos("Partition") == 19 && PartitionName.Length() == 28)
            }  // for (int p = 0; p < Partitions->Count; p++)
            delete Partitions;
          }  // if (Partitions)
        } else if (DeviceName.Pos("Floppy")) {
          DeviceType = driveFloppy;
        }  // if (DeviceName.Pos("Harddisk"))
        TDriveInfo *Drive = new TDriveInfo(DisplayName, DeviceName, MountPoint, DeviceType);
        FList->Add(Drive);
      }  // if (is a CdRom, Floppy, or Harddisk)
    }  // for (int n = 0; n < Devices->Count; n++)
    delete Devices;
    delete Volumes;
  } else {  // else if (!Devices)
    // If we don't have access to the Windows 2000/XP functions, we just get a straight list of drive letters.  When run
    // outside of an NT environment, we lose the ability to open partitions that haven't been mapped as drive letters.
    // We could use INT 0x21 functions for this, but that is beyond what I want to do.
    char buffer[105];

    GetLogicalDriveStrings(105, buffer);
    for (char *sDrive = buffer; *sDrive != '\0'; sDrive += 4) {
      if (GetDriveType(sDrive) == DRIVE_FIXED) {
        AnsiString DeviceName;
        HANDLE hDrive;
        bool bIsRealDrive;
        PARTITION_INFORMATION Partition;
        DWORD nDummy;
        SHFILEINFO Info;
        TDriveInfo *Drive;

        // Here is where we see if it's a real drive, or one created by SUBST.  You can perform a IOCTL_DISK_GET_PARTITION_INFO on
        // any read drive, but not on a SUBSTed one.
        DeviceName  = "\\\\.\\" + String(sDrive).SubString(1,2);
        hDrive = CreateFile(DeviceName.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_BACKUP_SEMANTICS, NULL);
        bIsRealDrive = DeviceIoControl(hDrive, IOCTL_DISK_GET_PARTITION_INFO, NULL, 0, &Partition, sizeof(Partition), &nDummy, NULL);
        CloseHandle(hDrive);
        if (!bIsRealDrive)
          continue;

        SHGetFileInfo(sDrive, 0, &Info, sizeof(Info), SHGFI_DISPLAYNAME);
        Drive = new TDriveInfo(Info.szDisplayName, DeviceName, sDrive, driveFixed);
        FList->Add(Drive);
      }  // if (GetDriveType(sDrive) == DRIVE_FIXED)
    }  // for (char *c = buffer; *c != '\0'; c += 4)
  }  // else if (!Devices)

  // Refresh all the drives - this can take some time as we attempt to lock each drive for writing to see which
  // drives can and can't be written to.  We optionally display a progress dialog for this part.
  if (ShowProgress) {
    Application->CreateForm(__classid(TProgressDialog), &ProgressDialog);
    ProgressDialog->Display("Enumerating drive partitions...", 0, Count);
  }  // if (ShowProgress)
  for (int n=0; n < Count; n++) {
    Items[n]->Refresh();
    if (ShowProgress)
      ProgressDialog->StepBy(1);
  }  // for (int n=0; n < Count; n++)
  if (ShowProgress) {
    ProgressDialog->Close();
    delete ProgressDialog;
  }  // if (ShowProgress)
}  // __fastcall TDriveList::TDriveList(void)


//---------------------------------------------------------------------------
// Destructor
//
__fastcall TDriveList::~TDriveList()
{
  for (int n = 0; n < FList->Count; n++)
    delete (TDriveInfo *)FList->Items[n];
}  // __fastcall TDriveList::~TDriveList()
//---------------------------------------------------------------------------


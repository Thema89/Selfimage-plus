//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_TDriveList.h - TDriveList and TDriveInfo class headers
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
#ifndef SelfImage_TDriveListH
#define SelfImage_TDriveListH
#if 0
#include <list>

//---------------------------------------------------------------------------
extern bool HaveNTCalls;
//---------------------------------------------------------------------------
enum TDeviceType { driveFixed, driveCD, driveFloppy };

//---------------------------------------------------------------------------
class TDriveInfo {
  public:
//    __property AnsiString  DisplayName = {read = FDisplayName};
//    __property AnsiString  DeviceName  = {read = FDeviceName};
//    __property AnsiString  MountPoint  = {read = FMountPoint};
//    __property TDeviceType DriveType   = {read = FDriveType};
//    __property __int64     Bytes = {read = FBytes};
//    __property __int64     Sectors = {read = FSectors};
//    __property int         BytesPerSector = {read = FBytesPerSector};
//    __property int         SectorsPerTrack = {read = FSectorsPerTrack};
//    __property bool        Readable = {read = FReadable};
//    __property bool        Writable = {read = FWritable};

    __fastcall TDriveInfo(AnsiString DisplayName, AnsiString DeviceName, AnsiString MountPoint, TDeviceType Type);
    void __fastcall Refresh(void);

  private:
    AnsiString FDisplayName, FDeviceName, FMountPoint;
    TDeviceType FDriveType;
    __int64 FBytes, FSectors;
    int FBytesPerSector, FSectorsPerTrack;
    bool FReadable;
    bool FWritable;
};

//---------------------------------------------------------------------------
class TDriveList {
  public:
//    __property TDriveInfo *Items[int Index] = {read = GetItem};
//    __property int Count = {read = GetCount};

    __fastcall TDriveList(bool ShowProgress);
    __fastcall ~TDriveList();

  private:
//    std::list FList;

    TDriveInfo *GetItem(int Index) {return (TDriveInfo *)(FList->Items[Index]); }
    int GetCount(void) { return FList->Count; }
};
//---------------------------------------------------------------------------

#endif
#endif

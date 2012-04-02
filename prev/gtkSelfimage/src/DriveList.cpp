#include <string.h>
#include <string>
#include <fstream>
#include <wchar.h>


#include "sort.h"
#include "global.h"
#include "DriveList.h"
#include "SelfImage_TImageStore.h"
#include "SelfImage_Utility.h"



TDriveList::TDriveList()
{
	Enumerate ();
}

TDriveList::~TDriveList()
{

}


void TDriveList::AddDrive (std::string Path, std::string Name, wchar_t *wPath, wchar_t *wName, std::string DriveSize, bool EntireDisk)
{
TDriveNamePath TempDriveNamePath;

	TempDriveNamePath.DriveName = Name;
	TempDriveNamePath.DrivePath = Path;
	TempDriveNamePath.wDriveName = wName;
	TempDriveNamePath.wDrivePath = wPath;
	TempDriveNamePath.SizeString = DriveSize;
	TempDriveNamePath.EntireDisk = EntireDisk;
	DriveNamePaths.push_back (TempDriveNamePath);
}

std::string TDriveList::GetDriveName (unsigned int index)
{
std::string DriveName;

	if (index > DriveNamePaths.size())
		return DriveName;

	return DriveNamePaths[index].DriveName;
}

bool TDriveList::IsEntireDisk (unsigned int index)
{
	if (index > DriveNamePaths.size())
		return false;

	return DriveNamePaths[index].EntireDisk;
}

std::string TDriveList::GetDrivePath (unsigned int index)
{
std::string DrivePath;

	if (index > DriveNamePaths.size())
		return DrivePath;

	return DriveNamePaths[index].DrivePath;
}

unsigned int TDriveList::GetDriveIndexByPath (std::string Path)
{
	for (unsigned int index = 0; index < DriveNamePaths.size(); index++){
		if (DriveNamePaths[index].DrivePath == Path)
			return index;
	}
	return 0;
}

unsigned int TDriveList::GetDriveIndexByName (std::string Name)
{
	for (unsigned int index = 0; index < DriveNamePaths.size(); index++){
		if (DriveNamePaths[index].DriveName == Name)
			return index;
	}
	return 0;
}

std::string TDriveList::GetDriveSizeString (unsigned int index)
{
std::string SizeString;

	if (index > DriveNamePaths.size())
		return SizeString;

	return DriveNamePaths[index].SizeString;
}

std::string TDriveList::GetDrivePath (const std::string DriveName)
{
std::string Empty;

	for (unsigned int i = 0; i < DriveNamePaths.size(); i++){
		if (DriveNamePaths[i].DriveName == DriveName)
			return DriveNamePaths[i].DrivePath;
	}
	return Empty;

}


int TDriveList::size (void){
	return DriveNamePaths.size();
}

void TDriveList::Enumerate (void)
{
#ifdef _WIN32
	Enumerate_win32 ();
#else
	Enumerate_lin ();
#endif
}
void TDriveList::Enumerate_lin (void)
{
#ifndef _WIN32
char* ListEntry;
std::string line;
std::ifstream proc_partitions( "/proc/partitions" );
TImageStore *Istore;
std::string DriveSize, DriveName, DrivePath;

	if ( proc_partitions )
	{
		char c_str[255] ;
		
		while ( getline( proc_partitions, line ) )
			if ( sscanf( line .c_str(), "%*d %*d %*d %255s", c_str ) == 1 )
			{
				line = "/dev/" ; 
				line += c_str;
				ListEntry = new char[line.size()+2];
				strncpy (ListEntry, line.c_str(), line.size()+1);
				Istore = new TImageStore( isDrive, ListEntry );
				DriveSize = MakeByteLabel(Istore->GetTotalSizeInBytes());
				DriveName = ListEntry;
				DriveName += " (" + DriveSize + ")";
				DrivePath = ListEntry;
				//printf ("Size = %i\n", DrivePath.size());
				if (DrivePath.size() == 8)
					AddDrive ( DrivePath, DriveName, NULL, NULL, DriveSize, true);
				else
					AddDrive ( DrivePath, DriveName, NULL, NULL, DriveSize, false);
				delete Istore;
			}
		proc_partitions .close() ;
	}
#endif
}

//*********************************************************************************************
// This only enumerates the entire disks not the seperate partitions
//*********************************************************************************************
void TDriveList::Enumerate_win32 (void)
{
#ifdef _WIN32
std::vector<wchar_t*> DList;
char c_str[255];
TImageStore		*Istore;
std::string		DriveSize,
			DriveName,
			DrivePath,
			Old;
std::string		PartitionDeviceName;
std::string		VolumeLink;
std::vector<VolumeInfo> Volumes;
VolumeInfo		VolInf;
bool			VolumeFound;

	DList = GetNTDirectoryObjectContents(L"\\Device");
	
	// First create a list with driveletters and volume link names
	char *buffer = (char *)malloc(1024);
	HANDLE hVolume;
	for (char d = 'C'; d <= 'Z'; d++) {
		std::string Drive;
		Drive = d;
		Drive += ":\\";
		VolInf.DriveLetter = Drive;
		if (GetVolumeNameForMountPoint(Drive.c_str(), buffer, 1000)) {
			AnsiString Volume = buffer;//String(buffer);
			std::string TmpVol = "\\??\\";
			TmpVol += Volume.substr(4,Volume.size()-5);
			VolInf.VolumeName = GetNTLinkDestination((WCHAR*)StringToWideString (TmpVol.c_str()));
			Volumes.push_back(VolInf);
		}
	}
	free(buffer);

	// Add all partitions to the drivelist
	for (int i = 0; i < DList.size(); i++){
		int Length = wcslen (DList[i]);

		if (wmemcmp (L"\\Device\\Harddisk", DList[i], 16) == 0 && Length == 17 ||  Length == 15 && wmemcmp (L"\\Device\\Floppy", DList[i], 14) == 0 ){
			for (int j = 0; j < Length; j++){
				c_str[j] = DList[i][j];
				c_str[j+1] = '\0';
			}
			DriveName = c_str;
			DrivePath = c_str;
		
			std::vector<wchar_t*> Partitions = GetNTDirectoryObjectContents((wchar_t*)StringToWideString(DrivePath));
			std::sort(Partitions.begin(), Partitions.end(), WideStringSorter);
		
			DrivePath += "\\Partition0";
			
            if (strncmp ("\\Device\\Floppy", DrivePath.c_str(), 14) == 0){
                        continue;
            }
  			//printf ("%s line %i: Try to open %s\n", __FILE__, __LINE__, DrivePath.c_str());
		
			Istore = new TImageStore( isDrive, DrivePath );
			DriveSize = MakeByteLabel(Istore->GetTotalSizeInBytes());
			//DriveName = DrivePath;
			//DriveName += " (entire disk " + DriveSize + ")";
			DriveName = "Complete disk ";
			DriveName += DrivePath.at(16);
			DriveName += " ( " + DriveSize + " )";

			DriveSize = MakeByteLabel(Istore->GetTotalSizeInBytes(), 2);

			if (Istore->GetTotalSizeInBytes() != 0)
				AddDrive (DrivePath, DriveName, DList[i], DList[i], DriveSize, true);
			delete Istore;

			// If we have found one or more partitions
			if (Partitions.size () > 0) {
				for (int p = 0; p < Partitions.size(); p++) {
					std::string PartitionDisplayName, PartitionMountPoint;
					PartitionDeviceName.clear();
					PartitionDeviceName = WideStringToString(Partitions[p]);
					if (PartitionDeviceName.find ("Partition", 0) != string::npos && PartitionDeviceName.find ("Partition0", 0) == string::npos){
						VolumeLink = GetNTLinkDestination(Partitions[p]);
					        Istore = new TImageStore( isDrive, PartitionDeviceName );

					        // Assume it is a invalid partition if it fails to open
					        if (Istore->IsOpen() && Istore->GetTotalSizeInBytes() != 0){
							DriveSize = MakeByteLabel(Istore->GetTotalSizeInBytes());
							DrivePath = PartitionDeviceName;
							DriveName = PartitionDeviceName;

							VolumeFound = false;
							for (int k = 0; k < Volumes.size(); k++){
								if (Volumes[k].VolumeName == VolumeLink){
									DriveName += " ( " + Volumes[k].DriveLetter + " " + DriveSize + ")";  
									VolumeFound = true;
								}
							}
							if (!VolumeFound)
								DriveName += " (" + DriveSize + ")";

							DriveSize = MakeByteLabel(Istore->GetTotalSizeInBytes(), 2);

							AddDrive (DrivePath, DriveName, Partitions[p], Partitions[p], DriveSize, false);
						}
						delete Istore;
					}
				}
			}else
				printf ("No partitions found\n");
		}
	}
#endif
}


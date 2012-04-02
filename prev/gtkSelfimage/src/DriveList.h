#ifndef DRIVELIST_H_
#define DRIVELIST_H_

#include <string>
#include <vector>

typedef struct{
	std::string	DriveName;
	std::string	DrivePath;
	wchar_t		*wDriveName;
	wchar_t		*wDrivePath;
    std::string SizeString;
    bool        EntireDisk;
}TDriveNamePath;



class TDriveList{
	private:
		std::vector <TDriveNamePath> DriveNamePaths;
		#ifdef _WIN32
		void TDriveList::enum_win32(void);
        #endif
	public:
		TDriveList();
		~TDriveList();
		void AddDrive (std::string Path, std::string Name, wchar_t *wPath, wchar_t *wName, std::string DriveSize, bool EntireDisk);
        bool IsEntireDisk (unsigned int index);
		std::string GetDrivePath (unsigned int index);
		std::string GetDrivePath (const std::string DriveName);
		std::string GetDriveName (unsigned int index);
        unsigned int GetDriveIndexByPath (std::string Path);
        unsigned int GetDriveIndexByName (std::string Name);
        std::string GetDriveSizeString (unsigned int index);
		int size (void);
		void Enumerate (void);
		void Enumerate_win32 (void);
		void Enumerate_lin (void);

};

#endif

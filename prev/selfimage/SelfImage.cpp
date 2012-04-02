//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage.cpp - Project source - framework for the program
// Copyright ©2005-2007 Kurt Fitzner <kfitzner@excelcia.org>
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
// VCS: $Version: 1 $ $Revision: 5 $
/*
$History: **** V 0.1 by kfitzner ****
$History: * selfimage.cpp - 2005-07-30 6:54:02 AM - 875 Bytes
$History: * Initial check-in
$History: **** V 0.2 by kfitzner ****
$History: * selfimage.cpp - 2005-08-06 6:28:53 PM - 3772 Bytes
$History: * Add shared TConfiguration.cpp, initialize utility funcs
$History: **** V 0.3 by kfitzner ****
$History: * selfimage.cpp - 2005-08-17 7:51:34 AM - 3887 Bytes
$History: * Change licensing - only version 2 of the GPL, no later versions
$History: **** V 0.4 by kfitzner ****
$History: * selfimage.cpp - 2005-11-07 2:41:32 AM - 4140 Bytes
$History: * Change architecture to threaded I/O
$History: **** V 0.5 by kfitzner ****
$History: * selfimage.cpp - 2005-11-10 10:05:06 AM - 4358 Bytes
$History: * Add version check
$History: **** V 0.6 by kfitzner ****
$History: * selfimage.cpp - 2005-11-18 3:54:54 PM - 5554 Bytes
$History: * Semi-permanent check-out - checked in for 0.2 Beta 2 release
$History: **** V 0.7 by kfitzner ****
$History: * selfimage.cpp - 2005-11-28 3:48:06 PM - 5746 Bytes
$History: * Semi-permanent check-out (BCB needs this file to be writable).
$History: **** V 1.0 by kfitzner ****
$History: * selfimage.cpp - 2005-11-30 7:21:31 AM - 5875 Bytes
$History: * Semi-permanent check-out
$History: **** V 1.1 by kfitzner ****
$History: * selfimage.cpp - 2005-12-02 9:43:30 AM - 6086 Bytes
$History: * Plug memory lead (Log not deleted)
$History: **** V 1.2 by kfitzner ****
$History: * selfimage.cpp - 2005-12-09 4:41:07 PM - 6294 Bytes
$History: * Semi-permanent checkout - checked in for 1.1
$History: **** V 1.3 by kfitzner ****
$History: * selfimage.cpp - 2005-12-27 6:02:09 PM - 6451 Bytes
$History: * Semi-permanent checkout - checked in for 1.1.1
$History: **** V 1.4 by kfitzner ****
$History: * selfimage.cpp - 2007-02-07 7:40:23 PM - 6620 Bytes
$History: * Semi-permanent checkout - check in for 1.1.2
$History: **** Latest ** V 1.5 by kfitzner ** 2007-03-30 7:18:56 PM ****
$History: * Semi-permanent checkout - checked in for 1.2
*/
//----------------------------  $NoKeywords ---------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TProgramLog.h"
#include "TConfiguration.h"
#include "SelfImage_Utility.cpp"
//---------------------------------------------------------------------------
TConfiguration *SelfImageConfig = NULL;
TProgramLog    *Log = NULL;
AnsiString      sBinDir;
AnsiString      sVersionString;
int             nBuildNumber;
bool            HaveNTCalls = false;
//---------------------------------------------------------------------------
USEFORM("SelfImage_MainForm.cpp", MainForm);
USEFORM("SelfImage_ProgressDialog.cpp", ProgressDialog);
USEFORM("SelfImage_NewVersionNotify.cpp", formSelfImageNewVersionNotify);
USEFORM("SelfImage_Preferences.cpp", formSelfImagePreferences);
USEFORM("SelfImage_AboutBox.cpp", formSelfImageAboutBox);
USEFORM("SelfImage_NBDDialog.cpp", formSelfImageConnect);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  char *FileName;
  //TBitArray *TestArray;
  //bool TestBit;

  //TestArray = new TBitArray;
  //TestArray->Bits[0] = true;
  //TestBit = TestArray->Bits[0];

  try {
    IsMultiThread = true;
    Application->Initialize();

    // Windows spawns off two or three more threads whenever you open a file requester.  To make the program's actual
    // main GUI thread easier to pick out, we name it here.
    SetThreadName("Main GUI Thread");

    FileName = (char *)SysGetMem(MAX_PATH);        // Find where the module is sitting and use that as the directory for where the help file is too.
    if (!GetModuleFileName(hInstance, FileName, MAX_PATH))
      sBinDir = "";
    else {
      char *buffer, *verbuff;
      unsigned verbuffcount;
      int nVersionInfoSize, nBuildPos;
      DWORD nDummy;

      sBinDir = ExtractFileDir(FileName);
      nVersionInfoSize = GetFileVersionInfoSize(FileName, &nDummy);
      buffer = (char *)SysGetMem(nVersionInfoSize);
      GetFileVersionInfo(FileName, NULL, nVersionInfoSize, buffer);
      VerQueryValue(buffer, "\\StringFileInfo\\100904E4\\FileVersion", (void **)&verbuff, &verbuffcount);
      sVersionString = verbuff;
      nBuildPos = sVersionString.LastDelimiter(".");
      nBuildNumber = sVersionString.SubString(nBuildPos + 1, sVersionString.Length() - nBuildPos).ToInt();
      sVersionString = sVersionString.SubString(1,nBuildPos-1);
      SysFreeMem(buffer);
    }  // else if (GetModuleFileName(hInstance, FileName, MAX_PATH))

    SysFreeMem(FileName);

    Application->HelpFile = sBinDir + "\\SelfImage.hlp";

    // LOGGING - the following statement initializes the log.  In addition to this, logging must be enabled either
    // by turning on debugging mode, or by #define-ing ENABLE_LOGGING.  If this is not done, then all the log macros
    // will expand to nothing.
    // Set the logging level in the last parameter below - see TProgramLog.h for all the log levels.
    Log = NEWLOG(sBinDir + "\\SelfImage.log", true, false, LOG_NONE);
    __ENTERPROGRAM__;

    InitializeUtilityFunctions();

    SelfImageConfig = new TConfiguration();
    CheckVersion();
    Application->Title = "SelfImage";
    Application->CreateForm(__classid(TMainForm), &MainForm);
    Application->Run();

    __LEAVEPROGRAM__;
    DELETELOG(Log);
  }  // try

  catch (Exception &exception) {
    Application->ShowException(&exception);
  }  // catch (Exception &exception)

  catch (...) {
    try {
       throw Exception("");
    }  // try
    catch (Exception &exception) {
       Application->ShowException(&exception);
    }  // catch (Exception &exception)
  }  // catch (...)

  return 0;
}  // WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//---------------------------------------------------------------------------





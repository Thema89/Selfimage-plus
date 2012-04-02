//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_MainForm.cpp - Main form
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
// VCS: $Version: 1 $ $Revision: 13 $
/*
$History: **** V 0.1 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-07-30 6:44:58 PM - 2973 Bytes
$History: * selfimage_mainform.dfm - 2005-07-30 5:52:20 PM - 3054 Bytes
$History: * selfimage_mainform.h - 2005-07-30 6:44:58 PM - 1495 Bytes
$History: * selfimage_mainform.ddp - 2005-07-30 6:44:56 PM - 51 Bytes
$History: * Initial check-in
$History: **** V 0.2 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-08-01 3:11:59 PM - 11269 Bytes
$History: * selfimage_mainform.dfm - 2005-08-01 2:48:30 PM - 6098 Bytes
$History: * selfimage_mainform.h - 2005-08-01 2:38:08 PM - 3569 Bytes
$History: * selfimage_mainform.ddp - 2005-08-01 3:09:38 PM - 51 Bytes
$History: * Initial coding.
$History: **** V 0.3 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-08-03 4:54:43 AM - 13643 Bytes
$History: * selfimage_mainform.dfm - 2005-08-01 2:48:30 PM - 6098 Bytes
$History: * selfimage_mainform.h - 2005-08-02 9:33:32 PM - 3585 Bytes
$History: * selfimage_mainform.ddp - 2005-08-02 10:21:38 PM - 51 Bytes
$History: * Change writes to use asynchronous I/O
$History: **** V 0.4 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-08-06 6:29:05 PM - 17290 Bytes
$History: * selfimage_mainform.dfm - 2005-08-06 5:44:58 PM - 6762 Bytes
$History: * selfimage_mainform.h - 2005-08-06 4:39:32 PM - 3586 Bytes
$History: * selfimage_mainform.ddp - 2005-08-06 6:26:46 PM - 51 Bytes
$History: * Add support for unmounted volumes
$History: **** V 0.5 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-08-07 1:43:29 AM - 17659 Bytes
$History: * selfimage_mainform.dfm - 2005-08-06 6:45:06 PM - 6767 Bytes
$History: * selfimage_mainform.h - 2005-08-06 4:39:32 PM - 3586 Bytes
$History: * selfimage_mainform.ddp - 2005-08-06 6:48:38 PM - 51 Bytes
$History: * Add version to form caption
$History: **** V 0.6 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-08-17 7:51:34 AM - 17999 Bytes
$History: * selfimage_mainform.dfm - 2005-08-06 6:45:06 PM - 6767 Bytes
$History: * selfimage_mainform.h - 2005-08-17 7:48:54 AM - 3521 Bytes
$History: * selfimage_mainform.ddp - 2005-08-17 7:51:34 AM - 51 Bytes
$History: * Change licensing - only version 2 of the GPL, no later versions
$History: **** V 0.7 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-09-04 1:22:56 AM - 18470 Bytes
$History: * selfimage_mainform.dfm - 2005-08-06 6:45:06 PM - 6767 Bytes
$History: * selfimage_mainform.h - 2005-08-17 7:48:54 AM - 3521 Bytes
$History: * selfimage_mainform.ddp - 2005-09-04 1:22:40 AM - 51 Bytes
$History: * Fix variable not used warning - this probably won't 
$History: * matter thought because of the new threading structure coming in
$History: **** V 0.8 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-11-07 2:41:23 AM - 23870 Bytes
$History: * selfimage_mainform.dfm - 2005-11-07 1:42:56 AM - 8194 Bytes
$History: * selfimage_mainform.h - 2005-11-06 6:14:52 AM - 4034 Bytes
$History: * selfimage_mainform.ddp - 2005-11-07 2:41:22 AM - 51 Bytes
$History: * Changes for 0.2 - too many to note
$History: **** V 0.9 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-11-07 3:30:09 AM - 24240 Bytes
$History: * selfimage_mainform.dfm - 2005-11-07 3:28:12 AM - 8203 Bytes
$History: * selfimage_mainform.h - 2005-11-06 6:14:52 AM - 4034 Bytes
$History: * selfimage_mainform.ddp - 2005-11-07 3:28:28 AM - 51 Bytes
$History: * Add BETA tag to form caption
$History: **** V 0.10 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-11-15 5:30:11 PM - 32896 Bytes
$History: * selfimage_mainform.dfm - 2005-11-15 1:41:54 PM - 10185 Bytes
$History: * selfimage_mainform.h - 2005-11-15 1:41:54 PM - 4817 Bytes
$History: * selfimage_mainform.ddp - 2005-11-15 1:41:52 PM - 51 Bytes
$History: * Add menus, help button, and main form positioning. 
$History: *  Also added sanity check warnings.
$History: **** V 0.11 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-11-18 12:13:29 PM - 33862 Bytes
$History: * selfimage_mainform.dfm - 2005-11-16 8:30:30 AM - 10294 Bytes
$History: * selfimage_mainform.h - 2005-11-15 8:37:44 PM - 4890 Bytes
$History: * selfimage_mainform.ddp - 2005-11-18 12:13:28 PM - 51 Bytes
$History: * Add about box
$History: **** V 0.12 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-11-18 1:27:40 PM - 35241 Bytes
$History: * selfimage_mainform.dfm - 2005-11-16 8:30:30 AM - 10294 Bytes
$History: * selfimage_mainform.h - 2005-11-15 8:37:44 PM - 4890 Bytes
$History: * selfimage_mainform.ddp - 2005-11-18 1:21:36 PM - 51 Bytes
$History: * Add test for FAT32 filesystem when writing image files 
$History: * (can't have more than 4GB) and a test for enough free space in general
$History: **** V 0.13 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-11-18 2:27:30 PM - 35985 Bytes
$History: * selfimage_mainform.dfm - 2005-11-18 2:25:46 PM - 11812 Bytes
$History: * selfimage_mainform.h - 2005-11-18 2:26:32 PM - 5036 Bytes
$History: * selfimage_mainform.ddp - 2005-11-18 2:26:42 PM - 51 Bytes
$History: * Add help button
$History: **** V 0.14 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-11-18 3:54:59 PM - 36354 Bytes
$History: * selfimage_mainform.dfm - 2005-11-18 3:47:12 PM - 11819 Bytes
$History: * selfimage_mainform.h - 2005-11-18 2:26:32 PM - 5036 Bytes
$History: * selfimage_mainform.ddp - 2005-11-18 3:47:24 PM - 51 Bytes
$History: * Add BETA 2 tag to caption
$History: **** V 0.15 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-11-20 8:01:28 PM - 36724 Bytes
$History: * selfimage_mainform.dfm - 2005-11-20 8:00:54 PM - 11812 Bytes
$History: * selfimage_mainform.h - 2005-11-18 2:26:32 PM - 5036 Bytes
$History: * selfimage_mainform.ddp - 2005-11-20 8:01:28 PM - 51 Bytes
$History: * Remove BETA 2 from caption
$History: **** V 0.16 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-11-27 6:09:10 AM - 37564 Bytes
$History: * selfimage_mainform.dfm - 2005-11-27 5:18:14 AM - 15634 Bytes
$History: * selfimage_mainform.h - 2005-11-27 2:24:10 AM - 6093 Bytes
$History: * selfimage_mainform.ddp - 2005-11-27 6:06:04 AM - 51 Bytes
$History: * Add compression support
$History: **** V 0.17 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-11-28 1:30:48 AM - 40252 Bytes
$History: * selfimage_mainform.dfm - 2005-11-28 1:29:52 AM - 16023 Bytes
$History: * selfimage_mainform.h - 2005-11-28 1:30:24 AM - 6183 Bytes
$History: * selfimage_mainform.ddp - 2005-11-28 1:30:46 AM - 51 Bytes
$History: * Update SanityCheck() for compression, update help entries
$History: **** V 0.18 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-11-28 1:55:56 PM - 41478 Bytes
$History: * selfimage_mainform.dfm - 2005-11-28 1:53:24 PM - 16023 Bytes
$History: * selfimage_mainform.h - 2005-11-28 1:30:24 AM - 6183 Bytes
$History: * selfimage_mainform.ddp - 2005-11-28 1:53:24 PM - 51 Bytes
$History: * Add support for thread count configuration, remember 
$History: * the last compression method, and update some label 
$History: * positions for the processing status
$History: **** V 0.19 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-11-29 5:20:57 AM - 43440 Bytes
$History: * selfimage_mainform.dfm - 2005-11-29 3:14:44 AM - 17386 Bytes
$History: * selfimage_mainform.h - 2005-11-29 2:45:58 AM - 6775 Bytes
$History: * selfimage_mainform.ddp - 2005-11-29 3:14:46 AM - 51 Bytes
$History: * Fix bug where manually editing the input filename 
$History: * doesn't change the greyed out status of the "Decompress" 
$History: * option.  Removed two JVCL components in the process.
$History: **** V 0.20 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-11-29 6:19:19 AM - 44012 Bytes
$History: * selfimage_mainform.dfm - 2005-11-29 6:19:06 AM - 17548 Bytes
$History: * selfimage_mainform.h - 2005-11-29 6:13:12 AM - 6754 Bytes
$History: * selfimage_mainform.ddp - 2005-11-29 6:19:18 AM - 51 Bytes
$History: * Remove remaining JVCS controls
$History: **** V 0.21 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-11-29 11:02:04 PM - 45545 Bytes
$History: * selfimage_mainform.dfm - 2005-11-29 10:38:38 PM - 17921 Bytes
$History: * selfimage_mainform.h - 2005-11-29 10:53:56 PM - 6817 Bytes
$History: * selfimage_mainform.ddp - 2005-11-29 10:58:28 PM - 51 Bytes
$History: * Didn't add RX component library's TFileNameEdit - 
$History: * instead greyed out the filename edit controls font 
$History: * to show that the filename isn't manually editable. 
$History: *  Added automatic extension switching when compression 
$History: * is turned on or changed.
$History: **** V 0.22 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-11-30 6:27:50 AM - 49169 Bytes
$History: * selfimage_mainform.dfm - 2005-11-30 6:18:20 AM - 18496 Bytes
$History: * selfimage_mainform.h - 2005-11-30 6:09:44 AM - 6824 Bytes
$History: * selfimage_mainform.ddp - 2005-11-30 6:27:48 AM - 51 Bytes
$History: * Change version to 1.0, update some comments, and make 
$History: * the percent doen visible in the title
$History: **** V 1.0 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-11-30 6:32:39 AM - 49561 Bytes
$History: * selfimage_mainform.dfm - 2005-11-30 6:28:58 AM - 18496 Bytes
$History: * selfimage_mainform.h - 2005-11-30 6:09:44 AM - 6824 Bytes
$History: * selfimage_mainform.ddp - 2005-11-30 6:32:38 AM - 33 Bytes
$History: * Left the operating status pane in the foreground
$History: **** V 1.1 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-12-02 2:05:02 AM - 49986 Bytes
$History: * selfimage_mainform.dfm - 2005-11-30 6:28:58 AM - 18496 Bytes
$History: * selfimage_mainform.h - 2005-11-30 6:09:44 AM - 6824 Bytes
$History: * selfimage_mainform.ddp - 2005-12-02 1:30:22 AM - 33 Bytes
$History: * Fix TDriveList memory leak (not destructed)
$History: **** V 1.2 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-12-02 9:21:42 AM - 50435 Bytes
$History: * selfimage_mainform.dfm - 2005-11-30 6:28:58 AM - 18496 Bytes
$History: * selfimage_mainform.h - 2005-12-02 6:28:42 AM - 6847 Bytes
$History: * selfimage_mainform.ddp - 2005-12-02 6:28:58 AM - 33 Bytes
$History: * Actively terminate the compression threads when "cancel" is pressed
$History: **** V 1.3 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-12-04 12:37:24 PM - 51279 Bytes
$History: * selfimage_mainform.dfm - 2005-12-03 6:04:54 PM - 18496 Bytes
$History: * selfimage_mainform.h - 2005-12-04 5:38:12 AM - 6847 Bytes
$History: * selfimage_mainform.ddp - 2005-12-04 11:06:52 AM - 33 Bytes
$History: * Add support for skipping unallocated blocks
$History: **** V 1.4 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-12-08 4:12:52 PM - 54675 Bytes
$History: * selfimage_mainform.dfm - 2005-12-08 4:05:46 PM - 22472 Bytes
$History: * selfimage_mainform.h - 2005-12-08 3:59:14 PM - 7553 Bytes
$History: * selfimage_mainform.ddp - 2005-12-08 4:06:12 PM - 33 Bytes
$History: * Add NBD support
$History: **** V 1.5 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-12-09 2:04:19 PM - 57472 Bytes
$History: * selfimage_mainform.dfm - 2005-12-09 11:43:54 AM - 22960 Bytes
$History: * selfimage_mainform.h - 2005-12-09 11:44:38 AM - 8281 Bytes
$History: * selfimage_mainform.ddp - 2005-12-09 11:44:44 AM - 33 Bytes
$History: * Experiment with a new look - adopted it.  Much cleaner.
$History: **** V 1.6 by kfitzner ****
$History: * selfimage_mainform.cpp - 2005-12-09 2:51:53 PM - 58089 Bytes
$History: * selfimage_mainform.dfm - 2005-12-09 2:12:02 PM - 22962 Bytes
$History: * selfimage_mainform.h - 2005-12-09 11:44:38 AM - 8281 Bytes
$History: * selfimage_mainform.ddp - 2005-12-09 2:43:34 PM - 33 Bytes
$History: * Update help file links, fix the annoying recurring 
$History: * file/NBD requester bug.
$History: **** V 1.7 by kfitzner ****
$History: * selfimage_mainform.cpp - 2007-02-07 7:30:26 PM - 58808 Bytes
$History: * selfimage_mainform.dfm - 2007-02-07 7:13:22 PM - 22965 Bytes
$History: * selfimage_mainform.h - 2005-12-09 11:44:38 AM - 8281 Bytes
$History: * selfimage_mainform.ddp - 2007-02-07 7:13:34 PM - 33 Bytes
$History: * Fix exception when target is a drive in certain cases 
$History: * (TargetDrive not set in TMainForm::SanityCheck), fix 
$History: * tab order on controls.
$History: **** V 1.8 by kfitzner ****
$History: * selfimage_mainform.cpp - 2007-02-14 3:01:48 AM - 59214 Bytes
$History: * selfimage_mainform.dfm - 2007-02-07 7:13:22 PM - 22965 Bytes
$History: * selfimage_mainform.h - 2005-12-09 11:44:38 AM - 8281 Bytes
$History: * selfimage_mainform.ddp - 2007-02-13 6:32:00 PM - 33 Bytes
$History: * Replace FileSizeByName() with my own GetFileSizeByName()
$History: **** V 1.9 by kfitzner ****
$History: * selfimage_mainform.cpp - 2007-03-12 10:25:18 AM - 59584 Bytes
$History: * selfimage_mainform.dfm - 2007-03-12 10:23:36 AM - 23033 Bytes
$History: * selfimage_mainform.h - 2005-12-09 11:44:38 AM - 8281 Bytes
$History: * selfimage_mainform.ddp - 2007-03-12 10:23:40 AM - 33 Bytes
$History: * Adjust anchors for labels
$History: **** V 1.10 by kfitzner ****
$History: * selfimage_mainform.cpp - 2007-03-14 12:25:30 PM - 60255 Bytes
$History: * selfimage_mainform.dfm - 2007-03-14 11:50:40 AM - 23031 Bytes
$History: * selfimage_mainform.h - 2007-03-14 11:40:32 AM - 8286 Bytes
$History: * selfimage_mainform.ddp - 2007-03-14 11:50:48 AM - 33 Bytes
$History: * Add LoadEmptyBlockMap() call for NBD as well to support 
$History: * remote EXT2 partitions, change cancel button label dynamically
$History: **** V 1.11 by kfitzner ****
$History: * selfimage_mainform.cpp - 2007-03-14 1:52:02 PM - 60698 Bytes
$History: * selfimage_mainform.dfm - 2007-03-14 11:50:40 AM - 23031 Bytes
$History: * selfimage_mainform.h - 2007-03-14 11:40:32 AM - 8286 Bytes
$History: * selfimage_mainform.ddp - 2007-03-14 1:24:56 PM - 33 Bytes
$History: * Rename LoadEmptyBlockMap() to LoadEmptyChunkMap() 
$History: * as part of clarification of code
$History: **** V 1.12 by kfitzner ****
$History: * selfimage_mainform.cpp - 2007-03-19 2:38:53 PM - 61686 Bytes
$History: * selfimage_mainform.dfm - 2007-03-19 2:36:16 PM - 23511 Bytes
$History: * selfimage_mainform.h - 2007-03-18 12:30:16 PM - 8368 Bytes
$History: * selfimage_mainform.ddp - 2007-03-19 2:37:38 PM - 33 Bytes
$History: * Add total bytes skipped to display, adjust number 
$History: * of buffer chunks depending on chunk size, change title to version 1.2
$History: **** Latest ** V 1.13 by kfitzner ** 2007-03-30 4:02:13 AM ****
$History: * Update title for 1.2, modify skipped bytes label, 
$History: * fix bug where threads won't terminate properly
*/
//----------------------------- $NoKeywords ---------------------------------

//---------------------------------------------------------------------------
// File Notes:
//---------------------------------------------------------------------------
// 2 Aug 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// The main read/write loop of this program uses Windows IO overlapping for
// asynchronous writes.  I have found that this doesn't significantly alter
// the throughput at all, but it does for some reason reduce the CPU
// overhead.
//---------------------------------------------------------------------------
// 30 Nov 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// Lot of changes since the above.  No overlapping IO any more since the
// advent of threads.
//
// There are two modes that the form can be in: setup and operating.
// In setup mode, the form accepts user input to configure the souce,
// processing, and output for the copy.  No restrictions are made on what
// sources can be used with what targets.
//
// When the user clicks Ok, the form transitions to operating mode.  It then
// starts up one input thread, possibly one or more processing threads
// (compression or decompression), and one output thread.  Each thread is
// linked to the next one in the chain through a TImageBuffer.  Once the
// threads are started, the form presents the operating mode visual which
// gives the user a progress bar and performance information.  It also
// monitors the progress of all the threads and informs the user of any
// errors.  Once the threads exit (or are terminated), the form reverts back
// to setup mode.
//---------------------------------------------------------------------------
#include <vcl.h>
#include <winnt.h>
#include <IdGlobal.hpp>
#pragma hdrstop

#include "SelfImage_MainForm.h"
#include "SelfImage_Preferences.h"
#include "SelfImage_NBDDialog.h"
#include "SelfImage_Exceptions.h"
#include "SelfImage_Utility.h"
#include "SelfImage_TImageStore.h"
#include "SelfImage_AboutBox.h"
#include "TConfiguration.h"
#include "TProgramLog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CGAUGES"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TMainForm *MainForm = NULL;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Main form constructor
//
__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner)
{
  CurrentMode = modeSetup;
  DriveList = NULL;
  ProcessingThreads = new TSelfImageThread*[MAX_PROCESSING_THREADS];
  AllThreads = new TSelfImageThread*[MAX_THREADS];
  ProcessingBuffer = NULL;
}  // __fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Main form destructor
//
__fastcall TMainForm::~TMainForm()
{
  delete [] ProcessingThreads;
  delete [] AllThreads;
  if (DriveList)
    delete DriveList;
}  // __fastcall TMainForm::~TMainForm()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// OnShow event handler - The form's objects are fully constructed but the
// form is not yet painted when this event occurs, so this is a great place
// to put code to initialize the initial state of the form's components.
//
void __fastcall TMainForm::FormShow(TObject *Sender)
{
  if (SelfImageConfig->Values["Remember Positions"]) {
    int nNewTop, nNewLeft, nNewWidth;
    nNewTop   = SelfImageConfig->Values["WindowTop"];
    nNewLeft  = SelfImageConfig->Values["WindowLeft"];
    nNewWidth = SelfImageConfig->Values["WindowWidth"];
    if (nNewTop != -1)
      Top = nNewTop;
    if (nNewLeft != -1)
      Left = nNewLeft;
    if (nNewWidth != -1)
      Width = nNewWidth;
  }  // if (SelfImageConfig->Values["Remember Positions"])
  switch ((int)SelfImageConfig->Values["LastCompression"]) {
    case cmGzip1: radioGzipFast->Checked = true; break;
    case cmGzip9: radioGzipBest->Checked = true; break;
    case cmBzip2: radioBzip2->Checked = true; break;
  }  // switch (SelfImageConfig->Values["LastCompression"])
  RefreshDriveList();
  if (String(SelfImageConfig->Values["LastOpenDir"]) != "")
    OpenDialog->InitialDir = SelfImageConfig->Values["LastOpenDir"];
    //edtSourceFile->InitialDir = SelfImageConfig->Values["LastOpenDir"];
  else
    OpenDialog->InitialDir = SelfImageConfig->Values["Personal"];
    //edtSourceFile->InitialDir = SelfImageConfig->Values["Personal"];
  if (String(SelfImageConfig->Values["LastSaveDir"]) != "") 
    SaveDialog->InitialDir = SelfImageConfig->Values["LastSaveDir"];
  else
    SaveDialog->InitialDir = SelfImageConfig->Values["Personal"];
  // cmbTargetDriveChange(NULL);
}  // void __fastcall TMainForm::FormShow(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// OnClose event handler - Save the position of the main form
//
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  if (SelfImageConfig->Values["Remember Positions"]) {
    SelfImageConfig->Values["WindowTop"] = Top;
    SelfImageConfig->Values["WindowLeft"] = Left;
    SelfImageConfig->Values["WindowWidth"] = Width;
  }  // if (SelfImageConfig->Values["Remember Positions"])
}  // void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// OnClick handler for the input "File" radio button
//
void __fastcall TMainForm::radioSourceFileClick(TObject *Sender)
{
  cmbSourceDrive->Visible = false;
  edtSourceNBD->Visible = false;
  btnSourceNBDConnect->Visible = false;
  edtSourceFileName->Visible = true;
  edtSourceFileName->BringToFront();
  btnSourceFileOpen->Visible = true;
  btnSourceFileOpen->BringToFront();
  btnSourceFileOpenClick(NULL);
}  // void __fastcall TMainForm::radioSourceFileClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// btnSourceFileOpen OnClick handler
//
void __fastcall TMainForm::btnSourceFileOpenClick(TObject *Sender)
{
  __int64 nnSize;

  if (Sender || !FileExists(SourceFileName) || !GetFileSizeByName(SourceFileName))
    if (OpenDialog->Execute())
      edtSourceFileName->Text = OpenDialog->FileName;
  if (FileExists(SourceFileName) && (nnSize = GetFileSizeByName(SourceFileName)) != 0) {
    this->SourceSize = nnSize;
    lblSourceSizeValue->Caption = MakeByteLabel(nnSize);
    radioDecompress->Enabled = IsFileCompressed(SourceFileName);
    radioDecompress->Checked = radioDecompress->Enabled;
    radioProcessingClick(NULL);
  } else {
    radioSourceFile->Checked = false;
    btnCancel->SetFocus();
    lblSourceSizeValue->Caption = "0";
    if (bEnableDecompression) {
      radioNoProcessing->Checked = true;
      radioProcessingClick(NULL);
    }  // if (bEnableDecompression)
    radioDecompress->Enabled = false;
  }  // else if (not valid file)
  SetVisualDefaults(NULL);
}  // void __fastcall TMainForm::btnSourceFileOpenClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// OnClick handler for the input "Drive" radio button and combo box
//
void __fastcall TMainForm::cmbSourceDriveChange(TObject *Sender)
{
  edtSourceNBD->Visible = false;
  btnSourceNBDConnect->Visible = false;
  edtSourceFileName->Visible = false;
  btnSourceFileOpen->Visible = false;
  cmbSourceDrive->Visible = true;
  cmbSourceDrive->BringToFront();

  TDriveInfo *Drive = (TDriveInfo *)cmbSourceDrive->Items->Objects[cmbSourceDrive->ItemIndex];
  if (Drive->DriveType == driveFloppy)
    Drive->Refresh();
  if (Drive->Readable)
    lblSourceSizeValue->Caption = MakeByteLabel(Drive->Bytes);
  else
    lblSourceSizeValue->Caption = "No Disk";
  SourceSize = Drive->Bytes;
  SetVisualDefaults(NULL);
}  // void __fastcall TMainForm::cmbSourceDriveChange(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// OnClick handler for the input "NBD" radio button
//
void __fastcall TMainForm::radioSourceNBDClick(TObject *Sender)
{
  edtSourceFileName->Visible = false;
  btnSourceFileOpen->Visible = false;
  cmbSourceDrive->Visible = false;
  edtSourceNBD->Visible = true;
  edtSourceNBD->BringToFront();
  btnSourceNBDConnect->Visible = true;
  btnSourceNBDConnect->BringToFront();
  btnSourceNBDConnectClick(NULL);
}  // void __fastcall TMainForm::radioSourceNBDClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// btnSourcNBDConnect OnClick event handler
//
void __fastcall TMainForm::btnSourceNBDConnectClick(TObject *Sender)
{
  formSelfImageConnect = new TformSelfImageConnect(Application);
  if (formSelfImageConnect->Execute(SourceHost) == mrOk) {
    edtSourceNBD->Text = formSelfImageConnect->ConnectionString;
    SourceSize = formSelfImageConnect->RemoteSize;
    lblSourceSizeValue->Caption = MakeByteLabel(SourceSize);
  } else {
    if (!Sender || SourceHost.IsEmpty()) {
      radioSourceNBD->Checked = false;
      btnCancel->SetFocus();
      lblSourceSizeValue->Caption = "0";
    }  // if (!Sender || SourceHost.IsEmpty)
  }  // else if (formSelfImageConnect->Execute(SourceHost) != mrOk)
  SetVisualDefaults(NULL);
}  // void __fastcall TMainForm::btnSourceNBDConnectClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// OnClick handler for the processing radio buttons.
//
void __fastcall TMainForm::radioProcessingClick(TObject *Sender)
{
  AnsiString OldExtension = ExtractFileExt(TargetFileName);

  bEnableCompression = radioCompress->Checked;
  bEnableDecompression = radioDecompress->Checked;

  gbCompressionMethod->Enabled = bEnableCompression;
  radioGzipFast->Enabled = bEnableCompression;
  radioGzipBest->Enabled = bEnableCompression;
  radioBzip2->Enabled = bEnableCompression;

  if (bEnableCompression)  {
    if (radioBzip2->Checked) {
      CompressionMethod = cmBzip2;
      SaveDialog->Filter = "bzip2-compressed images files (*.img.bz2)|*.img.bz2|All files (*.*)|*.*";
      SaveDialog->DefaultExt = "img.bz2";
      if (radioTargetFile->Checked);
        if (OldExtension == ".gz")
          TargetFileName = TargetFileName.SubString(1, TargetFileName.Length() - 3) + ".bz2";
        else if (OldExtension == ".img")
          TargetFileName = TargetFileName + ".bz2";
    } else {
      SaveDialog->Filter = "gzip-compressed images (*.img.gz)|*.img.gz|All files (*.*)|*.*";
      SaveDialog->DefaultExt = "img.gz";
      if (radioGzipFast->Checked)
        CompressionMethod = cmGzip1;
      else
        CompressionMethod = cmGzip9;
      if (radioTargetFile->Checked)
        if (OldExtension == ".bz2")
          TargetFileName = TargetFileName.SubString(1, TargetFileName.Length() - 4) + ".gz";
        else if (OldExtension == ".img")
          TargetFileName = TargetFileName + ".gz";
    }  // else if (!radioGzip->Checked)
  }  // if (bEnableCompression)
  else {
    SaveDialog->Filter = "Uncompressed image files (*.img)|*.img|All files (*.*)|*.*";
    SaveDialog->DefaultExt = "img";
    if (radioTargetFile->Checked)
      if (OldExtension == ".gz")
        TargetFileName = TargetFileName.SubString(1, TargetFileName.Length() - 3);
      else if (OldExtension == ".bz2")
        TargetFileName = TargetFileName.SubString(1, TargetFileName.Length() - 4);
  }  // else if (!bEnableCompression)
}  // void __fastcall TMainForm::chbEnableCompressionClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Onclick handler for the output "File" radio button
//
void __fastcall TMainForm::radioTargetFileClick(TObject *Sender)
{
  cmbTargetDrive->Visible = false;
  edtTargetNBD->Visible = false;
  btnTargetNBDConnect->Visible = false;
  edtTargetFileName->Visible = true;
  edtTargetFileName->BringToFront();
  btnTargetFileOpen->Visible = true;
  btnTargetFileOpen->BringToFront();
  btnTargetFileOpenClick(NULL);
}  // void __fastcall TMainForm::radioTargetFileClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// btnTargetFileOpen OnClick event handler
//
void __fastcall TMainForm::btnTargetFileOpenClick(TObject *Sender)
{
  if (Sender || TargetFileName.IsEmpty())
    if (SaveDialog->Execute())
      edtTargetFileName->Text = SaveDialog->FileName;
  if (TargetFileName.IsEmpty()) {
    radioTargetFile->Checked = false;
    btnCancel->SetFocus();
  }  // if (TargetFileName.IsEmpty())
  SetVisualDefaults(NULL);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  Onclick handler for the output "Drive" radio button
//
void __fastcall TMainForm::radioTargetDriveClick(TObject *Sender)
{
  edtTargetFileName->Visible = false;
  btnTargetFileOpen->Visible = false;
  edtTargetNBD->Visible = false;
  btnTargetNBDConnect->Visible = false;
  cmbTargetDrive->Visible = true;
  cmbTargetDrive->BringToFront();
  SetVisualDefaults(NULL);
}  // void __fastcall TMainForm::radioTargetDriveClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// cmbTargetDrive OnChange event handler
//
void __fastcall TMainForm::cmbTargetDriveChange(TObject *Sender)
{
  TDriveInfo *Drive = (TDriveInfo *)cmbTargetDrive->Items->Objects[cmbTargetDrive->ItemIndex];
  if (Drive->DriveType == driveFloppy)
    Drive->Refresh();
  if (Drive->Writable) {
    MaxTargetSize = Drive->Bytes;
    lblTargetDriveSizeValue->Caption = MakeByteLabel(Drive->Bytes);
  } else {
    MaxTargetSize = 0;
    lblTargetDriveSizeValue->Caption = "No Disk";
  }  // else if (!Drive->Writable)
  SetVisualDefaults(NULL);
}  // void __fastcall TMainForm::cmbTargetDriveChange(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// OnClick handler for the output "NBD" radio button
//
void __fastcall TMainForm::radioTargetNBDClick(TObject *Sender)
{
  edtTargetFileName->Visible = false;
  btnTargetFileOpen->Visible = false;
  cmbTargetDrive->Visible = false;
  edtTargetNBD->Visible = true;
  edtTargetNBD->BringToFront();
  btnTargetNBDConnect->Visible = true;
  btnTargetNBDConnect->BringToFront();
  btnTargetNBDConnectClick(NULL);
}  // void __fastcall TMainForm::radioTargetNBDClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// btnTargetNBDConnect OnClick event handler
//
void __fastcall TMainForm::btnTargetNBDConnectClick(TObject *Sender)
{
  formSelfImageConnect = new TformSelfImageConnect(Application);
  if (formSelfImageConnect->Execute(TargetHost) == mrOk) {
    edtTargetNBD->Text = formSelfImageConnect->ConnectionString;
    MaxTargetSize = formSelfImageConnect->RemoteSize;
    lblTargetDriveSizeValue->Caption = MakeByteLabel(MaxTargetSize);
  } else {
    if (!Sender || TargetHost.IsEmpty()) {
      radioTargetNBD->Checked = false;
      btnCancel->SetFocus();
      MaxTargetSize = 0;
      lblTargetDriveSizeValue->Caption = "0";
    }  // if (!Sender || SourceHost.IsEmpty)
  }  // else if (formSelfImageConnect->Execute(SourceHost) != mrOk)
  SetVisualDefaults(NULL);
}  // void __fastcall TMainForm::btnTargetNBDConnectClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// This method sets which visual controls are enabled/disabled based on
// user inputs
//
void __fastcall TMainForm::SetVisualDefaults(TObject *Sender)
{
  TDriveInfo *SourceDrive = NULL, *TargetDrive = NULL;

  if (radioSourceDrive->Checked && cmbSourceDrive->ItemIndex != -1)
    SourceDrive = (TDriveInfo *)cmbSourceDrive->Items->Objects[cmbSourceDrive->ItemIndex];
  if (radioTargetDrive->Checked && cmbTargetDrive->ItemIndex != -1)
    TargetDrive = (TDriveInfo *)cmbTargetDrive->Items->Objects[cmbTargetDrive->ItemIndex];

  edtSourceFileName->Enabled = radioSourceFile->Checked;
  btnSourceFileOpen->Enabled = radioSourceFile->Checked;
  cmbSourceDrive->Enabled = radioSourceDrive->Checked;
  edtSourceNBD->Enabled = radioSourceNBD->Checked;
  btnSourceNBDConnect->Enabled = radioSourceNBD->Checked;
  lblSourceSize->Enabled  = !lblSourceSizeValue->Caption.IsEmpty() && lblSourceSizeValue->Caption != "0";
  lblSourceSizeValue->Enabled = lblSourceSize->Enabled;
  edtTargetFileName->Enabled = radioTargetFile->Checked;
  btnTargetFileOpen->Enabled = radioTargetFile->Checked;
  cmbTargetDrive->Enabled = radioTargetDrive->Checked;
  edtTargetNBD->Enabled = radioTargetNBD->Checked;
  btnTargetNBDConnect->Enabled = radioTargetNBD->Checked;
  lblTargetDriveSize->Enabled = radioTargetDrive->Checked || radioTargetNBD->Checked;
  lblTargetDriveSizeValue->Enabled = lblTargetDriveSize->Enabled;
  btnOk->Enabled = (SourceType == isFile && !SourceFileName.IsEmpty() || SourceType == isDrive && SourceDrive || SourceType == isNBD && !SourceHost.IsEmpty())
                && (TargetType == isFile && !TargetFileName.IsEmpty() || TargetType == isDrive && TargetDrive || TargetType == isNBD && !TargetHost.IsEmpty())
                && !(SourceType == isDrive && TargetType == isDrive && cmbSourceDrive->Text == cmbTargetDrive->Text)
                && !(SourceType == isDrive && SourceDrive && !SourceDrive->Readable)
                && !(TargetType == isDrive && TargetDrive && !TargetDrive->Writable)
                && !(SourceType == isNBD && TargetType == isNBD && SourceHost == TargetHost);
  menu_File_Start->Enabled = btnOk->Enabled;
}  // void __fastcall TMainForm::SetVisualDefaults(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// File->Exit menu item OnClick event handler
//
void __fastcall TMainForm::menu_File_ExitClick(TObject *Sender)
{
  Close();
}  // void __fastcall TMainForm::menu_File_ExitClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Edit->Refresh Drive Lists menu item OnClick event handler
//
void __fastcall TMainForm::menu_Edit_RefreshClick(TObject *Sender)
{
  RefreshDriveList();
}  // void __fastcall TMainForm::menu_Edit_RefreshClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Edit->Preferences menu item OnClick event handler
//
void __fastcall TMainForm::menu_Edit_PreferencesClick(TObject *Sender)
{
  formSelfImagePreferences = new TformSelfImagePreferences(Application);
  formSelfImagePreferences->ShowModal();
  delete formSelfImagePreferences;
}  // void __fastcall TMainForm::menu_Edit_PreferencesClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Help->* menu items OnClick event handler
// Launch the help application - help page is stored in the menu item's
// HelpContext field
//
void __fastcall TMainForm::menu_Help_ItemsClick(TObject *Sender)
{
  Application->HelpContext(((TMenuItem *)Sender)->HelpContext);
}  // void __fastcall TMainForm::menu_Help_OverviewClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Help->About menu item OnClick event handler
// Show the about box
//
void __fastcall TMainForm::menu_Help_AboutClick(TObject *Sender)
{
  formSelfImageAboutBox = new TformSelfImageAboutBox(Application);
  formSelfImageAboutBox->ShowModal();
  delete formSelfImageAboutBox;
}  // void __fastcall TMainForm::About1Click(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// btnHelp OnClick event handler
//
void __fastcall TMainForm::btnHelpClick(TObject *Sender)
{
  Application->HelpContext(((TControl *)Sender)->HelpContext);
}  // void __fastcall TMainForm::btnHelpClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Ok button OnClick event handler - the user clicked Ok, so get to work
//
void __fastcall TMainForm::btnOkClick(TObject *Sender)
{
  TImageBuffer *EndBuffer;
  int ReadChunkSize, ProcessingChunkSize;

  SourceImage = NULL;
  TargetImage = NULL;
  InFileBuffer = NULL;
  ProcessingBuffer = NULL;
  ReadThread = NULL;
  WriteThread = NULL;
  int nBufferCount = 8;

  try {
    // Clear all the thread pointers
    memset(ProcessingThreads, 0, sizeof(void *) * MAX_PROCESSING_THREADS);
    memset(AllThreads, 0, sizeof(void *) * MAX_THREADS);

    // Perform last-minute sanity checks and give the user a chance to back out if something doesn't make sense
    if (!SanityCheck())
      return;

    nEmptyChunkCount = 0;

    // Save the compression method to be used as the default for next time
    if (bEnableCompression)
        SelfImageConfig->Values["LastCompression"] = CompressionMethod;

    // Determine how many processing threads there will be (if we are processing)
    nProcessingThreadCount = SelfImageConfig->Values["ProcessingThreads"];
    if (!nProcessingThreadCount)
      nProcessingThreadCount = GetCPUCount();

    // Determine the block sizes we'll be using
    if (bEnableCompression)
      if (CompressionMethod == cmBzip2) {
        // bzip2 uses 900000 byte blocks, but our reads have to be on sector boundaries, so 899584 is the best we can do
        ReadChunkSize = 899584;
        ProcessingChunkSize = 909600;
      } else {
        // Get the gzip chunk size from our preferences
        ReadChunkSize = SelfImageConfig->Values["gzipChunkSize"] * 1024;
        nBufferCount *= 2 * 1024 * 1024 / ReadChunkSize;
        ProcessingChunkSize = ReadChunkSize + (ReadChunkSize >> 2);
      }  // else if (CompressionMethod != cmBzip2)
    else if (bEnableDecompression) {
      ProcessingChunkSize = 2 * 1024 * 1024;
      ReadChunkSize = ProcessingChunkSize + (ProcessingChunkSize >> 2);
    }  // else if (bEnableDecompression)
    else if (SourceSize < 4 * 1024 * 1024)
      ReadChunkSize = 64 * 1024;
    else
      ReadChunkSize = 2 * 1024 * 1024;

    // Create the input image store
    switch (SourceType) {
      case isFile:
        SourceImage = new TImageStore(isFile, SourceFileName);
        // If decompression is enabled and we thought the file was compressed, but it turns out that it's not, then throw an exception
        if (bEnableDecompression && IsFileCompressed(SourceImage->FileName) && ! SourceImage->Compressed)
          throw ESelfImageFileError("Although the extension of the input file indicates it is compressed, SelfImage could not verify that it actually is compressed.  Either the file is corrupted, or has the wrong extension.  SelfImage cannot decompress it.");
        SelfImageConfig->Values["LastOpenDir"] = ExtractFileDir(SourceFileName);
        break;
      case isDrive: {
        TDriveInfo *SourceDrive = (TDriveInfo *)cmbSourceDrive->Items->Objects[cmbSourceDrive->ItemIndex];
        SourceImage = new TImageStore(isDrive, SourceDrive->DeviceName);
        if (SelfImageConfig->Values["SkipUnallocated"])
          nEmptyChunkCount = SourceImage->LoadEmptyChunkMap(ReadChunkSize);
        break;
      }  // case isDrive:
      case isNBD:
        SourceImage = new TImageStore(isNBD, SourceHost);
        if (SelfImageConfig->Values["SkipUnallocated"])
          nEmptyChunkCount = SourceImage->LoadEmptyChunkMap(ReadChunkSize);
        break;
      default:
        throw ESelfImageError("Input type not set.");
    }  // switch (SourceType)

    // Create the output image store
    switch (TargetType) {
      case isFile:
        TargetImage = new TImageStore(isFile, TargetFileName, true);
        SelfImageConfig->Values["LastSaveDir"] = ExtractFileDir(TargetFileName);
        break;
      case isDrive: {
        TDriveInfo *TargetDrive = (TDriveInfo *)cmbTargetDrive->Items->Objects[cmbTargetDrive->ItemIndex];
        TargetImage = new TImageStore(isDrive, TargetDrive->DeviceName, true);
        break;
      }  // case isDrive:
      case isNBD:
        TargetImage = new TImageStore(isNBD, TargetHost, true);
        break;
      default:
        throw ESelfImageError("Output type not set.");
    }  // else if NBD

    InFileBuffer = new TImageBuffer(ReadChunkSize, nBufferCount);
    EndBuffer = InFileBuffer;
    ReadThread = new TSelfImageReadThread(SourceImage, InFileBuffer, bEnableDecompression);
    AllThreads[0] = ReadThread;

    if (bEnableCompression) {
      ProcessingBuffer = new TImageBuffer(ProcessingChunkSize, nBufferCount);
      // InFileBuffer->AllocationSister = ProcessingBuffer;
      for (unsigned int n=0; n < nProcessingThreadCount; n++) {
        ProcessingThreads[n] = new TSelfImageCompressionThread(CompressionMethod, ReadChunkSize, InFileBuffer, ProcessingBuffer);
        AllThreads[n+2] = ProcessingThreads[n];
      }  // for (unsigned int n=0; n < nCPUCount; n++)
      EndBuffer = ProcessingBuffer;
    }  // if (chbEnableCompression->Checked)
    else if (bEnableDecompression && SourceImage->Compressed) {
      ProcessingBuffer = new TImageBuffer(ProcessingChunkSize, nBufferCount);
      // InFileBuffer->AllocationSister = ProcessingBuffer;
      // Can't have more than one decompression thread if it's not compressed in blocks
      if (!SourceImage->ChunkCompressed)
        nProcessingThreadCount = 1;
      for (unsigned int n=0; n < nProcessingThreadCount; n++) {
        ProcessingThreads[n] = new TSelfImageDecompressionThread(SourceImage->CompressionFormat, InFileBuffer, ProcessingBuffer);
        AllThreads[n+2] = ProcessingThreads[n];
      }  // for (unsigned int n=0; n < nCPUCount; n++)
      EndBuffer = ProcessingBuffer;
    }  // else if (bEnableDecompression && SourceImage->Compressed)

    WriteThread = new TSelfImageWriteThread(TargetImage, EndBuffer);
    AllThreads[1] = WriteThread;

    nnTotalBytes = SourceImage->Geometry.Bytes;
    VisualMode(modeOperating);
    UpdateStatus(true);
  }  // try

  catch (...) {
    // We've encountered an exception sometime before we started the operation, so close down everything
    // as gracefully as we can.
    if (SourceImage)
      delete SourceImage;
    if (TargetImage)
      delete TargetImage;
    if (InFileBuffer)
      delete InFileBuffer;
    if (ProcessingBuffer)
      delete ProcessingBuffer;
    if (ReadThread)
      delete ReadThread;
    for (unsigned n = 0; n < nProcessingThreadCount; n++)
      if (ProcessingThreads[n])
        delete ProcessingThreads[n];
    throw;
  }  // catch (Exception &e)

  // Start execution of all the threads
  ReadThread->Resume();
  if (bEnableCompression || bEnableDecompression)
   for (unsigned int n=0; n < nProcessingThreadCount; n++)
      ProcessingThreads[n]->Resume();
  WriteThread->Resume();
  UpdateTimer->Enabled = true;

}  // void __fastcall TMainForm::btnOkClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// btnCancel OnClick event - the user pressed the cancel button.
//
void __fastcall TMainForm::btnCancelClick(TObject *Sender)
{
  if (CurrentMode == modeSetup) {
    Close();
  } else {
    for (unsigned n = 0; n < MAX_THREADS && AllThreads[n]; n++)
      AllThreads[n]->Terminate();
  }  // else (VisualMode != modeSetup)
}  // void __fastcall TMainForm::btnCancelClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// UpdateTimer OnTimer event handler - the UpdateTimer timer is what
// determines when visual updates occur when a copy is in progress.  It
// also checks to see if any errors have occured during the copy and cleans
// up if one does.
//
void __fastcall TMainForm::UpdateTimerTimer(TObject *Sender)
{
  bool bErrorFlag = false;

  for (unsigned int n = 0; n < MAX_THREADS && AllThreads[n]; n++)
    if (AllThreads[n]->ErrorFlag) {
      bErrorFlag = true;
      break;
    }  // if ((*AllThreads[n])->ErrorFlag)

  // If the write thread is finished, or if any of the other threads have their error flags set, then terminate
  // all threads and shut down
  if (WriteThread->Finished || bErrorFlag) {
    AnsiString ErrorMessage;
    UpdateTimer->Enabled = false;
    // First, terminate them all...
    for (unsigned n = 0; n < MAX_THREADS && AllThreads[n]; n++)
      ((TSelfImageThread *)AllThreads[n])->Terminate();
    // Now make sure they all exit cleanly and build an error message
    for (unsigned n = 0; n < MAX_THREADS && AllThreads[n]; n++) {
      TSelfImageThread *Thread = AllThreads[n];
      if (!Thread->Finished) {
        Thread->Terminate();
        Thread->WaitFor();
      }  // if (!Thread->Finished)
      if (Thread->ErrorFlag) {
        if (!ErrorMessage.IsEmpty())
          ErrorMessage += "\r\n";
        ErrorMessage += Thread->ErrorMessage;
      }  // if (Thread->ErrorFlag)
      delete Thread;
    }  // for (int n = 0; n < nThreadCount; n++)
    if (!ErrorMessage.IsEmpty())
      Application->MessageBox(ErrorMessage.c_str(), "Image copy error", MB_OK | MB_ICONERROR);
    delete SourceImage;
    delete TargetImage;
    delete InFileBuffer;
    if (ProcessingBuffer)
      delete ProcessingBuffer;
    VisualMode(modeSetup);
  }  // if (WriteThread->Finished || bErrorFlag)
  else {
    if (bEnableCompression || bEnableDecompression) {
      nnBytesReadTotal = 0;
      for (unsigned n = 0; n < MAX_PROCESSING_THREADS && ProcessingThreads[n]; n++)
        nnBytesReadTotal += ProcessingThreads[n]->BytesRead;
    } else
      nnBytesReadTotal = ReadThread->BytesRead;
    nnBytesSkippedTotal = ReadThread->BytesSkipped;
    nnBytesWrittenTotal = WriteThread->BytesWritten;
    UpdateStatus();
  }  // else if !(WriteThread->Finished || bErrorFlag)
}  // void __fastcall TMainForm::UpdateTimerTimer(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Read (or re-read) the information for all the drive partitions, including
// which ones are writable, and set up the drive combo boxes.
//
void __fastcall TMainForm::RefreshDriveList(void)
{
  cmbSourceDrive->Clear();
  cmbTargetDrive->Clear();
  if (DriveList)
    delete DriveList;
  radioSourceDrive->Checked = false;
  radioTargetDrive->Checked = false;
  btnCancel->SetFocus();

  DriveList = new TDriveList(true);
  for (int n = 0; n < DriveList->Count; n++) {
    TDriveInfo *Drive = DriveList->Items[n];
    // cmbDriveList->ItemsEx->AddItem(Drive->DisplayName, 0, 0, 0, 0, Drive);
    cmbSourceDrive->AddItem(Drive->DisplayName, (TObject *)Drive);
    if (Drive->Writable)
      cmbTargetDrive->AddItem(Drive->DisplayName, (TObject *)Drive);
  }  // for (int n = 0; n < DriveList->Count; n++)
  // If there are no writable partitions then disable the output drive radio button
  if (!cmbTargetDrive->Items->Count)
    radioTargetDrive->Enabled = false;
  for (int n = 0; n < cmbSourceDrive->Items->Count; n++) {
    TDriveInfo *Drive = (TDriveInfo *)cmbSourceDrive->Items->Objects[n];
    if (Drive->MountPoint.Pos("C:")) {
      cmbSourceDrive->ItemIndex = n;
      break;
    }  // if (Drive->MountPoint.Pos("C:"))
  }  // for (int n = 0; n < cmbDriveList->Items->Count; n++)
}  // void __fastcall RefreshDriveList(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Switch visual modes between setup (user is setting up the copy) and
// operating (the copy is occuring).
//
void __fastcall TMainForm::VisualMode(TMainFormVisualMode Mode)
{
  if (Mode == modeSetup) {
    btnOk->Enabled = true;
    btnCancel->Caption = "Exit";
    menu_File->Enabled = true;
    menu_Edit->Enabled = true;
    menu_Help->Enabled = true;
    gbStatus->Visible = false;
    gbStatus->SendToBack();
    Application->Title = "SelfImage";
  }  // if (Mode == modeSetup)

  if (Mode == modeOperating) {
    btnOk->Enabled = false;
    btnCancel->Caption = "Cancel";
    menu_File->Enabled = false;
    menu_Edit->Enabled = false;
    menu_Help->Enabled = false;
    switch (SourceType) {
      case isFile:  edtInput->Text = SourceFileName; break;
      case isDrive: edtInput->Text = cmbSourceDrive->Text; break;
      case isNBD:   edtInput->Text = SourceHost; break;
    }  // switch (SourceType)
    switch (TargetType) {
      case isFile:  edtOutput->Text = TargetFileName; break;
      case isDrive: edtOutput->Text = cmbTargetDrive->Text; break;
      case isNBD:   edtOutput->Text = TargetHost; break;
    }  // switch (TargetType);
    gbStatus->Visible = true;
    gbStatus->BringToFront();
    ProgressBar->MaxValue = ProgressBar->Width;
    ProgressBar->Progress = 0;
    lblPercent->Caption = "0%";
    Application->Title = "SelfImage - 0%";
    lblBytesTotalValue->Caption = MakeByteLabel(nnTotalBytes);
    lblInputSize->Left = lblBytesTotalValue->Left;
    if (nEmptyChunkCount) {
      lblBytesSkippedValue->Enabled = true;
      lblBytesSkippedValue->Caption = "0";
    } else {
      lblBytesSkippedValue->Caption = "N/A";
      lblBytesSkippedValue->Enabled = false;
    }  // if (nEmptyChunkCount)
    lblBytesWrittenValue->Caption = "0";
    lblWriteSpeedValue->Caption = "";
    lblOverallSpeedValue->Caption = "";
  }  // if (Mode == modeOperating)
  CurrentMode = Mode;
  return;
}  // void __fastcall TMainForm::VisualMode(TMainFormVisualMode Mode)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void __fastcall TMainForm::UpdateStatus(bool bInit)
{
  static LARGE_INTEGER StartTimer, LastTimer, PerformanceFrequency;
  static __int64 nnLastTotal;
  LARGE_INTEGER Timer;
  long double ddTimeDifference, ddBytesDifference;
  int nProgressTicks, nProgressPercent;

  if (bInit) {
    QueryPerformanceFrequency(&PerformanceFrequency);
    QueryPerformanceCounter(&StartTimer);
    LastTimer.QuadPart = StartTimer.QuadPart;
    nnLastTotal = 0;
  } else {  // if (bInit)
    nProgressTicks = nnBytesReadTotal * ProgressBar->Width / nnTotalBytes;
    nProgressPercent = nnBytesReadTotal * 100 / nnTotalBytes;
    ProgressBar->Progress = nProgressTicks;
    lblPercent->Caption = String(nProgressPercent) + "%";
    Application->Title = "SelfImage - " + lblPercent->Caption;
    lblBytesReadValue->Caption = MakeByteLabel(nnBytesReadTotal);
    if (nnBytesSkippedTotal)
      lblBytesSkippedValue->Caption = MakeByteLabel(nnBytesSkippedTotal) + " (" + FloatToStrF(nnBytesSkippedTotal * 100.0 / nnBytesReadTotal, ffFixed, 3, 1) + "%)";
    lblBytesWrittenValue->Caption = MakeByteLabel(nnBytesWrittenTotal);
    QueryPerformanceCounter(&Timer);
    ddTimeDifference = Timer.QuadPart - LastTimer.QuadPart;
    ddTimeDifference /= PerformanceFrequency.QuadPart;
    if (ddTimeDifference > 2.0) {
      ddBytesDifference = (bEnableDecompression?nnBytesWrittenTotal:nnBytesReadTotal) - nnLastTotal;
      lblWriteSpeedValue->Caption = MakeByteLabel(ddBytesDifference/ddTimeDifference) + "/s";
      ddBytesDifference = bEnableDecompression?nnBytesWrittenTotal:nnBytesReadTotal;
      ddTimeDifference = Timer.QuadPart - StartTimer.QuadPart;
      ddTimeDifference /= PerformanceFrequency.QuadPart;
      lblOverallSpeedValue->Caption = MakeByteLabel(ddBytesDifference/ddTimeDifference) + "/s";
      LastTimer.QuadPart = Timer.QuadPart;
      nnLastTotal = bEnableDecompression?nnBytesWrittenTotal:nnBytesReadTotal;
    }  // if (dTimeDifference > 1.0)
  }  // if (bInit)
}  // void __fastcall StatusUpdateTimer(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Form's OnResize event handler - whenever the form is resized while
// a copy is in progress, set the progress bar's max value appropriately.
//
void __fastcall TMainForm::FormResize(TObject *Sender)
{
  if (CurrentMode == modeOperating)
    ProgressBar->MaxValue = ProgressBar->Width;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Perform a sanity check on what the user is doing.  This is the last line
// of defense to prevent the user from doing something stupid.  Presents
// a warning, but will let the user continue.
//
bool __fastcall TMainForm::SanityCheck(void)
{
  TDriveInfo /* *SourceDrive = NULL, */ *TargetDrive = NULL;
  AnsiString Warning;
  unsigned __int64 FreeDiskSpace;

  if (radioTargetFile->Checked)
    FreeDiskSpace = GetFreeDiskSpace(TargetFileName);

  // If the target is a drive then we'll need to grab its info for some of the sanity checks
  if (TargetType == isDrive)
    TargetDrive = (TDriveInfo *)cmbTargetDrive->Items->Objects[cmbTargetDrive->ItemIndex];

  // Output is a partition or NBD and compression is enabled
  if ((TargetType == isDrive || TargetType == isNBD) && bEnableCompression)
    Warning = "You are writing directly to a partition or network block device and compression is enabled.  This will almost certainly render the partition unusable.";
  // Input is a file, Output is a partition/NBD, and input size is larger than the target
  else if (SourceType == isFile && (TargetType == isDrive || TargetType == isNBD) && SourceSize > MaxTargetSize)
    Warning = "The input image is " + MakeByteLabel(SourceSize - TargetDrive->Bytes) + " larger than the partition it is being written to.  SelfImage will not write past the end of the target disk partition, but it is likely that this image will not work properly in the target partition.  Do not proceed unless you know what you are doing is correct.";
  // Input is a file, Output is a partition/NBD and input size is smaller than the target (and not decompressing)
  else if (SourceType == isFile && (TargetType == isDrive || TargetType == isNBD) && SourceSize < MaxTargetSize && !bEnableDecompression)
    Warning = "The input image is " + MakeByteLabel(TargetDrive->Bytes - SourceSize) + " smaller than the partition it is being written to.  This image did not come from the partition you are writing it to - writing it to this partition may not have the effect you wish.  Do not proceed unless you know what you are doing is correct.";
  // Input is anything, Output is an uncompressed file, not enough space on destination device
  else if (TargetType == isFile && SourceSize > FreeDiskSpace && !bEnableCompression)
    Warning = "Not enough space on destination volume.  Input size is " + MakeByteLabel(SourceSize) + ", available space is " + MakeByteLabel(GetFreeDiskSpace(TargetFileName)) + ".";
  // Same as above, compression enabled
  else if (TargetType == isFile && SourceSize > FreeDiskSpace && bEnableCompression) {
    double fRatio = (double)SourceSize / (double)FreeDiskSpace;
    Warning = "The input for this image is larger than the available space on the destination volume.  Input size is " + MakeByteLabel(SourceSize) + ", available space is " + MakeByteLabel(GetFreeDiskSpace(TargetFileName)) + ".  You will need to obtain a compression ratio of " + FloatToStrF(fRatio, ffFixed, 15, 1) + ":1 for this to fit.";
    if (fRatio < 2)
      Warning += "  While it is likely that compression will make this image fit in the available space, it is impossible to predict with certainty.  You may run out of disk space during the operation.";
    else if (fRatio < 3)
      Warning += "  While this amount of compression is possible, SelfImage may not be able to compress the image enough to fit.";
    else
      Warning += "  This amount of compression is extremely unlikely unless the source is mostly empty space that has been properly erased.  Unless this is the case, you will almost certainly run out of disk space if you continue.";
  }
  // Input is > 4GB, Output is file on a FAT32 partition, no compression
  else if (TargetType == isFile && SourceSize > 4294967296ui64 && GetFileSystemType(TargetFileName).SubString(1,3) == "FAT" && !bEnableCompression)
    Warning = "The source for this image is more than 4GB and the target is on a FAT/FAT32 volume that cannot store files larger than 4GB.  If you continue, the image will be cut off at 4GB.";
  // Same as above, compression enabled
  else if (TargetType == isFile && SourceSize > 4294967296ui64 && GetFileSystemType(TargetFileName).SubString(1,3) == "FAT" && bEnableCompression) {
    double fRatio = (double)SourceSize / 4294967296.0;
    Warning = "The source for this image is " + MakeByteLabel(SourceSize) + "in size and the target is on a FAT/FAT32 volume that cannot store files larger than 4GB.  Your compression ration will need to be " + FloatToStrF((double)SourceSize / 4294967296.0, ffFixed, 15, 1) + ":1 for this to fit in that space.";
    if (fRatio < 2)
      Warning += "  While it is likely that compression will make this image fit in the available space, it is impossible to predict with certainty.  You may run out of disk space during the operation.";
    else if (fRatio < 3)
      Warning += "  While this amount of compression is possible, SelfImage may not be able to compress the image enough to fit.";
    else
      Warning += "  This amount of compression is extremely unlikely unless the source is mostly empty space that has been properly erased.  Unless this is the case, you will almost certainly run out of disk space if you continue.";
  }
  // Copying one partition/NBD to another
  else if (SourceType == isDrive && TargetType == isDrive || SourceType == isNBD && TargetType == isNBD)
    // Source partition is larger than the target - really bad news
    if (SourceSize > TargetDrive->Bytes)
      Warning = "You are copying directly from one partition/NBD to another and the source is larger than the target.  SelfImage will not write past the end of the target, but it is likely that this operation will not work as intended.  It is probable that the target will be corrupted.  Do not proceed unless you know what you are doing is correct.";
    // Source is smaller than the target - sometimes bad news
    else if (SourceSize < TargetDrive->Bytes)
      Warning = "You are copying directly from one partition/NBD to another and the source partition is smaller than the target.  Do not proceed unless you know what you are doing is correct.";
    // Partitions are identical size, but the user may have made a mistake - most users never need to do this
    else
      Warning = "You are copying directly from one partition/NBD to another.  Make sure this is what you intended.";

  // Warn when writing directly to a disk partition/NBD
  if (!SelfImageConfig->Values["NoDirectWriteWarn"])
    if (TargetType == isDrive || TargetType == isNBD) {
      if (!Warning.IsEmpty())
        Warning += "\r\n\r\n";
      // If decompression is enabled, also tell the user that the original image size can't be verified.
      if (bEnableDecompression)
        Warning += "You are decompressing an image directly to a disk partition/NBD.  The uncompressed size cannot be verified - make sure you are sending the correct image to the correct partition.  ALL EXISTING DATA ON THE PARTITION OR NETWORK BLOCK DEVICE WILL BE OVERWRITTEN!";
      else
        Warning += "You are writing directly to a disk partition/NBD.  ALL EXISTING DATA ON THE PARTITION OR NETWORK BLOCK DEVICE WILL BE OVERWRITTEN!";
    }  // if (radioSourceDrive->Checked && radioTargetDrive->Checked)

  if (!Warning.IsEmpty()) {
    Warning += "\r\n\r\nAre you SURE you want to continue?";
    if (Application->MessageBox(Warning.c_str(), "WARNING  WARNING  WARNING", MB_YESNO | MB_ICONWARNING) == IDNO)
      return false;
  }  // if (!Warning.IsEmpty())

  return true;
}  // bool __fastcall TMainForm::SanityCheck(void)
//---------------------------------------------------------------------------


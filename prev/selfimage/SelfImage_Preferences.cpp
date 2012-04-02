//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_Preferences.cpp - Preferences form
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
// VCS: $Version: 1 $ $Revision: 5 $
/*
$History: **** V 0.1 by kfitzner ****
$History: * selfimage_preferences.cpp - 2005-11-12 5:17:31 PM - 4304 Bytes
$History: * selfimage_preferences.dfm - 2005-11-12 4:59:34 PM - 1374 Bytes
$History: * selfimage_preferences.h - 2005-11-12 5:04:06 PM - 2105 Bytes
$History: * selfimage_preferences.ddp - 2005-11-12 5:04:24 PM - 51 Bytes
$History: * Initial check-in
$History: **** V 0.2 by kfitzner ****
$History: * selfimage_preferences.cpp - 2005-11-15 5:29:33 PM - 4878 Bytes
$History: * selfimage_preferences.dfm - 2005-11-14 5:09:04 PM - 1574 Bytes
$History: * selfimage_preferences.h - 2005-11-14 5:09:04 PM - 2142 Bytes
$History: * selfimage_preferences.ddp - 2005-11-14 5:23:48 PM - 51 Bytes
$History: * Add option to disable warning when writing to a partition
$History: **** V 0.3 by kfitzner ****
$History: * selfimage_preferences.cpp - 2005-11-18 3:55:03 PM - 5254 Bytes
$History: * selfimage_preferences.dfm - 2005-11-18 1:34:52 PM - 2814 Bytes
$History: * selfimage_preferences.h - 2005-11-18 1:32:48 PM - 2203 Bytes
$History: * selfimage_preferences.ddp - 2005-11-18 1:35:06 PM - 51 Bytes
$History: * Add help button to form
$History: **** V 0.4 by kfitzner ****
$History: * selfimage_preferences.cpp - 2005-11-28 3:40:27 AM - 6469 Bytes
$History: * selfimage_preferences.dfm - 2005-11-28 3:39:24 AM - 5055 Bytes
$History: * selfimage_preferences.h - 2005-11-28 3:39:24 AM - 2526 Bytes
$History: * selfimage_preferences.ddp - 2005-11-28 3:39:22 AM - 51 Bytes
$History: * Attach help button, add preferences for compression 
$History: * and number of threads.
$History: **** V 0.5 by kfitzner ****
$History: * selfimage_preferences.cpp - 2005-11-28 1:56:16 PM - 6853 Bytes
$History: * selfimage_preferences.dfm - 2005-11-28 3:49:08 AM - 5076 Bytes
$History: * selfimage_preferences.h - 2005-11-28 3:39:24 AM - 2526 Bytes
$History: * selfimage_preferences.ddp - 2005-11-28 3:49:10 AM - 51 Bytes
$History: * Fix help button context setting
$History: **** V 1.0 by kfitzner ****
$History: * selfimage_preferences.cpp - 2005-11-28 2:06:09 PM - 7376 Bytes
$History: * selfimage_preferences.dfm - 2005-11-28 2:02:06 PM - 5076 Bytes
$History: * selfimage_preferences.h - 2005-11-28 3:39:24 AM - 2526 Bytes
$History: * selfimage_preferences.ddp - 2005-11-28 2:05:36 PM - 51 Bytes
$History: * Display number of threads for auto
$History: **** V 1.1 by kfitzner ****
$History: * selfimage_preferences.cpp - 2005-12-04 12:37:33 PM - 7917 Bytes
$History: * selfimage_preferences.dfm - 2005-12-04 6:21:32 AM - 5432 Bytes
$History: * selfimage_preferences.h - 2005-12-04 6:22:30 AM - 2589 Bytes
$History: * selfimage_preferences.ddp - 2005-12-04 6:23:08 AM - 51 Bytes
$History: * Add skip unallocated areas option
$History: **** V 1.2 by kfitzner ****
$History: * selfimage_preferences.cpp - 2005-12-08 9:41:40 AM - 8351 Bytes
$History: * selfimage_preferences.dfm - 2005-12-04 1:25:48 PM - 5437 Bytes
$History: * selfimage_preferences.h - 2005-12-04 6:22:30 AM - 2589 Bytes
$History: * selfimage_preferences.ddp - 2005-12-08 9:41:16 AM - 51 Bytes
$History: * Change wording of skip unallocated setting, remove 
$History: * extern defs and replace with #include of TypesDefs.h
$History: **** V 1.3 by kfitzner ****
$History: * selfimage_preferences.cpp - 2005-12-09 2:18:29 PM - 9401 Bytes
$History: * selfimage_preferences.dfm - 2005-12-09 2:09:06 PM - 8868 Bytes
$History: * selfimage_preferences.h - 2005-12-09 10:03:48 AM - 2833 Bytes
$History: * selfimage_preferences.ddp - 2005-12-09 2:09:14 PM - 51 Bytes
$History: * Add NBD options, and update the look - now multiple pages.
$History: **** V 1.4 by kfitzner ****
$History: * selfimage_preferences.cpp - 2005-12-27 6:02:19 PM - 9766 Bytes
$History: * selfimage_preferences.dfm - 2005-12-09 2:09:06 PM - 8868 Bytes
$History: * selfimage_preferences.h - 2005-12-09 10:03:48 AM - 2833 Bytes
$History: * selfimage_preferences.ddp - 2005-12-11 10:54:36 AM - 51 Bytes
$History: * Fix NBD options not saving
$History: **** Latest ** V 1.5 by kfitzner ** 2007-03-18 2:26:40 PM ****
$History: * Add gzipChunkSize field
*/
//----------------------------  $NoKeywords ---------------------------------

//---------------------------------------------------------------------------
// File Notes:
//---------------------------------------------------------------------------
// 12 Nov 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// A simple preferences form.  Right now it's probably not strictly needed
// (it only has two entried - window position saving and new version
// notification), but it's better to have something in place to make
// future expansion easier.  Besides, the TConfiguration class makes this
// a breeze - really this is just a visual interface to that class.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelfImage_TypesDefs.h"
#include "SelfImage_Utility.h"
#include "SelfImage_Preferences.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TformSelfImagePreferences *formSelfImagePreferences;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Form constructor
//
__fastcall TformSelfImagePreferences::TformSelfImagePreferences(TComponent* Owner) : TForm(Owner)
{
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// OnShow event handler
//
void __fastcall TformSelfImagePreferences::FormShow(TObject *Sender)
{
  chbVersionNotify->Checked = SelfImageConfig->Values["Update Notify"];
  chbWindowPositions->Checked = SelfImageConfig->Values["Remember Positions"];
  chbWriteWarning->Checked = SelfImageConfig->Values["NoDirectWriteWarn"];
  chbSkipUnallocated->Checked = SelfImageConfig->Values["SkipUnallocated"];
  edtTimeout->Text = String(SelfImageConfig->Values["NBDTimeout"]);
  edtTransaction->Text = String(SelfImageConfig->Values["NBDTransactionSize"]);
  udThreadCount->Position = SelfImageConfig->Values["ProcessingThreads"];
  udGzipFast->Position = SelfImageConfig->Values["gzipFastLevel"];
  udGzipBest->Position = SelfImageConfig->Values["gzipBestLevel"];
  udGzipChunk->Position = SelfImageConfig->Values["gzipChunkSize"];
  lblCompressionThreads->Caption = "# of Processing Threads (0=auto [" + String(GetCPUCount()) + "])";
}  // void __fastcall TformSelfImagePreferences::FormShow(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// udGzipChunk OnChangingEx event handler
// Makes sure that udGzipChunk->Position is a power of two
//
void __fastcall TformSelfImagePreferences::udGzipChunkChangingEx(TObject *Sender, bool &AllowChange, short NewValue, TUpDownDirection Direction)
{
  if (abs(NewValue - udGzipChunk->Position) == 1) {
    if (Direction == updUp)
      udGzipChunk->Position <<= 1;
    else if (Direction == updDown)
      udGzipChunk->Position >>=1;
    AllowChange = false;
  } else
    AllowChange = true;
}  // void __fastcall TformSelfImagePreferences::udGzipChunkChangingEx(blah)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Ok button OnClick event handler - user clicked 'Ok' so we update the
// configuration with the new preferences
//
void __fastcall TformSelfImagePreferences::btnOkClick(TObject *Sender)
{
  int nTemp;
  SelfImageConfig->Values["Update Notify"] = chbVersionNotify->Checked;
  SelfImageConfig->Values["Remember Positions"] = chbWindowPositions->Checked;
  SelfImageConfig->Values["NoDirectWriteWarn"] = chbWriteWarning->Checked;
  SelfImageConfig->Values["SkipUnallocated"] = chbSkipUnallocated->Checked;
  // If the user turns off the option to remember the window positions, then reset any
  // position information that may be stored.
  if (!chbWindowPositions->Checked) {
      SelfImageConfig->Values["WindowTop"] = -1;
      SelfImageConfig->Values["WindowLeft"] = -1;
      SelfImageConfig->Values["WindowWidth"] = -1;
  }  // if (!chbWindowPositions->Checked)
  if (TryStrToInt(edtTimeout->Text, nTemp)) {
    if (nTemp < 100) nTemp = 100; else if (nTemp > 99999) nTemp = 99999;
    SelfImageConfig->Values["NBDTimeout"] = nTemp;
  }  // if (TryStrToInt(edtTimeout->Text, nTemp))
  if (TryStrToInt(edtTransaction->Text, nTemp)) {
    if (nTemp < 64) nTemp = 64; else if (nTemp > 2048) nTemp = 2048;
    SelfImageConfig->Values["NBDTransactionSize"] = nTemp;
  }  // if (TryStrToInt(edtTransaction->Text, nTemp))
  SelfImageConfig->Values["ProcessingThreads"] = udThreadCount->Position;
  SelfImageConfig->Values["gzipFastLevel"] = udGzipFast->Position;
  SelfImageConfig->Values["gzipBestLevel"] = udGzipBest->Position;
  SelfImageConfig->Values["gzipChunkSize"] = udGzipChunk->Position;
  Close();
}  // void __fastcall TformSelfImagePreferences::btnOkClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Help button OnClick event handler
//
void __fastcall TformSelfImagePreferences::btnHelpClick(TObject *Sender)
{
  Application->HelpContext(((TControl *)Sender)->HelpContext);
}
//---------------------------------------------------------------------------


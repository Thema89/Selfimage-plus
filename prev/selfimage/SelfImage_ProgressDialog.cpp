//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_ProgressDialog.cpp - Simple progress bar dialog
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
// VCS: $Version: 1 $ $Revision: 1 $
/*
$History: **** V 0.1 by kfitzner ****
$History: * selfimage_progressdialog.cpp - 2005-11-07 12:52:50 AM - 1697 Bytes
$History: * selfimage_progressdialog.dfm - 2005-11-07 12:53:34 AM - 1330 Bytes
$History: * selfimage_progressdialog.h - 2005-11-07 12:51:40 AM - 1212 Bytes
$History: * selfimage_progressdialog.ddp - 2005-11-07 12:54:40 AM - 51 Bytes
$History: * Initial check-in
$History: **** V 0.2 by kfitzner ****
$History: * selfimage_progressdialog.cpp - 2005-11-12 5:10:52 PM - 4188 Bytes
$History: * selfimage_progressdialog.dfm - 2005-11-07 12:53:34 AM - 1330 Bytes
$History: * selfimage_progressdialog.h - 2005-11-12 5:10:24 PM - 2232 Bytes
$History: * selfimage_progressdialog.ddp - 2005-11-12 5:10:52 PM - 51 Bytes
$History: * Add copyright notice and VCS history/version tags
$History: **** V 0.3 by kfitzner ****
$History: * selfimage_progressdialog.cpp - 2005-11-12 5:16:10 PM - 4576 Bytes
$History: * selfimage_progressdialog.dfm - 2005-11-07 12:53:34 AM - 1330 Bytes
$History: * selfimage_progressdialog.h - 2005-11-12 5:15:56 PM - 2237 Bytes
$History: * selfimage_progressdialog.ddp - 2005-11-12 5:16:04 PM - 51 Bytes
$History: * Typo in copyright area
$History: **** V 0.4 by kfitzner ****
$History: * selfimage_progressdialog.cpp - 2005-11-27 9:07:54 AM - 4993 Bytes
$History: * selfimage_progressdialog.dfm - 2005-11-27 9:04:52 AM - 1375 Bytes
$History: * selfimage_progressdialog.h - 2005-11-27 9:05:16 AM - 2169 Bytes
$History: * selfimage_progressdialog.ddp - 2005-11-27 9:05:14 AM - 51 Bytes
$History: * Eliminate extra #pragma link directives - removed 
$History: * the JVCL bevel in favor of a plain panel
$History: **** V 1.0 by kfitzner ****
$History: * selfimage_progressdialog.cpp - 2005-11-29 6:45:25 AM - 5514 Bytes
$History: * selfimage_progressdialog.dfm - 2005-11-29 6:41:48 AM - 1352 Bytes
$History: * selfimage_progressdialog.h - 2005-11-29 6:41:56 AM - 2112 Bytes
$History: * selfimage_progressdialog.ddp - 2005-11-29 6:44:02 AM - 51 Bytes
$History: * Remove remaining JVCS controls
$History: **** Latest ** V 1.1 by kfitzner ** 2007-03-30 5:15:54 PM ****
$History: * Fix "The windows does not have scroll bars" error, 
$History: * add Position property
*/
//----------------------------- $NoKeywords ---------------------------------

//---------------------------------------------------------------------------
// File Notes:
//---------------------------------------------------------------------------
// 12 Nov 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// A very simple progress bar dialog - nothing special here.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelfImage_ProgressDialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CGAUGES"
#pragma resource "*.dfm"
TProgressDialog *ProgressDialog;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructor
//
__fastcall TProgressDialog::TProgressDialog(TComponent* Owner): TForm(Owner)
{
  // Work around a C++ Builder bug.  Seems like we do a lot of that.
  if (GetLastError() == ERROR_NO_SCROLLBARS)
    ::SetLastError(NO_ERROR);
  ProgressBar->Progress = 0;
  ProgressBar->MaxValue = 0;
  lblCaption->Caption = "";
  lblPercent->Caption = "0%";
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Display the form - call this function to make the form visible.  Caption
// is a blurb displayed just under the progress bar.  InitialPosition is
// the initial value of the progress bar (usually zero), and Max is the
// maximum value.
//
void __fastcall TProgressDialog::Display(AnsiString Caption, int InitialPosition, int Max)
{
  lblCaption->Caption = Caption;
  ProgressBar->MaxValue = Max;
  ProgressBar->Progress = InitialPosition;
  lblPercent->Caption = String(InitialPosition * 100 / Max) + "%";
  Show();
  Update();
}  // void __fastcall Display(int Position, int Max)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Set the progress bar's position.  The form is updated after to ensure the
// user sees the change.
//
void __fastcall TProgressDialog::SetPosition(int Position)
{
  FPosition = Position;
  ProgressBar->Progress = FPosition;
  lblPercent->Caption = String(FPosition * 100 / ProgressBar->MaxValue) + "%";
  Update();
}  // void __fastcall TProgressDialog::SetPosition(int Position)
//---------------------------------------------------------------------------


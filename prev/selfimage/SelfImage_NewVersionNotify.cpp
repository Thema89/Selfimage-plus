//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_NewVersionNotify.cpp - Inform the user of a new version
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
$History: * selfimage_newversionnotify.cpp - 2005-11-10 9:38:38 AM - 5044 Bytes
$History: * selfimage_newversionnotify.dfm - 2005-11-10 9:29:46 AM - 2203 Bytes
$History: * selfimage_newversionnotify.h - 2005-11-10 9:31:54 AM - 2373 Bytes
$History: * selfimage_newversionnotify.ddp - 2005-11-10 9:32:28 AM - 51 Bytes
$History: * Initial check-in
$History: **** V 0.2 by kfitzner ****
$History: * selfimage_newversionnotify.cpp - 2005-11-10 10:04:49 AM - 5439 Bytes
$History: * selfimage_newversionnotify.dfm - 2005-11-10 10:04:22 AM - 2225 Bytes
$History: * selfimage_newversionnotify.h - 2005-11-10 10:04:22 AM - 2373 Bytes
$History: * selfimage_newversionnotify.ddp - 2005-11-10 10:04:48 AM - 51 Bytes
$History: * Tweak label positions
$History: **** V 1.0 by kfitzner ****
$History: * selfimage_newversionnotify.cpp - 2005-11-12 5:15:10 PM - 5985 Bytes
$History: * selfimage_newversionnotify.dfm - 2005-11-10 10:04:22 AM - 2225 Bytes
$History: * selfimage_newversionnotify.h - 2005-11-12 5:14:32 PM - 2396 Bytes
$History: * selfimage_newversionnotify.ddp - 2005-11-12 5:15:10 PM - 51 Bytes
$History: * Change copyright area - said the file was part of 
$History: * GPGee, not SelfImage.  Whoops - forgot to change that 
$History: * when I copied it over.
$History: **** Latest ** V 1.1 by kfitzner ** 2005-12-08 9:43:14 AM ****
$History: * Remove externs, add #include of TypesDefs.h
*/
//----------------------------  $NoKeywords ---------------------------------


//---------------------------------------------------------------------------
// File Notes:
//---------------------------------------------------------------------------
// 31 Aug 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// A very simple form - simply tell the user that there is a new version
// available and give the URL.  No muss, no fuss.
//
// ShellExecute() is used on the URL in order to cause the OS to launch
// the default browser.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelfImage_TypesDefs.h"
#include "SelfImage_NewVersionNotify.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformSelfImageNewVersionNotify *formSelfImageNewVersionNotify;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Form constructor
//
__fastcall TformSelfImageNewVersionNotify::TformSelfImageNewVersionNotify(TComponent* Owner, AnsiString MyVersion, AnsiString NewVersion) : TForm(Owner)
{
  __ENTERFUNCTION__;
  sMyVersion = MyVersion;
  sNewVersion = NewVersion;
  __RETURNFUNCTION__;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// OnShow event handler - simply set the version label captions to the
// version strings we were provided at construction time
//
void __fastcall TformSelfImageNewVersionNotify::FormShow(TObject *Sender)
{
  __ENTERFUNCTION__;

  // Read in the language strings for the form's visual text labels and change them on the form.  Borland's language
  // editor is capable of doing this, but it's much easier for translators if we can give them a single resource
  // string table to translate instead of forcing them to use Borland's translation tool.
  /*
  this->Caption              = GetMessage(FORM_NOTIFY_CAPTION);
  btnOk->Caption             = GetMessage(FORM_BTN_OK);
  lblMessage->Caption        = GetMessage(FORM_NOTIFY_LABEL_MESSAGE);
  lblURL->Caption            = GetMessage(FORM_NOTIFY_LABEL_URL);
  lblMyVersion->Caption      = GetMessage(FORM_NOTIFY_LABEL_CURRENT);
  lblCurrentVersion->Caption = GetMessage(FORM_NOTIFY_LABEL_NEW);
  chbSquelch->Caption        = GetMessage(FORM_NOTIFY_CHECK_NOREPEAT);
  */
  
  lblMyVersionValue->Caption = sMyVersion;
  lblCurrentVersionValue->Caption = sNewVersion;
  __RETURNFUNCTION__;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// What to do when the user clicks on the URL
//
void __fastcall TformSelfImageNewVersionNotify::lblURLClick(TObject *Sender)
{
  __ENTERFUNCTION__;
  ShellExecute(HInstance, "open", lblURL->Caption.c_str(), NULL, NULL, 0);
  __RETURNFUNCTION__;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Ok button OnClick event handler.  What modal result we return depends
// on whether the user has checked the "Don't notify..." box
//
void __fastcall TformSelfImageNewVersionNotify::btnOkClick(TObject *Sender)
{
  __ENTERFUNCTION__;
  if (chbSquelch->Checked)
    ModalResult = mrCancel;
  else
    ModalResult = mrOk;
  __RETURNFUNCTION__;
}
//---------------------------------------------------------------------------



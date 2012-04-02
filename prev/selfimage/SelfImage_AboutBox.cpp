//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_AboutBox.cpp - About box
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
$History: * selfimage_aboutbox.cpp - 2005-11-15 8:35:46 PM - 3987 Bytes
$History: * selfimage_aboutbox.dfm - 2005-11-15 8:34:08 PM - 17397 Bytes
$History: * selfimage_aboutbox.h - 2005-11-15 7:29:12 PM - 2173 Bytes
$History: * selfimage_aboutbox.ddp - 2005-11-15 8:34:06 PM - 51 Bytes
$History: * Initial check-in
$History: **** V 0.2 by kfitzner ****
$History: * selfimage_aboutbox.cpp - 2005-11-18 12:22:10 PM - 4521 Bytes
$History: * selfimage_aboutbox.dfm - 2005-11-18 12:20:12 PM - 17412 Bytes
$History: * selfimage_aboutbox.h - 2005-11-18 12:20:52 PM - 2144 Bytes
$History: * selfimage_aboutbox.ddp - 2005-11-18 12:22:10 PM - 51 Bytes
$History: * Change name to formSelfImageAboutBox
$History: **** V 0.3 by kfitzner ****
$History: * selfimage_aboutbox.cpp - 2005-11-18 12:24:53 PM - 4890 Bytes
$History: * selfimage_aboutbox.dfm - 2005-11-18 12:20:12 PM - 17412 Bytes
$History: * selfimage_aboutbox.h - 2005-11-18 12:20:52 PM - 2144 Bytes
$History: * selfimage_aboutbox.ddp - 2005-11-18 12:23:38 PM - 51 Bytes
$History: * Fix name change
$History: **** V 0.4 by kfitzner ****
$History: * selfimage_aboutbox.cpp - 2005-11-28 1:40:08 PM - 5822 Bytes
$History: * selfimage_aboutbox.dfm - 2005-11-27 8:52:24 AM - 17000 Bytes
$History: * selfimage_aboutbox.h - 2005-11-27 9:06:46 AM - 2263 Bytes
$History: * selfimage_aboutbox.ddp - 2005-11-27 9:06:44 AM - 51 Bytes
$History: * Add multiple pages to the main text area for credits, 
$History: * acknowledgements, etc
$History: **** V 1.0 by kfitzner ****
$History: * selfimage_aboutbox.cpp - 2005-11-29 6:35:33 AM - 6560 Bytes
$History: * selfimage_aboutbox.dfm - 2005-11-29 6:34:22 AM - 16440 Bytes
$History: * selfimage_aboutbox.h - 2005-11-29 6:34:50 AM - 2259 Bytes
$History: * selfimage_aboutbox.ddp - 2005-11-29 6:34:48 AM - 51 Bytes
$History: * Remove remaining JVCS controls
$History: **** Latest ** V 1.1 by kfitzner ** 2005-12-08 9:44:23 AM ****
$History: * Remove externs, add #include of TypesDefs.h
*/
//----------------------------  $NoKeywords ---------------------------------


//---------------------------------------------------------------------------
// File Notes:
//---------------------------------------------------------------------------
// 15 Nov 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
//
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelfImage_TypesDefs.h"
#include "SelfImage_AboutBox.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------
TformSelfImageAboutBox *formSelfImageAboutBox;
//---------------------------------------------------------------------


//---------------------------------------------------------------------
__fastcall TformSelfImageAboutBox::TformSelfImageAboutBox(TComponent* AOwner) : TForm(AOwner)
{
}
//---------------------------------------------------------------------


//---------------------------------------------------------------------------
// OnShow event handler
// Load the text for the richedit component, set the version number labels
// and make all the moving parts move.
//
void __fastcall TformSelfImageAboutBox::FormShow(TObject *Sender)
{
  AnsiString sBuild;

  lblVersionNumber->Caption = sVersionString;
  sBuild = "   " + String(nBuildNumber);
  sBuild = sBuild.SubString(sBuild.Length()-3,4);
  lblBuildNumber->Caption = sBuild;

  // lblImageIsEverything->BehaviorOptions->Active = true;
  lblImage->Left = pnlAboutPanel->Width;
  ImageTimer->Enabled = true;

  TabControlChange(NULL);
}  // void __fastcall TSelfImageAboutBox::FormShow(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TabControl OnChange handler
//
void __fastcall TformSelfImageAboutBox::TabControlChange(TObject *Sender)
{
  HRSRC AboutBoxResource;
  HGLOBAL AboutBoxGlobal;
  TBufferStream *AboutBoxStream;
  int ResourceSize;
  char *AboutBoxText;
  AnsiString ResourceName;

  try {
    ResourceName = "ABOUTBOXTEXT_" + TabControl->Tabs->Strings[TabControl->TabIndex];

    // Load the about box text from a resource
    AboutBoxResource = FindResource(HInstance, ResourceName.c_str(), RT_RCDATA);
    if (!AboutBoxResource)
      throw EResNotFound("There was an error while attempting to locate the about box text resource.  This is probably a programming error and should be reported to " + Application->Name + "'s maintainers.");
    ResourceSize = SizeofResource(HInstance, AboutBoxResource);
    AboutBoxGlobal = LoadResource(HInstance, AboutBoxResource);
    AboutBoxText = (char *)LockResource(AboutBoxGlobal);
    AboutBoxStream = new TBufferStream(AboutBoxText, ResourceSize);
    AboutBoxStream->Seek(0, soFromBeginning);
    edtText->Clear();
    edtText->Lines->LoadFromStream(AboutBoxStream);
  }  // try
  __finally {
    if (AboutBoxStream)
      delete AboutBoxStream;
    return;
  }  // __finally


}  // void __fastcall TformSelfImageAboutBox::edtTextChange(TObject *Sender)
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// ImageTimer OnTimer event
//
void __fastcall TformSelfImageAboutBox::ImageTimerTimer(TObject *Sender)
{
  if (lblImage->Left > 97)
    lblImage->Left -= 2;
  else
    ImageTimer->Enabled = false;
}
//---------------------------------------------------------------------------


//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_NBDDialog.cpp - NBD Connection dialog
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
// VCS: $Version: 1 $ $Revision: 3 $
/*
$History: **** V 1.0 by kfitzner ****
$History: * selfimage_nbddialog.cpp - 2005-12-08 4:11:55 PM - 8155 Bytes
$History: * selfimage_nbddialog.dfm - 2005-12-08 1:05:52 PM - 5467 Bytes
$History: * selfimage_nbddialog.h - 2005-12-08 1:58:08 PM - 2724 Bytes
$History: * selfimage_nbddialog.ddp - 2005-12-08 1:59:56 PM - 51 Bytes
$History: * Initial check-in
$History: **** V 1.1 by kfitzner ****
$History: * selfimage_nbddialog.cpp - 2005-12-09 2:18:06 PM - 8909 Bytes
$History: * selfimage_nbddialog.dfm - 2005-12-09 2:12:24 PM - 5517 Bytes
$History: * selfimage_nbddialog.h - 2005-12-09 2:07:16 PM - 2879 Bytes
$History: * selfimage_nbddialog.ddp - 2005-12-09 2:12:30 PM - 51 Bytes
$History: * Get rid of the stupid inline warnings, update the 
$History: * button glyphs, and connect the help button.
$History: **** V 1.2 by kfitzner ****
$History: * selfimage_nbddialog.cpp - 2005-12-09 2:52:00 PM - 9326 Bytes
$History: * selfimage_nbddialog.dfm - 2005-12-09 2:46:18 PM - 5538 Bytes
$History: * selfimage_nbddialog.h - 2005-12-09 2:07:16 PM - 2879 Bytes
$History: * selfimage_nbddialog.ddp - 2005-12-09 2:48:00 PM - 51 Bytes
$History: * Fix "Ok" button being enabled when it shouldn't
$History: **** Latest ** V 1.3 by kfitzner ** 2007-03-14 12:25:43 PM ****
$History: * Use TImageStore to check size of remote partition 
$History: * so that the usable size of EXT2/3 filesystems can be interpreted
*/
//----------------------------  $NoKeywords ---------------------------------

//---------------------------------------------------------------------------
// File Notes:
//---------------------------------------------------------------------------
// 7 Dec 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// NBD connections require a hostname and a port number.  This dialog allows
// the user to save those connections under an "alias" name.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelfImage_TypesDefs.h"
#include "SelfImage_TImageStore.h"
#include "SelfImage_NBDDialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformSelfImageConnect *formSelfImageConnect;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Form constructor
//
__fastcall TformSelfImageConnect::TformSelfImageConnect(TComponent* Owner) : TForm(Owner)
{
  Aliases = new TStringList;
}  // __fastcall TformSelfImageConnect::TformSelfImageConnect(TComponent* Owner) : TForm(Owner)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Form destructor
//
__fastcall TformSelfImageConnect::~TformSelfImageConnect()
{
  delete Aliases;
}  // __fastcall TformSelfImageConnect::~TformSelfImageConnect()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Form's OnShow event handler - load up any connection aliases that might
// be present.
//
void __fastcall TformSelfImageConnect::FormShow(TObject *Sender)
{
  AnsiString sHost;
  int nPort;

  SetupAliases();

  if (SplitConnectionString(DefaultConnectionString, sHost, nPort)) {
    HostName = sHost;
    Port = nPort;
    for (int n = 0; n < cmbAliases->Items->Count; n++)
      if (!Aliases->Values[cmbAliases->Items->Strings[n]].AnsiCompareIC(DefaultConnectionString))
        cmbAliases->ItemIndex = n;
  }  // if (SplitConnectionString(DefaultConnectionString, sHost, nPort))
  UpdateComponents(NULL);
}  // void __fastcall TformSelfImageConnect::FormShow(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Load in existing aliases and set up the combo box
//
void __fastcall TformSelfImageConnect::SetupAliases(void)
{
  Aliases->Sorted = false;
  Aliases->Text = SelfImageConfig->Values["NBDConnections"];
  Aliases->Sorted = true;

  if (cmbAliases->Items->Count)
    cmbAliases->Items->Clear();
  for (int n = 0; n < Aliases->Count; n++)
    cmbAliases->Items->Add(Aliases->Names[n]);
}  // void __fastcall TformSelfImageConnect::SetupAliases(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Show the dialog with a default connection string displayed
//
int __fastcall TformSelfImageConnect::Execute(AnsiString DefaultConnection)
{
  DefaultConnectionString = DefaultConnection;
  return ShowModal();
}  // bool __fastcall TformSelfImageConnect::Execute(AnsiString DefaultConnection)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void __fastcall TformSelfImageConnect::UpdateComponents(TObject *Sender)
{
  int nAliasIndex = cmbAliases->Items->IndexOf(cmbAliases->Text);
  btnAddAlias->Enabled = !HostName.IsEmpty() && Port && !cmbAliases->Text.IsEmpty() && nAliasIndex == -1;
  btnRemoveAlias->Enabled = nAliasIndex != -1;
  btnOk->Enabled = !HostName.IsEmpty() && Port;
}  // void __fastcall TformSElfImageConnect::UpdateComponents(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void __fastcall TformSelfImageConnect::cmbAliasesChange(TObject *Sender)
{
  if (cmbAliases->ItemIndex != -1)
    ConnectionString = Aliases->Values[cmbAliases->Text];
  UpdateComponents(NULL);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// btnAddAlias OnClick event handler - user wants to add a new alias
//
void __fastcall TformSelfImageConnect::btnAddAliasClick(TObject *Sender)
{
  AnsiString sAlias = cmbAliases->Text;
  Aliases->Add(sAlias + "=" + ConnectionString);
  SelfImageConfig->Values["NBDConnections"] = Aliases->Text;
  SetupAliases();
  cmbAliases->ItemIndex = cmbAliases->Items->IndexOf(sAlias);
  UpdateComponents(NULL);
}  // void __fastcall TformSelfImageConnect::btnAddAliasClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// btnRemoveAliase OnClick event handler - user wants to delete an alias
//
void __fastcall TformSelfImageConnect::btnRemoveAliasClick(TObject *Sender)
{
  int nIndex = Aliases->IndexOfName(cmbAliases->Text);
  if (nIndex != -1) {
    Aliases->Delete(nIndex);
    SelfImageConfig->Values["NBDConnections"] = Aliases->Text;
    SetupAliases();
    // cmbAliases->Text = "";
    UpdateComponents(NULL);
  }  // if (nIndex != -1)
}  // void __fastcall TformSelfImageConnect::btnRemoveAliasClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// btnOk OnClick event handler - user pressed the Ok button - see if this
// NBD server actually exists...
//
void __fastcall TformSelfImageConnect::btnOkClick(TObject *Sender)
{
  bool bSuccess;
  TImageStore *NBDTest;

  try {
    NBDTest = new TImageStore(isNBD, HostName + ":" + String(Port), false);
    FRemoteSize = NBDTest->Geometry.Bytes;
    delete NBDTest;
  } // try
  catch (Exception &e) {
    FRemoteSize = 0;
    AnsiString ErrorMessage = String(e.ClassName()) + " error while trying to connect to \"" + ConnectionString + "\" with message: " + e.Message;
    Application->MessageBox(ErrorMessage.c_str(), "Connection Error", MB_OK | MB_ICONERROR);
    return;
  }  // catch
  ModalResult = mrOk;
}  // void __fastcall TformSelfImageConnect::btnOkClick(TObject *Sender)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void __fastcall TformSelfImageConnect::btnHelpClick(TObject *Sender)
{
  Application->HelpContext(((TControl *)Sender)->HelpContext);
}
//---------------------------------------------------------------------------


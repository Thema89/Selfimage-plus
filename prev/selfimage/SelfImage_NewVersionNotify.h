//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_NewVersionNotify.h - Inform the user of a new version - header
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
#ifndef SelfImage_NewVersionNotifyH
#define SelfImage_NewVersionNotifyH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TformSelfImageNewVersionNotify : public TForm
{
__published:	// IDE-managed Components
  TLabel *lblMessage;
  TLabel *lblURL;
  TLabel *lblMyVersion;
  TLabel *lblMyVersionValue;
  TLabel *lblCurrentVersion;
  TLabel *lblCurrentVersionValue;
  TCheckBox *chbSquelch;
  TButton *btnOk;
  void __fastcall lblURLClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
  AnsiString sMyVersion;
  AnsiString sNewVersion;
public:		// User declarations
  __fastcall TformSelfImageNewVersionNotify::TformSelfImageNewVersionNotify(TComponent* Owner, AnsiString MyVersion, AnsiString NewVersion);
};
//---------------------------------------------------------------------------
extern PACKAGE TformSelfImageNewVersionNotify *formSelfImageNewVersionNotify;
//---------------------------------------------------------------------------
#endif

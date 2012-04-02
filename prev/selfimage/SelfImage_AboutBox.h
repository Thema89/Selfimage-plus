//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_AboutBox.h - About box header
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
#ifndef SelfImage_AboutBoxH
#define SelfImage_AboutBoxH
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <Graphics.hpp>
//----------------------------------------------------------------------------
class TformSelfImageAboutBox : public TForm
{
__published:
  TPanel *pnlAboutPanel;
  TImage *ProgramIcon;
  TLabel *lblSelfImage;
  TLabel *lblImaging;
  TLabel *lblVersion;
  TLabel *lblVersionNumber;
  TLabel *lblBuild;
  TLabel *lblBuildNumber;
  TTabControl *TabControl;
  TRichEdit *edtText;
  TButton *OKButton;
  TLabel *lblImage;
  TTimer *ImageTimer;

  void __fastcall FormShow(TObject *Sender);
  void __fastcall TabControlChange(TObject *Sender);
  void __fastcall ImageTimerTimer(TObject *Sender);

public:
  virtual __fastcall TformSelfImageAboutBox(TComponent* AOwner);

private:
  //void __fastcall LoadTabPage
};
//----------------------------------------------------------------------------
extern PACKAGE TformSelfImageAboutBox *formSelfImageAboutBox;
//----------------------------------------------------------------------------
#endif

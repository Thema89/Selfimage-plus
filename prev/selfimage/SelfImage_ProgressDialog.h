//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_ProgressDialog.h - Simple progress bar dialog header
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
#ifndef SelfImage_ProgressDialogH
#define SelfImage_ProgressDialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "CGAUGES.h"
//---------------------------------------------------------------------------
class TProgressDialog : public TForm
{
__published:    // IDE-managed Components
  TLabel *lblCaption;
  TPanel *Panel;
  TCGauge *ProgressBar;
  TLabel *lblPercent;

public:         // User declarations
  __property int Position = {read = FPosition, write=SetPosition};

  __fastcall TProgressDialog(TComponent* Owner);
  void __fastcall Display(AnsiString Caption, int InitialPosition, int Max);
  void inline __fastcall StepBy(int Step) { SetPosition(FPosition + Step); };


private:        // User declarations
  int FPosition;

  void SetPosition(int Position);
};  // class TProgressDialog : public TForm
//---------------------------------------------------------------------------
extern PACKAGE TProgressDialog *ProgressDialog;
//---------------------------------------------------------------------------
#endif

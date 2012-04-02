//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_Preferences.cpp - Preferences form header
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
#ifndef SelfImage_PreferencesH
#define SelfImage_PreferencesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TformSelfImagePreferences : public TForm
{
__published:	// IDE-managed Components
  TPageControl *Pages;
  TTabSheet *tabGeneral;
  TGroupBox *gbGeneralOptions;
  TCheckBox *chbVersionNotify;
  TCheckBox *chbWindowPositions;
  TCheckBox *chbWriteWarning;
  TTabSheet *tabRead;
  TGroupBox *gbReadOptions;
  TCheckBox *chbSkipUnallocated;
  TTabSheet *tabNBD;
  TGroupBox *gbNBD;
  TEdit *edtTimeout;
  TLabel *lblTimeout;
  TEdit *edtTransaction;
  TLabel *lblTransaction;
  TTabSheet *tabCompression;
  TGroupBox *gbCompression;
  TLabel *lblCompressionThreads;
  TLabel *lblGzipFast;
  TLabel *lblGzipBest;
  TLabel *lblGzipChunk;
  TUpDown *udThreadCount;
  TEdit *edtThreadSpin;
  TUpDown *udGzipFast;
  TEdit *edtGzipFastSpin;
  TUpDown *udGzipBest;
  TEdit *edtGzipBestSpin;
  TEdit *edtGzipChunkSpin;
  TUpDown *udGzipChunk;
  TButton *btnOk;
  TButton *btnCancel;
  TSpeedButton *btnHelp;

  void __fastcall FormShow(TObject *Sender);
  void __fastcall btnOkClick(TObject *Sender);
  void __fastcall btnHelpClick(TObject *Sender);
        void __fastcall udGzipChunkChangingEx(TObject *Sender,
          bool &AllowChange, short NewValue, TUpDownDirection Direction);

private:	// User declarations

public:		// User declarations
  __fastcall TformSelfImagePreferences(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TformSelfImagePreferences *formSelfImagePreferences;
//---------------------------------------------------------------------------
#endif

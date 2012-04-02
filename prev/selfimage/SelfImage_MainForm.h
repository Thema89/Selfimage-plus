//------------------------------ $Keywords ----------------------------------
// SelfImage - Disk image manipulation program
// SelfImage_MainForm.h - Main form header
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
#ifndef SelfImage_MainFormH
#define SelfImage_MainFormH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include "CGAUGES.h"

#include "SelfImage_TDriveList.h"
#include "SelfImage_TypesDefs.h"
#include "SelfImage_ReadThread.h"
#include "SelfImage_WriteThread.h"
#include "SelfImage_CompressionThread.h"
#include "SelfImage_DecompressionThread.h"
//---------------------------------------------------------------------------
enum TMainFormVisualMode { modeSetup, modeOperating };
//---------------------------------------------------------------------------
#define MAX_PROCESSING_THREADS 16
#define MAX_THREADS 18
//---------------------------------------------------------------------------
#pragma warn -8026
#pragma warn -8027
class TMainForm : public TForm
{
__published:	// IDE-managed Components
  TGroupBox *gbInput;
  TLabel *lblInputLocation;
  TRadioButton *radioSourceFile;
  TEdit *edtSourceFileName;
  TSpeedButton *btnSourceFileOpen;
  TRadioButton *radioSourceDrive;
  TComboBox *cmbSourceDrive;
  TRadioButton *radioSourceNBD;
  TEdit *edtSourceNBD;
  TSpeedButton *btnSourceNBDConnect;
  TLabel *lblSourceSize;
  TLabel *lblSourceSizeValue;
  TGroupBox *gbProcessing;
  TRadioButton *radioNoProcessing;
  TRadioButton *radioDecompress;
  TRadioButton *radioCompress;
  TGroupBox *gbCompressionMethod;
  TRadioButton *radioGzipFast;
  TRadioButton *radioGzipBest;
  TRadioButton *radioBzip2;
  TGroupBox *gbOutput;
  TLabel *lblOutputLocation;
  TRadioButton *radioTargetFile;
  TEdit *edtTargetFileName;
  TSpeedButton *btnTargetFileOpen;
  TRadioButton *radioTargetDrive;
  TComboBox *cmbTargetDrive;
  TLabel *lblTargetDriveSize;
  TLabel *lblTargetDriveSizeValue;
  TButton *btnOk;
  TButton *btnCancel;
  TSpeedButton *btnHelp;
  TGroupBox *gbStatus;
  TCGauge *ProgressBar;
  TBevel *bvProgressBevel;
  TLabel *lblPercent;
  TShape *shpLine;
  TLabel *lblInputSize;
  TLabel *lblInput;
  TEdit *edtInput;
  TLabel *lblOutput;
  TEdit *edtOutput;
  TLabel *lblBytesRead;
  TLabel *lblBytesReadValue;
  TLabel *lblBytesSkipped;
  TLabel *lblBytesSkippedValue;
  TLabel *lblBytesWritten;
  TLabel *lblBytesWrittenValue;
  TLabel *lblBytesTotalValue;
  TLabel *lblWriteSpeedValue;
  TLabel *lblWriteSpeed;
  TLabel *lblOverallSpeed;
  TLabel *lblOverallSpeedValue;
  TMenuItem *menu_File;
  TMenuItem *menu_File_Start;
  TMenuItem *N1;
  TMenuItem *menu_File_Exit;
  TMenuItem *menu_Edit;
  TMenuItem *menu_Edit_Refresh;
  TMenuItem *N2;
  TMenuItem *menu_Edit_Preferences;
  TMenuItem *menu_Help;
  TMenuItem *menu_Help_Contents;
  TMenuItem *N3;
  TMenuItem *menu_Help_Overview;
  TMenuItem *menu_Help_Using;
  TMenuItem *menu_Help_Compression;
  TMenuItem *menu_Help_Improving;
  TMenuItem *N5;
  TMenuItem *menu_Help_Partitions;
  TMenuItem *N4;
  TMenuItem *menu_Help_About;
  TOpenDialog *OpenDialog;
  TSaveDialog *SaveDialog;
  TTimer *UpdateTimer;
  TMainMenu *MainMenu;
  TRadioButton *radioTargetNBD;
  TEdit *edtTargetNBD;
  TSpeedButton *btnTargetNBDConnect;

  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall radioSourceFileClick(TObject *Sender);
  void __fastcall cmbSourceDriveChange(TObject *Sender);
  void __fastcall radioProcessingClick(TObject *Sender);
  void __fastcall radioTargetFileClick(TObject *Sender);
  void __fastcall cmbTargetDriveChange(TObject *Sender);
  void __fastcall SetVisualDefaults(TObject *Sender);
  void __fastcall menu_File_ExitClick(TObject *Sender);
  void __fastcall menu_Edit_RefreshClick(TObject *Sender);
  void __fastcall menu_Edit_PreferencesClick(TObject *Sender);
  void __fastcall menu_Help_ItemsClick(TObject *Sender);
  void __fastcall menu_Help_AboutClick(TObject *Sender);
  void __fastcall btnOkClick(TObject *Sender);
  void __fastcall btnCancelClick(TObject *Sender);
  void __fastcall btnHelpClick(TObject *Sender);
  void __fastcall UpdateTimerTimer(TObject *Sender);
  void __fastcall btnSourceFileOpenClick(TObject *Sender);
  void __fastcall btnTargetFileOpenClick(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall radioSourceNBDClick(TObject *Sender);
  void __fastcall btnSourceNBDConnectClick(TObject *Sender);
  void __fastcall radioTargetNBDClick(TObject *Sender);
  void __fastcall btnTargetNBDConnectClick(TObject *Sender);
  void __fastcall radioTargetDriveClick(TObject *Sender);

public:		// User declarations
  __fastcall TMainForm(TComponent* Owner);
  __fastcall ~TMainForm();

private:	// User declarations
  __property AnsiString SourceFileName = {read = GetSourceFileName};
  __property AnsiString TargetFileName = {read = GetTargetFileName, write = SetTargetFileName};
  __property AnsiString SourceHost = {read = GetSourceHost};
  __property AnsiString TargetHost = {read = GetTargetHost};
  __property TImageStoreType SourceType = {read = GetSourceType};
  __property TImageStoreType TargetType = {read = GetTargetType};

  __int64 SourceSize;
  __int64 MaxTargetSize;
  TDriveList *DriveList;
  TImageStore *SourceImage, *TargetImage;
  TImageBuffer *InFileBuffer, *ProcessingBuffer;
  TSelfImageReadThread *ReadThread;
  TSelfImageWriteThread *WriteThread;
  TSelfImageThread **ProcessingThreads;
  TSelfImageThread **AllThreads;
  bool bEnableCompression;
  bool bEnableDecompression;
  TCompressionMethod CompressionMethod;
  unsigned int nProcessingThreadCount, nEmptyChunkCount;
  unsigned __int64 nnTotalBytes, nnBytesReadTotal, nnBytesSkippedTotal, nnBytesWrittenTotal;
  TMainFormVisualMode CurrentMode;
  bool bCancelOperation;

  void __fastcall RefreshDriveList(void);
  void __fastcall VisualMode(TMainFormVisualMode Mode);
  void __fastcall UpdateStatus(bool bInit = false);
  bool __fastcall SanityCheck(void);

  AnsiString __fastcall GetSourceFileName(void) { return edtSourceFileName->Text; };
  AnsiString __fastcall GetTargetFileName(void) { return edtTargetFileName->Text; };
  void __fastcall SetTargetFileName(AnsiString New) { edtTargetFileName->Text = New; SaveDialog->FileName = New; };
  AnsiString __fastcall GetSourceHost(void) { return edtSourceNBD->Text; };
  AnsiString __fastcall GetTargetHost(void) { return edtTargetNBD->Text; };
  TImageStoreType __fastcall GetSourceType(void) { if (radioSourceFile->Checked) return isFile; else if (radioSourceDrive->Checked) return isDrive; else if (radioSourceNBD->Checked) return isNBD; else return isUndefined; };
  TImageStoreType __fastcall GetTargetType(void) { if (radioTargetFile->Checked) return isFile; else if (radioTargetDrive->Checked) return isDrive; else if (radioTargetNBD->Checked) return isNBD; else return isUndefined; };

};
#pragma warn .8026
#pragma warn .8027
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif

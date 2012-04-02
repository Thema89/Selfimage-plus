//---------------------------------------------------------------------------

#ifndef SelfImage_NBDDialogH
#define SelfImage_NBDDialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include "SelfImage_Utility.h"
//---------------------------------------------------------------------------
#pragma warn -8026
#pragma warn -8027
class TformSelfImageConnect : public TForm
{
__published:  // IDE-managed Components
  TLabel *lblHost;
  TEdit *edtHost;
  TLabel *lblPort;
  TEdit *edtPort;
  TLabel *lblAlias;
  TComboBox *cmbAliases;
  TSpeedButton *btnAddAlias;
  TSpeedButton *btnRemoveAlias;
  TButton *btnOk;
  TButton *btnCancel;
  TSpeedButton *btnHelp;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall UpdateComponents(TObject *Sender);
  void __fastcall cmbAliasesChange(TObject *Sender);
  void __fastcall btnAddAliasClick(TObject *Sender);
  void __fastcall btnRemoveAliasClick(TObject *Sender);
  void __fastcall btnOkClick(TObject *Sender);
  void __fastcall btnHelpClick(TObject *Sender);

public:   // User declarations
  __property AnsiString HostName = {read = GetHostName, write = SetHostName};
  __property int Port = {read = GetPort, write = SetPort};
  __property AnsiString ConnectionString = {read = GetConnectionString, write = SetConnectionString};
  __property __int64 RemoteSize = {read = FRemoteSize};

  __fastcall TformSelfImageConnect(TComponent* Owner);
  __fastcall ~TformSelfImageConnect();
  int __fastcall Execute(AnsiString DefaultConnection);

private:  // User declarations
  TStringList *Aliases;
  AnsiString DefaultConnectionString;
  __int64 FRemoteSize;

  void SetupAliases(void);

  AnsiString __fastcall GetHostName(void) { return edtHost->Text; };
  void       __fastcall SetHostName(AnsiString sHost) { edtHost->Text = sHost; };
  int        __fastcall GetPort(void) { int nPort; if (TryStrToInt(edtPort->Text, nPort) && nPort > 0 && nPort < 65536) return nPort; else return 0; };
  void       __fastcall SetPort(int nPort) { if (nPort > 0 && nPort < 65536) edtPort->Text = String(nPort); };
  AnsiString __fastcall GetConnectionString(void) { if (!HostName.IsEmpty() && Port) return HostName + ":" + String(Port); else return ""; };
  void       __fastcall SetConnectionString(AnsiString sConnectionString) { int nPort; AnsiString sHost; if (SplitConnectionString(sConnectionString, sHost, nPort)) { HostName = sHost; Port = nPort; } };

};
#pragma warn .8026
#pragma warn .8027
//---------------------------------------------------------------------------
extern PACKAGE TformSelfImageConnect *formSelfImageConnect;
//---------------------------------------------------------------------------
#endif

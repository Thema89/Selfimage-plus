//------------------------------ $Keywords ----------------------------------
// TConfiguration.h - TConfiguration general purpose config class header
// Copyright ©2005, Kurt Fitzner <kfitzner@excelcia.org>
//---------------------------------------------------------------------------
// This file is part of the TConfiguration general purpose configuration
// class.
//
// The TConfiguration class is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License (Version 2)
// as published by the Free Software Foundation.
//
// The TConfiguration class is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
//---------------------------------------------------------------------------
#ifndef TConfigurationH
#define TConfigurationH
//---------------------------------------------------------------------------
#include <Registry.hpp>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TConfigurationEntry is a single entry in the configuration.
//
class TConfigurationEntry : public TObject
{
__published:
  __property Variant Value = {read=FValue, write=writeValue};

public:
  __fastcall TConfigurationEntry();
  __fastcall TConfigurationEntry(AnsiString Name, AnsiString RegistryKey, bool ReadOnly, Variant Value);
  __fastcall TConfigurationEntry(AnsiString ConfigEntry);
  bool __fastcall Refresh(void);
  AnsiString Name;

protected:
  void __fastcall ConstructEntry(AnsiString Name, AnsiString RegistryKey, bool ReadOnly, Variant DefaultValue);

private:
  Variant    FValue;
  HKEY       RootKey;
  AnsiString KeyPath;
  bool       ReadOnly;

  void __fastcall writeValue(Variant& Value);

}; // class TConfigurationEntry
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
class TConfiguration : public TObject
{
__published:
  __property TConfigurationEntry *Items[Variant index] = { read=getItems };
  __property Variant Values[Variant Index] = {read=getValues, write=setValues };
public:
  __fastcall TConfiguration(TStream *ConfigTableStream);
  __fastcall TConfiguration();
  __fastcall ~TConfiguration();

  void __fastcall LoadFromStream(TStream *ConfigTableStream);
  void __fastcall Clear(void);
private:
  TList *FItems;

  TConfigurationEntry * __fastcall getItems(Variant Index);
  Variant __fastcall getValues(Variant Index);
  void __fastcall setValues(Variant Index, Variant NewValue);

}; // class TConfiguration : public TRegistry
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Exception class thrown when an attempt is made to create an instance
// of TConfigurationEntry of a type not supported by the class.
//
class EUnsupportedType : public Exception {
public:
  __fastcall EUnsupportedType(const AnsiString Msg): Exception(Msg) {};
	__fastcall EUnsupportedType(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size): Exception(Msg, Args, Args_Size) {};
	__fastcall EUnsupportedType(int Ident): Exception(Ident) {};
	__fastcall EUnsupportedType(System::PResStringRec ResStringRec): Exception(ResStringRec) {};
	__fastcall EUnsupportedType(int Ident, const System::TVarRec * Args, const int Args_Size): Exception(Ident, Args, Args_Size) {};
	__fastcall EUnsupportedType(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size): Exception(ResStringRec, Args, Args_Size) {};
	__fastcall EUnsupportedType(const AnsiString Msg, int AHelpContext): Exception(Msg, AHelpContext) {};
	__fastcall EUnsupportedType(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(Msg, Args, Args_Size, AHelpContext) {};
	__fastcall EUnsupportedType(int Ident, int AHelpContext): Exception(Ident, AHelpContext) {};
	__fastcall EUnsupportedType(System::PResStringRec ResStringRec, int AHelpContext): Exception(ResStringRec, AHelpContext) {};
	__fastcall EUnsupportedType(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(ResStringRec, Args, Args_Size) {};
	__fastcall EUnsupportedType(int Ident, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(Ident, Args, Args_Size, AHelpContext) {};
}; // class EUnsupportedType
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Exception class thrown when an invalid registry key is specified to
// TConfigurationEntry
//
class EInvalidKey : public Exception {
public:
  __fastcall EInvalidKey(const AnsiString Msg): Exception(Msg) {};
	__fastcall EInvalidKey(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size): Exception(Msg, Args, Args_Size) {};
	__fastcall EInvalidKey(int Ident): Exception(Ident) {};
	__fastcall EInvalidKey(System::PResStringRec ResStringRec): Exception(ResStringRec) {};
	__fastcall EInvalidKey(int Ident, const System::TVarRec * Args, const int Args_Size): Exception(Ident, Args, Args_Size) {};
	__fastcall EInvalidKey(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size): Exception(ResStringRec, Args, Args_Size) {};
	__fastcall EInvalidKey(const AnsiString Msg, int AHelpContext): Exception(Msg, AHelpContext) {};
	__fastcall EInvalidKey(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(Msg, Args, Args_Size, AHelpContext) {};
	__fastcall EInvalidKey(int Ident, int AHelpContext): Exception(Ident, AHelpContext) {};
	__fastcall EInvalidKey(System::PResStringRec ResStringRec, int AHelpContext): Exception(ResStringRec, AHelpContext) {};
	__fastcall EInvalidKey(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(ResStringRec, Args, Args_Size) {};
	__fastcall EInvalidKey(int Ident, const System::TVarRec * Args, const int Args_Size, int AHelpContext): Exception(Ident, Args, Args_Size, AHelpContext) {};
}; // class EInvalidKey
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Utility functions
//---------------------------------------------------------------------------
AnsiString __fastcall VariantTypeName(Variant &Value);
bool __fastcall KeySplit(AnsiString sKey, HKEY& KeyRoot, AnsiString& sKeyRest);
AnsiString __fastcall RootKeyToString(HKEY RootKey);
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// #defines
//---------------------------------------------------------------------------
#define CONFIGFIELD_KEY      0
#define CONFIGFIELD_NAME     1
#define CONFIGFIELD_TYPE     2
#define CONFIGFIELD_READONLY 3
#define CONFIGFIELD_DEFAULT  4

//---------------------------------------------------------------------------
// Misc Shtuff™
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Class TBufferStream
//
// Borland's TMemoryStream hides the only really useful method that its
// base class offers, SetMemory.  Of course it hides it, because otherwise
// TMemoryStream would be far too useful.
//
class TBufferStream : public TMemoryStream {
  public:
    __fastcall inline      TBufferStream(void *Buffer, int Size) : TMemoryStream() { SetPointer(Buffer, Size); };
    void __fastcall inline SetBuffer(void *Buffer, int Size) { SetPointer(Buffer, Size); }
};  // class TBufferStream
//---------------------------------------------------------------------------

#endif

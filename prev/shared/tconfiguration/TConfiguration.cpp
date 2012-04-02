//------------------------------ $Keywords ----------------------------------
// TConfiguration.cpp - TConfiguration class general purpose config helper
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

//---------------------------------------------------------------------------
// VCS: $Version: 0 $ $Revision: 5 $
/*
$History: **** V 0.0 by kfitzner ****
$History: * tconfiguration.cpp - 2005-05-13 11:14:17 PM - 22104 Bytes
$History: * tconfiguration.h - 2005-05-07 1:21:22 PM - 8498 Bytes
$History: * Initial check-in
$History: **** V 0.1 by kfitzner ****
$History: * tconfiguration.cpp - 2005-07-31 2:07:43 AM - 22465 Bytes
$History: * tconfiguration.h - 2005-07-31 1:54:10 AM - 8564 Bytes
$History: * Alter header to reflect that it's a unit shared by 
$History: * several projects, not just GPGee.
$History: **** V 0.2 by kfitzner ****
$History: * tconfiguration.cpp - 2005-08-08 6:41:51 AM - 22647 Bytes
$History: * tconfiguration.h - 2005-08-08 6:41:38 AM - 8500 Bytes
$History: * License change - remove option for later versions of GPL
$History: **** V 0.3 by kfitzner ****
$History: * tconfiguration.cpp - 2005-11-12 11:39:41 AM - 23070 Bytes
$History: * tconfiguration.h - 2005-08-08 6:41:38 AM - 8500 Bytes
$History: * Add better error handling for cases where requested 
$History: * value isn't present
$History: **** V 0.4 by kfitzner ****
$History: * tconfiguration.cpp - 2005-11-30 1:40:18 PM - 23345 Bytes
$History: * tconfiguration.h - 2005-08-08 6:41:38 AM - 8500 Bytes
$History: * Add varSmallint as possible integer type to writeValue
$History: **** Latest ** V 0.5 by kfitzner ** 2005-12-15 12:25:24 PM ****
$History: * Log the names of what entries (Values) are accessed
*/
//----------------------------  $NoKeywords ---------------------------------


//---------------------------------------------------------------------------
// File Notes:
//---------------------------------------------------------------------------
// 1 Apr 2005 - Kurt Fitzner <kfitzner@excelcia.org>
//
// This is a generic class written to handle arbitrary registry configuration
// data.  TConfiguration reads a configuration table and creates one
// TConfigurationEntry object for each item in the table.  Accessing the data
// is similar to any TList-based class, with an Items property.  See the
// comments in TConfiguration.txt for information on the config table's
// structure.  This config table can be read from a stream, or it can be
// stored as a resource right in the application.  As this class was
// originally implemented (in GPGee), that's how it was used.
//
// TODO: Low priority
// At some point, if there gets to be a large number of configuration
// settings, items going into TConfiguration should be inserted into sorted
// order (by name), and a binary search algorithm used when searching by
// name.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TProgramLog.h"
#include "TConfiguration.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
#ifndef __ENTERFUNCTION__
  #define __ENTERFUNCTION__ ;
  #define __LEAVEFUNCTION__ ;
  #define __RETURNFUNCTION__ return;
  #define __RETURNFUNTION(r) return r;
#endif
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TConfigurationEntry class - this is a single entry in the configuration.
// The value is stored in a Variant.
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Basic constructor - all the others will call this one to set up the
// initial state of things
//
__fastcall TConfigurationEntry::TConfigurationEntry(): TObject()
{
  __ENTERFUNCTION__;
  FValue.Clear();
  RootKey = HKEY_CURRENT_USER;
  ReadOnly = true;              // Read only unless something says otherwise
  __RETURNFUNCTION__;
}  // __fastcall TConfigurationEntry::TConfigurationEntry()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructor that makes an entry given the name, registry key, and value.
// The type of the registry entry made will depend on the type of Value.
// Any unsupported types will throw an exception.
//
__fastcall TConfigurationEntry::TConfigurationEntry(AnsiString Name, AnsiString RegistryKey, bool ReadOnly, Variant DefaultValue): TObject()
{
  __ENTERFUNCTION__;
  ConstructEntry(Name, RegistryKey, ReadOnly, DefaultValue);
  __RETURNFUNCTION__;
}  // __fastcall TConfigurationEntry::TConfigurationEntry(AnsiString Name, AnsiString Key, Variant Value)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// This actually does most of the construction work.  It's a separate method
// so it can be used in multiple overloaded constructors.
//
void __fastcall TConfigurationEntry::ConstructEntry(AnsiString Name, AnsiString RegistryKey, bool ReadOnly, Variant DefaultValue)
{
  TRegistry *Registry = NULL;
  AnsiString sKeyRoot, sKeyRest;
  bool bKeyValid = false;

  __ENTERFUNCTION__;

  try {
    this->Name = Name;
    this->ReadOnly = ReadOnly;
    if (KeySplit(RegistryKey, this->RootKey, this->KeyPath)) {
      Registry = new TRegistry();
      Registry->LazyWrite = false;
      Registry->RootKey = RootKey;
      bKeyValid = Registry->OpenKey(KeyPath, !ReadOnly);
    }  // if (KeySplit(RegistryKey, RootKey, KeyPath))
    if (!bKeyValid && !ReadOnly)
      throw EInvalidKey("The registry key specified as the location of configuration data for " + Application->Name + " was invalid.  The key was \"" + RegistryKey + "\" for configuration entry \"" + Name + "\".");

    FValue = DefaultValue;              // Need to set this here so the variant's type is correct before the call to Refresh()
    Registry->CloseKey();               // Close the key here so that when we call Refresh() we don't have the same key open twice
    if (!Refresh() && !ReadOnly) {
      Registry->OpenKey(KeyPath, false);
      Registry->DeleteValue(Name);
      Registry->CloseKey();
    }  // if (!Refresh() && !ReadOnly)
    if (!ReadOnly && FValue == DefaultValue)
      writeValue(DefaultValue);
  }  // try
  __finally {
    if (Registry)
      delete Registry;
    __RETURNFUNCTION__;
  }  // __finally
}  // void __fastcall TConfigurationEntry::Contruct(AnsiString Name, AnsiString RegistryKey, bool ReadOnly, Variant DefaultValue)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructor that takes a single string with a line from the TConfiguration
// config "file".
//
__fastcall TConfigurationEntry::TConfigurationEntry(AnsiString ConfigEntry): TObject()
{
  TStringList *Fields = NULL;
  AnsiString sType;
  Variant Value;

  __ENTERFUNCTION__;

  try {
    Fields = new TStringList();
    ExtractStrings(TSysCharSet() << ',' << ' ' << '\t', TSysCharSet() << ' ' << '\t', ConfigEntry.c_str(), Fields);
    for (int n=0; n < Fields->Count; n++) {
      char *Field = Fields->Strings[n].c_str();
      if (*Field == '"')
        Fields->Strings[n] = AnsiExtractQuotedStr(Field, '"');
    }  // for (int n=0; n < Fields->Count; n++)
    sType = Fields->Strings[CONFIGFIELD_TYPE];
    if (sType.AnsiCompareIC("Integer") == 0) {
      if (Fields->Count > CONFIGFIELD_READONLY && !Fields->Strings[CONFIGFIELD_DEFAULT].IsEmpty())
        Value = StrToInt(Fields->Strings[CONFIGFIELD_DEFAULT]);
      else
        Value = 0;
    } else if (sType.AnsiCompareIC("Boolean") == 0) {
      if (Fields->Count > CONFIGFIELD_READONLY && !Fields->Strings[CONFIGFIELD_DEFAULT].IsEmpty())
        Value = StrToBool(Fields->Strings[CONFIGFIELD_DEFAULT]);
      else
        Value = false;
    } else if (sType.AnsiCompareIC("String") == 0) {
      if (Fields->Count > CONFIGFIELD_READONLY+1 && !Fields->Strings[CONFIGFIELD_DEFAULT].IsEmpty())
        Value = Fields->Strings[CONFIGFIELD_DEFAULT];
      else
        Value = String("");
    }  // if ... check type
    this->ConstructEntry(Fields->Strings[CONFIGFIELD_NAME], Fields->Strings[CONFIGFIELD_KEY], StrToBool(Fields->Strings[CONFIGFIELD_READONLY]), Value);
  }  // try
  __finally {
    delete Fields;
    __RETURNFUNCTION__;
  }  // __finally
}  // __fastcall TConfigurationEntry::TConfigurationEntry(AnsiString ConfigEntry)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Refresh the value of the config entry by reading it from the registry.
// Only necesary when the value is being first read or if it is a read-only
// value that is volatile.  Non-read-only values should only ever be
// changed by the application.
//
bool __fastcall TConfigurationEntry::Refresh(void)
{
  bool retval = true;
  TRegistry *Registry = NULL;
  AnsiString t;

  __ENTERFUNCTION__;

  try {
    Registry = new TRegistry();
    Registry->RootKey = RootKey;
    Registry->OpenKeyReadOnly(KeyPath);
    if (Registry->ValueExists(Name)) {
      switch (FValue.Type()) {
        case varInteger:
          try { FValue = Registry->ReadInteger(Name); }
          catch (Exception &e) { retval = false; }
          break;
        case varBoolean:
          try { FValue = Registry->ReadBool(Name); }
          catch (Exception &e) { retval = false; }
          break;
        case varString:
          try { FValue = Registry->ReadString(Name); }
          catch (Exception &e) { retval = false; }
          break;
        default:
          throw EUnsupportedType("An attempt has been made to create a configuration entry object with a type (0x" + IntToHex(FValue.Type(), 4) + " - " + VariantTypeName(FValue) + ") that is unsupported by the class.  This is a programming error and should be reported to " + Application->Name + "'s maintainers.");
      }  // switch (Value.Type())
    }  // if (Registry->ValueExists(Name)
    Registry->CloseKey();
  }  // try
  __finally {
    if (Registry)
      delete Registry;
    __RETURNFUNCTION(retval);
  }  // __finally
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Property write method for the Value property.  After the internal copy is
// changed, write it to the registry.
//
void __fastcall TConfigurationEntry::writeValue(Variant& Value)
{
  TRegistry *Registry = NULL;

  __ENTERFUNCTION__;

  try {
    if (ReadOnly)
      throw EPropReadOnly("The application has attempted to write to the read-only configuration setting \"" + Name + "\".  This is a programming error and should be reported to " + Application->Name + "'s maintainers.");
    Registry = new TRegistry();
    Registry->RootKey = RootKey;
    if (!Registry->OpenKey(KeyPath, true))
      throw EInvalidKey("An error was encountered while attempting to write to the registry at \"" + RootKeyToString(RootKey) + "\\" + KeyPath + "\\" + Name + "\"");
    switch (Value.Type()) {
        case varInteger:
        case varSmallint:
          Registry->WriteInteger(Name, Value);
          break;
        case varBoolean:
          Registry->WriteBool(Name, Value);
          break;
        case varString:
          Registry->WriteString(Name, Value);
          break;
    }  // switch (Value.Type())
    FValue = Value;
    Registry->CloseKey();
  }  // try
  __finally {
    if (Registry)
      delete Registry;
    __RETURNFUNCTION__;
  }  // __finally
}  // void __fastcall TConfigurationEntry::writeValue(Variant& Value)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TConfiguration methods
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Default constructor for TConfiguration - try to find and load a resource
// with the configuration file.  Looks for a resource called "CONFIGTABLE".
//
__fastcall TConfiguration::TConfiguration(): TObject()
{
  TBufferStream *ConfigTableStream = NULL;
  HRSRC TableResource;
  HGLOBAL TableGlobal;
  char *ConfigTable;
  unsigned TableSize;

  __ENTERFUNCTION__;

  try {
    FItems = new TList();
    TableResource = FindResource(HInstance, "CONFIGTABLE", RT_RCDATA);
    if (!TableResource)
      throw EResNotFound("There was an error while attempting to locate the configuration table resource.  This is probably a programming error and should be reported to " + Application->Name + "'s maintainers.");
    TableSize = SizeofResource(HInstance, TableResource);
    TableGlobal = LoadResource(HInstance, TableResource);
    ConfigTable = (char *)LockResource(TableGlobal);
    ConfigTableStream = new TBufferStream(ConfigTable, TableSize);
    ConfigTableStream->Seek(0, soFromBeginning);
    LoadFromStream(ConfigTableStream);
  }  // try
  __finally {
    if (ConfigTableStream)
      delete ConfigTableStream;
    __RETURNFUNCTION__;
  }  // __finally
}  // __fastcall TConfiguration::TConfiguration()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructor for TConfiguration - load in the config table from a stream
//
__fastcall TConfiguration::TConfiguration(TStream *ConfigTableStream): TObject()
{
  __ENTERFUNCTION__;
  FItems = new TList();
  LoadFromStream(ConfigTableStream);
  __RETURNFUNCTION__;
}  // __fastcall TConfiguration::TConfiguration(TStream *ConfigFileStream)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TConfiguration destructor
//
__fastcall TConfiguration::~TConfiguration()
{
  __ENTERFUNCTION__;
  Clear();
  delete FItems;
  __RETURNFUNCTION__;
}  // __fastcall TConfiguration::~TConfiguration()
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Load the configuration table from a stream
//
void __fastcall TConfiguration::LoadFromStream(TStream *ConfigTableStream)
{
  TStringList *ConfigTable = NULL;
  TConfigurationEntry *Entry;

  __ENTERFUNCTION__;

  Clear();
  try {
    ConfigTable = new TStringList();
    ConfigTable->LoadFromStream(ConfigTableStream);
    for (int n = 0; n < ConfigTable->Count; n++)
      if (ConfigTable->Strings[n].Pos("HKEY_") == 1 || ConfigTable->Strings[n].Pos("\"HKEY_")) {
        Entry = new TConfigurationEntry(ConfigTable->Strings[n]);
        FItems->Add(Entry);
      }  // if (ConfigFile->Strings[n].Pos("HKEY_") == 1)
    FItems->Capacity = FItems->Count;
  }  // try
  __finally {
    if (ConfigTable)
      delete ConfigTable;
    __RETURNFUNCTION__;
  }  // __finally
}  // void __fastcall TConfiguration::LoadFromStream(TStream *ConfigTableStream)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Clear the items list, freeing all TConfigurationEntry objects found
//
void __fastcall TConfiguration::Clear(void)
{
  __ENTERFUNCTION__;
  for (int n = 0; n < FItems->Count; n++)
    delete (TConfigurationEntry *)FItems->Items[n];
  FItems->Clear();
  __RETURNFUNCTION__;
}  // void __fastcall TConfiguration::Clear(void)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Retrieval method for the Items property.  Can return an entry based on
// its numeric index, or on its name.
//
TConfigurationEntry * __fastcall TConfiguration::getItems(Variant Index)
{
  TConfigurationEntry *retval = NULL;

  __ENTERFUNCTION__;

  switch (Index.Type()) {
    case varSmallint:
    case varInteger:
    case varShortInt:
    case varByte:
    case varWord:
    case varLongWord:
    case varInt64:
      retval = (TConfigurationEntry *)FItems->Items[Index];
      break;
    case varString: {
      AnsiString sIndex = Index;
      for (int n = 0; n < FItems->Count; n++)
        if ( ((TConfigurationEntry *)FItems->Items[n])->Name.AnsiCompareIC(sIndex) == 0)
          retval = (TConfigurationEntry *)FItems->Items[n];
      break;
    }  // case varString:
    default:
      retval = NULL;
  }  // switch (Index.Type())
  __RETURNFUNCTION(retval);
}  // TConfigurationEntry * __fastcall getItems(int Index)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Retrieval method for the Values property.  Uses getItems() to search, so
// it can return based on the integer index or name.
//
Variant __fastcall TConfiguration::getValues(Variant Index)
{
  Variant retval;
  TConfigurationEntry *Entry = getItems(Index);

  retval.ChangeType(varEmpty);
  if (Entry) {
    retval = Entry->Value;
    LOG(LOG_DEBUG, "Found configuration entry \"" + String(Index) + "\" with value \"" + String(retval) + "\".");
  }  // if (Entry)
  #ifdef _DEBUG
  else
    throw EVariantBadIndexError("TConfiguration error: Values index \"" + Index + "\" doesn't exist.");
  #endif
  __RETURNFUNCTION(retval);
}  // Variant __fastcall TConfiguration::getValues(AnsiString Name)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Storage method for the Values property.
//
void __fastcall TConfiguration::setValues(Variant Index, Variant NewValue)
{
  TConfigurationEntry *Entry;

  __ENTERFUNCTION__;
  Entry = getItems(Index);
  if (Entry)
    Entry->Value = NewValue;
  __RETURNFUNCTION__;
}  // void __fastcall TConfiguration::setValues(Variant Index, Variant NewValue)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Utility functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Return a string with a textual representation of a Variant's type.
//
AnsiString __fastcall VariantTypeName(Variant& Value)
{
  AnsiString retval;
  int type;

  __ENTERFUNCTION__;

  type = Value.Type();

  switch (type & varTypeMask) {
    case varEmpty:      retval = "empty"; break;
    case varNull:       retval = "null"; break;
    case varSmallint:   retval = "small integer"; break;
    case varInteger:    retval = "integer"; break;
    case varSingle:     retval = "single"; break;
    case varDouble:     retval = "double"; break;
    case varCurrency:   retval = "currency"; break;
    case varDate:       retval = "date/time"; break;
    case varOleStr:     retval = "unicode (OLE) string"; break;
    case varDispatch:   retval = "OLE automation object reference"; break;
    case varError:      retval = "OS error code"; break;
    case varBoolean:    retval = "boolean"; break;
    case varVariant:    retval = "variant"; break;
    case varUnknown:    retval = "unknown OLE object"; break;
    case varByte:       retval = "unsigned char"; break;
    case varString:     retval = "string"; break;
    default:            retval = "unknown"; break;
  }  // switch (Value.Type())
  if (type & varByRef)
    retval += " reference";
  if (type & varArray) {
    retval = "array of " + retval;
    if (type & varByRef)
      retval += "s";
  }  // if (type & varArray)
  __RETURNFUNCTION(retval);
}  // __fastcall AnsiString VariantTypeName(Variant &Value)


//---------------------------------------------------------------------------
// Split a registry key into two - the root key and the rest of the key's
// path.  Returns true on success, false on failure.
//
bool __fastcall KeySplit(AnsiString sKey, HKEY& KeyRoot, AnsiString& sKeyRest)
{
  int pos;
  AnsiString sRoot, sRest;
  HKEY Root = NULL;
  bool retval;

  __ENTERFUNCTION__;
  
  if ( (pos = sKey.Pos("\\")) != 0) {
    sRoot = sKey.SubString(1, pos-1);
    sRest = sKey.SubString(pos+1, sKey.Length()-pos);
    if (sRoot == "HKEY_CLASSES_ROOT")
      Root = HKEY_CLASSES_ROOT;
    else if (sRoot == "HKEY_CURRENT_USER")
      Root = HKEY_CURRENT_USER;
    else if (sRoot == "HKEY_LOCAL_MACHINE")
      Root = HKEY_LOCAL_MACHINE;
    else if (sRoot == "HKEY_USERS")
      Root = HKEY_USERS;
    else if (sRoot == "HKEY_CURRENT_CONFIG")
      Root = HKEY_CURRENT_CONFIG;
  }  // if ( (pos = sKey.Pos("\\")) != 0)
  if (Root && !sRest.IsEmpty()) {
    KeyRoot = Root;
    sKeyRest = sRest;
    retval = true;
  } else
        retval = false;
  __RETURNFUNCTION(retval);
}  // bool __fastcall KeySplit(AnsiString sKey, AnsiString& sKeyRoot, AnsiString& sKeyRest)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Return a string representation of a root registry key
//
AnsiString __fastcall RootKeyToString(HKEY RootKey)
{
  AnsiString retval;

  __ENTERFUNCTION__;

  switch ((int)RootKey) {
    case HKEY_CLASSES_ROOT:   retval = "HKEY_CLASSES_ROOT";    break;
    case HKEY_CURRENT_USER:   retval = "HKEY_CURRENT_USER";    break;
    case HKEY_LOCAL_MACHINE:  retval = "HKEY_LOCAL_MACHINE";   break;
    case HKEY_USERS:          retval = "HKEY_USERS";           break;
    case HKEY_CURRENT_CONFIG: retval = "HKEY_CURRENT_CONFIG";  break;
    default:                  retval = "UNKNOWN_ROOT_KEY";     break;
  }  // switch (RootKey)
  __RETURNFUNCTION(retval);
}  // AnsiString __fastcall RootKeyToString(HKEY RootKey)
//---------------------------------------------------------------------------


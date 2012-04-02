//------------------------------ $Keywords ----------------------------------
// TProgramLog.cpp - TProgramLog general purpose debug logger class header
// Copyright ©2003-2005, Kurt Fitzner <kfitzner@excelcia.org>
//---------------------------------------------------------------------------
// This file is part of the TProgramLog general purpose debug logging
// class.
//
// The TProgramLog class is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License (Version 2)
// as published by the Free Software Foundation.
//
// The TProgramLog class is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
//---------------------------------------------------------------------------
#ifndef TProgramLogH
#define TProgramLogH
//---------------------------------------------------------------------------
typedef enum {
  LOG_STRUCTURE = 0,
  LOG_DEBUG     = 1,
  LOG_MESSAGE   = 2,
  LOG_WARNING   = 3,
  LOG_ERROR     = 4,
  LOG_NONE      = 99
} TLogLevels;
//---------------------------------------------------------------------------
class TProgramLog {
  public:
    __fastcall  TProgramLog(AnsiString FileName, bool Synchronous = true, bool OverWrite = true, TLogLevels LogLevel = LOG_WARNING);
    __fastcall ~TProgramLog();
    void inline LogEntry(TLogLevels LogLevel, AnsiString File, AnsiString Line, AnsiString Message, ...) {
      #if defined(_DEBUG) || defined(ENABLE_LOGGING)
      va_list ArgList; va_start(ArgList, Message);
      _LogEntry(LogLevel, File, Line, Message, ArgList);
      va_end(ArgList);
      #endif
    }  // void LogEntry(TLogLevels LogLevel, AnsiString File, AnsiString Line, AnsiString Message, ...)
    void inline PushFunction(char *FunctionName) {
      #if defined(_DEBUG) || defined(ENABLE_LOGGING)
      AnsiString *func = new AnsiString(FunctionName);
      FunctionStack->Push(func);
      #endif
    }  // void inline PushFunction(char *Filename)
    void inline PopFunction(void) {
      #if defined(_DEBUG) || defined(ENABLE_LOGGING)
      if (FunctionStack->AtLeast(1))
      delete (AnsiString *)FunctionStack->Pop();
      #endif
    }  // void inline PopFunction(void)

  protected:
    AnsiString   FileName;
    bool         Synchronous;
    TLogLevels   LogLevel;
    TStack      *FunctionStack;

    void __fastcall _LogEntry(TLogLevels LogLevel, AnsiString File, AnsiString Line, AnsiString Message, va_list ArgList);

  private:
    int          FileHandle;
};  // class TProgramLog
//---------------------------------------------------------------------------
extern TProgramLog *Log;
//---------------------------------------------------------------------------
#if defined(_DEBUG) || defined(ENABLE_LOGGING)

  #define __LOGGINGENABLED__

  #define NEWLOG(F,S,O,L)   new TProgramLog(F, S, O, L);
  #define DELETELOG(L)      delete L;
  #define __ENTERPROGRAM__  Log->PushFunction(__FUNC__); \
                            Log->LogEntry(LOG_MESSAGE, __FILE__, __LINE__, "-> Log initialized in %s()", __FUNC__);
  #define __ENTERFUNCTION__ Log->PushFunction(__FUNC__); \
                            Log->LogEntry(LOG_STRUCTURE, __FILE__, __LINE__, "-> Entering %s()", __FUNC__);
  #define __LEAVEFUNCTION__ Log->PopFunction(); \
                            Log->LogEntry(LOG_STRUCTURE, __FILE__, __LINE__, "<- Leaving  %s()", __FUNC__);
  #define __RETURNFUNCTION__ { Log->PopFunction(); \
                            Log->LogEntry(LOG_STRUCTURE, __FILE__, __LINE__, "<- Leaving  %s()", __FUNC__); \
                            return; }
  #define __RETURNFUNCTION(r) { Log->PopFunction(); \
                            Log->LogEntry(LOG_STRUCTURE, __FILE__, __LINE__, "<- Leaving  %s()", __FUNC__); \
                            return r; }
  #define __LEAVEPROGRAM__  Log->PopFunction(); \
                            Log->LogEntry(LOG_MESSAGE, __FILE__, __LINE__, "<- Terminating log in %s()", __FUNC__);

  #define LOG(L,S)        Log->LogEntry(L, __FILE__, __LINE__, S);
  #define LOG1(L,S,A1)    Log->LogEntry(L, __FILE__, __LINE__, S, A1);
  #define LOG2(L,S,A1,A2) Log->LogEntry(L, __FILE__, __LINE__, S, A1, A2);

#else

  #define __LOGGINGDISABLED__

  #define __ENTERPROGRAM__ ;
  #define __ENTERFUNCTION__ ;
  #define __LEAVEFUNCTION__ ;
  #define __RETURNFUNCTION__ return;
  #define __RETURNFUNCTION(r) return r;
  #define __LEAVEPROGRAM__ ;
  #define LOG(L,S)        ;
  #define LOG1(L,S,A1)    ;
  #define LOG2(L,S,A1,A2) ;
  #define NEWLOG(F,S,O,L) NULL
  #define DELETELOG(L) ;
#endif

#endif


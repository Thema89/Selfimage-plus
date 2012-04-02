;------------------------------ $Keywords ----------------------------------
; SelfImage - Windows disk image backup
; SelfImage.nsi - Nullsoft Installer script
; Copyright ©2005, Kurt Fitzner <kfitzner@excelcia.org>
;---------------------------------------------------------------------------
; This file is part of SelfImage.
;
; SelfImage is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License (Version 2) as
; published by the Free Software Foundation.
;
; SelfImage is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program; if not, write to the Free Software
; Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
;---------------------------------------------------------------------------
; VCS: $Version: 1 $ $Revision: 7 $
/*
$History: **** V 0.1 by kfitzner ****
$History: * selfimage.nsi - 2005-11-07 3:15:32 AM - 3661 Bytes
$History: * Initial check-in
$History: **** V 0.2 by kfitzner ****
$History: * selfimage.nsi - 2005-11-07 3:19:39 AM - 5042 Bytes
$History: * Add license and history keywords
$History: **** V 0.3 by kfitzner ****
$History: * selfimage.nsi - 2005-11-15 1:06:34 PM - 5323 Bytes
$History: * Add fonts to script
$History: **** V 0.4 by kfitzner ****
$History: * selfimage.nsi - 2005-11-15 1:32:33 PM - 6483 Bytes
$History: * Add help file and option to turn on auto-updates
$History: **** V 0.5 by kfitzner ****
$History: * selfimage.nsi - 2005-11-15 5:28:56 PM - 6553 Bytes
$History: * Change icon
$History: **** V 0.6 by kfitzner ****
$History: * selfimage.nsi - 2005-11-20 8:20:32 PM - 6688 Bytes
$History: * Update for 0.2.0 release
$History: **** V 0.7 by kfitzner ****
$History: * selfimage.nsi - 2005-11-28 3:47:47 PM - 6819 Bytes
$History: * Update for 0.3
$History: **** V 1.0 by kfitzner ****
$History: * selfimage.nsi - 2005-11-30 7:21:17 AM - 6948 Bytes
$History: * Update for 1.0
$History: **** V 1.1 by kfitzner ****
$History: * selfimage.nsi - 2005-12-02 12:12:13 PM - 7084 Bytes
$History: * Update for 1.0.1
$History: **** V 1.2 by kfitzner ****
$History: * selfimage.nsi - 2005-12-09 4:40:22 PM - 7213 Bytes
$History: * Update for 1.1
$History: **** V 1.3 by kfitzner ****
$History: * selfimage.nsi - 2005-12-27 6:01:12 PM - 7348 Bytes
$History: * Update for 1.1.1
$History: **** V 1.4 by kfitzner ****
$History: * selfimage.nsi - 2007-02-07 9:04:59 PM - 7481 Bytes
$History: * Update for 1.1.2
$History: **** V 1.5 by kfitzner ****
$History: * selfimage.nsi - 2007-02-12 10:42:38 AM - 7615 Bytes
$History: * Update for 1.1.3
$History: **** V 1.6 by kfitzner ****
$History: * selfimage.nsi - 2007-04-01 12:05:08 PM - 7745 Bytes
$History: * Update for 1.2
$History: **** Latest ** V 1.7 by kfitzner ** 2007-04-01 2:29:19 PM ****
$History: * Update for 1.2.1, alter uninstall to remove extra 
$History: * help file contents/index.
*/
;----------------------------  $NoKeywords ---------------------------------
; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "SelfImage"
!define PRODUCT_VERSION "1.2.1"
!define PRODUCT_PUBLISHER "Kurt Fitzner"
!define PRODUCT_WEB_SITE "http://selfimage.excelcia.org"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\SelfImage.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_KEY "Software\SelfImage"
!define PRODUCT_ROOT_KEY "HKCU"

SetCompressor lzma

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "..\images\SelfImage.ico"
!define MUI_UNICON "..\images\SelfImageUn.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "gpl.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Automatic updates page
Page custom AutoUpdatesPage
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\SelfImage.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; Reserve files
ReserveFile "SelfImageAutoUpdates.ini"
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

; Variables
Var AUTO_UPDATES

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "SelfImageSetup-${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES\SelfImage"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "..\SelfImage.exe"
  File "readme.txt"
  File "..\help\SelfImage.hlp"
  SetOutPath "$FONTS"
  File "01 DigiGraphics.ttf"
  File "VeraMoBd.ttf"
  File "VeraMoBI.ttf"
  File "VeraMoIt.ttf"
  File "VeraMono.ttf"
  CreateDirectory "$SMPROGRAMS\SelfImage"
  CreateShortCut "$SMPROGRAMS\SelfImage\SelfImage.lnk" "$INSTDIR\SelfImage.exe"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\SelfImage\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\SelfImage\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\SelfImage.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\SelfImage.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  !insertmacro MUI_INSTALLOPTIONS_READ $AUTO_UPDATES "SelfImageAutoUpdates.ini" "Field 2" "State"
  StrCmp $AUTO_UPDATES "1" "" +3
    WriteRegDWORD ${PRODUCT_ROOT_KEY} ${PRODUCT_KEY} "Update Notify" 1
    Goto +2
  WriteRegDWORD ${PRODUCT_ROOT_KEY} ${PRODUCT_KEY} "Update Notify" 0
SectionEnd

Function .onInit
  ;Extract InstallOptions INI files
  !insertmacro MUI_INSTALLOPTIONS_EXTRACT "SelfImageAutoUpdates.ini"
FunctionEnd

LangString TEXT_IO_TITLE ${LANG_ENGLISH} "Automatic Updates"
LangString TEXT_IO_SUBTITLE ${LANG_ENGLISH} "Choose your automatic updates setting:"

Function AutoUpdatesPage
  !insertmacro MUI_HEADER_TEXT "$(TEXT_IO_TITLE)" "$(TEXT_IO_SUBTITLE)"
  !insertmacro MUI_INSTALLOPTIONS_DISPLAY "SelfImageAutoUpdates.ini"
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\readme.txt"
  Delete "$INSTDIR\SELFIMAGE.HLP"
  Delete "$INSTDIR\SelfImage.GID"
  Delete "$INSTDIR\SelfImage.exe"

  Delete "$SMPROGRAMS\SelfImage\Uninstall.lnk"
  Delete "$SMPROGRAMS\SelfImage\Website.lnk"
  Delete "$SMPROGRAMS\SelfImage\SelfImage.lnk"

  RMDir "$SMPROGRAMS\SelfImage"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd

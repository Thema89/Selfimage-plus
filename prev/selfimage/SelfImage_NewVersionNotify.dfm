object formSelfImageNewVersionNotify: TformSelfImageNewVersionNotify
  Left = 287
  Top = 77
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'New Version Available'
  ClientHeight = 191
  ClientWidth = 314
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lblMessage: TLabel
    Left = 4
    Top = 8
    Width = 299
    Height = 40
    Caption = 
      'A new version of SelfImage has been released and is now availabl' +
      'e at:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    WordWrap = True
  end
  object lblMyVersion: TLabel
    Left = 24
    Top = 88
    Width = 74
    Height = 13
    Caption = 'Current version:'
  end
  object lblMyVersionValue: TLabel
    Left = 104
    Top = 88
    Width = 77
    Height = 13
    Caption = '1.1.1 (Build 666)'
  end
  object lblCurrentVersion: TLabel
    Left = 24
    Top = 104
    Width = 62
    Height = 13
    Caption = 'New version:'
  end
  object lblCurrentVersionValue: TLabel
    Left = 104
    Top = 104
    Width = 77
    Height = 13
    Caption = '2.2.2 (Build 777)'
  end
  object lblURL: TLabel
    Left = 4
    Top = 52
    Width = 301
    Height = 20
    Cursor = crHandPoint
    Alignment = taCenter
    AutoSize = False
    Caption = 'http://selfimage.excelcia.org'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold, fsUnderline]
    ParentFont = False
    OnClick = lblURLClick
  end
  object chbSquelch: TCheckBox
    Left = 4
    Top = 128
    Width = 217
    Height = 17
    Caption = 'Don'#39't notify again until next new version'
    TabOrder = 0
  end
  object btnOk: TButton
    Left = 120
    Top = 160
    Width = 75
    Height = 25
    Caption = '&Ok'
    TabOrder = 1
    OnClick = btnOkClick
  end
end

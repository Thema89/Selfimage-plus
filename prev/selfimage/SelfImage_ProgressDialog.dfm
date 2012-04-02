object ProgressDialog: TProgressDialog
  Left = 321
  Top = 100
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'One moment...'
  ClientHeight = 68
  ClientWidth = 330
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object lblCaption: TLabel
    Left = 8
    Top = 48
    Width = 313
    Height = 13
    Alignment = taCenter
    AutoSize = False
  end
  object Panel: TPanel
    Left = 8
    Top = 8
    Width = 313
    Height = 33
    BevelOuter = bvLowered
    TabOrder = 0
    object ProgressBar: TCGauge
      Left = 2
      Top = 2
      Width = 309
      Height = 29
      ShowText = False
      BorderStyle = bsNone
      ForeColor = clBtnHighlight
      BackColor = clBtnFace
    end
    object lblPercent: TLabel
      Left = 0
      Top = 8
      Width = 313
      Height = 19
      Alignment = taCenter
      AutoSize = False
      Caption = '0%'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Bitstream Vera Sans Mono'
      Font.Style = []
      ParentFont = False
      Transparent = True
    end
  end
end

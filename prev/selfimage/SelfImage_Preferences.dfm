object formSelfImagePreferences: TformSelfImagePreferences
  Left = 275
  Top = 95
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Preferences'
  ClientHeight = 201
  ClientWidth = 264
  Color = clBtnFace
  Constraints.MinHeight = 212
  Constraints.MinWidth = 272
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
  object btnHelp: TSpeedButton
    Left = 236
    Top = 172
    Width = 25
    Height = 25
    HelpContext = 9
    Glyph.Data = {
      DE010000424DDE01000000000000760000002800000024000000120000000100
      0400000000006801000000000000000000001000000010000000000000000000
      80000080000000808000800000008000800080800000C0C0C000808080000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00666666666666
      66666666666666666666666600006666666600666666666666666688F6666666
      000066666600850666666666666688668F666666000066660087F75066666666
      6688666668F666660000660087FF877506666666886666F6668F666600006087
      FF88087750666668F666888F6668F6660000658F8800508775066668F6886668
      F6668F66000065880055550877506668886666668F6668F60000650055555550
      87750668F666866668F6668F00006585555B3555087006668F666886668F6886
      0000665855557BB55080666668F666688668F8F600006665855335BB55006666
      668F6886886688F6000066665855BBB3555066666668F688886668F600006666
      658555555500666666668F6666668866000066666658555500666666666668F6
      668866660000666666658500666666666666668F886666660000666666665066
      6666666666666668666666660000666666666666666666666666666666666666
      0000}
    NumGlyphs = 2
    OnClick = btnHelpClick
  end
  object btnOk: TButton
    Left = 48
    Top = 172
    Width = 75
    Height = 25
    Caption = 'Ok'
    Default = True
    TabOrder = 0
    OnClick = btnOkClick
  end
  object btnCancel: TButton
    Left = 136
    Top = 172
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object Pages: TPageControl
    Left = 4
    Top = 4
    Width = 256
    Height = 161
    ActivePage = tabGeneral
    TabIndex = 0
    TabOrder = 2
    object tabGeneral: TTabSheet
      Caption = 'General'
      object gbGeneralOptions: TGroupBox
        Left = 4
        Top = 0
        Width = 240
        Height = 129
        Caption = 'General Options'
        TabOrder = 0
        object chbVersionNotify: TCheckBox
          Left = 8
          Top = 24
          Width = 193
          Height = 17
          Caption = 'Notify when new version is released'
          TabOrder = 0
        end
        object chbWindowPositions: TCheckBox
          Left = 8
          Top = 48
          Width = 129
          Height = 17
          Caption = 'Save window positions'
          TabOrder = 1
        end
        object chbWriteWarning: TCheckBox
          Left = 8
          Top = 72
          Width = 209
          Height = 17
          Caption = 'Do not warn when writing to a partition'
          TabOrder = 2
        end
      end
    end
    object tabRead: TTabSheet
      Caption = 'Read'
      ImageIndex = 1
      object gbReadOptions: TGroupBox
        Left = 4
        Top = 0
        Width = 240
        Height = 129
        Caption = 'Read Options'
        TabOrder = 0
        object chbSkipUnallocated: TCheckBox
          Left = 8
          Top = 24
          Width = 217
          Height = 17
          Caption = 'Automatically skip a partition'#39's free space'
          TabOrder = 0
        end
      end
    end
    object tabNBD: TTabSheet
      Caption = 'NBD'
      ImageIndex = 3
      object gbNBD: TGroupBox
        Left = 4
        Top = 0
        Width = 240
        Height = 129
        Caption = 'Network Block Device Options'
        TabOrder = 0
        object lblTimeout: TLabel
          Left = 56
          Top = 28
          Width = 114
          Height = 13
          Caption = 'Timeout (in milliseconds)'
        end
        object lblTransaction: TLabel
          Left = 56
          Top = 52
          Width = 138
          Height = 13
          Caption = 'Transaction size (in kilobytes)'
        end
        object edtTimeout: TEdit
          Left = 8
          Top = 24
          Width = 46
          Height = 21
          BevelInner = bvLowered
          BevelKind = bkTile
          BevelOuter = bvRaised
          BorderStyle = bsNone
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Bitstream Vera Sans Mono'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = '50000'
        end
        object edtTransaction: TEdit
          Left = 8
          Top = 48
          Width = 46
          Height = 21
          BevelInner = bvLowered
          BevelKind = bkTile
          BevelOuter = bvRaised
          BorderStyle = bsNone
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Bitstream Vera Sans Mono'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          Text = '1024'
        end
      end
    end
    object tabCompression: TTabSheet
      Caption = 'Compression'
      ImageIndex = 2
      object gbCompression: TGroupBox
        Left = 4
        Top = 0
        Width = 240
        Height = 129
        Caption = 'Compression'
        TabOrder = 0
        object lblCompressionThreads: TLabel
          Left = 64
          Top = 28
          Width = 153
          Height = 13
          Caption = '# of processing threads (0=auto)'
        end
        object lblGzipFast: TLabel
          Left = 64
          Top = 52
          Width = 132
          Height = 13
          Caption = 'gzip (fast) compression level'
        end
        object lblGzipBest: TLabel
          Left = 64
          Top = 76
          Width = 135
          Height = 13
          Caption = 'gzip (best) compression level'
        end
        object lblGzipChunk: TLabel
          Left = 64
          Top = 100
          Width = 123
          Height = 13
          Caption = 'gzip chunk size (kilobytes)'
        end
        object udThreadCount: TUpDown
          Left = 45
          Top = 24
          Width = 16
          Height = 21
          Associate = edtThreadSpin
          Min = 0
          Max = 16
          Position = 0
          TabOrder = 0
          Wrap = False
        end
        object edtThreadSpin: TEdit
          Left = 24
          Top = 24
          Width = 21
          Height = 21
          BevelInner = bvLowered
          BevelKind = bkTile
          BevelOuter = bvRaised
          BorderStyle = bsNone
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Bitstream Vera Sans Mono'
          Font.Style = []
          ParentFont = False
          ReadOnly = True
          TabOrder = 1
          Text = '0'
        end
        object udGzipFast: TUpDown
          Left = 45
          Top = 48
          Width = 16
          Height = 21
          Associate = edtGzipFastSpin
          Min = 1
          Max = 9
          Position = 1
          TabOrder = 2
          Wrap = False
        end
        object edtGzipFastSpin: TEdit
          Left = 24
          Top = 48
          Width = 21
          Height = 21
          BevelInner = bvLowered
          BevelKind = bkTile
          BevelOuter = bvRaised
          BorderStyle = bsNone
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Bitstream Vera Sans Mono'
          Font.Style = []
          ParentFont = False
          ReadOnly = True
          TabOrder = 3
          Text = '1'
        end
        object udGzipBest: TUpDown
          Left = 45
          Top = 72
          Width = 16
          Height = 21
          Associate = edtGzipBestSpin
          Min = 1
          Max = 9
          Position = 9
          TabOrder = 4
          Wrap = False
        end
        object edtGzipBestSpin: TEdit
          Left = 24
          Top = 72
          Width = 21
          Height = 21
          BevelInner = bvLowered
          BevelKind = bkTile
          BevelOuter = bvRaised
          BorderStyle = bsNone
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Bitstream Vera Sans'
          Font.Style = []
          ParentFont = False
          ReadOnly = True
          TabOrder = 5
          Text = '9'
        end
        object edtGzipChunkSpin: TEdit
          Left = 8
          Top = 96
          Width = 37
          Height = 21
          BevelInner = bvLowered
          BevelKind = bkTile
          BevelOuter = bvRaised
          BorderStyle = bsNone
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Bitstream Vera Sans'
          Font.Style = []
          ParentFont = False
          ReadOnly = True
          TabOrder = 6
          Text = '2048'
        end
        object udGzipChunk: TUpDown
          Left = 45
          Top = 96
          Width = 17
          Height = 21
          Associate = edtGzipChunkSpin
          Min = 256
          Max = 2048
          Position = 2048
          TabOrder = 7
          Thousands = False
          Wrap = False
          OnChangingEx = udGzipChunkChangingEx
        end
      end
    end
  end
end

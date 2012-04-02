object MainForm: TMainForm
  Left = 302
  Top = 25
  Width = 360
  Height = 380
  HelpContext = 1
  Caption = 'SelfImage v1.2'
  Color = clBtnFace
  Constraints.MaxHeight = 380
  Constraints.MinHeight = 380
  Constraints.MinWidth = 360
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  Position = poDefaultPosOnly
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    352
    326)
  PixelsPerInch = 96
  TextHeight = 13
  object btnHelp: TSpeedButton
    Left = 324
    Top = 295
    Width = 25
    Height = 25
    HelpContext = 3
    Anchors = [akRight, akBottom]
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
  object gbStatus: TGroupBox
    Left = 2
    Top = 0
    Width = 347
    Height = 289
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Status'
    TabOrder = 5
    Visible = False
    DesignSize = (
      347
      289)
    object lblBytesWritten: TLabel
      Left = 24
      Top = 208
      Width = 63
      Height = 13
      Caption = 'Bytes written:'
    end
    object lblBytesWrittenValue: TLabel
      Left = 96
      Top = 204
      Width = 11
      Height = 22
      Caption = '0'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Bitstream Vera Sans Mono'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lblBytesTotalValue: TLabel
      Left = 249
      Top = 76
      Width = 90
      Height = 19
      Alignment = taRightJustify
      Anchors = [akTop, akRight]
      Caption = '123.456GB'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Bitstream Vera Sans Mono'
      Font.Style = []
      ParentFont = False
    end
    object lblWriteSpeedValue: TLabel
      Left = 96
      Top = 228
      Width = 11
      Height = 22
      Caption = '0'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Bitstream Vera Sans Mono'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lblWriteSpeed: TLabel
      Left = 18
      Top = 232
      Width = 69
      Height = 13
      Caption = 'Current speed:'
    end
    object lblOverallSpeed: TLabel
      Left = 12
      Top = 256
      Width = 75
      Height = 13
      Caption = 'Average speed:'
    end
    object lblOverallSpeedValue: TLabel
      Left = 96
      Top = 252
      Width = 11
      Height = 22
      Caption = '0'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Bitstream Vera Sans Mono'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lblBytesRead: TLabel
      Left = 34
      Top = 160
      Width = 53
      Height = 13
      Caption = 'Bytes read:'
    end
    object lblBytesReadValue: TLabel
      Left = 96
      Top = 156
      Width = 11
      Height = 22
      Caption = '0'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Bitstream Vera Sans Mono'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object shpLine: TShape
      Left = 340
      Top = 57
      Width = 1
      Height = 17
      Anchors = [akTop, akRight]
    end
    object lblInputSize: TLabel
      Left = 252
      Top = 64
      Width = 47
      Height = 13
      Anchors = [akTop, akRight]
      Caption = 'Input Size'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object lblOutput: TLabel
      Left = 12
      Top = 128
      Width = 35
      Height = 13
      Caption = 'Output:'
    end
    object lblInput: TLabel
      Left = 20
      Top = 104
      Width = 27
      Height = 13
      Caption = 'Input:'
    end
    object ProgressBar: TCGauge
      Left = 10
      Top = 26
      Width = 329
      Height = 29
      Anchors = [akLeft, akTop, akRight]
      ShowText = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clNone
      Font.Height = -16
      Font.Name = 'Bitstream Vera Sans Mono'
      Font.Pitch = fpFixed
      Font.Style = []
      BorderStyle = bsNone
      ForeColor = clBtnHighlight
      BackColor = clBtnFace
      MaxValue = 329
      ParentFont = False
      Progress = 116
    end
    object bvProgressBevel: TBevel
      Left = 8
      Top = 24
      Width = 333
      Height = 33
      Anchors = [akLeft, akTop, akRight]
    end
    object lblPercent: TLabel
      Left = 8
      Top = 31
      Width = 333
      Height = 19
      Alignment = taCenter
      Anchors = [akLeft, akTop, akRight]
      AutoSize = False
      Caption = '33%'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Bitstream Vera Sans Mono'
      Font.Style = []
      ParentFont = False
      Transparent = True
    end
    object lblBytesSkipped: TLabel
      Left = 45
      Top = 184
      Width = 42
      Height = 13
      Caption = 'Skipped:'
    end
    object lblBytesSkippedValue: TLabel
      Left = 96
      Top = 180
      Width = 11
      Height = 22
      Caption = '0'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Bitstream Vera Sans Mono'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object edtInput: TEdit
      Left = 56
      Top = 102
      Width = 281
      Height = 18
      Anchors = [akLeft, akTop, akRight]
      BevelInner = bvLowered
      BevelKind = bkSoft
      BevelOuter = bvRaised
      BorderStyle = bsNone
      Color = clBtnFace
      TabOrder = 0
    end
    object edtOutput: TEdit
      Left = 56
      Top = 126
      Width = 281
      Height = 18
      Anchors = [akLeft, akTop, akRight]
      BevelInner = bvLowered
      BevelKind = bkSoft
      BevelOuter = bvRaised
      BorderStyle = bsNone
      Color = clBtnFace
      TabOrder = 1
    end
  end
  object btnOk: TButton
    Left = 106
    Top = 295
    Width = 75
    Height = 25
    Anchors = [akBottom]
    Caption = 'Start'
    Enabled = False
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = btnOkClick
  end
  object btnCancel: TButton
    Left = 190
    Top = 295
    Width = 75
    Height = 25
    Anchors = [akBottom]
    Cancel = True
    Caption = 'Exit'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ModalResult = 2
    ParentFont = False
    TabOrder = 1
    OnClick = btnCancelClick
  end
  object gbInput: TGroupBox
    Left = 2
    Top = 0
    Width = 347
    Height = 89
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Input'
    TabOrder = 2
    DesignSize = (
      347
      89)
    object btnSourceNBDConnect: TSpeedButton
      Left = 319
      Top = 40
      Width = 22
      Height = 22
      Anchors = [akTop, akRight]
      Enabled = False
      Glyph.Data = {
        DE010000424DDE01000000000000760000002800000024000000120000000100
        0400000000006801000000000000000000001000000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00666666666666
        6666666666666F6666666666000066666636666666666666666686F666666666
        0000666666306666666666666666886F66666666000066666663066666666666
        66666886F6666666000066666663B066666666666666F8F86F66666600006666
        63000F06666666666668888686F666660000666663FBFBF06666666666686F66
        686F666600006666663FB0333666666666668F688886666600006666663BFB06
        6666666666FF8FF686F66666000066630000BFB06666666668888866686F6666
        00006663FBFBFBFB06666666686F6666668F6666000066663FBFB03336666666
        668F666888866666000066663BFBFB06666666666686F66686F6666600006666
        63BFBFB0666666666668F666686F66660000666663FBFBFB0666666666686F66
        6686F66600006666663FBFBFB066666666668FFFFFF8FF660000666666333333
        3336666666668888888886660000666666666666666666666666666666666666
        0000}
      NumGlyphs = 2
      Visible = False
      OnClick = btnSourceNBDConnectClick
    end
    object lblSourceSize: TLabel
      Left = 7
      Top = 68
      Width = 50
      Height = 13
      Caption = 'Input Size:'
      Enabled = False
    end
    object lblSourceSizeValue: TLabel
      Left = 63
      Top = 64
      Width = 11
      Height = 22
      Caption = '0'
      Enabled = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Bitstream Vera Sans Mono'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object btnSourceFileOpen: TSpeedButton
      Left = 319
      Top = 40
      Width = 22
      Height = 22
      Anchors = [akTop, akRight]
      Enabled = False
      Glyph.Data = {
        D6000000424DD60000000000000076000000280000000E0000000C0000000100
        0400000000006000000000000000000000001000000000000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00D0000000000D
        DD00D00333333330DD00D0B0333333330D00D0BB033333333000D0BBB0000000
        0000D0BBBBBBBB0DDD00D0BBB000000DDD00DD000DDDDDDDDD00DDDDDDDDDD00
        0D00DDDDDDDDDDD00D00DDDDDD0DDD0D0D00DDDDDDD000DDDD00}
      OnClick = btnSourceFileOpenClick
    end
    object lblInputLocation: TLabel
      Left = 270
      Top = 24
      Width = 71
      Height = 13
      Anchors = [akTop, akRight]
      Caption = 'Input Location:'
    end
    object edtSourceNBD: TEdit
      Left = 8
      Top = 40
      Width = 313
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      Enabled = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clGrayText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 5
      Visible = False
    end
    object cmbSourceDrive: TComboBox
      Left = 8
      Top = 40
      Width = 333
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Bitstream Vera Sans Mono'
      Font.Style = []
      ItemHeight = 13
      ParentFont = False
      Sorted = True
      TabOrder = 3
      Visible = False
      OnChange = cmbSourceDriveChange
    end
    object radioSourceFile: TRadioButton
      Left = 8
      Top = 18
      Width = 41
      Height = 17
      Caption = 'File'
      TabOrder = 0
      OnClick = radioSourceFileClick
    end
    object radioSourceDrive: TRadioButton
      Left = 64
      Top = 18
      Width = 48
      Height = 17
      Caption = 'Drive'
      TabOrder = 1
      OnClick = cmbSourceDriveChange
    end
    object edtSourceFileName: TEdit
      Left = 8
      Top = 40
      Width = 313
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      Enabled = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clGrayText
      Font.Height = -11
      Font.Name = 'Bitstream Vera Sans Mono'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 4
    end
    object radioSourceNBD: TRadioButton
      Left = 128
      Top = 18
      Width = 49
      Height = 17
      Caption = 'NBD'
      TabOrder = 2
      OnClick = radioSourceNBDClick
    end
  end
  object gbProcessing: TGroupBox
    Left = 2
    Top = 96
    Width = 347
    Height = 97
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Processing'
    TabOrder = 3
    object gbCompressionMethod: TGroupBox
      Left = 132
      Top = 54
      Width = 209
      Height = 33
      Caption = 'Compression Method'
      Enabled = False
      TabOrder = 3
      DesignSize = (
        209
        33)
      object radioGzipFast: TRadioButton
        Left = 8
        Top = 12
        Width = 65
        Height = 17
        Caption = 'gzip (fast)'
        Enabled = False
        TabOrder = 0
        OnClick = radioProcessingClick
      end
      object radioGzipBest: TRadioButton
        Left = 80
        Top = 12
        Width = 73
        Height = 17
        Anchors = [akTop]
        Caption = 'gzip (best)'
        Checked = True
        Enabled = False
        TabOrder = 1
        TabStop = True
        OnClick = radioProcessingClick
      end
      object radioBzip2: TRadioButton
        Left = 152
        Top = 12
        Width = 49
        Height = 17
        Anchors = [akTop, akRight]
        Caption = 'bzip2'
        Enabled = False
        TabOrder = 2
        OnClick = radioProcessingClick
      end
    end
    object radioNoProcessing: TRadioButton
      Left = 8
      Top = 18
      Width = 49
      Height = 17
      Caption = 'None'
      Checked = True
      TabOrder = 0
      TabStop = True
      OnClick = radioProcessingClick
    end
    object radioDecompress: TRadioButton
      Left = 8
      Top = 42
      Width = 121
      Height = 17
      Caption = 'Decompress Source'
      Enabled = False
      TabOrder = 1
      OnClick = radioProcessingClick
    end
    object radioCompress: TRadioButton
      Left = 8
      Top = 66
      Width = 97
      Height = 17
      Caption = 'Compress Image'
      TabOrder = 2
      OnClick = radioProcessingClick
    end
  end
  object gbOutput: TGroupBox
    Left = 2
    Top = 200
    Width = 347
    Height = 89
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Output'
    TabOrder = 4
    DesignSize = (
      347
      89)
    object btnTargetNBDConnect: TSpeedButton
      Left = 319
      Top = 40
      Width = 22
      Height = 22
      Anchors = [akTop, akRight]
      Enabled = False
      Glyph.Data = {
        DE010000424DDE01000000000000760000002800000024000000120000000100
        0400000000006801000000000000000000001000000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00666666666666
        6666666666666F6666666666000066666636666666666666666686F666666666
        0000666666306666666666666666886F66666666000066666663066666666666
        66666886F6666666000066666663B066666666666666F8F86F66666600006666
        63000F06666666666668888686F666660000666663FBFBF06666666666686F66
        686F666600006666663FB0333666666666668F688886666600006666663BFB06
        6666666666FF8FF686F66666000066630000BFB06666666668888866686F6666
        00006663FBFBFBFB06666666686F6666668F6666000066663FBFB03336666666
        668F666888866666000066663BFBFB06666666666686F66686F6666600006666
        63BFBFB0666666666668F666686F66660000666663FBFBFB0666666666686F66
        6686F66600006666663FBFBFB066666666668FFFFFF8FF660000666666333333
        3336666666668888888886660000666666666666666666666666666666666666
        0000}
      NumGlyphs = 2
      Visible = False
      OnClick = btnTargetNBDConnectClick
    end
    object lblTargetDriveSize: TLabel
      Left = 7
      Top = 68
      Width = 98
      Height = 13
      Caption = 'Target Partition Size:'
      Enabled = False
    end
    object lblTargetDriveSizeValue: TLabel
      Left = 112
      Top = 64
      Width = 11
      Height = 22
      Caption = '0'
      Enabled = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Bitstream Vera Sans Mono'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object btnTargetFileOpen: TSpeedButton
      Left = 319
      Top = 40
      Width = 22
      Height = 22
      Anchors = [akTop, akRight]
      Enabled = False
      Glyph.Data = {
        D6000000424DD60000000000000076000000280000000E0000000C0000000100
        04000000000060000000120B0000120B00001000000010000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00D0000000000D
        DD00D00333333330DD00D0B0333333330D00D0BB033333333000D0BBB0000000
        0000D0BBBBBBBB0DDD00D0BBB000000DDD00DD000DDDDDDDDD00DDDDDD000DDD
        DD00DDDDDD00DDDDDD00DDDDDD0D0DDD0D00DDDDDDDDD000DD00}
      OnClick = btnTargetFileOpenClick
    end
    object lblOutputLocation: TLabel
      Left = 262
      Top = 24
      Width = 79
      Height = 13
      Anchors = [akTop, akRight]
      Caption = 'Output Location:'
    end
    object edtTargetNBD: TEdit
      Left = 8
      Top = 40
      Width = 313
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      Enabled = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clGrayText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 5
      Visible = False
    end
    object radioTargetFile: TRadioButton
      Left = 8
      Top = 18
      Width = 41
      Height = 17
      Caption = 'File'
      TabOrder = 0
      OnClick = radioTargetFileClick
    end
    object radioTargetDrive: TRadioButton
      Left = 64
      Top = 18
      Width = 49
      Height = 17
      Caption = 'Drive'
      TabOrder = 1
      OnClick = radioTargetDriveClick
    end
    object cmbTargetDrive: TComboBox
      Left = 8
      Top = 40
      Width = 333
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Bitstream Vera Sans Mono'
      Font.Style = []
      ItemHeight = 13
      ParentFont = False
      Sorted = True
      TabOrder = 3
      Visible = False
      OnChange = cmbTargetDriveChange
    end
    object edtTargetFileName: TEdit
      Left = 8
      Top = 40
      Width = 313
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      Enabled = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clGrayText
      Font.Height = -11
      Font.Name = 'Bitstream Vera Sans Mono'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 4
    end
    object radioTargetNBD: TRadioButton
      Left = 128
      Top = 18
      Width = 49
      Height = 17
      Caption = 'NBD'
      TabOrder = 2
      OnClick = radioTargetNBDClick
    end
  end
  object UpdateTimer: TTimer
    Enabled = False
    Interval = 250
    OnTimer = UpdateTimerTimer
    Left = 8
    Top = 296
  end
  object MainMenu: TMainMenu
    Left = 24
    Top = 296
    object menu_File: TMenuItem
      Caption = 'File'
      object menu_File_Start: TMenuItem
        Caption = 'Start'
        Enabled = False
        OnClick = btnOkClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object menu_File_Exit: TMenuItem
        Caption = 'Exit'
        OnClick = menu_File_ExitClick
      end
    end
    object menu_Edit: TMenuItem
      Caption = 'Edit'
      object menu_Edit_Refresh: TMenuItem
        Caption = 'Refresh Drive Lists'
        OnClick = menu_Edit_RefreshClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object menu_Edit_Preferences: TMenuItem
        Caption = 'Preferences'
        OnClick = menu_Edit_PreferencesClick
      end
    end
    object menu_Help: TMenuItem
      Caption = 'Help'
      object menu_Help_Contents: TMenuItem
        Caption = 'Contents'
        HelpContext = 1
        OnClick = menu_Help_ItemsClick
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object menu_Help_Overview: TMenuItem
        Tag = 1
        Caption = 'Overview'
        HelpContext = 2
        OnClick = menu_Help_ItemsClick
      end
      object menu_Help_Using: TMenuItem
        Tag = 2
        Caption = 'Using SelfImage'
        HelpContext = 3
        OnClick = menu_Help_ItemsClick
      end
      object menu_Help_Compression: TMenuItem
        Caption = 'Compression'
        HelpContext = 8
        OnClick = menu_Help_ItemsClick
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object menu_Help_Partitions: TMenuItem
        Tag = 3
        Caption = 'Tutorial on Disks && Partitions'
        HelpContext = 11
        OnClick = menu_Help_ItemsClick
      end
      object menu_Help_Improving: TMenuItem
        Caption = 'Improving Compression Ratios'
        HelpContext = 12
        OnClick = menu_Help_ItemsClick
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object menu_Help_About: TMenuItem
        Caption = 'About'
        OnClick = menu_Help_AboutClick
      end
    end
  end
  object OpenDialog: TOpenDialog
    Filter = 
      'All Image files (*.img;*.img.gz;*.img.bz2)|*.img;*.img.gz;*.img.' +
      'bz2|Uncompressed images (*.img)|*.img|gzipp-compressed images (*' +
      '.img.gz)|*.img.gz|bzip2-compressed images (*.img.bz2)|*.img.bz2|' +
      'All files (*.*)|*.*'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Select source file...'
    Left = 40
    Top = 296
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'img'
    Filter = 'Image files (*.img)|*.img|All files (*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = 'Save image file...'
    Left = 56
    Top = 296
  end
end

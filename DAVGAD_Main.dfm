object Form1: TForm1
  Left = 63
  Top = 131
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  BorderWidth = 2
  Caption = 'DAVGAD Vector Inspector'
  ClientHeight = 868
  ClientWidth = 1290
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 51
    Top = 120
    Width = 3
    Height = 13
    Alignment = taCenter
  end
  object lbConnected: TLabel
    Left = 371
    Top = 0
    Width = 3
    Height = 13
    Transparent = True
    Visible = False
  end
  object lbID_String: TLabel
    Left = 472
    Top = 0
    Width = 3
    Height = 13
  end
  object Label2: TLabel
    Left = 152
    Top = 8
    Width = 18
    Height = 13
    Caption = '000'
  end
  object Label3: TLabel
    Left = 192
    Top = 8
    Width = 18
    Height = 13
    Caption = '000'
  end
  object Label4: TLabel
    Left = 240
    Top = 8
    Width = 32
    Height = 13
    AutoSize = False
  end
  object PageControl1: TPageControl
    Left = 8
    Top = 24
    Width = 1281
    Height = 841
    ActivePage = Logs
    TabOrder = 0
    object Logs: TTabSheet
      Caption = 'Viewer'
      ImageIndex = 2
      object Label43: TLabel
        Left = 272
        Top = 248
        Width = 3
        Height = 13
        Alignment = taCenter
      end
      object Image1: TImage
        Left = 248
        Top = 40
        Width = 1024
        Height = 768
        OnClick = Image1Click
        OnMouseMove = Image1MouseMove
      end
      object Label5: TLabel
        Left = 448
        Top = 8
        Width = 60
        Height = 13
        Caption = 'Vec Address'
      end
      object lbAssemble_Error: TLabel
        Left = 977
        Top = 21
        Width = 3
        Height = 13
      end
      object lbAssembly_Line: TLabel
        Left = 977
        Top = 0
        Width = 6
        Height = 13
        Caption = '0'
      end
      object Memo1: TMemo
        Left = 0
        Top = 40
        Width = 241
        Height = 769
        TabStop = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clDefault
        Font.Height = -11
        Font.Name = 'Times New Roman'
        Font.Style = []
        Lines.Strings = (
          '')
        ParentFont = False
        ScrollBars = ssVertical
        TabOrder = 0
        WantTabs = True
        WordWrap = False
        OnMouseDown = Memo1MouseDown
      end
      object Button1: TButton
        Left = 24
        Top = 0
        Width = 113
        Height = 17
        Caption = 'Read Vector Data'
        TabOrder = 1
        OnClick = Button1Click
      end
      object Button2: TButton
        Left = 24
        Top = 16
        Width = 113
        Height = 17
        Caption = 'Write Vector Data'
        TabOrder = 2
        OnClick = Button2Click
      end
      object bnDraw_Button: TBitBtn
        Left = 304
        Top = 8
        Width = 75
        Height = 25
        Caption = 'Draw'
        TabOrder = 3
      end
      object Edit1: TEdit
        Left = 392
        Top = 8
        Width = 49
        Height = 21
        TabOrder = 4
        Text = '0'
      end
      object CheckBox1: TCheckBox
        Left = 536
        Top = 8
        Width = 113
        Height = 17
        Caption = 'Show retrace lines'
        TabOrder = 5
      end
      object Button5: TButton
        Left = 896
        Top = 8
        Width = 75
        Height = 25
        Caption = 'Assemble'
        TabOrder = 6
        OnClick = Button5Click
      end
      object Button3: TButton
        Left = 744
        Top = 0
        Width = 113
        Height = 17
        Caption = 'Read Vector Source'
        TabOrder = 7
        OnClick = Button3Click
      end
      object Button4: TButton
        Left = 744
        Top = 16
        Width = 113
        Height = 17
        Caption = 'Write Vector Source'
        TabOrder = 8
        OnClick = Button4Click
      end
      object pbLoad_Progress: TProgressBar
        Left = 143
        Top = 3
        Width = 98
        Height = 17
        Smooth = True
        TabOrder = 9
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'HEX data'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label7: TLabel
        Left = 0
        Top = 24
        Width = 329
        Height = 13
        Caption = 
          'Hex data in single byte format, 2 bytes per vector address. Chec' +
          'ksum:'
      end
      object lbChecksum: TLabel
        Left = 335
        Top = 24
        Width = 24
        Height = 13
        Caption = '0000'
      end
      object sgHex_Data: TStringGrid
        Left = -4
        Top = 43
        Width = 1118
        Height = 566
        ColCount = 33
        DefaultColWidth = 32
        DefaultRowHeight = 16
        RowCount = 513
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goThumbTracking]
        ScrollBars = ssVertical
        TabOrder = 0
        ColWidths = (
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32
          32)
        RowHeights = (
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16)
      end
    end
    object TabSheet1: TTabSheet
      Caption = 'Import'
      ImageIndex = 1
      object Label6: TLabel
        Left = 46
        Top = 144
        Width = 32
        Height = 13
        Hint = 'Here'
        Caption = 'Label6'
        ParentShowHint = False
        ShowHint = True
      end
      object btConvert_PLT: TButton
        Left = 39
        Top = 103
        Width = 145
        Height = 25
        Caption = 'Convert PLT'
        TabOrder = 0
        OnClick = btConvert_PLTClick
      end
      object btChange_Game: TButton
        Left = 39
        Top = 41
        Width = 145
        Height = 25
        Caption = 'Change game tyoe'
        TabOrder = 1
        OnClick = btChange_GameClick
      end
      object cbError_Check: TCheckBox
        Left = 228
        Top = 49
        Width = 134
        Height = 17
        Caption = 'Re-assembly error check'
        TabOrder = 2
      end
    end
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'bin'
    Filter = 'All|*.*'
    Options = [ofHideReadOnly, ofFileMustExist, ofEnableSizing]
    Left = 32
  end
  object SaveDialog1: TSaveDialog
    Filter = 'Bin|*.bin|All|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 100
    Left = 64
  end
  object SaveDialog2: TSaveDialog
    DefaultExt = 'txt'
    Filter = 'TXT|*.txt|All|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Title = 'Save source'
    Left = 288
  end
  object OpenDialog2: TOpenDialog
    DefaultExt = 'txt'
    Filter = 'TXT|*.txt|All|*.*'
    Options = [ofHideReadOnly, ofFileMustExist, ofEnableSizing]
    Left = 88
  end
  object OpenDialog3: TOpenDialog
    DefaultExt = 'txt'
    Filter = 'PLT|*.plt;*.hpgl|All|*.*'
    FilterIndex = 0
    Options = [ofHideReadOnly, ofFileMustExist, ofEnableSizing]
    Left = 320
  end
end

object SysAltAISForm: TSysAltAISForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Alternative AIS Tracks'
  ClientHeight = 310
  ClientWidth = 613
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label4: TLabel
    Left = 8
    Top = 8
    Width = 31
    Height = 13
    Caption = 'Offset'
  end
  object Label5: TLabel
    Left = 52
    Top = 8
    Width = 50
    Height = 13
    Caption = 'Ship Name'
  end
  object Label6: TLabel
    Left = 272
    Top = 8
    Width = 26
    Height = 13
    Caption = 'MMSI'
  end
  object Label7: TLabel
    Left = 332
    Top = 8
    Width = 30
    Height = 13
    Caption = 'Speed'
  end
  object Label8: TLabel
    Left = 372
    Top = 8
    Width = 34
    Height = 13
    Caption = 'Course'
  end
  object TrackLB: TListBox
    Left = 4
    Top = 24
    Width = 441
    Height = 277
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ItemHeight = 14
    Items.Strings = (
      '12345 Ship Name                      12345678 13.3  254')
    ParentFont = False
    TabOrder = 0
    OnDblClick = TrackLBDblClick
  end
  object GroupBox1: TGroupBox
    Left = 456
    Top = 0
    Width = 149
    Height = 125
    Caption = 'Current Track'
    TabOrder = 1
    object Label1: TLabel
      Left = 12
      Top = 48
      Width = 26
      Height = 13
      Caption = 'MMSI'
    end
    object Label2: TLabel
      Left = 12
      Top = 72
      Width = 30
      Height = 13
      Caption = 'Speed'
    end
    object Label3: TLabel
      Left = 8
      Top = 96
      Width = 34
      Height = 13
      Caption = 'Course'
    end
    object Label9: TLabel
      Left = 88
      Top = 72
      Width = 14
      Height = 13
      Caption = 'kts'
    end
    object Label10: TLabel
      Left = 88
      Top = 96
      Width = 18
      Height = 13
      Caption = 'deg'
    end
    object CNameEd: TEdit
      Left = 8
      Top = 20
      Width = 129
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 0
    end
    object CMMSIEd: TEdit
      Left = 52
      Top = 44
      Width = 61
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 1
    end
    object CSpeedEd: TEdit
      Left = 52
      Top = 68
      Width = 33
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 2
    end
    object CCourseEd: TEdit
      Left = 52
      Top = 92
      Width = 33
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 3
    end
  end
  object CancelBut: TBitBtn
    Left = 528
    Top = 280
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 2
  end
  object ReplaceBut: TButton
    Left = 452
    Top = 136
    Width = 153
    Height = 41
    Cursor = crHandPoint
    Caption = 'Replace  Track'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = ReplaceButClick
  end
end

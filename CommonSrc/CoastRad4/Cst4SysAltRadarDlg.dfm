object SysAltRadarForm: TSysAltRadarForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Alternative Radar Tracks'
  ClientHeight = 310
  ClientWidth = 603
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
    Width = 33
    Height = 13
    Caption = 'Source'
  end
  object Label6: TLabel
    Left = 272
    Top = 8
    Width = 26
    Height = 13
    Caption = 'Track'
  end
  object Label7: TLabel
    Left = 308
    Top = 8
    Width = 30
    Height = 13
    Caption = 'Speed'
  end
  object Label8: TLabel
    Left = 348
    Top = 8
    Width = 34
    Height = 13
    Caption = 'Course'
  end
  object TrackLB: TListBox
    Left = 4
    Top = 24
    Width = 381
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
  object CurrentGrp: TGroupBox
    Left = 392
    Top = 4
    Width = 205
    Height = 125
    Caption = 'Current Track'
    TabOrder = 1
    object Label1: TLabel
      Left = 12
      Top = 48
      Width = 26
      Height = 13
      Caption = 'Track'
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
    object SourceEd: TEdit
      Left = 8
      Top = 20
      Width = 189
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 0
    end
    object TrackEd: TEdit
      Left = 52
      Top = 44
      Width = 61
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 1
    end
    object SpeedEd: TEdit
      Left = 52
      Top = 68
      Width = 33
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 2
    end
    object CourseEd: TEdit
      Left = 52
      Top = 92
      Width = 33
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 3
    end
  end
  object ReplaceBut: TButton
    Left = 396
    Top = 160
    Width = 193
    Height = 41
    Cursor = crHandPoint
    Caption = 'Replace  Radar'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = ReplaceButClick
  end
  object CancelBut: TBitBtn
    Left = 512
    Top = 276
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 3
  end
end

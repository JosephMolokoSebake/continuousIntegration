object AISTargetInfoForm: TAISTargetInfoForm
  Left = 385
  Top = 266
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'AIS Information'
  ClientHeight = 507
  ClientWidth = 414
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 108
    Top = 8
    Width = 149
    Height = 73
    Caption = 'Position'
    TabOrder = 1
    object Label1: TLabel
      Left = 12
      Top = 20
      Width = 38
      Height = 13
      Caption = 'Latitude'
    end
    object Label2: TLabel
      Left = 12
      Top = 44
      Width = 47
      Height = 13
      Caption = 'Longitude'
    end
    object LatEd: TEdit
      Left = 72
      Top = 16
      Width = 65
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 0
    end
    object LongEd: TEdit
      Left = 72
      Top = 40
      Width = 65
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 1
    end
  end
  object DTGrp: TGroupBox
    Left = 8
    Top = 8
    Width = 89
    Height = 73
    Caption = 'Date/Time'
    TabOrder = 0
    object DateEd: TEdit
      Left = 12
      Top = 20
      Width = 65
      Height = 21
      Color = cl3DLight
      MaxLength = 10
      ReadOnly = True
      TabOrder = 0
    end
    object TimeEd: TEdit
      Left = 12
      Top = 44
      Width = 50
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 1
    end
  end
  object GroupBox3: TGroupBox
    Left = 268
    Top = 8
    Width = 137
    Height = 73
    Caption = 'Vector'
    TabOrder = 2
    object Label5: TLabel
      Left = 12
      Top = 20
      Width = 31
      Height = 13
      Caption = 'Speed'
    end
    object Label6: TLabel
      Left = 12
      Top = 44
      Width = 33
      Height = 13
      Caption = 'Course'
    end
    object Label7: TLabel
      Left = 108
      Top = 20
      Width = 14
      Height = 13
      Caption = 'kts'
    end
    object Label8: TLabel
      Left = 108
      Top = 44
      Width = 18
      Height = 13
      Caption = 'deg'
    end
    object SpeedEd: TEdit
      Left = 60
      Top = 16
      Width = 45
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 0
    end
    object CourseEd: TEdit
      Left = 60
      Top = 40
      Width = 45
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 1
    end
  end
  object StaticGrp: TGroupBox
    Left = 12
    Top = 120
    Width = 397
    Height = 333
    Caption = 'Static Vessel Information'
    TabOrder = 3
    object Label3: TLabel
      Left = 12
      Top = 20
      Width = 62
      Height = 13
      Caption = 'Vessel Name'
    end
    object Label4: TLabel
      Left = 280
      Top = 20
      Width = 36
      Height = 13
      Caption = 'Callsign'
    end
    object Grid: TStringGrid
      Left = 12
      Top = 44
      Width = 373
      Height = 277
      ColCount = 2
      DefaultRowHeight = 20
      FixedCols = 0
      RowCount = 13
      FixedRows = 0
      ScrollBars = ssNone
      TabOrder = 2
      ColWidths = (
        64
        64)
      RowHeights = (
        20
        20
        20
        20
        20
        20
        20
        20
        20
        20
        20
        20
        20)
    end
    object NameEd: TEdit
      Left = 84
      Top = 16
      Width = 189
      Height = 21
      Color = cl3DLight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      ReadOnly = True
      TabOrder = 0
      Text = 'NameEd'
    end
    object CallEd: TEdit
      Left = 324
      Top = 16
      Width = 61
      Height = 21
      Color = cl3DLight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      ReadOnly = True
      TabOrder = 1
      Text = 'CallEd'
    end
  end
  object MMSIGrp: TGroupBox
    Left = 12
    Top = 88
    Width = 173
    Height = 49
    Caption = 'Vessel Information'
    TabOrder = 4
    object Label10: TLabel
      Left = 12
      Top = 20
      Width = 62
      Height = 13
      Caption = 'Vessel MMSI'
    end
    object MMSIEd: TEdit
      Left = 88
      Top = 16
      Width = 73
      Height = 21
      TabOrder = 0
      Text = 'MMSIEd'
    end
  end
  object SysTrackBut: TButton
    Left = 56
    Top = 464
    Width = 265
    Height = 33
    Cursor = crHandPoint
    Caption = 'Create System Track'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 5
    OnClick = SysTrackButClick
  end
end

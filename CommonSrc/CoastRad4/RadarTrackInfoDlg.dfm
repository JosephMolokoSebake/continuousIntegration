object RadarTrackInfoForm: TRadarTrackInfoForm
  Left = 385
  Top = 266
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Radar Track Information'
  ClientHeight = 184
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
    Left = 104
    Top = 56
    Width = 149
    Height = 73
    Caption = 'Position'
    TabOrder = 2
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
    Top = 56
    Width = 89
    Height = 73
    Caption = 'Date/Time'
    TabOrder = 1
    object DateEd: TEdit
      Left = 12
      Top = 20
      Width = 65
      Height = 21
      Color = cl3DLight
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
    Left = 260
    Top = 56
    Width = 137
    Height = 73
    Caption = 'Vector'
    TabOrder = 3
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
  object GroupBox2: TGroupBox
    Left = 8
    Top = 4
    Width = 189
    Height = 45
    Caption = 'Track Info'
    TabOrder = 0
    object Label3: TLabel
      Left = 12
      Top = 20
      Width = 42
      Height = 13
      Caption = 'Track ID'
    end
    object Label4: TLabel
      Left = 120
      Top = 20
      Width = 32
      Height = 13
      Caption = 'Quality'
    end
    object IDEd: TEdit
      Left = 60
      Top = 16
      Width = 53
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 0
    end
    object QualEd: TEdit
      Left = 156
      Top = 16
      Width = 25
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 1
    end
  end
  object SystemBut: TButton
    Left = 60
    Top = 143
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
    TabOrder = 4
    OnClick = SystemButClick
  end
end

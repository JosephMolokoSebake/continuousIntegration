object NavArcForm: TNavArcForm
  Left = 382
  Top = 324
  BorderStyle = bsDialog
  Caption = 'Edit Shape'
  ClientHeight = 326
  ClientWidth = 589
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label13: TLabel
    Left = 152
    Top = 12
    Width = 28
    Height = 13
    Caption = 'Descr'
  end
  object OKBut: TBitBtn
    Left = 472
    Top = 236
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 8
    OnClick = OKButClick
  end
  object CancelBut: TBitBtn
    Left = 472
    Top = 276
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 9
    OnClick = CancelButClick
  end
  object FirePosGrp: TGroupBox
    Left = 8
    Top = 96
    Width = 161
    Height = 73
    Caption = 'Firing Position'
    TabOrder = 1
    object ArcLab1: TLabel
      Left = 12
      Top = 20
      Width = 38
      Height = 13
      Caption = 'Latitude'
    end
    object ArcLab2: TLabel
      Left = 12
      Top = 44
      Width = 47
      Height = 13
      Caption = 'Longitude'
    end
    object LatEd: TEdit
      Left = 84
      Top = 16
      Width = 65
      Height = 21
      CharCase = ecUpperCase
      TabOrder = 0
    end
    object LongEd: TEdit
      Left = 84
      Top = 40
      Width = 65
      Height = 21
      CharCase = ecUpperCase
      TabOrder = 1
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 172
    Width = 201
    Height = 121
    Caption = 'Safety Arc'
    TabOrder = 2
    object ArcLab3: TLabel
      Left = 12
      Top = 20
      Width = 73
      Height = 13
      Caption = 'Forward Range'
    end
    object ArcLab5: TLabel
      Left = 12
      Top = 44
      Width = 48
      Height = 13
      Caption = 'Aft Range'
    end
    object ArcLab7: TLabel
      Left = 12
      Top = 68
      Width = 70
      Height = 13
      Caption = 'Centre Bearing'
    end
    object ArcLab9: TLabel
      Left = 12
      Top = 92
      Width = 47
      Height = 13
      Caption = 'Arc Width'
    end
    object ArcLab4: TLabel
      Left = 172
      Top = 20
      Width = 8
      Height = 13
      Caption = 'm'
    end
    object ArcLab6: TLabel
      Left = 172
      Top = 44
      Width = 8
      Height = 13
      Caption = 'm'
    end
    object ArcLab8: TLabel
      Left = 172
      Top = 68
      Width = 18
      Height = 13
      Caption = 'deg'
    end
    object ArcLab10: TLabel
      Left = 172
      Top = 92
      Width = 18
      Height = 13
      Caption = 'deg'
    end
    object FRaEd: TEdit
      Left = 124
      Top = 16
      Width = 45
      Height = 21
      TabOrder = 0
      Text = '999999'
    end
    object ARaEd: TEdit
      Left = 124
      Top = 40
      Width = 45
      Height = 21
      TabOrder = 1
      Text = 'ARaEd'
    end
    object BeEd: TEdit
      Left = 124
      Top = 64
      Width = 45
      Height = 21
      TabOrder = 2
      Text = 'BeEd'
    end
    object ArcEd: TEdit
      Left = 124
      Top = 88
      Width = 45
      Height = 21
      TabOrder = 3
      Text = 'ArcEd'
    end
  end
  object GroupBox3: TGroupBox
    Left = 440
    Top = 76
    Width = 141
    Height = 133
    Caption = 'Frame'
    TabOrder = 4
    object Label11: TLabel
      Left = 12
      Top = 20
      Width = 62
      Height = 13
      Caption = 'Frame Colour'
    end
    object Label12: TLabel
      Left = 12
      Top = 44
      Width = 60
      Height = 13
      Caption = 'Frame Width'
    end
    object FillLab: TLabel
      Left = 12
      Top = 104
      Width = 45
      Height = 13
      Caption = 'Fill Colour'
    end
    object Bevel1: TBevel
      Left = 12
      Top = 72
      Width = 113
      Height = 2
    end
    object FillChk: TCheckBox
      Left = 12
      Top = 80
      Width = 117
      Height = 17
      Caption = 'Fill Frame'
      TabOrder = 3
      OnClick = FillChkClick
    end
    object FramePan: TPanel
      Left = 112
      Top = 16
      Width = 21
      Height = 21
      Cursor = crHandPoint
      Color = clYellow
      ParentBackground = False
      TabOrder = 0
      OnClick = FramePanClick
    end
    object WidthEd: TEdit
      Left = 100
      Top = 40
      Width = 17
      Height = 21
      TabOrder = 1
      Text = '1'
    end
    object WidthUD: TUpDown
      Left = 117
      Top = 40
      Width = 15
      Height = 21
      Associate = WidthEd
      Min = 1
      Max = 9
      Position = 1
      TabOrder = 2
    end
    object FillPan: TPanel
      Left = 112
      Top = 100
      Width = 21
      Height = 21
      Cursor = crHandPoint
      Color = clYellow
      ParentBackground = False
      TabOrder = 4
      OnClick = FillPanClick
    end
  end
  object DelBut: TButton
    Left = 156
    Top = 44
    Width = 57
    Height = 21
    Cursor = crHandPoint
    Caption = 'Delete'
    TabOrder = 7
    OnClick = DelButClick
  end
  object DescrEd: TEdit
    Left = 188
    Top = 8
    Width = 245
    Height = 21
    TabOrder = 5
  end
  object DispChk: TCheckBox
    Left = 228
    Top = 36
    Width = 117
    Height = 17
    Caption = 'Display Shape'
    TabOrder = 6
  end
  object GroupBox4: TGroupBox
    Left = 8
    Top = 4
    Width = 133
    Height = 85
    Caption = 'Type'
    TabOrder = 0
    object ArcRad: TRadioButton
      Left = 8
      Top = 20
      Width = 105
      Height = 17
      Caption = 'Safety Arc'
      TabOrder = 0
      OnClick = ArcRadClick
    end
    object SegRad: TRadioButton
      Left = 8
      Top = 60
      Width = 113
      Height = 17
      Caption = 'User Shape'
      TabOrder = 2
      OnClick = SegRadClick
    end
    object CircleRad: TRadioButton
      Left = 8
      Top = 40
      Width = 93
      Height = 17
      Caption = 'Circle'
      TabOrder = 1
      OnClick = CircleRadClick
    end
  end
  object GroupBox5: TGroupBox
    Left = 220
    Top = 56
    Width = 209
    Height = 261
    Hint = 'Add Lat/Long Point'
    Caption = 'User Shape'
    TabOrder = 10
    object SegList: TListBox
      Left = 12
      Top = 20
      Width = 137
      Height = 229
      ItemHeight = 13
      TabOrder = 0
      OnClick = SegListClick
      OnDblClick = SegListDblClick
    end
    object AddSegBut: TButton
      Left = 156
      Top = 20
      Width = 41
      Height = 21
      Cursor = crHandPoint
      Hint = 'Add New Point'
      Caption = 'Add'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnClick = AddSegButClick
    end
    object DelSegBut: TButton
      Left = 156
      Top = 68
      Width = 41
      Height = 21
      Cursor = crHandPoint
      Hint = 'Delete Lat/Long Point'
      Caption = 'Delete'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnClick = DelSegButClick
    end
    object UpBut: TButton
      Left = 156
      Top = 96
      Width = 17
      Height = 25
      Cursor = crHandPoint
      Hint = 'Move Point Up'
      Caption = #173
      Font.Charset = SYMBOL_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Symbol'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnClick = UpButClick
    end
    object DownBut: TButton
      Left = 180
      Top = 96
      Width = 17
      Height = 25
      Cursor = crHandPoint
      Hint = 'Move Point Down'
      Caption = #175
      Font.Charset = SYMBOL_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Symbol'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 4
      OnClick = DownButClick
    end
    object EditSegBut: TButton
      Left = 156
      Top = 44
      Width = 41
      Height = 21
      Cursor = crHandPoint
      Hint = 'Edit Current Point'
      Caption = 'Edit'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 5
      OnClick = EditSegButClick
    end
  end
  object CircleGrp: TGroupBox
    Left = 440
    Top = 4
    Width = 141
    Height = 69
    Caption = 'Circle'
    TabOrder = 3
    object CircleLab1: TLabel
      Left = 12
      Top = 20
      Width = 42
      Height = 13
      Caption = 'Diameter'
    end
    object CircleLab2: TLabel
      Left = 116
      Top = 20
      Width = 8
      Height = 13
      Caption = 'm'
    end
    object CircleEd: TEdit
      Left = 68
      Top = 16
      Width = 45
      Height = 21
      TabOrder = 0
      Text = '999999'
    end
    object CircleDotChk: TCheckBox
      Left = 12
      Top = 40
      Width = 121
      Height = 21
      Caption = 'Show Centre Dot'
      TabOrder = 1
    end
  end
end

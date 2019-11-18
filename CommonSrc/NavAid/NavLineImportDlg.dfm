object NavLineImportForm: TNavLineImportForm
  Left = 267
  Top = 223
  BorderStyle = bsDialog
  Caption = 'Line Data Import'
  ClientHeight = 183
  ClientWidth = 475
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
  object FileLab: TLabel
    Left = 8
    Top = 98
    Width = 170
    Height = 13
    Caption = 'Source File Name (Bearing - Range)'
  end
  object GroupBox1: TGroupBox
    Left = 244
    Top = 4
    Width = 141
    Height = 77
    Caption = 'Radar Position'
    TabOrder = 0
    object LatLab: TLabel
      Left = 12
      Top = 24
      Width = 38
      Height = 13
      Caption = 'Latitude'
    end
    object LongLab: TLabel
      Left = 12
      Top = 48
      Width = 47
      Height = 13
      Caption = 'Longitude'
    end
    object LatEd: TEdit
      Left = 64
      Top = 20
      Width = 65
      Height = 21
      TabOrder = 0
    end
    object LongEd: TEdit
      Left = 64
      Top = 44
      Width = 65
      Height = 21
      TabOrder = 1
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 4
    Width = 133
    Height = 85
    Caption = 'Import Data Type'
    TabOrder = 1
    object LLRad: TRadioButton
      Left = 8
      Top = 20
      Width = 113
      Height = 17
      Caption = 'Latitude/Longitude'
      Checked = True
      TabOrder = 0
      TabStop = True
      OnClick = LLRadClick
    end
    object RaBeRad: TRadioButton
      Left = 8
      Top = 40
      Width = 101
      Height = 17
      Caption = 'Range/Bearing'
      TabOrder = 1
      OnClick = RaBeRadClick
    end
    object BeRaRad: TRadioButton
      Left = 8
      Top = 60
      Width = 101
      Height = 17
      Caption = 'Bearing/Range'
      TabOrder = 2
      OnClick = BeRaRadClick
    end
  end
  object GroupBox3: TGroupBox
    Left = 152
    Top = 4
    Width = 81
    Height = 85
    Caption = 'Delimiter'
    TabOrder = 2
    object SpaceRad: TRadioButton
      Left = 8
      Top = 20
      Width = 65
      Height = 17
      Caption = 'Spaces'
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object CommaRad: TRadioButton
      Left = 8
      Top = 40
      Width = 65
      Height = 17
      Caption = 'Comma'
      TabOrder = 1
    end
    object TabRad: TRadioButton
      Left = 8
      Top = 60
      Width = 49
      Height = 17
      Caption = 'Tab'
      TabOrder = 2
    end
  end
  object SourceEd: TEdit
    Left = 8
    Top = 112
    Width = 441
    Height = 21
    Color = cl3DLight
    ReadOnly = True
    TabOrder = 3
  end
  object SourceBut: TButton
    Left = 452
    Top = 112
    Width = 13
    Height = 21
    Cursor = crHandPoint
    Caption = '..'
    TabOrder = 4
    OnClick = SourceButClick
  end
  object OKBut: TBitBtn
    Left = 300
    Top = 148
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 5
  end
  object CancelBut: TBitBtn
    Left = 388
    Top = 148
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 6
  end
  object GroupBox4: TGroupBox
    Left = 396
    Top = 4
    Width = 73
    Height = 65
    Caption = 'Range Unit'
    TabOrder = 7
    object mRad: TRadioButton
      Left = 12
      Top = 20
      Width = 37
      Height = 17
      Caption = 'm'
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object kmRad: TRadioButton
      Left = 12
      Top = 40
      Width = 37
      Height = 17
      Caption = 'km'
      TabOrder = 1
    end
  end
end

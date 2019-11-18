object EditTempSiteForm: TEditTempSiteForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Edit Temporary Site'
  ClientHeight = 196
  ClientWidth = 301
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
  object BitBtn1: TBitBtn
    Left = 137
    Top = 162
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 0
  end
  object BitBtn2: TBitBtn
    Left = 218
    Top = 162
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 1
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 185
    Height = 45
    Caption = 'Site Location'
    TabOrder = 2
    object LocEd: TEdit
      Left = 12
      Top = 16
      Width = 165
      Height = 21
      TabOrder = 0
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 59
    Width = 137
    Height = 97
    Caption = 'AIS Position'
    TabOrder = 3
    object Label2: TLabel
      Left = 3
      Top = 16
      Width = 39
      Height = 13
      Caption = 'Latitude'
    end
    object Label3: TLabel
      Left = 8
      Top = 43
      Width = 47
      Height = 13
      Caption = 'Longitude'
    end
    object Label4: TLabel
      Left = 12
      Top = 72
      Width = 31
      Height = 13
      Caption = 'Height'
    end
    object Label1: TLabel
      Left = 94
      Top = 72
      Width = 8
      Height = 13
      Caption = 'm'
    end
    object AISLatEd: TEdit
      Left = 58
      Top = 13
      Width = 65
      Height = 21
      TabOrder = 0
    end
    object AISLongEd: TEdit
      Left = 58
      Top = 40
      Width = 65
      Height = 21
      TabOrder = 1
    end
    object AISHeightEd: TEdit
      Left = 58
      Top = 67
      Width = 30
      Height = 21
      TabOrder = 2
    end
  end
  object GroupBox3: TGroupBox
    Left = 151
    Top = 59
    Width = 137
    Height = 97
    Caption = 'Radar Position'
    TabOrder = 4
    object Label5: TLabel
      Left = 3
      Top = 16
      Width = 39
      Height = 13
      Caption = 'Latitude'
    end
    object Label6: TLabel
      Left = 8
      Top = 43
      Width = 47
      Height = 13
      Caption = 'Longitude'
    end
    object Label7: TLabel
      Left = 12
      Top = 72
      Width = 31
      Height = 13
      Caption = 'Height'
    end
    object Label8: TLabel
      Left = 94
      Top = 72
      Width = 8
      Height = 13
      Caption = 'm'
    end
    object RadarLatEd: TEdit
      Left = 58
      Top = 13
      Width = 65
      Height = 21
      TabOrder = 0
    end
    object RadarLongEd: TEdit
      Left = 58
      Top = 40
      Width = 65
      Height = 21
      TabOrder = 1
    end
    object RadarHeightEd: TEdit
      Left = 58
      Top = 67
      Width = 30
      Height = 21
      TabOrder = 2
    end
  end
  object ReadBut: TButton
    Left = 8
    Top = 162
    Width = 55
    Height = 25
    Cursor = crHandPoint
    Caption = 'Read'
    TabOrder = 5
    OnClick = ReadButClick
  end
  object SaveBut: TButton
    Left = 69
    Top = 162
    Width = 55
    Height = 25
    Cursor = crHandPoint
    Caption = 'Save As'
    TabOrder = 6
    OnClick = SaveButClick
  end
end

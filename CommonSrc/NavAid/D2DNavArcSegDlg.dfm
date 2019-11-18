object NavArcSegForm: TNavArcSegForm
  Left = 403
  Top = 382
  BorderStyle = bsDialog
  Caption = 'User Shape Point'
  ClientHeight = 101
  ClientWidth = 272
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 257
    Height = 49
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 20
      Width = 38
      Height = 13
      Caption = 'Latitude'
    end
    object Label2: TLabel
      Left = 128
      Top = 20
      Width = 47
      Height = 13
      Caption = 'Longitude'
    end
    object LatEd: TEdit
      Left = 52
      Top = 16
      Width = 65
      Height = 21
      TabOrder = 0
    end
    object LongEd: TEdit
      Left = 180
      Top = 16
      Width = 65
      Height = 21
      TabOrder = 1
    end
  end
  object OKBut: TBitBtn
    Left = 96
    Top = 68
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 1
  end
  object CancelBut: TBitBtn
    Left = 184
    Top = 68
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 2
  end
end

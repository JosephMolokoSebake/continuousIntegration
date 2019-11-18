object NavReadForm: TNavReadForm
  Left = 268
  Top = 222
  BorderStyle = bsDialog
  Caption = 'NavReadForm'
  ClientHeight = 311
  ClientWidth = 371
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 12
    Top = 8
    Width = 169
    Height = 93
    Caption = 'Text/Symbols'
    TabOrder = 0
    object Label1: TLabel
      Left = 12
      Top = 24
      Width = 34
      Height = 13
      Caption = 'Current'
    end
    object Label2: TLabel
      Left = 96
      Top = 24
      Width = 16
      Height = 13
      Caption = 'File'
    end
    object CTextEd: TEdit
      Left = 52
      Top = 20
      Width = 33
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 0
    end
    object NTextEd: TEdit
      Left = 124
      Top = 20
      Width = 33
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 1
    end
    object CurSymRad: TRadioButton
      Left = 12
      Top = 48
      Width = 145
      Height = 17
      Caption = 'Replace Current Symbols'
      Checked = True
      TabOrder = 2
      TabStop = True
    end
    object NewSymRad: TRadioButton
      Left = 12
      Top = 68
      Width = 113
      Height = 17
      Caption = 'Add New Symbols'
      TabOrder = 3
    end
  end
  object GroupBox2: TGroupBox
    Left = 192
    Top = 8
    Width = 169
    Height = 93
    Caption = 'Lines'
    TabOrder = 1
    object Label3: TLabel
      Left = 12
      Top = 24
      Width = 34
      Height = 13
      Caption = 'Current'
    end
    object Label4: TLabel
      Left = 96
      Top = 24
      Width = 16
      Height = 13
      Caption = 'File'
    end
    object CLineEd: TEdit
      Left = 52
      Top = 20
      Width = 33
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 0
    end
    object NLineEd: TEdit
      Left = 124
      Top = 20
      Width = 33
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 1
    end
    object CurLineRad: TRadioButton
      Left = 12
      Top = 48
      Width = 145
      Height = 17
      Caption = 'Replace Current Lines'
      Checked = True
      TabOrder = 2
      TabStop = True
    end
    object NewLineRad: TRadioButton
      Left = 12
      Top = 68
      Width = 113
      Height = 17
      Caption = 'Add New Lines'
      TabOrder = 3
    end
  end
  object GroupBox3: TGroupBox
    Left = 12
    Top = 108
    Width = 349
    Height = 153
    Caption = 'View'
    TabOrder = 2
    object GroupBox4: TGroupBox
      Left = 12
      Top = 16
      Width = 157
      Height = 97
      Caption = 'Current'
      TabOrder = 0
      object Label5: TLabel
        Left = 12
        Top = 20
        Width = 38
        Height = 13
        Caption = 'Latitude'
      end
      object Label6: TLabel
        Left = 12
        Top = 44
        Width = 47
        Height = 13
        Caption = 'Longitude'
      end
      object Label7: TLabel
        Left = 12
        Top = 68
        Width = 62
        Height = 13
        Caption = 'DiagonalSize'
      end
      object CLatEd: TEdit
        Left = 76
        Top = 16
        Width = 65
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 0
      end
      object CLongEd: TEdit
        Left = 76
        Top = 40
        Width = 65
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 1
      end
      object CSizeEd: TEdit
        Left = 92
        Top = 64
        Width = 49
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 2
      end
    end
    object GroupBox5: TGroupBox
      Left = 180
      Top = 16
      Width = 157
      Height = 97
      Caption = 'File'
      TabOrder = 1
      object Label8: TLabel
        Left = 12
        Top = 20
        Width = 38
        Height = 13
        Caption = 'Latitude'
      end
      object Label9: TLabel
        Left = 12
        Top = 44
        Width = 47
        Height = 13
        Caption = 'Longitude'
      end
      object Label10: TLabel
        Left = 12
        Top = 68
        Width = 62
        Height = 13
        Caption = 'DiagonalSize'
      end
      object NLatEd: TEdit
        Left = 76
        Top = 16
        Width = 65
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 0
      end
      object NLongEd: TEdit
        Left = 76
        Top = 40
        Width = 65
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 1
      end
      object NSizeEd: TEdit
        Left = 92
        Top = 64
        Width = 49
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 2
      end
    end
    object ViewChk: TCheckBox
      Left = 12
      Top = 124
      Width = 197
      Height = 17
      Caption = 'Replace Current View with File View'
      TabOrder = 2
    end
  end
  object OKBut: TBitBtn
    Left = 196
    Top = 276
    Width = 75
    Height = 25
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 3
  end
  object CancelBut: TBitBtn
    Left = 288
    Top = 276
    Width = 75
    Height = 25
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 4
  end
end

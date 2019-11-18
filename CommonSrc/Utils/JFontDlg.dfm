object FontForm: TFontForm
  Left = 326
  Top = 182
  BorderStyle = bsDialog
  Caption = 'FontForm'
  ClientHeight = 193
  ClientWidth = 374
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
  object Panel1: TPanel
    Left = 12
    Top = 12
    Width = 353
    Height = 137
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 8
      Width = 52
      Height = 13
      Caption = 'Font Name'
    end
    object Label2: TLabel
      Left = 172
      Top = 8
      Width = 47
      Height = 13
      Caption = 'Font Style'
    end
    object Label3: TLabel
      Left = 272
      Top = 24
      Width = 20
      Height = 13
      Caption = 'Size'
    end
    object Bevel1: TBevel
      Left = 16
      Top = 56
      Width = 145
      Height = 65
    end
    object PaintBox: TPaintBox
      Left = 16
      Top = 56
      Width = 145
      Height = 65
      Color = clWhite
      ParentColor = False
      OnPaint = PaintBoxPaint
    end
    object FontCombo: TComboBox
      Left = 16
      Top = 24
      Width = 145
      Height = 21
      Style = csDropDownList
      Sorted = True
      TabOrder = 0
      OnChange = FontComboChange
    end
    object GroupBox1: TGroupBox
      Left = 172
      Top = 20
      Width = 89
      Height = 101
      TabOrder = 1
      object BoldChk: TCheckBox
        Left = 12
        Top = 12
        Width = 53
        Height = 17
        Caption = 'Bold'
        TabOrder = 0
        OnClick = BoldChkClick
      end
      object ItalicChk: TCheckBox
        Left = 12
        Top = 32
        Width = 49
        Height = 17
        Caption = 'Italic'
        TabOrder = 1
        OnClick = ItalicChkClick
      end
      object StrikeChk: TCheckBox
        Left = 12
        Top = 52
        Width = 69
        Height = 17
        Caption = 'Strikeout'
        TabOrder = 2
        OnClick = StrikeChkClick
      end
      object UnderChk: TCheckBox
        Left = 12
        Top = 72
        Width = 69
        Height = 17
        Caption = 'Underline'
        TabOrder = 3
        OnClick = UnderChkClick
      end
    end
    object SizeEd: TEdit
      Left = 300
      Top = 20
      Width = 25
      Height = 21
      TabOrder = 2
      Text = '5'
      OnChange = SizeEdChange
    end
    object SizeUD: TUpDown
      Left = 325
      Top = 20
      Width = 15
      Height = 21
      Associate = SizeEd
      Min = 5
      Max = 200
      Position = 5
      TabOrder = 3
    end
    object ColPan: TPanel
      Left = 276
      Top = 52
      Width = 65
      Height = 69
      Cursor = crHandPoint
      Color = clYellow
      ParentBackground = False
      TabOrder = 4
      OnClick = ColPanClick
    end
  end
  object OKBut: TBitBtn
    Left = 196
    Top = 160
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 1
  end
  object CancelBut: TBitBtn
    Left = 288
    Top = 160
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 2
  end
end

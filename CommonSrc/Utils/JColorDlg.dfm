object ColorForm: TColorForm
  Left = 289
  Top = 180
  BorderStyle = bsDialog
  Caption = 'Select Colour'
  ClientHeight = 302
  ClientWidth = 449
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 240
    Width = 8
    Height = 13
    Caption = 'R'
  end
  object Label2: TLabel
    Left = 52
    Top = 240
    Width = 8
    Height = 13
    Caption = 'G'
  end
  object Label3: TLabel
    Left = 96
    Top = 240
    Width = 7
    Height = 13
    Caption = 'B'
  end
  object PrintLab: TLabel
    Left = 152
    Top = 240
    Width = 54
    Height = 13
    Caption = 'Print Colour'
  end
  object Panel1: TPanel
    Left = 12
    Top = 12
    Width = 285
    Height = 215
    ParentBackground = False
    TabOrder = 0
    object PaintBox: TPaintBox
      Left = 1
      Top = 1
      Width = 283
      Height = 213
      Align = alClient
      OnPaint = PaintBoxPaint
    end
  end
  object OKBut: TBitBtn
    Left = 260
    Top = 268
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 1
  end
  object CancelBut: TBitBtn
    Left = 348
    Top = 268
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 2
  end
  object PrintPan: TPanel
    Left = 236
    Top = 236
    Width = 61
    Height = 21
    Cursor = crHandPoint
    ParentBackground = False
    TabOrder = 3
    OnClick = PrintPanClick
  end
  object GroupBox1: TGroupBox
    Left = 304
    Top = 12
    Width = 137
    Height = 245
    TabOrder = 4
    object RedLab: TLabel
      Left = 12
      Top = 228
      Width = 8
      Height = 13
      Caption = 'R'
    end
    object GreenLab: TLabel
      Left = 56
      Top = 228
      Width = 8
      Height = 13
      Caption = 'G'
    end
    object BlueLab: TLabel
      Left = 100
      Top = 228
      Width = 7
      Height = 13
      Caption = 'B'
    end
    object RedTB: TTrackBar
      Left = 8
      Top = 51
      Width = 21
      Height = 178
      Max = 255
      Orientation = trVertical
      TabOrder = 0
      TickMarks = tmBoth
      TickStyle = tsNone
      OnChange = RedTBChange
    end
    object UserChk: TCheckBox
      Left = 12
      Top = 8
      Width = 125
      Height = 17
      Caption = 'User Defined'
      TabOrder = 1
      OnClick = UserChkClick
    end
    object GreenTB: TTrackBar
      Left = 52
      Top = 51
      Width = 21
      Height = 178
      Max = 255
      Orientation = trVertical
      TabOrder = 2
      TickMarks = tmBoth
      TickStyle = tsNone
      OnChange = GreenTBChange
    end
    object BlueTB: TTrackBar
      Left = 96
      Top = 51
      Width = 21
      Height = 178
      Max = 255
      Orientation = trVertical
      TabOrder = 3
      TickMarks = tmBoth
      TickStyle = tsNone
      OnChange = BlueTBChange
    end
    object UserPan: TPanel
      Left = 12
      Top = 28
      Width = 105
      Height = 21
      Color = clYellow
      ParentBackground = False
      TabOrder = 4
    end
  end
  object RedEd: TEdit
    Left = 21
    Top = 237
    Width = 25
    Height = 21
    Color = cl3DLight
    TabOrder = 5
  end
  object GreenEd: TEdit
    Left = 65
    Top = 237
    Width = 25
    Height = 21
    Color = cl3DLight
    TabOrder = 6
  end
  object BlueEd: TEdit
    Left = 109
    Top = 237
    Width = 25
    Height = 21
    Color = cl3DLight
    TabOrder = 7
  end
end

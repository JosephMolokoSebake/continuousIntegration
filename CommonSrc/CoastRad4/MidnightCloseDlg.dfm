object MidnightCloseForm: TMidnightCloseForm
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'MidnightCloseForm'
  ClientHeight = 116
  ClientWidth = 283
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 8
    Width = 257
    Height = 37
    Caption = 'Program Closing'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object ProgBar: TProgressBar
    Left = 16
    Top = 48
    Width = 257
    Height = 17
    Max = 60
    Smooth = True
    TabOrder = 0
  end
  object CancelBut: TBitBtn
    Left = 96
    Top = 76
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 1
    OnClick = CancelButClick
  end
end

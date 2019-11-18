object MapDisplayForm: TMapDisplayForm
  Left = 335
  Top = 264
  BorderStyle = bsDialog
  Caption = 'Map Display Colours to Print/Metafile Colours'
  ClientHeight = 275
  ClientWidth = 445
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
  object OKBut: TBitBtn
    Left = 252
    Top = 240
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 0
  end
  object CancelBut: TBitBtn
    Left = 348
    Top = 240
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 1
  end
  object Panel: TPanel
    Left = 8
    Top = 8
    Width = 429
    Height = 217
    TabOrder = 2
  end
  object ResetBut: TButton
    Left = 8
    Top = 232
    Width = 53
    Height = 21
    Cursor = crHandPoint
    Caption = 'Reset'
    TabOrder = 3
    OnClick = ResetButClick
  end
end

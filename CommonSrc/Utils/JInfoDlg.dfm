object JInfoForm: TJInfoForm
  Left = 527
  Top = 388
  BorderStyle = bsDialog
  Caption = 'JInfoForm'
  ClientHeight = 85
  ClientWidth = 344
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
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 329
    Height = 29
  end
  object InfoLab: TLabel
    Left = 16
    Top = 16
    Width = 44
    Height = 13
    Caption = 'InfoLab'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object OKBut: TBitBtn
    Left = 176
    Top = 48
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 0
  end
  object CancelBut: TBitBtn
    Left = 260
    Top = 48
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 1
  end
  object PB: TProgressBar
    Left = 8
    Top = 56
    Width = 157
    Height = 9
    Smooth = True
    TabOrder = 2
  end
  object Timer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = TimerTimer
    Left = 84
    Top = 44
  end
end

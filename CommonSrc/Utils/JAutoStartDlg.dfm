object AutoStartForm: TAutoStartForm
  Left = 337
  Top = 433
  BorderStyle = bsDialog
  Caption = 'Automatic Capture Start'
  ClientHeight = 93
  ClientWidth = 391
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 377
    Height = 41
    TabOrder = 0
    object Bar: TProgressBar
      Left = 12
      Top = 12
      Width = 353
      Height = 16
      TabOrder = 0
    end
  end
  object CancelBut: TBitBtn
    Left = 160
    Top = 60
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 1
  end
  object SkipBut: TButton
    Left = 308
    Top = 60
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Caption = 'Skip This >>'
    TabOrder = 2
    OnClick = SkipButClick
  end
  object Timer: TTimer
    Interval = 100
    OnTimer = TimerTimer
    Left = 248
    Top = 60
  end
end

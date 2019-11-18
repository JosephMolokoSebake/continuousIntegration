object NMEAForm: TNMEAForm
  Left = 272
  Top = 175
  BorderStyle = bsDialog
  Caption = 'NMEA Messages'
  ClientHeight = 465
  ClientWidth = 626
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
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 440
    Width = 51
    Height = 13
    Caption = 'Sentences'
  end
  object Label2: TLabel
    Left = 200
    Top = 440
    Width = 80
    Height = 13
    Caption = 'Checksum Errors'
  end
  object Memo: TMemo
    Left = 8
    Top = 12
    Width = 609
    Height = 413
    Lines.Strings = (
      'Memo')
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object StoreBut: TButton
    Left = 444
    Top = 436
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Caption = 'Store'
    TabOrder = 1
    OnClick = StoreButClick
  end
  object ClearBut: TButton
    Left = 136
    Top = 436
    Width = 45
    Height = 21
    Cursor = crHandPoint
    Caption = 'Clear'
    TabOrder = 2
    OnClick = ClearButClick
  end
  object CloseBut: TBitBtn
    Left = 536
    Top = 436
    Width = 75
    Height = 25
    Cursor = crHandPoint
    DoubleBuffered = True
    Kind = bkClose
    ParentDoubleBuffered = False
    TabOrder = 3
  end
  object SentenceEd: JNumEdit
    Left = 64
    Top = 436
    Width = 61
    Height = 21
    Color = cl3DLight
    MaxLength = 8
    ReadOnly = True
    TabOrder = 4
    MaxValue = 99999999.000000000000000000
    BlankValue = -1.000000000000000000
  end
  object CheckEd: JNumEdit
    Left = 288
    Top = 436
    Width = 61
    Height = 21
    Color = cl3DLight
    MaxLength = 8
    ReadOnly = True
    TabOrder = 5
    MaxValue = 99999999.000000000000000000
    BlankValue = -1.000000000000000000
  end
  object Timer: TTimer
    Enabled = False
    Interval = 20
    OnTimer = TimerTimer
    Left = 404
    Top = 436
  end
end

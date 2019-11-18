object GPSTimeRX101Form: TGPSTimeRX101Form
  Left = 353
  Top = 378
  BorderStyle = bsDialog
  Caption = 'GPS Time and Position'
  ClientHeight = 366
  ClientWidth = 448
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 312
    Width = 161
    Height = 45
  end
  object Bevel2: TBevel
    Left = 8
    Top = 68
    Width = 429
    Height = 237
  end
  object Label3: TLabel
    Left = 16
    Top = 84
    Width = 114
    Height = 37
    Caption = 'Latitude'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 16
    Top = 136
    Width = 141
    Height = 37
    Caption = 'Longitude'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label1: TLabel
    Left = 24
    Top = 276
    Width = 73
    Height = 13
    Caption = 'ZDA Messages'
  end
  object Label2: TLabel
    Left = 312
    Top = 276
    Width = 52
    Height = 13
    Caption = 'CRC Errors'
  end
  object Label5: TLabel
    Left = 16
    Top = 328
    Width = 64
    Height = 13
    Caption = 'Ship Heading'
  end
  object Label6: TLabel
    Left = 128
    Top = 328
    Width = 18
    Height = 13
    Caption = 'deg'
  end
  object Label7: TLabel
    Left = 324
    Top = 192
    Width = 70
    Height = 13
    Caption = 'Satellites Used'
  end
  object Label8: TLabel
    Left = 20
    Top = 192
    Width = 48
    Height = 13
    Caption = 'UTC Time'
  end
  object Label9: TLabel
    Left = 232
    Top = 192
    Width = 50
    Height = 13
    Caption = 'Differential'
  end
  object Label10: TLabel
    Left = 216
    Top = 240
    Width = 31
    Height = 13
    Caption = 'HDOP'
  end
  object Label11: TLabel
    Left = 20
    Top = 240
    Width = 35
    Height = 13
    Caption = 'Altitude'
  end
  object Label12: TLabel
    Left = 120
    Top = 240
    Width = 8
    Height = 13
    Caption = 'm'
  end
  object Bevel3: TBevel
    Left = 20
    Top = 264
    Width = 401
    Height = 2
  end
  object Label13: TLabel
    Left = 168
    Top = 276
    Width = 73
    Height = 13
    Caption = 'POS Messages'
  end
  object Label14: TLabel
    Left = 132
    Top = 192
    Width = 23
    Height = 13
    Caption = 'Date'
  end
  object Label15: TLabel
    Left = 20
    Top = 216
    Width = 33
    Height = 13
    Caption = 'Course'
  end
  object Label16: TLabel
    Left = 144
    Top = 216
    Width = 31
    Height = 13
    Caption = 'Speed'
  end
  object Label17: TLabel
    Left = 252
    Top = 216
    Width = 75
    Height = 13
    Caption = 'Vertical Velocity'
  end
  object Label18: TLabel
    Left = 112
    Top = 216
    Width = 18
    Height = 13
    Caption = 'deg'
  end
  object Label19: TLabel
    Left = 220
    Top = 216
    Width = 14
    Height = 13
    Caption = 'kts'
  end
  object Label20: TLabel
    Left = 376
    Top = 216
    Width = 18
    Height = 13
    Caption = 'm/s'
  end
  object Label21: TLabel
    Left = 148
    Top = 240
    Width = 30
    Height = 13
    Caption = 'PDOP'
  end
  object Label22: TLabel
    Left = 288
    Top = 240
    Width = 30
    Height = 13
    Caption = 'VDOP'
  end
  object Label23: TLabel
    Left = 356
    Top = 240
    Width = 30
    Height = 13
    Caption = 'TDOP'
  end
  object Label24: TLabel
    Left = 300
    Top = 40
    Width = 78
    Height = 13
    Caption = 'Time CRC Errors'
  end
  object TimeEd: TEdit
    Left = 16
    Top = 12
    Width = 205
    Height = 45
    Color = cl3DLight
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 0
  end
  object LatEd: TEdit
    Left = 172
    Top = 80
    Width = 185
    Height = 45
    Color = cl3DLight
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 1
  end
  object LongEd: TEdit
    Left = 172
    Top = 132
    Width = 185
    Height = 45
    Color = cl3DLight
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 2
  end
  object ClkPan: TPanel
    Left = 240
    Top = 8
    Width = 113
    Height = 21
    Caption = 'HDLC Clock Missing'
    Color = clRed
    TabOrder = 3
  end
  object ZDAMsgEd: TEdit
    Left = 104
    Top = 272
    Width = 49
    Height = 21
    Color = cl3DLight
    MaxLength = 9
    ReadOnly = True
    TabOrder = 4
  end
  object CRCEd: TEdit
    Left = 368
    Top = 272
    Width = 53
    Height = 21
    Color = cl3DLight
    MaxLength = 9
    ReadOnly = True
    TabOrder = 5
  end
  object HeadEd: TEdit
    Left = 84
    Top = 324
    Width = 41
    Height = 21
    Color = cl3DLight
    MaxLength = 6
    ReadOnly = True
    TabOrder = 6
    Text = '0.0'
  end
  object SatEd: TEdit
    Left = 400
    Top = 188
    Width = 21
    Height = 21
    Color = cl3DLight
    MaxLength = 6
    ReadOnly = True
    TabOrder = 7
  end
  object AltEd: TEdit
    Left = 72
    Top = 236
    Width = 45
    Height = 21
    Color = cl3DLight
    MaxLength = 6
    ReadOnly = True
    TabOrder = 8
  end
  object UTCTimeEd: TEdit
    Left = 72
    Top = 188
    Width = 50
    Height = 21
    Color = cl3DLight
    MaxLength = 8
    ReadOnly = True
    TabOrder = 9
  end
  object HDOPEd: TEdit
    Left = 248
    Top = 236
    Width = 29
    Height = 21
    Color = cl3DLight
    MaxLength = 6
    ReadOnly = True
    TabOrder = 10
  end
  object DiffEd: TEdit
    Left = 288
    Top = 188
    Width = 29
    Height = 21
    Color = cl3DLight
    ReadOnly = True
    TabOrder = 11
  end
  object UpdateTimeBut: TButton
    Left = 216
    Top = 324
    Width = 113
    Height = 25
    Cursor = crHandPoint
    Caption = 'Update PC Time'
    TabOrder = 12
    OnClick = UpdateTimeButClick
  end
  object POSMsgEd: TEdit
    Left = 248
    Top = 272
    Width = 49
    Height = 21
    Color = cl3DLight
    MaxLength = 9
    ReadOnly = True
    TabOrder = 13
  end
  object UTCDateEd: TEdit
    Left = 160
    Top = 188
    Width = 65
    Height = 21
    Color = cl3DLight
    MaxLength = 10
    ReadOnly = True
    TabOrder = 14
  end
  object CourseEd: TEdit
    Left = 72
    Top = 212
    Width = 33
    Height = 21
    Color = cl3DLight
    MaxLength = 6
    ReadOnly = True
    TabOrder = 15
  end
  object SpeedEd: TEdit
    Left = 180
    Top = 212
    Width = 37
    Height = 21
    Color = cl3DLight
    MaxLength = 6
    ReadOnly = True
    TabOrder = 16
  end
  object VertVelEd: TEdit
    Left = 336
    Top = 212
    Width = 37
    Height = 21
    Color = cl3DLight
    MaxLength = 6
    ReadOnly = True
    TabOrder = 17
  end
  object PDOPEd: TEdit
    Left = 180
    Top = 236
    Width = 29
    Height = 21
    Color = cl3DLight
    MaxLength = 6
    ReadOnly = True
    TabOrder = 18
  end
  object VDOPEd: TEdit
    Left = 320
    Top = 236
    Width = 29
    Height = 21
    Color = cl3DLight
    MaxLength = 6
    ReadOnly = True
    TabOrder = 19
  end
  object TDOPEd: TEdit
    Left = 388
    Top = 236
    Width = 29
    Height = 21
    Color = cl3DLight
    MaxLength = 6
    ReadOnly = True
    TabOrder = 20
  end
  object TimeCRCEd: TEdit
    Left = 384
    Top = 36
    Width = 53
    Height = 21
    Color = cl3DLight
    MaxLength = 9
    ReadOnly = True
    TabOrder = 21
  end
  object Timer: TTimer
    Enabled = False
    Interval = 50
    OnTimer = TimerTimer
    Left = 344
    Top = 324
  end
end

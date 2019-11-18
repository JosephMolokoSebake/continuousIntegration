object GPSTimeRX200Form: TGPSTimeRX200Form
  Left = 381
  Top = 265
  BorderStyle = bsDialog
  Caption = 'GPS Time and Position'
  ClientHeight = 352
  ClientWidth = 384
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
    Top = 300
    Width = 161
    Height = 45
  end
  object Bevel2: TBevel
    Left = 8
    Top = 116
    Width = 365
    Height = 177
  end
  object Label3: TLabel
    Left = 16
    Top = 132
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
    Top = 184
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
  object Label2: TLabel
    Left = 248
    Top = 264
    Width = 52
    Height = 13
    Caption = 'CRC Errors'
  end
  object Label5: TLabel
    Left = 16
    Top = 316
    Width = 64
    Height = 13
    Caption = 'Ship Heading'
  end
  object Label6: TLabel
    Left = 128
    Top = 316
    Width = 18
    Height = 13
    Caption = 'deg'
  end
  object Label7: TLabel
    Left = 216
    Top = 236
    Width = 42
    Height = 13
    Caption = 'Satellites'
  end
  object Label9: TLabel
    Left = 16
    Top = 236
    Width = 50
    Height = 13
    Caption = 'Differential'
  end
  object Label10: TLabel
    Left = 292
    Top = 236
    Width = 31
    Height = 13
    Caption = 'HDOP'
  end
  object Label11: TLabel
    Left = 112
    Top = 236
    Width = 35
    Height = 13
    Caption = 'Altitude'
  end
  object Label12: TLabel
    Left = 200
    Top = 236
    Width = 8
    Height = 13
    Caption = 'm'
  end
  object Label24: TLabel
    Left = 244
    Top = 92
    Width = 52
    Height = 13
    Caption = 'CRC Errors'
  end
  object Label1: TLabel
    Left = 16
    Top = 264
    Width = 74
    Height = 13
    Caption = 'GGA Messages'
  end
  object Label8: TLabel
    Left = 264
    Top = 44
    Width = 46
    Height = 13
    Caption = 'Time Bias'
  end
  object Label13: TLabel
    Left = 344
    Top = 44
    Width = 6
    Height = 13
    Caption = 'h'
  end
  object TimeEd: TEdit
    Left = 8
    Top = 12
    Width = 245
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
    Top = 128
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
    Top = 180
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
    Left = 260
    Top = 12
    Width = 113
    Height = 21
    Caption = 'HDLC Clock Missing'
    Color = clRed
    TabOrder = 3
  end
  object CRCEd: TEdit
    Left = 304
    Top = 260
    Width = 53
    Height = 21
    Color = cl3DLight
    MaxLength = 9
    ReadOnly = True
    TabOrder = 4
  end
  object HeadEd: TEdit
    Left = 84
    Top = 312
    Width = 41
    Height = 21
    Color = cl3DLight
    MaxLength = 6
    ReadOnly = True
    TabOrder = 5
    Text = '0.0'
  end
  object SatEd: TEdit
    Left = 260
    Top = 232
    Width = 21
    Height = 21
    Color = cl3DLight
    MaxLength = 6
    ReadOnly = True
    TabOrder = 6
  end
  object AltEd: TEdit
    Left = 152
    Top = 232
    Width = 45
    Height = 21
    Color = cl3DLight
    MaxLength = 6
    ReadOnly = True
    TabOrder = 7
  end
  object HDOPEd: TEdit
    Left = 328
    Top = 232
    Width = 29
    Height = 21
    Color = cl3DLight
    MaxLength = 6
    ReadOnly = True
    TabOrder = 8
  end
  object DiffEd: TEdit
    Left = 72
    Top = 232
    Width = 29
    Height = 21
    Color = cl3DLight
    ReadOnly = True
    TabOrder = 9
  end
  object UpdateTimeBut: TButton
    Left = 216
    Top = 308
    Width = 113
    Height = 25
    Cursor = crHandPoint
    Caption = 'Update PC Time'
    TabOrder = 10
    OnClick = UpdateTimeButClick
  end
  object TimeCRCEd: TEdit
    Left = 300
    Top = 88
    Width = 53
    Height = 21
    Color = cl3DLight
    MaxLength = 9
    ReadOnly = True
    TabOrder = 11
  end
  object DateEd: TEdit
    Left = 8
    Top = 64
    Width = 169
    Height = 45
    Color = cl3DLight
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 12
  end
  object GGAMsgEd: TEdit
    Left = 96
    Top = 260
    Width = 53
    Height = 21
    Color = cl3DLight
    MaxLength = 9
    ReadOnly = True
    TabOrder = 13
  end
  object TimeBiasEd: TEdit
    Left = 316
    Top = 40
    Width = 25
    Height = 21
    Color = cl3DLight
    MaxLength = 6
    ReadOnly = True
    TabOrder = 14
  end
  object Timer: TTimer
    Enabled = False
    Interval = 50
    OnTimer = TimerTimer
    Left = 336
    Top = 304
  end
end

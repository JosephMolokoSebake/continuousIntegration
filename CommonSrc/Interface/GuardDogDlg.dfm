object GuardDogForm: TGuardDogForm
  Left = 0
  Top = 0
  Caption = 'GuardDog Test'
  ClientHeight = 439
  ClientWidth = 638
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 456
    Top = 15
    Width = 113
    Height = 13
    Caption = 'Firmware Serial Number'
  end
  object SerialEd: JNumEdit
    Left = 575
    Top = 12
    Width = 34
    Height = 21
    Color = cl3DLight
    MaxLength = 6
    ReadOnly = True
    TabOrder = 0
    MaxValue = 999999.000000000000000000
    BlankValue = -1.000000000000000000
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 233
    Width = 622
    Height = 196
    Caption = 'GPS Interface'
    TabOrder = 1
    object Label7: TLabel
      Left = 423
      Top = 27
      Width = 118
      Height = 13
      Caption = 'NAV-TIMEUTC Messages'
    end
    object GPSBut: TButton
      Left = 17
      Top = 163
      Width = 75
      Height = 25
      Cursor = crHandPoint
      Caption = 'Start GPS'
      TabOrder = 0
      OnClick = GPSButClick
    end
    object StopGPSBut: TButton
      Left = 106
      Top = 163
      Width = 75
      Height = 25
      Cursor = crHandPoint
      Caption = 'Stop GPS'
      TabOrder = 1
      OnClick = StopGPSButClick
    end
    object GPSTimeEd: TEdit
      Left = 203
      Top = 24
      Width = 214
      Height = 45
      Color = cl3DLight
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -32
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ReadOnly = True
      TabOrder = 2
    end
    object GPSDateEd: TEdit
      Left = 13
      Top = 24
      Width = 184
      Height = 45
      Color = cl3DLight
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -32
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ReadOnly = True
      TabOrder = 3
    end
    object GPSLatEd: TEdit
      Left = 13
      Top = 106
      Width = 184
      Height = 45
      Color = cl3DLight
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -32
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ReadOnly = True
      TabOrder = 4
    end
    object GPSLongEd: TEdit
      Left = 203
      Top = 106
      Width = 214
      Height = 45
      Color = cl3DLight
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -32
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ReadOnly = True
      TabOrder = 5
    end
    object GPSHeightEd: TEdit
      Left = 421
      Top = 106
      Width = 193
      Height = 45
      Color = cl3DLight
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -32
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ReadOnly = True
      TabOrder = 6
    end
    object GPSMsgCntEd: JNumEdit
      Left = 547
      Top = 24
      Width = 64
      Height = 21
      Color = cl3DLight
      MaxLength = 8
      ReadOnly = True
      TabOrder = 7
      MaxValue = 99999999.000000000000000000
      BlankValue = -1.000000000000000000
      DataType = dtDWord
    end
    object TimePan: TPanel
      Left = 477
      Top = 51
      Width = 130
      Height = 23
      Color = clYellow
      ParentBackground = False
      TabOrder = 8
    end
    object DatePan: TPanel
      Left = 477
      Top = 77
      Width = 130
      Height = 23
      Color = clYellow
      ParentBackground = False
      TabOrder = 9
    end
    object LeapPan: TPanel
      Left = 341
      Top = 77
      Width = 130
      Height = 23
      Color = clYellow
      ParentBackground = False
      TabOrder = 10
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 8
    Width = 137
    Height = 215
    Caption = 'Watchdog (PC)'
    TabOrder = 2
    object Label2: TLabel
      Left = 16
      Top = 21
      Width = 38
      Height = 13
      Caption = 'Timeout'
    end
    object Label3: TLabel
      Left = 103
      Top = 21
      Width = 16
      Height = 13
      Caption = 'sec'
    end
    object Label4: TLabel
      Left = 16
      Top = 111
      Width = 44
      Height = 13
      Caption = 'Time Left'
    end
    object Label5: TLabel
      Left = 109
      Top = 111
      Width = 16
      Height = 13
      Caption = 'sec'
    end
    object EnWatch0But: TButton
      Left = 12
      Top = 45
      Width = 113
      Height = 25
      Cursor = crHandPoint
      Caption = 'Enable Watchdog'
      TabOrder = 0
      OnClick = EnWatch0ButClick
    end
    object DisWatch0But: TButton
      Left = 12
      Top = 76
      Width = 113
      Height = 25
      Cursor = crHandPoint
      Caption = 'Disable Watchdog'
      TabOrder = 1
      OnClick = DisWatch0ButClick
    end
    object WatchTime0Ed: JNumEdit
      Left = 60
      Top = 18
      Width = 37
      Height = 21
      MaxLength = 5
      TabOrder = 2
      Text = '10'
      MinValue = 1.000000000000000000
      MaxValue = 65535.000000000000000000
      BlankValue = -1.000000000000000000
    end
    object WatchLeft0Ed: JNumEdit
      Left = 66
      Top = 107
      Width = 37
      Height = 21
      Color = cl3DLight
      MaxLength = 6
      ReadOnly = True
      TabOrder = 3
      MaxValue = 999999.000000000000000000
      BlankValue = -1.000000000000000000
    end
    object Trig0But: TButton
      Left = 13
      Top = 157
      Width = 112
      Height = 45
      Cursor = crHandPoint
      Caption = 'Trigger'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      OnClick = Trig0ButClick
    end
    object Status0Ed: TEdit
      Left = 13
      Top = 134
      Width = 113
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 5
    end
  end
  object GSMPan: TPanel
    Left = 460
    Top = 103
    Width = 129
    Height = 33
    Caption = 'GSM'
    Color = clYellow
    ParentBackground = False
    TabOrder = 3
  end
  object GroupBox4: TGroupBox
    Left = 157
    Top = 8
    Width = 137
    Height = 215
    Caption = 'Watchdog (Laptop 1)'
    TabOrder = 4
    object Label6: TLabel
      Left = 16
      Top = 21
      Width = 38
      Height = 13
      Caption = 'Timeout'
    end
    object Label8: TLabel
      Left = 103
      Top = 21
      Width = 16
      Height = 13
      Caption = 'sec'
    end
    object Label9: TLabel
      Left = 16
      Top = 111
      Width = 44
      Height = 13
      Caption = 'Time Left'
    end
    object Label10: TLabel
      Left = 109
      Top = 111
      Width = 16
      Height = 13
      Caption = 'sec'
    end
    object EnWatch1But: TButton
      Left = 12
      Top = 45
      Width = 113
      Height = 25
      Cursor = crHandPoint
      Caption = 'Enable Watchdog'
      TabOrder = 0
      OnClick = EnWatch1ButClick
    end
    object DisWatch1But: TButton
      Left = 12
      Top = 76
      Width = 113
      Height = 25
      Cursor = crHandPoint
      Caption = 'Disable Watchdog'
      TabOrder = 1
      OnClick = DisWatch1ButClick
    end
    object WatchTime1Ed: JNumEdit
      Left = 60
      Top = 18
      Width = 37
      Height = 21
      MaxLength = 5
      TabOrder = 2
      Text = '10'
      MinValue = 1.000000000000000000
      MaxValue = 65535.000000000000000000
      BlankValue = -1.000000000000000000
    end
    object WatchLeft1Ed: JNumEdit
      Left = 66
      Top = 107
      Width = 37
      Height = 21
      Color = cl3DLight
      MaxLength = 6
      ReadOnly = True
      TabOrder = 3
      MaxValue = 999999.000000000000000000
      BlankValue = -1.000000000000000000
    end
    object Trig1But: TButton
      Left = 13
      Top = 157
      Width = 112
      Height = 45
      Cursor = crHandPoint
      Caption = 'Trigger'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      OnClick = Trig1ButClick
    end
    object Status1Ed: TEdit
      Left = 13
      Top = 134
      Width = 113
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 5
    end
  end
  object GroupBox5: TGroupBox
    Left = 305
    Top = 8
    Width = 137
    Height = 215
    Caption = 'Watchdog (Laptop 2)'
    TabOrder = 5
    object Label11: TLabel
      Left = 16
      Top = 21
      Width = 38
      Height = 13
      Caption = 'Timeout'
    end
    object Label12: TLabel
      Left = 103
      Top = 21
      Width = 16
      Height = 13
      Caption = 'sec'
    end
    object Label13: TLabel
      Left = 16
      Top = 111
      Width = 44
      Height = 13
      Caption = 'Time Left'
    end
    object Label14: TLabel
      Left = 109
      Top = 111
      Width = 16
      Height = 13
      Caption = 'sec'
    end
    object EnWatch2But: TButton
      Left = 12
      Top = 45
      Width = 113
      Height = 25
      Cursor = crHandPoint
      Caption = 'Enable Watchdog'
      TabOrder = 0
      OnClick = EnWatch2ButClick
    end
    object DisWatch2But: TButton
      Left = 12
      Top = 76
      Width = 113
      Height = 25
      Cursor = crHandPoint
      Caption = 'Disable Watchdog'
      TabOrder = 1
      OnClick = DisWatch2ButClick
    end
    object WatchTime2Ed: JNumEdit
      Left = 60
      Top = 18
      Width = 37
      Height = 21
      MaxLength = 5
      TabOrder = 2
      Text = '10'
      MinValue = 1.000000000000000000
      MaxValue = 65535.000000000000000000
      BlankValue = -1.000000000000000000
    end
    object WatchLeft2Ed: JNumEdit
      Left = 66
      Top = 107
      Width = 37
      Height = 21
      Color = cl3DLight
      MaxLength = 6
      ReadOnly = True
      TabOrder = 3
      MaxValue = 999999.000000000000000000
      BlankValue = -1.000000000000000000
    end
    object Trig2But: TButton
      Left = 13
      Top = 157
      Width = 112
      Height = 45
      Cursor = crHandPoint
      Caption = 'Trigger'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      OnClick = Trig2ButClick
    end
    object Status2Ed: TEdit
      Left = 13
      Top = 134
      Width = 113
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 5
    end
  end
  object GPSTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = GPSTimerTimer
    Left = 504
    Top = 196
  end
  object WatchTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = WatchTimerTimer
    Left = 456
    Top = 192
  end
end

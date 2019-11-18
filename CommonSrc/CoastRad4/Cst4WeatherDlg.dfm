object WeatherForm: TWeatherForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Weather Station Information'
  ClientHeight = 379
  ClientWidth = 822
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox4: TGroupBox
    Left = 396
    Top = 4
    Width = 417
    Height = 365
    Caption = 'Metereological Information'
    TabOrder = 0
    object GroupBox3: TGroupBox
      Left = 12
      Top = 92
      Width = 405
      Height = 265
      Caption = 'Weather Information'
      TabOrder = 0
      object Label5: TLabel
        Left = 12
        Top = 24
        Width = 215
        Height = 29
        Caption = 'Barometric Pressure'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label6: TLabel
        Left = 352
        Top = 24
        Width = 35
        Height = 29
        Caption = 'bar'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label7: TLabel
        Left = 16
        Top = 64
        Width = 174
        Height = 29
        Caption = 'Air Temperature'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label8: TLabel
        Left = 352
        Top = 64
        Width = 25
        Height = 29
        Caption = #176'C'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label9: TLabel
        Left = 16
        Top = 104
        Width = 184
        Height = 29
        Caption = 'Relative Humidity'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label10: TLabel
        Left = 352
        Top = 104
        Width = 23
        Height = 29
        Caption = '%'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label11: TLabel
        Left = 16
        Top = 144
        Width = 107
        Height = 29
        Caption = 'Dew Point'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label12: TLabel
        Left = 352
        Top = 144
        Width = 25
        Height = 29
        Caption = #176'C'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label13: TLabel
        Left = 16
        Top = 184
        Width = 154
        Height = 29
        Caption = 'Wind Direction'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label14: TLabel
        Left = 16
        Top = 224
        Width = 126
        Height = 29
        Caption = 'Wind Speed'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label15: TLabel
        Left = 352
        Top = 184
        Width = 39
        Height = 29
        Caption = 'deg'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label16: TLabel
        Left = 352
        Top = 224
        Width = 31
        Height = 29
        Caption = 'kts'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object BarEd: TEdit
        Left = 236
        Top = 20
        Width = 109
        Height = 37
        Color = cl3DLight
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
      end
      object TempEd: TEdit
        Left = 236
        Top = 60
        Width = 109
        Height = 37
        Color = cl3DLight
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 1
      end
      object HumidEd: TEdit
        Left = 236
        Top = 100
        Width = 109
        Height = 37
        Color = cl3DLight
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 2
      end
      object DewEd: TEdit
        Left = 236
        Top = 140
        Width = 109
        Height = 37
        Color = cl3DLight
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 3
      end
      object DirEd: TEdit
        Left = 236
        Top = 180
        Width = 109
        Height = 37
        Color = cl3DLight
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 4
      end
      object SpeedEd: TEdit
        Left = 236
        Top = 220
        Width = 109
        Height = 37
        Color = cl3DLight
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 5
      end
    end
    object GroupBox2: TGroupBox
      Left = 276
      Top = 16
      Width = 141
      Height = 45
      Caption = 'Last Update'
      TabOrder = 1
      object DateEd: TEdit
        Left = 12
        Top = 16
        Width = 65
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 0
      end
      object TimeEd: TEdit
        Left = 84
        Top = 16
        Width = 50
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 1
      end
    end
    object GroupBox1: TGroupBox
      Left = 12
      Top = 16
      Width = 257
      Height = 69
      Caption = 'Sensor Position'
      TabOrder = 2
      object Label1: TLabel
        Left = 12
        Top = 20
        Width = 39
        Height = 13
        Caption = 'Latitude'
      end
      object Label2: TLabel
        Left = 132
        Top = 20
        Width = 47
        Height = 13
        Caption = 'Longitude'
      end
      object Label3: TLabel
        Left = 12
        Top = 44
        Width = 37
        Height = 13
        Caption = 'Altitude'
      end
      object Label4: TLabel
        Left = 96
        Top = 44
        Width = 8
        Height = 13
        Caption = 'm'
      end
      object LatEd: TEdit
        Left = 56
        Top = 16
        Width = 65
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 0
      end
      object LongEd: TEdit
        Left = 184
        Top = 16
        Width = 65
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 1
      end
      object AltEd: TEdit
        Left = 56
        Top = 40
        Width = 37
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 2
      end
    end
  end
  object GroupBox5: TGroupBox
    Left = 8
    Top = 4
    Width = 237
    Height = 365
    Caption = 'Location'
    TabOrder = 1
    object LocLB: TListBox
      Left = 12
      Top = 20
      Width = 213
      Height = 333
      ItemHeight = 13
      TabOrder = 0
      OnClick = LocLBClick
    end
  end
  object GroupBox6: TGroupBox
    Left = 252
    Top = 4
    Width = 137
    Height = 365
    Caption = 'Date/Time (local)'
    TabOrder = 2
    object TimeLB: TListBox
      Left = 12
      Top = 20
      Width = 113
      Height = 305
      ItemHeight = 13
      Items.Strings = (
        '12:30 25/12/2012'
        '12:30 25/12/2012'
        '12:30 25/12/2012'
        '12:30 25/12/2012'
        '12:30 25/12/2012'
        '12:30 25/12/2012'
        '')
      TabOrder = 0
      OnClick = TimeLBClick
    end
    object StoreBut: TButton
      Left = 80
      Top = 332
      Width = 45
      Height = 21
      Cursor = crHandPoint
      Caption = 'Store'
      TabOrder = 1
      OnClick = StoreButClick
    end
  end
end

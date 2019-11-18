object WeatherForm: TWeatherForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Weather Station'
  ClientHeight = 356
  ClientWidth = 421
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
  object GroupBox1: TGroupBox
    Left = 8
    Top = 4
    Width = 257
    Height = 69
    Caption = 'Sensor Position'
    TabOrder = 0
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
      Left = 57
      Top = 17
      Width = 64
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 0
    end
    object LongEd: TEdit
      Left = 185
      Top = 17
      Width = 64
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 1
    end
    object AltEd: TEdit
      Left = 55
      Top = 39
      Width = 35
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 2
    end
  end
  object GroupBox2: TGroupBox
    Left = 272
    Top = 4
    Width = 141
    Height = 69
    Caption = 'Last Update'
    TabOrder = 1
    object Label17: TLabel
      Left = 12
      Top = 48
      Width = 29
      Height = 13
      Caption = 'Count'
    end
    object DateEd: TEdit
      Left = 12
      Top = 18
      Width = 66
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 0
    end
    object TimeEd: TEdit
      Left = 84
      Top = 18
      Width = 49
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 1
    end
    object CountEd: TEdit
      Left = 47
      Top = 41
      Width = 61
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 2
    end
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 80
    Width = 405
    Height = 265
    Caption = 'Weather Information'
    TabOrder = 2
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
end

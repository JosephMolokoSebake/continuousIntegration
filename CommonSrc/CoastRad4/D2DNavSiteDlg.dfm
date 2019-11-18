object NavSiteForm: TNavSiteForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Edit Sites Display'
  ClientHeight = 276
  ClientWidth = 575
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object SiteLB: TCheckListBox
    Left = 8
    Top = 12
    Width = 205
    Height = 229
    ItemHeight = 13
    TabOrder = 0
    OnClick = SiteLBClick
  end
  object SiteGrp: TGroupBox
    Left = 224
    Top = 8
    Width = 345
    Height = 197
    Caption = 'Selected Site'
    TabOrder = 1
    object Label1: TLabel
      Left = 12
      Top = 23
      Width = 53
      Height = 13
      Caption = 'Description'
    end
    object GroupBox2: TGroupBox
      Left = 12
      Top = 47
      Width = 141
      Height = 93
      Caption = 'Location'
      TabOrder = 2
      object Label2: TLabel
        Left = 12
        Top = 20
        Width = 39
        Height = 13
        Caption = 'Latitude'
      end
      object Label3: TLabel
        Left = 12
        Top = 45
        Width = 47
        Height = 13
        Caption = 'Longitude'
      end
      object Label4: TLabel
        Left = 16
        Top = 70
        Width = 31
        Height = 13
        Caption = 'Height'
      end
      object Label7: TLabel
        Left = 107
        Top = 68
        Width = 8
        Height = 13
        Caption = 'm'
      end
      object LatEd: TEdit
        Left = 65
        Top = 17
        Width = 65
        Height = 21
        TabOrder = 0
      end
      object LongEd: TEdit
        Left = 65
        Top = 41
        Width = 65
        Height = 21
        TabOrder = 1
      end
      object HeightEd: TEdit
        Left = 64
        Top = 64
        Width = 37
        Height = 21
        TabOrder = 2
        Text = 'HeightEd'
      end
    end
    object DescrEd: TEdit
      Left = 71
      Top = 20
      Width = 174
      Height = 21
      TabOrder = 0
      Text = 'DescrEd'
      OnChange = DescrEdChange
    end
    object GroupBox3: TGroupBox
      Left = 164
      Top = 47
      Width = 169
      Height = 138
      Caption = 'Coverage Area'
      TabOrder = 3
      object RaLab1: TLabel
        Left = 12
        Top = 60
        Width = 78
        Height = 13
        Caption = 'Maximum Range'
      end
      object RaLab2: TLabel
        Left = 151
        Top = 61
        Width = 13
        Height = 13
        Caption = 'km'
      end
      object LineLab: TLabel
        Left = 12
        Top = 84
        Width = 50
        Height = 13
        Caption = 'Line Width'
      end
      object CoverChk: TCheckBox
        Left = 12
        Top = 19
        Width = 149
        Height = 17
        Caption = 'Display Coverage'
        TabOrder = 0
        OnClick = CoverChkClick
      end
      object AutoChk: TCheckBox
        Left = 12
        Top = 38
        Width = 145
        Height = 17
        Caption = 'Auto Maximum Range'
        TabOrder = 1
        OnClick = AutoChkClick
      end
      object MaxRaEd: TEdit
        Left = 92
        Top = 57
        Width = 53
        Height = 21
        TabOrder = 2
        Text = '0.00'
      end
      object WidthEd: TEdit
        Left = 92
        Top = 80
        Width = 21
        Height = 21
        TabOrder = 3
        Text = 'WidthEd'
      end
      object LinePan: TPanel
        Left = 120
        Top = 81
        Width = 37
        Height = 20
        Cursor = crHandPoint
        Color = clYellow
        ParentBackground = False
        TabOrder = 4
        OnClick = LinePanClick
      end
      object FillPan: TPanel
        Left = 120
        Top = 104
        Width = 37
        Height = 21
        Cursor = crHandPoint
        Color = clYellow
        ParentBackground = False
        TabOrder = 6
        OnClick = FillPanClick
      end
      object FillChk: TCheckBox
        Left = 12
        Top = 107
        Width = 97
        Height = 17
        Caption = 'Fill Area'
        TabOrder = 5
        OnClick = FillChkClick
      end
    end
    object SymbolBut: TButton
      Left = 256
      Top = 20
      Width = 77
      Height = 21
      Cursor = crHandPoint
      Caption = 'Edit Symbol'
      TabOrder = 1
      OnClick = SymbolButClick
    end
  end
  object OKBut: TBitBtn
    Left = 403
    Top = 243
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 4
  end
  object CancelBut: TBitBtn
    Left = 492
    Top = 243
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 5
  end
  object AddSiteBut: TButton
    Left = 8
    Top = 247
    Width = 73
    Height = 21
    Cursor = crHandPoint
    Caption = 'Add Site'
    TabOrder = 2
    OnClick = AddSiteButClick
  end
  object DelSiteBut: TButton
    Left = 87
    Top = 247
    Width = 70
    Height = 21
    Cursor = crHandPoint
    Caption = 'Delete Site'
    TabOrder = 3
    OnClick = DelSiteButClick
  end
  object UpBut: TButton
    Left = 161
    Top = 247
    Width = 23
    Height = 21
    Cursor = crHandPoint
    Caption = #173
    Font.Charset = SYMBOL_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Symbol'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    OnClick = UpButClick
  end
  object DownBut: TButton
    Left = 190
    Top = 247
    Width = 23
    Height = 21
    Cursor = crHandPoint
    Caption = #175
    Font.Charset = SYMBOL_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Symbol'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    OnClick = DownButClick
  end
  object RefreshBut: TButton
    Left = 220
    Top = 248
    Width = 113
    Height = 21
    Cursor = crHandPoint
    Caption = 'Refresh Sites'
    TabOrder = 8
    OnClick = RefreshButClick
  end
  object DeleteAllBut: TButton
    Left = 220
    Top = 220
    Width = 113
    Height = 21
    Cursor = crHandPoint
    Caption = 'Delete All Sites'
    TabOrder = 9
    OnClick = DeleteAllButClick
  end
end

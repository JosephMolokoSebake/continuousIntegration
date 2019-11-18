object NavPlacesForm: TNavPlacesForm
  Left = 294
  Top = 202
  BorderStyle = bsDialog
  Caption = 'Create Nav Aid for Place Names'
  ClientHeight = 447
  ClientWidth = 707
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
  object GroupBox2: TGroupBox
    Left = 8
    Top = 8
    Width = 457
    Height = 429
    Caption = 'Source'
    TabOrder = 0
    object Label10: TLabel
      Left = 132
      Top = 292
      Width = 28
      Height = 13
      Caption = 'Listed'
    end
    object Label6: TLabel
      Left = 12
      Top = 56
      Width = 25
      Height = 13
      Caption = 'Jump'
    end
    object Label11: TLabel
      Left = 12
      Top = 292
      Width = 39
      Height = 13
      Caption = 'Skipped'
    end
    object SourceList: TListBox
      Left = 12
      Top = 80
      Width = 201
      Height = 201
      Color = cl3DLight
      ItemHeight = 13
      Sorted = True
      TabOrder = 5
      OnClick = SourceListClick
      OnDblClick = SourceListDblClick
    end
    object SourceBut: TButton
      Left = 12
      Top = 20
      Width = 201
      Height = 25
      Cursor = crHandPoint
      Hint = 'Read Place Name File (*.csv)'
      Caption = 'Read Source File'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnClick = SourceButClick
    end
    object GroupBox1: TGroupBox
      Left = 12
      Top = 316
      Width = 205
      Height = 101
      Caption = 'Site Parameters'
      TabOrder = 8
      object Label1: TLabel
        Left = 12
        Top = 24
        Width = 42
        Height = 13
        Caption = 'Province'
      end
      object Label2: TLabel
        Left = 12
        Top = 48
        Width = 36
        Height = 13
        Caption = 'Feature'
      end
      object Label3: TLabel
        Left = 12
        Top = 72
        Width = 37
        Height = 13
        Caption = 'Position'
      end
      object ProvEd: TEdit
        Left = 60
        Top = 20
        Width = 45
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 0
      end
      object FeatureEd: TEdit
        Left = 60
        Top = 44
        Width = 133
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 1
      end
      object LatEd: TEdit
        Left = 60
        Top = 68
        Width = 65
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 2
      end
      object LongEd: TEdit
        Left = 128
        Top = 68
        Width = 65
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 3
      end
    end
    object SelectBut: TButton
      Left = 228
      Top = 356
      Width = 125
      Height = 25
      Cursor = crHandPoint
      Hint = 'Select Source Place Name for Conversion'
      Caption = 'Select >>'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnClick = SelectButClick
    end
    object GroupBox3: TGroupBox
      Left = 224
      Top = 12
      Width = 221
      Height = 333
      Caption = 'Filter'
      TabOrder = 2
      object Label7: TLabel
        Left = 12
        Top = 104
        Width = 37
        Height = 13
        Caption = 'Position'
      end
      object Bevel1: TBevel
        Left = 12
        Top = 68
        Width = 197
        Height = 2
      end
      object Label4: TLabel
        Left = 12
        Top = 20
        Width = 42
        Height = 13
        Caption = 'Province'
      end
      object Label5: TLabel
        Left = 12
        Top = 44
        Width = 36
        Height = 13
        Caption = 'Feature'
      end
      object Label9: TLabel
        Left = 188
        Top = 80
        Width = 14
        Height = 13
        Caption = 'km'
      end
      object Bevel3: TBevel
        Left = 12
        Top = 128
        Width = 197
        Height = 2
      end
      object Bevel4: TBevel
        Left = 12
        Top = 284
        Width = 197
        Height = 2
      end
      object Bevel5: TBevel
        Left = 12
        Top = 216
        Width = 197
        Height = 2
      end
      object Label8: TLabel
        Left = 12
        Top = 168
        Width = 40
        Height = 13
        Caption = 'Top Left'
      end
      object Label12: TLabel
        Left = 12
        Top = 192
        Width = 61
        Height = 13
        Caption = 'Bottom Right'
      end
      object Label13: TLabel
        Left = 76
        Top = 150
        Width = 38
        Height = 13
        Caption = 'Latitude'
      end
      object Label14: TLabel
        Left = 144
        Top = 150
        Width = 47
        Height = 13
        Caption = 'Longitude'
      end
      object Label15: TLabel
        Left = 188
        Top = 228
        Width = 14
        Height = 13
        Caption = 'km'
      end
      object FilterLatEd: TEdit
        Left = 76
        Top = 100
        Width = 65
        Height = 21
        TabOrder = 3
        OnChange = FilterLatEdChange
      end
      object FilterLongEd: TEdit
        Left = 144
        Top = 100
        Width = 65
        Height = 21
        TabOrder = 4
        OnChange = FilterLongEdChange
      end
      object ProvCombo: TComboBox
        Left = 60
        Top = 16
        Width = 149
        Height = 21
        Style = csDropDownList
        Sorted = True
        TabOrder = 0
        OnChange = ProvComboChange
      end
      object FeatureCombo: TComboBox
        Left = 60
        Top = 40
        Width = 149
        Height = 21
        Style = csDropDownList
        Sorted = True
        TabOrder = 1
        OnChange = FeatureComboChange
      end
      object FilterDistEd: TEdit
        Left = 144
        Top = 76
        Width = 41
        Height = 21
        TabOrder = 2
        Text = '5.0'
      end
      object FilterChk: TCheckBox
        Left = 12
        Top = 78
        Width = 89
        Height = 17
        Caption = 'Filter Positions'
        TabOrder = 5
        OnClick = FilterChkClick
      end
      object ExportBut: TBitBtn
        Left = 12
        Top = 296
        Width = 181
        Height = 25
        Cursor = crHandPoint
        Hint = 'Export Listed Place Names to a Place Name (.csv) File'
        Caption = '<< Export Place Names'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 14
        OnClick = ExportButClick
      end
      object ChartEd: TEdit
        Left = 12
        Top = 252
        Width = 181
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 15
      end
      object ChartBut: TButton
        Left = 196
        Top = 252
        Width = 13
        Height = 21
        Cursor = crHandPoint
        Hint = 'Select a Chart File'
        Caption = '..'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 12
        OnClick = ChartButClick
      end
      object ChartChk: TCheckBox
        Left = 12
        Top = 226
        Width = 89
        Height = 17
        Caption = 'Chart Limited'
        TabOrder = 13
        OnClick = ChartChkClick
      end
      object AreaChk: TCheckBox
        Left = 12
        Top = 136
        Width = 73
        Height = 17
        Caption = 'Filter Area'
        TabOrder = 10
        OnClick = AreaChkClick
      end
      object TLLatEd: TEdit
        Left = 76
        Top = 164
        Width = 65
        Height = 21
        TabOrder = 6
        OnChange = TLLatEdChange
      end
      object TLLongEd: TEdit
        Left = 144
        Top = 164
        Width = 65
        Height = 21
        TabOrder = 7
        OnChange = TLLongEdChange
      end
      object BRLatEd: TEdit
        Left = 76
        Top = 188
        Width = 65
        Height = 21
        TabOrder = 8
        OnChange = BRLatEdChange
      end
      object BRLongEd: TEdit
        Left = 144
        Top = 188
        Width = 65
        Height = 21
        TabOrder = 9
        OnChange = BRLongEdChange
      end
      object ChartDistEd: TEdit
        Left = 144
        Top = 224
        Width = 41
        Height = 21
        TabOrder = 11
        Text = '5.0'
        OnChange = FilterDistEdChange
      end
    end
    object SkippedEd: TEdit
      Left = 56
      Top = 288
      Width = 45
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 6
    end
    object JumpNameEd: TEdit
      Left = 44
      Top = 52
      Width = 169
      Height = 21
      TabOrder = 1
      OnChange = JumpNameEdChange
    end
    object ListedEd: TEdit
      Left = 164
      Top = 288
      Width = 45
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 7
    end
    object SelectAllBut: TButton
      Left = 368
      Top = 356
      Width = 71
      Height = 25
      Cursor = crHandPoint
      Hint = 'Select All Source Place Names for Conversion'
      Caption = 'Select All >>'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 4
      OnClick = SelectAllButClick
    end
  end
  object GroupBox5: TGroupBox
    Left = 476
    Top = 8
    Width = 225
    Height = 385
    Caption = 'Convert'
    TabOrder = 1
    object Bevel2: TBevel
      Left = 12
      Top = 336
      Width = 201
      Height = 2
    end
    object ConvertList: TListBox
      Left = 12
      Top = 20
      Width = 201
      Height = 281
      ItemHeight = 13
      MultiSelect = True
      TabOrder = 0
      OnClick = ConvertListClick
      OnDblClick = ConvertListDblClick
    end
    object RemoveBut: TButton
      Left = 12
      Top = 308
      Width = 57
      Height = 21
      Cursor = crHandPoint
      Hint = 'Remove All Selected Place Names from Nav Aid'
      Caption = 'Remove'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnClick = RemoveButClick
    end
    object EditBut: TButton
      Left = 12
      Top = 348
      Width = 53
      Height = 25
      Cursor = crHandPoint
      Hint = 'Edit All Selected Nav Aid Place Names'
      Caption = 'Edit'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnClick = EditButClick
    end
    object CreateBut: TButton
      Left = 76
      Top = 348
      Width = 137
      Height = 25
      Cursor = crHandPoint
      Hint = 'Store Navigational Aids in a File'
      Caption = 'Create Nav Aid File'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnClick = CreateButClick
    end
    object ConvertEd: TEdit
      Left = 164
      Top = 308
      Width = 48
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 4
    end
  end
  object CloseBut: TBitBtn
    Left = 616
    Top = 408
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkClose
    NumGlyphs = 2
    TabOrder = 2
  end
  object AbortBut: TBitBtn
    Left = 480
    Top = 408
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Hint = 'Abort Current Operation'
    Caption = 'Abort'
    Glyph.Data = {
      DE010000424DDE01000000000000760000002800000024000000120000000100
      0400000000006801000000000000000000001000000000000000000000000000
      80000080000000808000800000008000800080800000C0C0C000808080000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      333333333333333333333333000033338833333333333333333F333333333333
      0000333911833333983333333388F333333F3333000033391118333911833333
      38F38F333F88F33300003339111183911118333338F338F3F8338F3300003333
      911118111118333338F3338F833338F3000033333911111111833333338F3338
      3333F8330000333333911111183333333338F333333F83330000333333311111
      8333333333338F3333383333000033333339111183333333333338F333833333
      00003333339111118333333333333833338F3333000033333911181118333333
      33338333338F333300003333911183911183333333383338F338F33300003333
      9118333911183333338F33838F338F33000033333913333391113333338FF833
      38F338F300003333333333333919333333388333338FFF830000333333333333
      3333333333333333333888330000333333333333333333333333333333333333
      0000}
    NumGlyphs = 2
    ParentShowHint = False
    ShowHint = True
    TabOrder = 3
    Visible = False
    OnClick = AbortButClick
  end
end

object Cst4PictureForm: TCst4PictureForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'System Track Picture'
  ClientHeight = 278
  ClientWidth = 713
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
  OnPaint = FormPaint
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PB: TPaintBox
    Left = 0
    Top = 29
    Width = 713
    Height = 230
    Align = alClient
    OnMouseDown = PBMouseDown
    OnMouseMove = PBMouseMove
    OnMouseUp = PBMouseUp
    ExplicitLeft = 312
    ExplicitTop = 84
    ExplicitWidth = 105
    ExplicitHeight = 105
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 713
    Height = 29
    Align = alTop
    TabOrder = 0
    object ZoomInBut: TSpeedButton
      Left = 308
      Top = 4
      Width = 23
      Height = 22
      Cursor = crHandPoint
      Hint = 'Zoom In'
      Flat = True
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000130B0000130B00001000000000000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
        33033333333333333F733333333333333000333333333333F777333333333333
        000333333333333F777333337000733000333333877783F777333370FFFFF000
        03333337888887777333370FFFFFFF003333337888888877333330FFFF2FFFF0
        333337888878888733337FFFFF2FFFFF733388888878888883330FFFFF2FFFFF
        03337888FF8FF88873330FF2222222FF033378877787778873330FFFFF2FFFFF
        03337888888F888873337FFFFF2FFFFF73338888887F8888833330FFFF2FFFF0
        33333788887888873333370FFFFFFF07333333788888887333333370FFFFF073
        3333333788888733333333337000733333333333877783333333}
      NumGlyphs = 2
      ParentShowHint = False
      ShowHint = True
      OnClick = ZoomInButClick
    end
    object ZoomOutBut: TSpeedButton
      Left = 336
      Top = 4
      Width = 23
      Height = 22
      Cursor = crHandPoint
      Hint = 'Zoom Out'
      Flat = True
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000130B0000130B00001000000000000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
        33033333333333333F733333333333333000333333333333F777333333333333
        000333333333333F777333337000733000333333877783F777333370FFFFF000
        03333337888887777333370FFFFFFF003333337888888877333330FFFFFFFFF0
        333337888888888733337FFFFFFFFFFF733388888888888883330FFFFFFFFFFF
        03337888FFFF88F873330FF2222222FF033378877788778873330FFFFFFFFFFF
        033378888888888873337FFFFFFFFFFF7333888888888888833330FFFFFFFFF0
        33333788888888873333370FFFFFFF07333333788888887333333370FFFFF073
        3333333788888733333333337000733333333333877783333333}
      NumGlyphs = 2
      ParentShowHint = False
      ShowHint = True
      OnClick = ZoomOutButClick
    end
    object ResetBut: TButton
      Left = 368
      Top = 4
      Width = 75
      Height = 21
      Cursor = crHandPoint
      Caption = 'Reset Zoom'
      TabOrder = 0
      OnClick = ResetButClick
    end
    object ReadBut: TButton
      Left = 552
      Top = 4
      Width = 75
      Height = 21
      Cursor = crHandPoint
      Caption = 'Read Picture'
      TabOrder = 1
      OnClick = ReadButClick
    end
    object SaveBut: TButton
      Left = 632
      Top = 4
      Width = 75
      Height = 21
      Cursor = crHandPoint
      Caption = 'Save Picture'
      TabOrder = 2
      OnClick = SaveButClick
    end
    object FileCB: TComboBox
      Left = 12
      Top = 4
      Width = 293
      Height = 21
      Style = csDropDownList
      TabOrder = 3
      OnChange = FileCBChange
    end
    object DeleteBut: TButton
      Left = 464
      Top = 4
      Width = 83
      Height = 21
      Cursor = crHandPoint
      Caption = 'Delete Picture'
      TabOrder = 4
      OnClick = DeleteButClick
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 259
    Width = 713
    Height = 19
    Panels = <
      item
        Text = 'Pan: Mouse Drag'
        Width = 100
      end
      item
        Width = 100
      end>
  end
end

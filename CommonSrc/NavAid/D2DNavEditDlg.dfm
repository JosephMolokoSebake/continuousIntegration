object NavEditForm: TNavEditForm
  Left = 355
  Top = 248
  BorderStyle = bsDialog
  Caption = 'Manually Edit Navigational Aids'
  ClientHeight = 360
  ClientWidth = 554
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
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 6
    Width = 60
    Height = 13
    Caption = 'Symbol/Text'
  end
  object Label2: TLabel
    Left = 196
    Top = 6
    Width = 25
    Height = 13
    Caption = 'Lines'
  end
  object Label3: TLabel
    Left = 288
    Top = 6
    Width = 36
    Height = 13
    Caption = 'Shapes'
  end
  object Bevel1: TBevel
    Left = 444
    Top = 20
    Width = 2
    Height = 329
  end
  object Bevel2: TBevel
    Left = 456
    Top = 104
    Width = 73
    Height = 2
  end
  object SymList: TListBox
    Left = 8
    Top = 20
    Width = 177
    Height = 297
    ItemHeight = 13
    TabOrder = 0
    OnClick = SymListClick
    OnDblClick = SymListDblClick
  end
  object LineList: TListBox
    Left = 196
    Top = 20
    Width = 81
    Height = 297
    ItemHeight = 13
    TabOrder = 1
    OnClick = LineListClick
    OnDblClick = LineListDblClick
  end
  object ArcList: TListBox
    Left = 288
    Top = 20
    Width = 145
    Height = 297
    ItemHeight = 13
    TabOrder = 2
    OnClick = ArcListClick
    OnDblClick = ArcListDblClick
  end
  object DescrEd: TEdit
    Left = 8
    Top = 328
    Width = 397
    Height = 21
    Color = cl3DLight
    ReadOnly = True
    TabOrder = 3
  end
  object EditBut: TButton
    Left = 456
    Top = 20
    Width = 89
    Height = 21
    Cursor = crHandPoint
    Caption = 'Edit'
    TabOrder = 4
    OnClick = EditButClick
  end
  object CancelBut: TBitBtn
    Left = 460
    Top = 324
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 5
    OnClick = CancelButClick
  end
  object ColPan: TPanel
    Left = 412
    Top = 328
    Width = 21
    Height = 21
    TabOrder = 6
  end
  object DelBut: TButton
    Left = 456
    Top = 76
    Width = 89
    Height = 21
    Cursor = crHandPoint
    Caption = 'Delete'
    TabOrder = 7
    OnClick = DelButClick
  end
  object UpBut: TButton
    Left = 456
    Top = 48
    Width = 33
    Height = 21
    Cursor = crHandPoint
    Caption = #173
    Font.Charset = SYMBOL_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Symbol'
    Font.Style = []
    ParentFont = False
    TabOrder = 8
    OnClick = UpButClick
  end
  object DownBut: TButton
    Left = 512
    Top = 48
    Width = 33
    Height = 21
    Cursor = crHandPoint
    Caption = #175
    Font.Charset = SYMBOL_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Symbol'
    Font.Style = []
    ParentFont = False
    TabOrder = 9
    OnClick = DownButClick
  end
  object OKBut: TBitBtn
    Left = 460
    Top = 288
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 10
    OnClick = OKButClick
  end
  object AddSymbolBut: TButton
    Left = 456
    Top = 112
    Width = 89
    Height = 21
    Cursor = crHandPoint
    Caption = 'Add Symbol'
    TabOrder = 11
    OnClick = AddSymbolButClick
  end
  object AddLineBut: TButton
    Left = 456
    Top = 140
    Width = 89
    Height = 21
    Cursor = crHandPoint
    Caption = 'Add Line'
    TabOrder = 12
    OnClick = AddLineButClick
  end
  object AddFootBut: TButton
    Left = 456
    Top = 168
    Width = 89
    Height = 21
    Cursor = crHandPoint
    Caption = 'Add Shape'
    TabOrder = 13
    OnClick = AddFootButClick
  end
end

object IPAddrForm: TIPAddrForm
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Select IP Address'
  ClientHeight = 277
  ClientWidth = 300
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 113
    Top = 5
    Width = 53
    Height = 13
    Caption = 'Description'
  end
  object Label1: TLabel
    Left = 14
    Top = 5
    Width = 39
    Height = 13
    Caption = 'Address'
  end
  object BitBtn1: TBitBtn
    Left = 131
    Top = 244
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 0
  end
  object BitBtn2: TBitBtn
    Left = 212
    Top = 244
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 1
  end
  object DescrEd: TEdit
    Left = 113
    Top = 20
    Width = 179
    Height = 21
    TabOrder = 2
  end
  object AddrEd: TEdit
    Left = 10
    Top = 20
    Width = 97
    Height = 21
    TabOrder = 3
  end
  object LB: TListBox
    Left = 8
    Top = 51
    Width = 283
    Height = 178
    ItemHeight = 13
    Items.Strings = (
      '196.123.456.234'#9'Description')
    TabOrder = 4
    TabWidth = 32
    OnDblClick = LBDblClick
  end
  object DeleteBut: TButton
    Left = 74
    Top = 244
    Width = 51
    Height = 21
    Cursor = crHandPoint
    Caption = 'Delete'
    TabOrder = 5
    OnClick = DeleteButClick
  end
  object AddBut: TButton
    Left = 39
    Top = 244
    Width = 29
    Height = 21
    Cursor = crHandPoint
    Caption = 'Add'
    TabOrder = 6
    OnClick = AddButClick
  end
  object UseBut: TButton
    Left = 4
    Top = 244
    Width = 29
    Height = 21
    Cursor = crHandPoint
    Caption = 'Use'
    TabOrder = 7
    OnClick = UseButClick
  end
end

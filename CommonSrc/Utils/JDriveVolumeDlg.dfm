object DriveVolumeForm: TDriveVolumeForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Select Drive Volume'
  ClientHeight = 249
  ClientWidth = 185
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
  object VolLB: TListBox
    Left = 4
    Top = 8
    Width = 173
    Height = 201
    ItemHeight = 13
    TabOrder = 0
    OnDblClick = VolLBDblClick
  end
  object OKBut: TBitBtn
    Left = 12
    Top = 216
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 1
    OnClick = OKButClick
  end
  object CancelBut: TBitBtn
    Left = 100
    Top = 216
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 2
  end
end

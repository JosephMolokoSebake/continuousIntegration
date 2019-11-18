object Cst4SelectSiteForm: TCst4SelectSiteForm
  Left = 477
  Top = 221
  BorderStyle = bsDialog
  Caption = 'Select VistaNet Site ID'
  ClientHeight = 343
  ClientWidth = 673
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
    Left = 12
    Top = 8
    Width = 209
    Height = 293
    Caption = 'Country/System'
    TabOrder = 0
    object CountryLB: TListBox
      Left = 12
      Top = 20
      Width = 185
      Height = 261
      ItemHeight = 13
      TabOrder = 0
      OnClick = CountryLBClick
    end
  end
  object GroupBox3: TGroupBox
    Left = 232
    Top = 8
    Width = 209
    Height = 293
    Caption = 'Location'
    TabOrder = 1
    object LocLB: TListBox
      Left = 12
      Top = 20
      Width = 185
      Height = 261
      ItemHeight = 13
      TabOrder = 0
      OnClick = LocLBClick
    end
  end
  object OKBut: TBitBtn
    Left = 500
    Top = 307
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 3
  end
  object CancelBut: TBitBtn
    Left = 584
    Top = 307
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 4
  end
  object TypeGrp: TGroupBox
    Left = 452
    Top = 8
    Width = 209
    Height = 249
    Caption = 'Type'
    TabOrder = 2
    object TypeLB: TListBox
      Left = 12
      Top = 20
      Width = 185
      Height = 217
      ItemHeight = 13
      TabOrder = 0
      OnDblClick = TypeLBDblClick
    end
  end
  object FullInfoBut: TButton
    Left = 584
    Top = 264
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Caption = 'Full Site Info'
    TabOrder = 5
    OnClick = FullInfoButClick
  end
  object DispAllChk: TCheckBox
    Left = 508
    Top = 268
    Width = 73
    Height = 17
    Caption = 'Display All'
    TabOrder = 6
    OnClick = DispAllChkClick
  end
  object EditTempBut: TButton
    Left = 12
    Top = 307
    Width = 137
    Height = 25
    Cursor = crHandPoint
    Caption = 'Edit Temporary Site'
    TabOrder = 7
    OnClick = EditTempButClick
  end
end

object AISExtInfoForm: TAISExtInfoForm
  Left = 529
  Top = 442
  BorderStyle = bsDialog
  Caption = 'Extended AIS Information'
  ClientHeight = 229
  ClientWidth = 337
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
  object Grid: TStringGrid
    Left = 8
    Top = 8
    Width = 320
    Height = 213
    ColCount = 2
    DefaultRowHeight = 20
    FixedCols = 0
    RowCount = 10
    FixedRows = 0
    ScrollBars = ssNone
    TabOrder = 0
  end
end

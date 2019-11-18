object SiteStatusForm: TSiteStatusForm
  Left = 462
  Top = 209
  BorderStyle = bsDialog
  Caption = 'Site Status'
  ClientHeight = 512
  ClientWidth = 811
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
  object StatGrid: TStringGrid
    Left = 12
    Top = 12
    Width = 789
    Height = 321
    ColCount = 6
    DefaultRowHeight = 20
    FixedCols = 0
    RowCount = 101
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
    ScrollBars = ssVertical
    TabOrder = 0
    OnDrawCell = StatGridDrawCell
    OnSelectCell = StatGridSelectCell
  end
  object SaveBut: TButton
    Left = 672
    Top = 476
    Width = 133
    Height = 25
    Cursor = crHandPoint
    Caption = 'Save to File'
    TabOrder = 1
    OnClick = SaveButClick
  end
  object SiteGrp: TGroupBox
    Left = 8
    Top = 339
    Width = 649
    Height = 165
    TabOrder = 2
    object PB: TPaintBox
      Left = 8
      Top = 16
      Width = 629
      Height = 113
      Cursor = crHandPoint
      OnMouseMove = PBMouseMove
      OnPaint = PBPaint
    end
    object Label1: TLabel
      Left = 496
      Top = 140
      Width = 85
      Height = 13
      Caption = 'Messages/Minute'
    end
    object DateEd: TEdit
      Left = 368
      Top = 136
      Width = 113
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 0
    end
    object MsgEd: TEdit
      Left = 587
      Top = 138
      Width = 41
      Height = 21
      Color = cl3DLight
      NumbersOnly = True
      ReadOnly = True
      TabOrder = 1
    end
  end
  object GroupBox1: TGroupBox
    Left = 676
    Top = 340
    Width = 125
    Height = 125
    Caption = 'Colour Key'
    TabOrder = 3
    object Panel1: TPanel
      Left = 16
      Top = 20
      Width = 89
      Height = 21
      Hint = 'Last Data < 1 minute'
      Color = clLime
      ParentBackground = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
    end
    object Panel2: TPanel
      Left = 16
      Top = 44
      Width = 89
      Height = 21
      Hint = '1 Minute < Last Data < 1 Hour'
      Color = clAqua
      ParentBackground = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
    end
    object Panel3: TPanel
      Left = 16
      Top = 68
      Width = 89
      Height = 21
      Hint = '1 Hour < Last Data < 24 Hours'
      Color = clYellow
      ParentBackground = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
    end
    object Panel4: TPanel
      Left = 16
      Top = 92
      Width = 89
      Height = 21
      Hint = 'Last Data > 24 Hours'
      Color = clRed
      ParentBackground = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
    end
  end
end

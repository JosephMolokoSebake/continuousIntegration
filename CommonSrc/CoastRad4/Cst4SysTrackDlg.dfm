object SysTrackForm: TSysTrackForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'System Track'
  ClientHeight = 595
  ClientWidth = 657
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
  object SubmitBut: TButton
    Left = 524
    Top = 552
    Width = 107
    Height = 29
    Cursor = crHandPoint
    Caption = 'Submit'
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = SubmitButClick
  end
  object GroupBox1: TGroupBox
    Left = 4
    Top = 4
    Width = 309
    Height = 73
    Caption = 'Origin'
    TabOrder = 1
    object Label1: TLabel
      Left = 228
      Top = 20
      Width = 37
      Height = 13
      Caption = 'Position'
    end
    object Label5: TLabel
      Left = 96
      Top = 44
      Width = 23
      Height = 13
      Caption = 'Date'
    end
    object Label6: TLabel
      Left = 12
      Top = 44
      Width = 22
      Height = 13
      Caption = 'Time'
    end
    object OrEd: TEdit
      Left = 8
      Top = 16
      Width = 209
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 0
    end
    object OrSeatEd: TEdit
      Left = 272
      Top = 16
      Width = 25
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 1
    end
    object OrTimeEd: TEdit
      Left = 36
      Top = 40
      Width = 50
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 2
    end
    object OrDateEd: TEdit
      Left = 124
      Top = 40
      Width = 65
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 3
    end
  end
  object GroupBox2: TGroupBox
    Left = 320
    Top = 4
    Width = 329
    Height = 317
    Caption = 'AIS Track'
    TabOrder = 2
    object Label2: TLabel
      Left = 8
      Top = 96
      Width = 26
      Height = 13
      Caption = 'MMSI'
    end
    object Label8: TLabel
      Left = 120
      Top = 96
      Width = 20
      Height = 13
      Caption = 'IMO'
    end
    object Label9: TLabel
      Left = 8
      Top = 120
      Width = 50
      Height = 13
      Caption = 'Ship Name'
    end
    object Label15: TLabel
      Left = 208
      Top = 120
      Width = 36
      Height = 13
      Caption = 'Callsign'
    end
    object Label22: TLabel
      Left = 8
      Top = 144
      Width = 73
      Height = 13
      Caption = 'Ship and Cargo'
    end
    object Label23: TLabel
      Left = 4
      Top = 192
      Width = 33
      Height = 13
      Caption = 'Length'
    end
    object Label25: TLabel
      Left = 112
      Top = 192
      Width = 28
      Height = 13
      Caption = 'Width'
    end
    object Label26: TLabel
      Left = 8
      Top = 216
      Width = 54
      Height = 13
      Caption = 'Destination'
    end
    object Label27: TLabel
      Left = 212
      Top = 192
      Width = 39
      Height = 13
      Caption = 'Draught'
    end
    object Label28: TLabel
      Left = 228
      Top = 212
      Width = 19
      Height = 13
      Caption = 'ETA'
    end
    object Label35: TLabel
      Left = 80
      Top = 192
      Width = 8
      Height = 13
      Caption = 'm'
    end
    object Label36: TLabel
      Left = 180
      Top = 192
      Width = 8
      Height = 13
      Caption = 'm'
    end
    object Label37: TLabel
      Left = 292
      Top = 192
      Width = 8
      Height = 13
      Caption = 'm'
    end
    object Label38: TLabel
      Left = 8
      Top = 168
      Width = 53
      Height = 13
      Caption = 'Nav Status'
    end
    object Label39: TLabel
      Left = 12
      Top = 256
      Width = 58
      Height = 13
      Caption = 'Last Update'
    end
    object Bevel2: TBevel
      Left = 8
      Top = 240
      Width = 313
      Height = 2
    end
    object MMSIEd: TEdit
      Left = 40
      Top = 92
      Width = 64
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 0
    end
    object AltAISBut: TButton
      Left = 164
      Top = 280
      Width = 137
      Height = 25
      Cursor = crHandPoint
      Caption = 'Alternative AIS Tracks'
      TabOrder = 1
      OnClick = AltAISButClick
    end
    object IMOEd: TEdit
      Left = 144
      Top = 92
      Width = 64
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 2
    end
    object GroupBox8: TGroupBox
      Left = 164
      Top = 12
      Width = 133
      Height = 73
      Caption = 'Vector'
      TabOrder = 3
      object Label16: TLabel
        Left = 12
        Top = 20
        Width = 30
        Height = 13
        Caption = 'Speed'
      end
      object Label17: TLabel
        Left = 12
        Top = 44
        Width = 34
        Height = 13
        Caption = 'Course'
      end
      object Label18: TLabel
        Left = 108
        Top = 20
        Width = 14
        Height = 13
        Caption = 'kts'
      end
      object Label19: TLabel
        Left = 108
        Top = 44
        Width = 18
        Height = 13
        Caption = 'deg'
      end
      object SpeedEd: TEdit
        Left = 60
        Top = 16
        Width = 45
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 0
      end
      object CourseEd: TEdit
        Left = 60
        Top = 40
        Width = 45
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 1
      end
    end
    object GroupBox9: TGroupBox
      Left = 8
      Top = 12
      Width = 149
      Height = 73
      Caption = 'Position'
      TabOrder = 4
      object Label20: TLabel
        Left = 12
        Top = 20
        Width = 39
        Height = 13
        Caption = 'Latitude'
      end
      object Label21: TLabel
        Left = 12
        Top = 44
        Width = 47
        Height = 13
        Caption = 'Longitude'
      end
      object LatEd: TEdit
        Left = 72
        Top = 16
        Width = 65
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 0
      end
      object LongEd: TEdit
        Left = 72
        Top = 40
        Width = 65
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 1
      end
    end
    object NameEd: TEdit
      Left = 64
      Top = 116
      Width = 133
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 5
    end
    object CallsignEd: TEdit
      Left = 248
      Top = 116
      Width = 73
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 6
    end
    object CargoEd: TEdit
      Left = 96
      Top = 140
      Width = 153
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 7
    end
    object LengthEd: TEdit
      Left = 44
      Top = 188
      Width = 33
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 8
    end
    object WidthEd: TEdit
      Left = 144
      Top = 188
      Width = 33
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 9
    end
    object DraughtEd: TEdit
      Left = 256
      Top = 188
      Width = 33
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 10
    end
    object DestEd: TEdit
      Left = 68
      Top = 212
      Width = 153
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 11
    end
    object ETAEd: TEdit
      Left = 256
      Top = 212
      Width = 37
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 12
    end
    object NavStatusEd: TEdit
      Left = 96
      Top = 164
      Width = 153
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 13
    end
    object LastAISDate: TEdit
      Left = 128
      Top = 252
      Width = 65
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 14
    end
    object LastAISTime: TEdit
      Left = 76
      Top = 252
      Width = 50
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 15
    end
    object DiscardAISBut: TButton
      Left = 16
      Top = 280
      Width = 121
      Height = 25
      Cursor = crHandPoint
      Caption = 'Discard AIS Track'
      TabOrder = 16
      OnClick = DiscardAISButClick
    end
  end
  object GroupBox3: TGroupBox
    Left = 320
    Top = 328
    Width = 329
    Height = 209
    Caption = 'Radar Track'
    TabOrder = 3
    object Label3: TLabel
      Left = 16
      Top = 44
      Width = 26
      Height = 13
      Caption = 'Track'
    end
    object Label4: TLabel
      Left = 16
      Top = 20
      Width = 33
      Height = 13
      Caption = 'Source'
    end
    object Label34: TLabel
      Left = 128
      Top = 44
      Width = 34
      Height = 13
      Caption = 'Quality'
    end
    object Label42: TLabel
      Left = 4
      Top = 152
      Width = 58
      Height = 13
      Caption = 'Last Update'
    end
    object RadarTrackEd: TEdit
      Left = 56
      Top = 40
      Width = 53
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 0
    end
    object RadarSrcEd: TEdit
      Left = 56
      Top = 16
      Width = 209
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 1
    end
    object AltRadarBut: TButton
      Left = 160
      Top = 176
      Width = 149
      Height = 25
      Cursor = crHandPoint
      Caption = 'Alternative Radar Tracks'
      TabOrder = 2
      OnClick = AltRadarButClick
    end
    object GroupBox10: TGroupBox
      Left = 156
      Top = 64
      Width = 133
      Height = 73
      Caption = 'Vector'
      TabOrder = 3
      object Label24: TLabel
        Left = 12
        Top = 20
        Width = 30
        Height = 13
        Caption = 'Speed'
      end
      object Label29: TLabel
        Left = 12
        Top = 44
        Width = 34
        Height = 13
        Caption = 'Course'
      end
      object Label30: TLabel
        Left = 108
        Top = 20
        Width = 14
        Height = 13
        Caption = 'kts'
      end
      object Label31: TLabel
        Left = 108
        Top = 44
        Width = 18
        Height = 13
        Caption = 'deg'
      end
      object RadarSpeedEd: TEdit
        Left = 60
        Top = 16
        Width = 45
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 0
      end
      object RadarCourseEd: TEdit
        Left = 60
        Top = 40
        Width = 45
        Height = 21
        Color = cl3DLight
        MaxLength = 6
        ReadOnly = True
        TabOrder = 1
      end
    end
    object GroupBox11: TGroupBox
      Left = 0
      Top = 64
      Width = 149
      Height = 73
      Caption = 'Position'
      TabOrder = 4
      object Label32: TLabel
        Left = 12
        Top = 20
        Width = 39
        Height = 13
        Caption = 'Latitude'
      end
      object Label33: TLabel
        Left = 12
        Top = 44
        Width = 47
        Height = 13
        Caption = 'Longitude'
      end
      object RadarLatEd: TEdit
        Left = 72
        Top = 20
        Width = 65
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 0
      end
      object RadarLongEd: TEdit
        Left = 72
        Top = 40
        Width = 65
        Height = 21
        Color = cl3DLight
        ReadOnly = True
        TabOrder = 1
      end
    end
    object RadarQualityEd: TEdit
      Left = 168
      Top = 40
      Width = 25
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 5
    end
    object LastRadarDate: TEdit
      Left = 116
      Top = 148
      Width = 65
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 6
    end
    object LastRadarTime: TEdit
      Left = 64
      Top = 148
      Width = 50
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 7
    end
    object DiscardRadarBut: TButton
      Left = 16
      Top = 176
      Width = 121
      Height = 25
      Cursor = crHandPoint
      Caption = 'Discard Radar Track'
      TabOrder = 8
      OnClick = DiscardRadarButClick
    end
  end
  object GroupBox4: TGroupBox
    Left = 4
    Top = 260
    Width = 309
    Height = 101
    Caption = 'Classification'
    TabOrder = 4
    object ClassCB: TComboBox
      Left = 152
      Top = 20
      Width = 145
      Height = 21
      Style = csDropDownList
      TabOrder = 0
    end
  end
  object GroupBox5: TGroupBox
    Left = 4
    Top = 368
    Width = 305
    Height = 121
    Caption = 'Comments'
    TabOrder = 5
    object CommentMemo: TMemo
      Left = 8
      Top = 20
      Width = 285
      Height = 89
      Lines.Strings = (
        '')
      TabOrder = 0
    end
  end
  object GroupBox6: TGroupBox
    Left = 4
    Top = 200
    Width = 309
    Height = 53
    Caption = 'Track Description'
    TabOrder = 6
    object DescrEd: TEdit
      Left = 12
      Top = 20
      Width = 289
      Height = 21
      TabOrder = 0
    end
  end
  object CloseBut: TBitBtn
    Left = 420
    Top = 556
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Kind = bkClose
    NumGlyphs = 2
    TabOrder = 7
  end
  object DeleteBut: TButton
    Left = 332
    Top = 556
    Width = 57
    Height = 25
    Cursor = crHandPoint
    Caption = 'Delete'
    TabOrder = 8
    OnClick = DeleteButClick
  end
  object GroupBox7: TGroupBox
    Left = 4
    Top = 84
    Width = 309
    Height = 109
    Caption = 'Last Update'
    TabOrder = 9
    object Label10: TLabel
      Left = 228
      Top = 20
      Width = 37
      Height = 13
      Caption = 'Position'
    end
    object Label11: TLabel
      Left = 96
      Top = 44
      Width = 23
      Height = 13
      Caption = 'Date'
    end
    object Label12: TLabel
      Left = 12
      Top = 44
      Width = 22
      Height = 13
      Caption = 'Time'
    end
    object Bevel1: TBevel
      Left = 12
      Top = 68
      Width = 277
      Height = 2
    end
    object Label13: TLabel
      Left = 12
      Top = 84
      Width = 62
      Height = 13
      Caption = 'Message Life'
    end
    object Label14: TLabel
      Left = 108
      Top = 84
      Width = 27
      Height = 13
      Caption = 'hours'
    end
    object Label7: TLabel
      Left = 196
      Top = 84
      Width = 44
      Height = 13
      Caption = 'Time Left'
    end
    object UpdEd: TEdit
      Left = 8
      Top = 16
      Width = 209
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 0
    end
    object UpdSeatEd: TEdit
      Left = 272
      Top = 16
      Width = 25
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 1
    end
    object UpdTimeEd: TEdit
      Left = 36
      Top = 40
      Width = 50
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 2
    end
    object UpdDateEd: TEdit
      Left = 124
      Top = 40
      Width = 65
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 3
    end
    object LifeEd: TEdit
      Left = 80
      Top = 80
      Width = 25
      Height = 21
      TabOrder = 4
      Text = '999'
    end
    object TimeLeftEd: TEdit
      Left = 244
      Top = 80
      Width = 53
      Height = 21
      Color = cl3DLight
      ReadOnly = True
      TabOrder = 5
      Text = '10:24:04'
    end
  end
  object GroupBox12: TGroupBox
    Left = 4
    Top = 496
    Width = 305
    Height = 89
    Caption = 'Designation to Camera'
    TabOrder = 10
    object SourceLab: TLabel
      Left = 8
      Top = 68
      Width = 49
      Height = 13
      Caption = 'No Source'
    end
    object DesLab2: TLabel
      Left = 204
      Top = 52
      Width = 20
      Height = 13
      Caption = 'Port'
    end
    object DesLab1: TLabel
      Left = 80
      Top = 52
      Width = 23
      Height = 13
      Caption = 'Addr'
    end
    object DispPictBut: TButton
      Left = 164
      Top = 16
      Width = 129
      Height = 25
      Cursor = crHandPoint
      Caption = 'Display Picture'
      TabOrder = 0
      OnClick = DispPictButClick
    end
    object UploadPictBut: TButton
      Left = 16
      Top = 16
      Width = 129
      Height = 25
      Cursor = crHandPoint
      Caption = 'Upload Picture'
      TabOrder = 1
      OnClick = UploadPictButClick
    end
    object DesAddrEd: TEdit
      Left = 104
      Top = 48
      Width = 93
      Height = 21
      TabOrder = 2
      Text = '123.123.123.123'
    end
    object DesPortEd: TEdit
      Left = 228
      Top = 48
      Width = 37
      Height = 21
      TabOrder = 3
      Text = '12345'
    end
    object DesAddrBut: TButton
      Left = 268
      Top = 48
      Width = 21
      Height = 21
      Cursor = crHandPoint
      Hint = 'Reload Designation Address nand Port'
      Caption = '..'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 4
      OnClick = DesAddrButClick
    end
    object CancelPictBut: TBitBtn
      Left = 80
      Top = 16
      Width = 75
      Height = 25
      Cursor = crHandPoint
      Cancel = True
      Caption = 'Cancel'
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
      TabOrder = 5
      OnClick = CancelPictButClick
    end
  end
  object Timer: TTimer
    Enabled = False
    OnTimer = TimerTimer
    Left = 604
    Top = 152
  end
end

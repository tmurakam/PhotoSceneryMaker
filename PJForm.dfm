object PrjForm: TPrjForm
  Left = 351
  Top = 465
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Project Property'
  ClientHeight = 410
  ClientWidth = 526
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 12
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 526
    Height = 374
    ActivePage = TabSheet3
    Align = alClient
    TabIndex = 2
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'Bitmaps'
      object Label1: TLabel
        Left = 16
        Top = 40
        Width = 12
        Height = 12
        Caption = #22799
      end
      object Label2: TLabel
        Left = 16
        Top = 72
        Width = 12
        Height = 12
        Caption = #26149
      end
      object Label3: TLabel
        Left = 16
        Top = 104
        Width = 12
        Height = 12
        Caption = #31179
      end
      object Label4: TLabel
        Left = 16
        Top = 136
        Width = 12
        Height = 12
        Caption = #20908
      end
      object Label5: TLabel
        Left = 16
        Top = 168
        Width = 24
        Height = 12
        Caption = #21427#20908
      end
      object Label6: TLabel
        Left = 16
        Top = 200
        Width = 59
        Height = 12
        Caption = #12521#12452#12488#12510#12483#12503
      end
      object Label7: TLabel
        Left = 16
        Top = 232
        Width = 24
        Height = 12
        Caption = #36879#36942
      end
      object EditBmpSummer: TEdit
        Left = 80
        Top = 32
        Width = 329
        Height = 20
        TabOrder = 0
      end
      object EditBmpSpring: TEdit
        Left = 80
        Top = 64
        Width = 329
        Height = 20
        TabOrder = 1
      end
      object CheckSeason: TCheckBox
        Left = 16
        Top = 8
        Width = 97
        Height = 17
        Caption = 'Support Seasons'
        TabOrder = 2
      end
      object EditBmpFall: TEdit
        Left = 80
        Top = 96
        Width = 329
        Height = 20
        TabOrder = 3
      end
      object EditBmpWinter: TEdit
        Left = 80
        Top = 128
        Width = 329
        Height = 20
        TabOrder = 4
      end
      object EditBmpHSWinter: TEdit
        Left = 80
        Top = 160
        Width = 329
        Height = 20
        TabOrder = 5
      end
      object EditBmpLightmap: TEdit
        Left = 80
        Top = 192
        Width = 329
        Height = 20
        TabOrder = 6
      end
      object EditBmpAlpha: TEdit
        Left = 80
        Top = 224
        Width = 329
        Height = 20
        TabOrder = 7
      end
      object ButtonRefBmpSummer: TButton
        Left = 416
        Top = 32
        Width = 41
        Height = 25
        Caption = #21442#29031'...'
        TabOrder = 8
        OnClick = ButtonRefBmpSummerClick
      end
      object ButtonRefBmpSpring: TButton
        Left = 416
        Top = 64
        Width = 41
        Height = 25
        Caption = #21442#29031'...'
        TabOrder = 9
        OnClick = ButtonRefBmpSpringClick
      end
      object ButtonRefBmpFall: TButton
        Left = 416
        Top = 96
        Width = 41
        Height = 25
        Caption = #21442#29031'...'
        TabOrder = 10
        OnClick = ButtonRefBmpFallClick
      end
      object ButtonRefBmpWinter: TButton
        Left = 416
        Top = 128
        Width = 41
        Height = 25
        Caption = #21442#29031'...'
        TabOrder = 11
        OnClick = ButtonRefBmpWinterClick
      end
      object ButtonRefBmpHSWinter: TButton
        Left = 416
        Top = 160
        Width = 41
        Height = 25
        Caption = #21442#29031'...'
        TabOrder = 12
        OnClick = ButtonRefBmpHSWinterClick
      end
      object ButtonRefBmpLightmap: TButton
        Left = 416
        Top = 192
        Width = 41
        Height = 25
        Caption = #21442#29031'...'
        TabOrder = 13
        OnClick = ButtonRefBmpLightmapClick
      end
      object ButtonRefBmpAlpha: TButton
        Left = 416
        Top = 224
        Width = 41
        Height = 25
        Caption = #21442#29031'...'
        TabOrder = 14
        OnClick = ButtonRefBmpAlphaClick
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Coordinates'
      ImageIndex = 1
      object Label10: TLabel
        Left = 208
        Top = 152
        Width = 92
        Height = 12
        Caption = '*** NOT YET ***'
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Output'
      ImageIndex = 2
      object Label8: TLabel
        Left = 16
        Top = 32
        Width = 85
        Height = 12
        Caption = 'Output Directory'
      end
      object Label9: TLabel
        Left = 16
        Top = 72
        Width = 82
        Height = 12
        Caption = 'Base File Name'
      end
      object EditOutDir: TEdit
        Left = 120
        Top = 24
        Width = 281
        Height = 20
        TabOrder = 0
      end
      object EditBaseFile: TEdit
        Left = 120
        Top = 64
        Width = 121
        Height = 20
        TabOrder = 1
      end
      object ButtonRefOutDir: TButton
        Left = 416
        Top = 24
        Width = 25
        Height = 25
        Caption = '...'
        TabOrder = 2
        OnClick = ButtonRefOutDirClick
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 374
    Width = 526
    Height = 36
    Align = alBottom
    TabOrder = 1
    object Button1: TButton
      Left = 344
      Top = 8
      Width = 75
      Height = 25
      Caption = 'OK'
      ModalResult = 1
      TabOrder = 0
    end
    object Button2: TButton
      Left = 432
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
    end
  end
  object OpenDialog: TOpenDialog
    DefaultExt = 'bmp'
    Left = 16
    Top = 376
  end
end

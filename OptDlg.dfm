object OptionDlg: TOptionDlg
  Left = 326
  Top = 208
  BorderStyle = bsDialog
  Caption = #12458#12503#12471#12519#12531
  ClientHeight = 120
  ClientWidth = 421
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object Label1: TLabel
    Left = 8
    Top = 16
    Width = 88
    Height = 12
    Caption = 'Terrain SDK path'
  end
  object Label2: TLabel
    Left = 8
    Top = 48
    Width = 75
    Height = 12
    Caption = 'Imagetool path'
  end
  object ButtonOK: TButton
    Left = 120
    Top = 80
    Width = 75
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 0
    OnClick = ButtonOKClick
  end
  object Button2: TButton
    Left = 216
    Top = 80
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object EditSDKPath: TEdit
    Left = 128
    Top = 12
    Width = 209
    Height = 20
    TabOrder = 2
  end
  object ButtonBrowseSDK: TButton
    Left = 352
    Top = 10
    Width = 25
    Height = 25
    Caption = '...'
    TabOrder = 3
    OnClick = ButtonBrowseSDKClick
  end
  object EditImagetoolPath: TEdit
    Left = 128
    Top = 44
    Width = 209
    Height = 20
    TabOrder = 4
  end
  object ButtonBrowserImagetool: TButton
    Left = 352
    Top = 42
    Width = 25
    Height = 25
    Caption = '...'
    TabOrder = 5
    OnClick = ButtonBrowserImagetoolClick
  end
end

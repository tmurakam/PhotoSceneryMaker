object OptionDlg: TOptionDlg
  Left = 302
  Top = 252
  BorderStyle = bsDialog
  Caption = 'Options'
  ClientHeight = 141
  ClientWidth = 516
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
    Left = 12
    Top = 16
    Width = 95
    Height = 12
    Caption = 'Terrain SDK folder'
  end
  object Label2: TLabel
    Left = 12
    Top = 48
    Width = 82
    Height = 12
    Caption = 'Imagetool folder'
  end
  object Label3: TLabel
    Left = 12
    Top = 80
    Width = 48
    Height = 12
    Caption = 'Language'
  end
  object ButtonOK: TButton
    Left = 160
    Top = 104
    Width = 75
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 0
    OnClick = ButtonOKClick
  end
  object Button2: TButton
    Left = 256
    Top = 104
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object EditSDKPath: TEdit
    Left = 128
    Top = 12
    Width = 281
    Height = 20
    TabOrder = 2
  end
  object ButtonBrowseSDK: TButton
    Left = 424
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
    Width = 281
    Height = 20
    TabOrder = 4
  end
  object ButtonBrowserImagetool: TButton
    Left = 424
    Top = 42
    Width = 25
    Height = 25
    Caption = '...'
    TabOrder = 5
    OnClick = ButtonBrowserImagetoolClick
  end
  object ListLang: TComboBox
    Left = 128
    Top = 76
    Width = 129
    Height = 20
    Style = csDropDownList
    ItemHeight = 12
    TabOrder = 6
  end
  object OpenDialog: TOpenDialog
    DefaultExt = 'exe'
    Left = 344
    Top = 80
  end
end

object LatLonDlg: TLatLonDlg
  Left = 574
  Top = 372
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'LatLonDlg'
  ClientHeight = 124
  ClientWidth = 228
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
  object Label1: TLabel
    Left = 16
    Top = 24
    Width = 41
    Height = 12
    Caption = 'Latitude'
  end
  object Label2: TLabel
    Left = 16
    Top = 56
    Width = 49
    Height = 12
    Caption = 'Longitude'
  end
  object LatEdit: TEdit
    Left = 72
    Top = 16
    Width = 121
    Height = 20
    TabOrder = 0
  end
  object LonEdit: TEdit
    Left = 72
    Top = 48
    Width = 121
    Height = 20
    TabOrder = 1
  end
  object Button1: TButton
    Left = 80
    Top = 88
    Width = 75
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 2
  end
end

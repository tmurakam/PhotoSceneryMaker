object SCGenForm: TSCGenForm
  Left = 434
  Top = 242
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Scenery Generation'
  ClientHeight = 261
  ClientWidth = 447
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
  object ButtonDoAll: TButton
    Left = 32
    Top = 32
    Width = 161
    Height = 73
    Caption = 'Run whole process'
    TabOrder = 0
    OnClick = ButtonDoAllClick
  end
  object ButtonClose: TButton
    Left = 128
    Top = 176
    Width = 75
    Height = 25
    Caption = 'Close'
    ModalResult = 1
    TabOrder = 2
  end
  object GroupBox1: TGroupBox
    Left = 248
    Top = 24
    Width = 177
    Height = 193
    Caption = 'Step execution'
    TabOrder = 3
    object ButtonMakeInf: TButton
      Left = 16
      Top = 24
      Width = 145
      Height = 25
      Caption = '1. Generate inf file'
      TabOrder = 0
      OnClick = ButtonMakeInfClick
    end
    object ButtonResample: TButton
      Left = 16
      Top = 56
      Width = 145
      Height = 25
      Caption = '2. Resample'
      TabOrder = 1
      OnClick = ButtonResampleClick
    end
    object ButtonConvertTexture: TButton
      Left = 16
      Top = 120
      Width = 145
      Height = 25
      Caption = '4. Convert Texture'
      TabOrder = 3
      OnClick = ButtonConvertTextureClick
    end
    object ButtonGenBGL: TButton
      Left = 16
      Top = 152
      Width = 145
      Height = 25
      Caption = '5. Generate BGL'
      TabOrder = 4
      OnClick = ButtonGenBGLClick
    end
    object ButtonMergeAlpha: TButton
      Left = 16
      Top = 88
      Width = 145
      Height = 25
      Caption = '3. Merge Alpha Texture'
      TabOrder = 2
      OnClick = ButtonMergeAlphaClick
    end
  end
  object ButtonStop: TButton
    Left = 32
    Top = 176
    Width = 75
    Height = 25
    Caption = 'Stop'
    Enabled = False
    TabOrder = 1
    OnClick = ButtonStopClick
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 242
    Width = 447
    Height = 19
    Panels = <>
    SimplePanel = True
    SimpleText = 'Ready'
  end
end

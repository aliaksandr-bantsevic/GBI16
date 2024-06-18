object Form_show_hint: TForm_show_hint
  Left = 0
  Top = 0
  BorderStyle = bsNone
  BorderWidth = 4
  Caption = 'Form_show_hint'
  ClientHeight = 119
  ClientWidth = 160
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poDesigned
  OnCreate = FormCreate
  TextHeight = 15
  object Memo_hint: TMemo
    Left = 0
    Top = 0
    Width = 160
    Height = 119
    Align = alClient
    BorderStyle = bsNone
    Color = clInfoBk
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Segoe UI'
    Font.Style = [fsBold]
    Lines.Strings = (
      'Memo_hint')
    ParentFont = False
    TabOrder = 0
  end
end

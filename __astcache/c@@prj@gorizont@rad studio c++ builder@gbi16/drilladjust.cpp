//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DrillAdjust.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_DrillAdjust *Form_DrillAdjust;
//---------------------------------------------------------------------------
__fastcall TForm_DrillAdjust::TForm_DrillAdjust(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_DrillAdjust::Button_applyClick(TObject *Sender)
{
	 Update();

		CutSpacesEdit(Edit_name);
		FormToDrill();
		OK = true;
        *OOK = true;
		Close();
}

void TForm_DrillAdjust::Start(TDrill* d, int m)
{

	   this->drill = d;
	   this->mode = m;

	   Timer_start->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TForm_DrillAdjust::Timer_startTimer(TObject *Sender)
{

	Timer_start->Enabled = false;

	if (mode == 0)
	{
		Button_apply->Caption = L"Добавить";
		this->Caption = L"Добавить скважину";
		DrillToForm();
	}
	else
	{
		Button_apply->Caption = L"Применить";
		this->Caption = L"Изменить скважину";
		DrillToForm();
	}

	//this->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm_DrillAdjust::Button_cancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_DrillAdjust::FormCreate(TObject *Sender)
{

	MACRO_FORM_MOVE_TO_THE_CENTER

	//name
	this->Edit_name->Visible = true;
	this->Edit_name->Text = L"Скважина";

	//orient
	this->ComboBox_orient->Clear();
	this->ComboBox_orient->Items->Add(L"Горизонтальная");
	this->ComboBox_orient->Items->Add(L"Вертикальная");
	this->ComboBox_orient->ItemIndex = 0;

	//asimut
	this->Edit_asimut->Visible = true;
	this->Edit_asimut->Text = L"0";

	//levels
	double dl1 = 0.f;
	double dl2 = 0.f;
	WideString sl1("");
	WideString sl2("");

	int ilcnt = (int)(100.f/0.5f);

	this->ComboBox_level_start->Clear();
	this->ComboBox_level_end->Clear();

	for (int i = 0; i < ilcnt; i++)
	{
		sl1.printf(L"%.1f",dl1);
		sl2.printf(L"%.1f",dl2);

		this->ComboBox_level_start->Items->Add(sl1);
		this->ComboBox_level_end->Items->Add(sl2);

		dl1 += 0.5f;
		dl2 += 0.5f;
	}

	this->ComboBox_level_start->ItemIndex = 0;
	this->ComboBox_level_end->ItemIndex = 20;

	//calc start TOP/BOTTOM vertical only
	this->RadioGroup_calc_start->Visible = true;
	this->RadioButton_top->Visible = TRUE;
	this->RadioButton_bot->Visible = TRUE;
	this->RadioButton_top->Checked = TRUE;
	this->RadioButton_bot->Checked = FALSE;

	//start collect data
	GroupBox_start_collect_data->Visible = true;
	RadioButton_start_first->Visible = TRUE;
	RadioButton_start_last->Visible = TRUE;
	RadioButton_start_first->Checked = TRUE;
	RadioButton_start_last->Checked = FALSE;

	//pass 1/2
	GroupBox_pass->Visible = true;
	RadioButton_pass_1->Visible = TRUE;
	RadioButton_pass_2->Visible = TRUE;
	RadioButton_pass_1->Checked = TRUE;
	RadioButton_pass_2->Checked = FALSE;

	//Geo bind
	GroupBox_geo->Visible = true;
	Edit_Input_point->Visible = true;
	Edit_Output_point->Visible = true;

	Edit_Input_point->Text = L"0";
	Edit_Output_point->Text = L"0";

}
//---------------------------------------------------------------------------

void TForm_DrillAdjust::DrillToForm()
{

	if (drill == NULL)
	{
		return; //drill is not defined
	}

	//name
	this->Edit_name->Text = drill->name;

	//orient
	this->ComboBox_orient->ItemIndex = drill->drill_orient;


	//asimut vertical only
	if (drill->drill_orient == DRILL_ORIENT_HORIZONT)
	{
		this->Edit_asimut->Visible = false;
	}
	else
	{
		this->Edit_asimut->Visible = true;
		WideString sda("");
		sda.printf(L"%.1f", drill->drill_asimut);
		this->Edit_asimut->Text = sda;
	}

	//levels
	double dl1 = drill->level_start;
	double dl2 = drill->level_end;
	WideString sl1("");
	WideString sl2("");

	sl1.printf(L"%.1f",dl1);
	sl2.printf(L"%.1f",dl2);

	this->ComboBox_level_start->Text = sl1;
	this->ComboBox_level_end->Text = sl2;

	//calc start TOP/BOTTOM vertical only
	if (drill->drill_orient == DRILL_ORIENT_HORIZONT)
	{
		this->RadioGroup_calc_start->Visible = false;
		this->RadioButton_top->Visible = false;
		this->RadioButton_bot->Visible = false;
	}
	else if (drill->drill_orient == DRILL_ORIENT_VERTICAL)
	{
		this->RadioGroup_calc_start->Visible = true;

		if (drill->start_point == DRILL_TOP_POINT)
		{
			this->RadioGroup_calc_start->Visible = true;
			this->RadioButton_top->Visible = TRUE;
			this->RadioButton_bot->Visible = TRUE;
			this->RadioButton_top->Checked = TRUE;
			this->RadioButton_bot->Checked = FALSE;
		}
		else if (drill->start_point == DRILL_BOT_POINT)
		{
			this->RadioGroup_calc_start->Visible = true;
			this->RadioButton_top->Visible = TRUE;
			this->RadioButton_bot->Visible = TRUE;
			this->RadioButton_top->Checked = FALSE;
			this->RadioButton_bot->Checked = TRUE;
		}

	}

	//start collect data
    GroupBox_start_collect_data->Enabled = false;
	//forward
	if (dl2>dl1)
	{
			RadioButton_start_first->Checked = TRUE;
			RadioButton_start_last->Checked = FALSE;
	}
	//backward
	else if (dl1>dl2)
	{
			RadioButton_start_first->Checked = FALSE;
			RadioButton_start_last->Checked = TRUE;
	}
	//error
	{
			//RadioButton_start_first->Checked = FALSE;
			//RadioButton_start_last->Checked = FALSE;
			//utils_ShowMessage(L"Ошибка задания уровней скважины!");
	}

	//pass 1/2
	if (drill->single_way == 1)
	{
		RadioButton_pass_1->Checked = TRUE;
		RadioButton_pass_2->Checked = FALSE;
	}
	else
	{
		RadioButton_pass_1->Checked = FALSE;
		RadioButton_pass_2->Checked = TRUE;
	}

	//Geo bind horizontal only
	if (drill->drill_orient == DRILL_ORIENT_VERTICAL)
	{
		GroupBox_geo->Visible = false;
	}
	else if (drill->drill_orient == DRILL_ORIENT_HORIZONT)
	{

		if (drill->geo_data.geo_on == true)
		{
			GroupBox_geo->Visible = true;

			CheckBox_GeoOn->Checked = true;
			Edit_Input_point->Enabled = true;
			Edit_Output_point->Enabled = true;

			WideString sip("");
			WideString sop("");

			sip.printf(L"%.1f", drill->geo_data.input_point);
			sop.printf(L"%.1f", drill->geo_data.output_point);

			Edit_Input_point->Text = sip;
			Edit_Output_point->Text = sop;
		}
		else
		{
			GroupBox_geo->Visible = true;
			Edit_Input_point->Enabled = false;
			Edit_Output_point->Enabled = false;

			Edit_Input_point->Text = L"0";
			Edit_Output_point->Text = L"0";

        }
	}

	Update();
}

int TForm_DrillAdjust::FormToDrill()
{
	Update();

	if (drill == NULL)
	{
		return -1; //drill is not defined
	}

	//name
	drill->name = this->Edit_name->Text;

	//orient
	if (this->ComboBox_orient->ItemIndex == 0)
	{
		drill->drill_orient = DRILL_ORIENT_HORIZONT;
	}
	else if (this->ComboBox_orient->ItemIndex == 1)
	{
		drill->drill_orient = DRILL_ORIENT_VERTICAL;
	}
	else
	{
		utils_ShowMessage(L"Ошибка задания типа скважины!");
        return -1; //drill type error
	}

	//asimut only for vertical
	if (drill->drill_orient == DRILL_ORIENT_HORIZONT)
	{
		double das = 0.;

		try {

			das = this->Edit_asimut->Text.ToDouble();

		}
		 catch (...) {

		   utils_ShowMessage(L"Ошибка задания азимута скважины!");
		}

		drill->drill_asimut = das;
	}

	//levels
	double dl1 = 0;
	double dl2 = 0;

	try {

		dl1 = this->ComboBox_level_start->Text.ToDouble();
		dl2 = this->ComboBox_level_end->Text.ToDouble();
	}
	 catch (...) {

		   utils_ShowMessage(L"Ошибка задания уровней скважины!");
	}


	if (dl1 == dl2)
	{
		utils_ShowMessage(L"Ошибка задания уровней скважины!");
	}
	else
	{
		drill->level_start = dl1;
		drill->level_end = dl2;

		double dlmax = dl1;
		if (dl2>dl1)  dlmax = dl2;

		drill->records_cnt = (int)(dlmax/0.5) + 1;

		if (dl2 > dl1)
		{
			drill->start_point == DRILL_TOP_POINT; //from me
		}
		else
		{
			drill->i_first_request_point = DRILL_FIRST_REQUEST_BOT; //to me
		}
	}

	//calc start TOP/BOTTOM vertical only
	if (this->RadioButton_top->Checked == TRUE)
	{
		drill->start_point = DRILL_TOP_POINT;
	}
	else if (this->RadioButton_bot->Checked == TRUE)
	{
		drill->start_point = DRILL_BOT_POINT;
	}

	//start collect data
	/*
	*   This patrameter is adjusted auto as level_start<>level_end
	*/

	/*
	if (RadioButton_start_first->Checked == TRUE)
	{
		drill->i_first_request_point = DRILL_FIRST_REQUEST_TOP;
	}
	else if (RadioButton_start_last->Checked == TRUE)
	{
		drill->i_first_request_point = DRILL_FIRST_REQUEST_BOT;
	}
	*/

	//pass 1/2
	if (RadioButton_pass_1->Checked == TRUE)
	{
		drill->single_way = 1;
	}
	else  if (RadioButton_pass_2->Checked == TRUE)
	{
		drill->single_way = 0;
	}

	//Geo bind
	if (CheckBox_GeoOn->Checked == true)
	{
		drill->geo_data.geo_on = true;

		try
		{
			drill->geo_data.input_point = Edit_Input_point->Text.ToDouble();
			drill->geo_data.output_point = Edit_Output_point->Text.ToDouble();
		}
		catch(...)
		{
			drill->geo_data.input_point = 0;
			drill->geo_data.output_point = 0;

			ShowMessage(L"Ошибка ввода данных геопривязки!");
		}

	}
	else
	{
		drill->geo_data.geo_on = false;
		drill->geo_data.input_point = 0;
		drill->geo_data.output_point = 0;
	}

	return 0;
}

void TForm_DrillAdjust::FormChange()
{
	 FormToDrill();
	 DrillToForm();
}


void __fastcall TForm_DrillAdjust::ComboBox_orientChange(TObject *Sender)
{
	FormChange();
}
//---------------------------------------------------------------------------

void __fastcall TForm_DrillAdjust::CheckBox_GeoOnClick(TObject *Sender)
{
	Update();

	if (CheckBox_GeoOn->Checked == TRUE)
	{
		this->Edit_Input_point->Enabled = true;
		this->Edit_Output_point->Enabled = true;
	}
	else
	{
		this->Edit_Input_point->Enabled = false;
		this->Edit_Output_point->Enabled = false;
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm_DrillAdjust::ComboBox_level_startChange(TObject *Sender)
{
   	FormChange();
}
//---------------------------------------------------------------------------

void __fastcall TForm_DrillAdjust::ComboBox_level_endChange(TObject *Sender)
{
	FormChange();
}
//---------------------------------------------------------------------------

void __fastcall TForm_DrillAdjust::Edit_nameChange(TObject *Sender)
{
     //CutSpacesEdit(Edit_name);
}
//---------------------------------------------------------------------------


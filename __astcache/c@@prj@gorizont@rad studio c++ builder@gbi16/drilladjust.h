//---------------------------------------------------------------------------

#ifndef DrillAdjustH
#define DrillAdjustH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

#include "defs.h"
#include "Drill.h"

//---------------------------------------------------------------------------

#define DRILL_CREATE 	0
#define DRILL_EDIT   	1

class TForm_DrillAdjust : public TForm
{
__published:	// IDE-managed Components
	TButton *Button_cancel;
	TButton *Button_apply;
	TEdit *Edit_name_old;
	TTimer *Timer_start;
	TComboBox *ComboBox_meas_cnt;
	TLabel *Label2;
	TEdit *Edit_asimut_old;
	TLabel *Label_asimut;
	TComboBox *ComboBox_orient_old;
	TLabel *Label4;
	TCheckBox *CheckBox_single;
	TEdit *Edit_zshift;
	TRadioGroup *RadioGroup_calc_start;
	TRadioButton *RadioButton_top;
	TRadioButton *RadioButton_bot;
	TGroupBox *GroupBox_start_collect_data;
	TRadioButton *RadioButton_start_first;
	TRadioButton *RadioButton_start_last;
	TGroupBox *GroupBox_geo;
	TCheckBox *CheckBox_GeoOn;
	TEdit *Edit_Input_point;
	TEdit *Edit_Output_point;
	TLabel *Label3;
	TLabel *Label5;
	TGroupBox *GroupBox_pass;
	TRadioButton *RadioButton_pass_1;
	TRadioButton *RadioButton_pass_2;
	TGroupBox *GroupBox3;
	TComboBox *ComboBox_level_start;
	TComboBox *ComboBox_level_end;
	TLabel *Label6;
	TLabel *Label7;
	TGroupBox *GroupBox4;
	TEdit *Edit_name;
	TGroupBox *GroupBox5;
	TComboBox *ComboBox_orient;
	TGroupBox *GroupBox6;
	TEdit *Edit_asimut;
	void __fastcall Button_applyClick(TObject *Sender);
	void __fastcall Timer_startTimer(TObject *Sender);
	void __fastcall Button_cancelClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ComboBox_orientChange(TObject *Sender);
	void __fastcall CheckBox_GeoOnClick(TObject *Sender);
	void __fastcall ComboBox_level_startChange(TObject *Sender);
	void __fastcall ComboBox_level_endChange(TObject *Sender);
	void __fastcall Edit_nameChange(TObject *Sender);
	void __fastcall Edit_asimutChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_DrillAdjust(TComponent* Owner);


	void Start(TDrill* d, int m);
	void DrillToForm();
	int FormToDrill();
    void FormChange();

	int mode;
	TDrill* drill;


	int* records_cnt;
	int records_cnt_def;


	bool OK;
    bool* OOK;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_DrillAdjust *Form_DrillAdjust;
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------

#ifndef ProfileAdjustH
#define ProfileAdjustH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "Drill.h"
#include <Vcl.ComCtrls.hpp>

#include "ProfileItemEdit.h"
#include "GostProfileSet.h"
//---------------------------------------------------------------------------
class TForm_ProfileAdjust : public TForm
{
__published:	// IDE-managed Components
	TButton *Button_calcel;
	TButton *Button_apply;
	TButton *Button_add;
	TListView *ListView_profile;
	TEdit *Edit_new;
	TButton *Button_clr;
	TButton *Button_gost;
	TButton *Button_sort;
	void __fastcall Button_applyClick(TObject *Sender);
	void __fastcall Button_calcelClick(TObject *Sender);
	void __fastcall Button_addClick(TObject *Sender);
	void __fastcall Button_gostClick(TObject *Sender);
	void __fastcall Button_clrClick(TObject *Sender);
	void __fastcall Edit_newChange(TObject *Sender);
	void __fastcall ListView_profileDblClick(TObject *Sender);
	void __fastcall Button_sortClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_ProfileAdjust(TComponent* Owner);

public:

	TDrill* drill;
	TProfile profile_local;

public:

	void set(TDrill* drill);
	void prof_to_form(void);
	void form_to_prof(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_ProfileAdjust *Form_ProfileAdjust;
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------

#ifndef ObjAdjustH
#define ObjAdjustH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include "drill.h"
#include "defs.h"
#include "ProfileAdjust.h"
//---------------------------------------------------------------------------
class TForm_ObjAdjust : public TForm
{
__published:	// IDE-managed Components
	TButton *Button_apply;
	TButton *Button_cancel;
	TGroupBox *GroupBox1;
	TEdit *Edit_name;
	TButton *Button_profile;
	void __fastcall Button_applyClick(TObject *Sender);
	void __fastcall Button_cancelClick(TObject *Sender);
	void __fastcall Button_profileClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_ObjAdjust(TComponent* Owner);

public:

	TDrill* drill;
	int mode;
	void set(int mode, TDrill* drill, bool* OK);
	void obj_to_form(void);
	void form_to_obj(void);
	bool* OK;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_ObjAdjust *Form_ObjAdjust;
//---------------------------------------------------------------------------
#endif

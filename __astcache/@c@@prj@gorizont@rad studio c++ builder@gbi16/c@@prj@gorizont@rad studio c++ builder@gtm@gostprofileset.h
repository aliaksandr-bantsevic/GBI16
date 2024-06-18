//---------------------------------------------------------------------------

#ifndef GostProfileSetH
#define GostProfileSetH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include "defs.h"
//---------------------------------------------------------------------------
class TForm_GostPrfileSet : public TForm
{
__published:	// IDE-managed Components
	TEdit *Edit_depth;
	TStaticText *StaticText1;
	TButton *Button_Cancel;
	TButton *Button_OK;
	void __fastcall Button_CancelClick(TObject *Sender);
	void __fastcall Button_OKClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_GostPrfileSet(TComponent* Owner);

public:

	bool* OK;
	double* depth;

public:

    void Set(bool* OK, double* depth);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_GostPrfileSet *Form_GostPrfileSet;
//---------------------------------------------------------------------------
#endif

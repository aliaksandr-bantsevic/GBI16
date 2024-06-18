//---------------------------------------------------------------------------

#ifndef ProfileItemEditH
#define ProfileItemEditH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include "Drill.h"
//---------------------------------------------------------------------------
class TForm_ProfileItemEdit : public TForm
{
__published:	// IDE-managed Components
	TEdit *Edit_item_idx;
	TEdit *Edit_item_val;
	TLabel *Label1;
	TLabel *Label2;
	TButton *Button_update;
	TButton *Button_delete;
	TButton *Button_exit;
	void __fastcall Button_exitClick(TObject *Sender);
	void __fastcall Edit_item_valChange(TObject *Sender);
	void __fastcall Button_updateClick(TObject *Sender);
	void __fastcall Button_deleteClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_ProfileItemEdit(TComponent* Owner);

public:

	TProfile* profile;
	int idx;

public:

	void set(TProfile* p, int idx);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_ProfileItemEdit *Form_ProfileItemEdit;
//---------------------------------------------------------------------------
#endif

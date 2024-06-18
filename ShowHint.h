//---------------------------------------------------------------------------

#ifndef ShowHintH
#define ShowHintH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include "defs.h"
//---------------------------------------------------------------------------
class TForm_show_hint : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo_hint;
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_show_hint(TComponent* Owner);

public:

	void Show(WideString txt);
	void Hide();
    void Move(int x, int y);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_show_hint *Form_show_hint;
//---------------------------------------------------------------------------
#endif

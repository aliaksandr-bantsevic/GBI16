//---------------------------------------------------------------------------

#ifndef WaitReportCreateH
#define WaitReportCreateH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>

#include "defs.h"
//---------------------------------------------------------------------------
class TForm_wait_report_create : public TForm
{
__published:	// IDE-managed Components
	TProgressBar *ProgressBar_create;
	TStaticText *StaticText1;
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_wait_report_create(TComponent* Owner);

public:

	void Setup(int range);
	void Update(int range);
	void Hide();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_wait_report_create *Form_wait_report_create;
//---------------------------------------------------------------------------
#endif

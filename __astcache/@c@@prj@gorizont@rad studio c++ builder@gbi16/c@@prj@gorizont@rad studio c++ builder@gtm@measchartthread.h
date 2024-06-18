//---------------------------------------------------------------------------

#ifndef MeasChartThreadH
#define MeasChartThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>

#include <tchar.h>
#include <VCLTee.Chart.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.Series.hpp>

#include "meas.h"
#include <string.h>

//---------------------------------------------------------------------------
class TMeasChartThread : public TThread
{
protected:
	void __fastcall Execute();
public:
	__fastcall TMeasChartThread(bool CreateSuspended, TChart* c);

public:

	TChart* chart;
	bool redraw_flag;
	TMeas* meas;

public:

	void __fastcall DrawChart();
	void Set(TMeas* m);
	void Draw();
    void SetAxisLimits();

};
//---------------------------------------------------------------------------
#endif

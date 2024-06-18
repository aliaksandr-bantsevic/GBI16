//---------------------------------------------------------------------------

#ifndef ComplexChartThreadH
#define ComplexChartThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>

#include <tchar.h>
#include <VCLTee.Chart.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.Series.hpp>

#include "defs.h"
#include "meas.h"
//---------------------------------------------------------------------------


class TComplexChartThread : public TThread
{
protected:
	void __fastcall Execute();
public:
	__fastcall TComplexChartThread(bool CreateSuspended, TMeas** m, TChart* c);

public:

	TMeas** meas_list;
	TChart* chart;
	bool redraw_flag;
	int series_cnt;

public:

	void __fastcall DrawChart();
   	void Draw();
	void SetSeries();
	void SetAxisLimits();

 };
//---------------------------------------------------------------------------
#endif

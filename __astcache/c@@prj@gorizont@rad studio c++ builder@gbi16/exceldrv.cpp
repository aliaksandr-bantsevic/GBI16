//---------------------------------------------------------------------------

#pragma hdrstop

#include "ExcelDrv.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//Excel report table
Variant mExcel;
Variant mSheet;

bool ShowExcel = false;
bool FormatCellsOn = false;

TCHAR* XCOL[]={

(TCHAR*)L"-",
(TCHAR*)L"A",(TCHAR*)L"B",(TCHAR*)L"C",(TCHAR*)L"D",(TCHAR*)L"E",(TCHAR*)L"F",(TCHAR*)L"G",(TCHAR*)L"H",(TCHAR*)L"I",(TCHAR*)L"J",(TCHAR*)L"K",(TCHAR*)L"L",(TCHAR*)L"M",(TCHAR*)L"N",(TCHAR*)L"O",(TCHAR*)L"P",(TCHAR*)L"Q",(TCHAR*)L"R",(TCHAR*)L"S",(TCHAR*)L"T",(TCHAR*)L"U",(TCHAR*)L"V",(TCHAR*)L"W",(TCHAR*)L"X",(TCHAR*)L"Y",(TCHAR*)L"Z",
(TCHAR*)L"AA",(TCHAR*)L"AB",(TCHAR*)L"AC",(TCHAR*)L"AD",(TCHAR*)L"AE",(TCHAR*)L"AF",(TCHAR*)L"AG",(TCHAR*)L"AH",(TCHAR*)L"AI",(TCHAR*)L"AJ",(TCHAR*)L"AK",(TCHAR*)L"AL",(TCHAR*)L"AM",(TCHAR*)L"AN",(TCHAR*)L"AO",(TCHAR*)L"AP",(TCHAR*)L"AQ",(TCHAR*)L"AR",(TCHAR*)L"AS",(TCHAR*)L"AT",(TCHAR*)L"AU",(TCHAR*)L"AV",(TCHAR*)L"AW",(TCHAR*)L"AX",
(TCHAR*)L"AY",(TCHAR*)L"AZ",(TCHAR*)L"BA",(TCHAR*)L"BB",(TCHAR*)L"BC",(TCHAR*)L"BD",(TCHAR*)L"BE",(TCHAR*)L"BF",(TCHAR*)L"BG",(TCHAR*)L"BH",(TCHAR*)L"BI",(TCHAR*)L"BJ",(TCHAR*)L"BK",(TCHAR*)L"BL",(TCHAR*)L"BM",(TCHAR*)L"BN",(TCHAR*)L"BO",(TCHAR*)L"BP",(TCHAR*)L"BQ",(TCHAR*)L"BR",(TCHAR*)L"BS",(TCHAR*)L"BT",(TCHAR*)L"BU",(TCHAR*)L"BV",
(TCHAR*)L"BW",(TCHAR*)L"BX",(TCHAR*)L"BY",(TCHAR*)L"BZ",(TCHAR*)L"CA",(TCHAR*)L"CB",(TCHAR*)L"CC",(TCHAR*)L"CD",(TCHAR*)L"CE",(TCHAR*)L"CF",(TCHAR*)L"CG",(TCHAR*)L"CH",(TCHAR*)L"CI",(TCHAR*)L"CJ",(TCHAR*)L"CK",(TCHAR*)L"CL",(TCHAR*)L"CM",(TCHAR*)L"CN",(TCHAR*)L"CO",(TCHAR*)L"CP",(TCHAR*)L"CQ",(TCHAR*)L"CR",(TCHAR*)L"CS",(TCHAR*)L"CT",
(TCHAR*)L"CU",(TCHAR*)L"CV",(TCHAR*)L"CW",(TCHAR*)L"CX",(TCHAR*)L"CY",(TCHAR*)L"CZ",
};


bool OpenExcelReportTable(int sheets)
{

		bool bres = true;

		try
		{
				mExcel = CreateOleObject(L"Excel.Application");

				if (ShowExcel)  mExcel.OlePropertySet(L"Visible", True);
				else            mExcel.OlePropertySet(L"Visible", False);

				mExcel.OlePropertySet(L"SheetsInNewWorkbook",sheets);
				mExcel.OlePropertyGet(L"WorkBooks").OleProcedure(L"Add");

		}
		catch(...)
		{
				bres = false;
		}


		return bres;
}


bool OpenExcelReportSheet(WideString sheetname, int sheet)
{

		bool bres = true;

		try
		{
				mSheet = mExcel.OlePropertyGet(L"Worksheets").OlePropertyGet(L"Item",sheet);
				mSheet.OlePropertySet(L"Name",WideString(sheetname));
				mSheet.OleProcedure(L"Activate");

		}
		catch(...)
		{
				bres = false;
		}


		return bres;
}


bool WriteExcelReportCell(int row, int col, WideString sval)
{
		bool bres = true;

		try
		{
				mExcel.OlePropertyGet(L"ActiveSheet").OlePropertyGet(L"Cells", row, WideString(XCOL[col])).OlePropertySet(WideString(L"NumberFormat"),WideString(L"@"));
				//mExcel.OlePropertyGet(L"ActiveSheet").OlePropertyGet(L"Cells", row, WideString(XCOL[col])).OlePropertySet(WideString(L"CommonFormat"),WideString(L"0,00"));
				mExcel.OlePropertyGet(L"ActiveSheet").OlePropertyGet(L"Cells", row, WideString(XCOL[col])).OlePropertySet(WideString(L"Value"),WideString(sval));

		}
		catch(...)
		{
				 bres = false;
		}

		return bres;

}

bool SaveExcelReport(WideString spath)
{
		bool bres=true;

		try
		{
		  mExcel.OlePropertyGet(L"WorkBooks", 1).OleProcedure(WideString(L"SaveAs"),WideString(spath));
		  mExcel.OlePropertyGet(L"ActiveWindow").OleProcedure(L"Close");
		  mExcel.OleProcedure(WideString(L"Quit"));
		}
		catch(...)
		{

		}


		WideString s(L"");
		s.printf(L"Отчет сохранен в файле %s",spath.c_bstr());
		utils_ShowMessage(s);

		return bres;
}


bool SetExcelColumnWidth(int row, int col, int w)
{
		bool res = true;

        try
        {
			 mExcel.OlePropertyGet(L"ActiveSheet").OlePropertyGet(L"Cells", row, XCOL[col]).OlePropertySet(L"ColumnWidth",w);
        }
        catch(...)
        {
              res=false;
        }

        return res;
}

bool SetExcelCellFontBold(int row, int col)
{
		if (FormatCellsOn == false) return false;

		bool res=true;

		try
		{
			 mExcel.OlePropertyGet(L"ActiveSheet").OlePropertyGet(L"Cells", row, XCOL[col]).OlePropertyGet(L"Font").OlePropertySet(L"Bold",true);
		}
		catch(...)
		{
			  res=false;
		}

		return res;

}

//https://docs.microsoft.com/en-us/office/vba/api/excel.colorindex
bool SetExellCellColor(int row, int col, int color)
{
		if (FormatCellsOn == false) return false;

		bool res=true;

        try
        {
             mExcel.OlePropertyGet(L"ActiveSheet").OlePropertyGet(L"Cells", row, XCOL[col]).OlePropertyGet(L"Interior").OlePropertySet(L"ColorIndex",color);
        }
        catch(...)
        {
              res=false;
        }

        return res;

}

bool SetExcelRangeBorder(int row1, int col1, int row2, int col2, int line)
{
		if (FormatCellsOn == false) return false;

	   bool res=false;

		Variant vVarCell;
		WideString srange(L"");
		WideString s1=XCOL[col1];
		WideString s2=XCOL[col2];
		srange.printf(L"%s%d:%s%d",s1.c_bstr(),row1,s2.c_bstr(),row2);

			  try
			  {
					  vVarCell=mExcel.OlePropertyGet(L"ActiveSheet").OlePropertyGet(L"Range",srange.c_bstr());
					  for (int b=7; b<=10; b++)
					  {
						vVarCell.OlePropertyGet(L"Borders",b).OlePropertySet(L"Weight",line);
					  }
			  }
			  catch(...)
			  {
					  res=false;
			  }

			  return res;
}

bool ExcelMergeAndWrite(int row1, int col1, int row2, int col2, WideString s)
{
		bool res=true;

		try
		{
				Variant vVarCell;
				WideString srange(L"");
				WideString s1=XCOL[col1];
				WideString s2=XCOL[col2];
				srange.printf(L"%s%d:%s%d",s1.c_bstr(),row1,s2.c_bstr(),row2);

				vVarCell=mExcel.OlePropertyGet(L"ActiveSheet").OlePropertyGet(L"Range",srange.c_bstr());
				vVarCell.OleProcedure(L"Merge");

				WriteExcelReportCell(row1,col1,s);
		}
		catch(...)
		{
				res=false;
		}

		return res;
}


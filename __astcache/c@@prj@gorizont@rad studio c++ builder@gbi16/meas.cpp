//---------------------------------------------------------------------------

#pragma hdrstop

#include "Meas.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

extern TSysConfMgr* scmgr;

TMeas::TMeas ()
{
	table = NULL;
	finalized = false;
}


TMeas::TMeas (TStringGrid* t, WideString n)
{
	table = t;
	finalized = false;
	create_time = (double)0;

	for (int i = 0; i < MAX_RECORDS_MEAS; i++) {

		this->records[i].depth = 0;
		this->records[i].tuberr = 0;

		this->records[i].X1 = 0;
		this->records[i].X2 = 0;

		this->records[i].Y1 = 0;
		this->records[i].Y2 = 0;

		this->records[i].Xres = 0;
		this->records[i].Yres = 0;

		this->records[i].LX = 0;
		this->records[i].LY = 0;
		this->records[i].LR = 0;
		this->records[i].AR = 0;

	}

	node = NULL;

	num = 0;
	dnum = 0;
	pnum = 0;

	name = n;
	selected = false;

	name_place = "";
	name_drill = "";

	finalize_time = (double)0;

	records_cnt_def = 21;
	records_cnt = 21;

	SysConfMgr = scmgr;

	own_zero_shift = 0;

	drill_start_point = 0;
	type_drill = 0;

    mark = "Маркировка измерения";
}


TMeas::~TMeas()
{


}


int PutIntCell(TStringGrid* table, int col, int row, int d)
{
	if (table == NULL) {

		return -1; //Нет указателя на таблицу
	}

		WideString s;

		if ((d == 0)&&(row>1)) {
			s = "";
		}
		else
		{
			s.printf(L"%d",d);
		}
		table->Cells[col][row] = s;

		return 0;
}

int PutDblCell(TStringGrid* table, int col, int row, double d)
{
	if (table == NULL) {

		return -1; //Нет указателя на таблицу
	}

		WideString s;

		if (d == 0)
		{
			s = "0";

		}
		else
		{
			s.printf(L"%.1f",d);
		}
		table->Cells[col][row] = s;

		return 0;
}


int TMeas::DataToTable(void)
{
	if (table == NULL) {

		return -1; //Нет указателя на таблицу
	}



/*
Иициализация таблицы
*/

   table->RowCount = records_cnt+1;

	for (int i = 0; i < records_cnt; i++) {

		PutDblCell(table,1,i+1,this->records[i].depth);
		PutDblCell(table,2,i+1,this->records[i].tuberr);

		PutDblCell(table,3,i+1,this->records[i].X1);
		PutDblCell(table,4,i+1,this->records[i].Y1);

		PutDblCell(table,5,i+1,this->records[i].X2);
		PutDblCell(table,6,i+1,this->records[i].Y2);

		PutDblCell(table,7,i+1,this->records[i].Xres);
		PutDblCell(table,8,i+1,this->records[i].Yres);

		PutDblCell(table,9,i+1,this->records[i].LX);
		PutDblCell(table,10,i+1,this->records[i].LY);

		PutDblCell(table,11,i+1,this->records[i].LR);
		PutDblCell(table,12,i+1,this->records[i].AR);

	}


	return 0;
}


int GetIntCell(TStringGrid* table, int col, int row, int* d)
{
	if (table == NULL) {

		return -1; //Нет указателя на таблицу
	}

	WideString s;

	s = table->Cells[col][row];

	if (s=="") {

		*d = 0;
	}
	else
    {
		try {

			*d = s.ToInt();

		} catch (...) {

			*d = 0;
		}

	}

	return 0;
}

int GetDblCell(TStringGrid* table, int col, int row, double* d)
{
	if (table == NULL) {

		return -1; //Нет указателя на таблицу
	}

	WideString s;

	s = table->Cells[col][row];

	if (s=="") {

		*d=0;
	}
	else
	{

		try {

			*d = s.ToDouble();

		} catch (...) {

			*d = 0;
		}

	}


	return 0;
}

int TMeas::TableToData(void)
{
	if (table == NULL) {

		return -1; //Нет указателя на таблицу
	}


	for (int i = 0; i < records_cnt; i++) {

		GetDblCell(table,1,i+1,&this->records[i].depth);
		GetDblCell(table,2,i+1,&this->records[i].tuberr);

		GetDblCell(table,3,i+1,&this->records[i].X1);
		GetDblCell(table,4,i+1,&this->records[i].Y1);

		GetDblCell(table,5,i+1,&this->records[i].X2);
		GetDblCell(table,6,i+1,&this->records[i].Y2);

		GetDblCell(table,7,i+1,&this->records[i].Xres);
		GetDblCell(table,8,i+1,&this->records[i].Yres);

		GetDblCell(table,9,i+1,&this->records[i].LX);
		GetDblCell(table,10,i+1,&this->records[i].LY);

		GetDblCell(table,11,i+1,&this->records[i].LR);
		GetDblCell(table,12,i+1,&this->records[i].AR);

	}

	return 0;
}

double xt = 10;
double yt = 10;
double rt = 0.5;

int TMeas::Calculate(void)
{

   const double PI = 3.14159265;

   double x1 = 0;
   double x2 = 0;

   double y1 = 0;
   double y2 = 0;

   double xres = 0;
   double yres = 0;

   double lx = 0;
   double ly = 0;

   double depth = 0;

   double d = 0;
   double dd = 0;

   double zshift = 0;


	for (int i = 0; i < records_cnt; i++) {

		x1 = records[i].X1;
		x2 = records[i].X2;

		y1 = records[i].Y1;
		y2 = records[i].Y2;

		d  = records[i].depth;


		if (records[0].X2 == 0) {

			x2 =  (records[0].X1)*(-1.);

		}

		//Горизонтальная
		if (type_drill == 0)
		{
			//Двухпроходная
			if (single_way == 0)
			{
				xres = (x1-x2)/2;
				yres = (y1-y2)/2;
			}
			//Однопроходная
			else
			{
				if (i == 0) {

					xres = (x1-x2)/2;
					zshift = (x1+x2)/2;
				}
				else
				{
					xres = x1 - zshift;
					//xres = zshift - x1;
				}

				yres = 0;
			}

			records[i].Xres = xres;
			records[i].Yres = yres;

			if ( i==0 ) {

				 lx = 0;
				 ly = 0;
			}
			else
			{

					if (single_way == 1) {

					   /* в таблице заказчика расчет делается не по предыдущей точке а по текущей */
					   lx = (records[i].depth - records[i-1].depth) * sin(records[i].Xres/3600*PI/180) * 1000 + records[i-1].LX;
					   /* для горизонтальной однопроходной применяется гео поправка */
					   if (geo_on == true) {

							double geo_delta = (input_point - output_point)/(double)records_cnt;
							lx = lx + geo_delta*(double)(i+1);


					   }

					}
					else
					{
                        lx = (records[i].depth - records[i-1].depth) * sin(records[i-1].Xres/3600*PI/180) * 1000 + records[i-1].LX;
						ly = (records[i].depth - records[i-1].depth) * sin(records[i-1].Yres/3600*PI/180) * 1000 + records[i-1].LY;
					}

			}

			records[i].LX = lx;
			records[i].LY = ly;

			//Двухпроходная
			if (single_way == 0)
			{
				records[i].LR = sqrt((lx*lx)+(ly*ly));
			}
			//Однопроходная
			else
			{
				records[i].LR = lx;
			}

		}


		//Вертикальная
		if (type_drill == 1)
		{


			xres = (x1-x2)/2;
			yres = (y1-y2)/2;

			records[i].Xres = xres;
			records[i].Yres = yres;

			if (drill_start_point == 0)
			{

				if ( i==0 ) {

					 lx = 0;
					 ly = 0;
				}
				else
				{

						lx = (records[i].depth - records[i-1].depth) * sin(records[i-1].Xres/3600*PI/180) * 1000 + records[i-1].LX;
						ly = (records[i].depth - records[i-1].depth) * sin(records[i-1].Yres/3600*PI/180) * 1000 + records[i-1].LY;
				}

				records[i].LX = lx;
				records[i].LY = ly;

				records[i].LR = sqrt((lx*lx)+(ly*ly));

				if (abs(records[i].LX) > 0.0001) {

					records[i].AR = atan(records[i].LY/records[i].LX);
					//Переводим в градусы
					records[i].AR *= PI;
					records[i].AR /= 180;
					//переводим в секунды
					records[i].AR *= 3600;

				  }

			}
			else
			{
				int ii = records_cnt-i-1;

				if ( ii==records_cnt-1 ) {

					 lx = 0;
					 ly = 0;
				}
				else
				{
						double dxcorrection = 0;

						lx = (records[ii+1].depth - records[ii].depth) * sin((records[ii+1].Xres)/3600*PI/180) * 1000 + records[ii+1].LX;
						ly = (records[ii+1].depth - records[ii].depth) * sin((records[ii+1].Yres)/3600*PI/180) * 1000 + records[ii+1].LY;

						if (ii==records_cnt-2) {

							  dxcorrection =  lx;
						}

						lx-=dxcorrection;
				}


				records[ii].LX = lx;
				records[ii].LY = ly;

				records[ii].LR = sqrt((lx*lx)+(ly*ly));

				 if (abs(records[ii].LX) > 0.0001) {

					records[ii].AR = atan(records[ii].LY/records[ii].LX);
					//gthtcxbnsdftv hflbfys d uhflecs
					records[ii].AR *= PI;
					records[ii].AR /= 180;
					//переводим в секунды
					records[ii].AR *= 3600;

				  }

            }



		}


	}



	this->finalize_time = Now();

	finalized = true;


	SaveData(0);
	DataToTable();

	return 0;
}

void TMeas::UpdateMark(WideString mark)
{
	if (this->node != NULL)
	{

		this->mark = mark;
		WideString s(L"");
		WideString st(L"");

		st = FormatDateTime("[yyyy-mm-dd hh:nn:ss]", this->create_time);
		s.printf(L"%d.%d.%d %s [%s]",pnum, dnum, num, st.c_bstr(), this->mark.c_bstr());
		this->node->Text = s;

	}

}

TTreeNode* TMeas::Redraw(TTreeView* t, TTreeNode* n)
{
	if (n == NULL) {

		return NULL;
	}

	WideString s(L"");
	WideString st(L"");

	st = FormatDateTime("[yyyy-mm-dd hh:nn:ss]", this->create_time);
	s.printf(L"%d.%d.%d %s [%s]",pnum, dnum, num, st.c_bstr(), this->mark.c_bstr());

	node  =	t->Items->AddChild(n,s);

	if (selected == false) {

		utils_set_tree_node_view(node, SYSTEM_NODE_MEAS, SYSTEM_NODE_STATE_NORMAL);
	}
	else
	{
		utils_set_tree_node_view(node, SYSTEM_NODE_MEAS, SYSTEM_NODE_STATE_ALARM);
	}

	return node;

}


void TMeas::Select()
{
	utils_set_tree_node_view(node, SYSTEM_NODE_MEAS, SYSTEM_NODE_STATE_ALARM);
	selected = true;
}

void TMeas::Unselect()
{
	utils_set_tree_node_view(node, SYSTEM_NODE_MEAS, SYSTEM_NODE_STATE_NORMAL);
	selected = false;
}

void TMeas::dtc(TCHAR* str)
{

	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == '.')
		{
			str [i] = ',';
		}

		if (i > 1023)
		{
			break;
		}
	}

}

int TMeas::SaveData(int par)
{
	if (par) this->TableToData();

	TCHAR dir[1024];
	TCHAR tdir[1024];
	TCHAR csvdir[1024];

	wcscpy(dir,SysConfMgr->GetCurBase());
	if (!DirectoryExists(dir))  CreateDirectory(dir,0);

	wcscat(dir,this->name_place.c_bstr());
	if (!DirectoryExists(dir))  CreateDirectory(dir,0);

	wcscat(dir,L"\\");
	wcscat(dir,this->name_drill.c_bstr());
	if (!DirectoryExists(dir))  CreateDirectory(dir,0);

	WideString s(L"");
	s.printf(L"Измерение_%d", this->num);

	wcscat(dir,L"\\");
	wcscat(dir,s.c_bstr());
	if (!DirectoryExists(dir))  CreateDirectory(dir,0);

	wcscat(dir,L"\\");
	wcscat(dir,s.c_bstr());

	wcscpy(tdir,dir);
	wcscpy(csvdir,dir);
	wcscat(dir,L".gms");

	/* dir for csv file */
	wcscat(csvdir,L".csv");


	FILE* f = _wfopen(dir,L"wb");
	if (f == NULL) {

		ShowMessage(L"Не удалось открыть файл для сохранения данных измерения!");
		return -1;
	}

	fwrite(&pnum, sizeof(pnum),1,f);
	fwrite(&dnum, sizeof(dnum),1,f);
	fwrite(&num, sizeof(num),1,f);
	fwrite(&create_time, sizeof(create_time),1,f);
	fwrite(&finalize_time, sizeof(finalize_time),1,f);
	fwrite(&finalized, sizeof(finalized),1,f);
	fwrite(&records, sizeof(records),1,f);

	fclose(f);

	wcscat(tdir,L".txt");
	::DeleteFileW(tdir);
	CreateTextFile_UTF16LEBOM (tdir);
	FILE* ft = _wfopen(tdir,L"ab");

	if (ft == NULL) {

		ShowMessage(L"Не удалось открыть файл для сохранения данных измерения!");
		return -1;
	}

	::DeleteFileW(csvdir);
	CreateTextFile_UTF16LEBOM (csvdir);
	FILE* fc = _wfopen(csvdir,L"ab");

	if (fc == NULL) {

		ShowMessage(L"Не удалось открыть файл CSV для сохранения данных измерения!");
		return -1;
	}


	WideString spar(L"");
	TCHAR cpar[1024];

	WideString spar_csv(L"");
	TCHAR cpar_csv[1024];

	/* Place */
	spar.printf(L"Место:\t\t%s\r\n", name_place.c_bstr());
	wcscpy(cpar,spar.c_bstr());
	fwrite(cpar,wcslen(cpar)*2,1,ft);

	spar.printf(L"Скважина:\t%s\r\n", name_drill.c_bstr());
	wcscpy(cpar,spar.c_bstr());
	fwrite(cpar,wcslen(cpar)*2,1,ft);

	/* CSV */

	spar_csv.printf(L"Место;%s\r\n", name_place.c_bstr());
	wcscpy(cpar_csv,spar_csv.c_bstr());
	fwrite(cpar_csv,wcslen(cpar_csv)*2,1,fc);

	spar_csv.printf(L"Скважина;%s\r\n", name_drill.c_bstr());
	wcscpy(cpar_csv,spar_csv.c_bstr());
	fwrite(cpar_csv,wcslen(cpar_csv)*2,1,fc);

	/* Drill */
	if (type_drill == 0)
	{
		spar.printf(L"Направление:\tГОРИЗОНТАЛЬНАЯ\r\n");
	}
	else
	{
		spar.printf(L"Направление:\tВЕРТИКАЛЬНАЯ\r\n");
	}
	wcscpy(cpar,spar.c_bstr());
	fwrite(cpar,wcslen(cpar)*2,1,ft);

	/* CSV */
    if (type_drill == 0)
	{
		spar_csv.printf(L"Направление;ГОРИЗОНТАЛЬНАЯ\r\n");
	}
	else
	{
		spar_csv.printf(L"Направление;ВЕРТИКАЛЬНАЯ\r\n");
	}
	wcscpy(cpar_csv,spar_csv.c_bstr());
	fwrite(cpar_csv,wcslen(cpar_csv)*2,1,fc);

	//----------------------------------------------------

	if (single_way == 0)
	{
		spar.printf(L"Один проход:\tНЕТ\r\n");
	}
	else
	{
		spar.printf(L"Один проход:\tДА\r\n");
	}
	wcscpy(cpar,spar.c_bstr());
	fwrite(cpar,wcslen(cpar)*2,1,ft);

	/* CSV */

	if (single_way == 0)
	{
		spar_csv.printf(L"Один проход;НЕТ\r\n");
	}
	else
	{
		spar.printf(L"Один проход;ДА\r\n");
	}
	wcscpy(cpar_csv,spar_csv.c_bstr());
	fwrite(cpar_csv,wcslen(cpar_csv)*2,1,fc);

	//----------------------------------------------------

	spar.printf(L"Измерение:\t%d\r\n", this->num);
	wcscpy(cpar,spar.c_bstr());
	fwrite(cpar,wcslen(cpar)*2,1,ft);

	/* CSV */
	spar_csv.printf(L"Измерение;%d\r\n", this->num);
	wcscpy(cpar_csv,spar_csv.c_bstr());
	fwrite(cpar_csv,wcslen(cpar_csv)*2,1,fc);

	//----------------------------------------------------

	spar = FormatDateTime(L"Создано:\tdd-mm-yyyy hh:nn:ss\r\n",create_time);
	wcscpy(cpar,spar.c_bstr());
	fwrite(cpar,wcslen(cpar)*2,1,ft);

	/* CSV */

	spar_csv = FormatDateTime(L"Создано;dd-mm-yyyy hh:nn:ss\r\n",create_time);
	wcscpy(cpar_csv,spar_csv.c_bstr());
	fwrite(cpar_csv,wcslen(cpar_csv)*2,1,fc);

	//------------------------------------------------------
	if (finalized == true) {

		spar = FormatDateTime(L"Завершено:\tdd-mm-yyyy hh:nn:ss\r\n\r\n",finalize_time);
		wcscpy(cpar,spar.c_bstr());
	}
	else
	{
		spar = L"Завершено:\tНЕТ\r\n\r\n";
	}

	wcscpy(cpar,spar.c_bstr());
	fwrite(cpar,wcslen(cpar)*2,1,ft);

	/* CSV */

	if (finalized == true) {

		spar_csv = FormatDateTime(L"Завершено;dd-mm-yyyy hh:nn:ss\r\n\r\n",finalize_time);
		wcscpy(cpar_csv,spar_csv.c_bstr());
	}
	else
	{
		spar_csv = L"Завершено;НЕТ\r\n\r\n";
	}

	wcscpy(cpar_csv,spar_csv.c_bstr());
	fwrite(cpar_csv,wcslen(cpar_csv)*2,1,fc);

	//-----------------------------------------------------------
	spar = L"Номер\tГлубина\t\tОшибка\t\tX1\t\tY1\t\tX2\t\tY2\t\tLX\t\tLY\t\tLR\t\tAR\r\n";
	wcscpy(cpar,spar.c_bstr());
	fwrite(cpar,wcslen(cpar)*2,1,ft);

	/* CSV */
	spar_csv = L"Номер;Глубина;Ошибка;X1;Y1;X2;Y2;LX;LY;LR;AR\r\n";
	wcscpy(cpar_csv,spar_csv.c_bstr());
	fwrite(cpar_csv,wcslen(cpar_csv)*2,1,fc);

	//-----------------------------------------------------------

	for (int i = 0; i < records_cnt; i++) {

		spar.printf(L"%d\t%.1f\t\t%.1f\t\t%.1f\t\t%.1f\t\t%.1f\t\t%.1f\t\t%.1f\t\t%.1f\t\t%.1f\t\t%.1f\r\n",

		i,
		this->records[i].depth,
		this->records[i].tuberr,
		this->records[i].X1,
		this->records[i].X2,
		this->records[i].Y1,
		this->records[i].Y2,
		this->records[i].LX,
		this->records[i].LY,
		this->records[i].LR,
		this->records[i].AR

		);

	wcscpy(cpar,spar.c_bstr());
	fwrite(cpar,wcslen(cpar)*2,1,ft);

	}

	/* CSV */

	for (int i = 0; i < records_cnt; i++) {

		spar_csv.printf(L"%d;%.1f;%.1f;%.1f;%.1f;%.1f;%.1f;%.1f;%.1f;%.1f;%.1f\r\n",

		i,
		this->records[i].depth,
		this->records[i].tuberr,
		this->records[i].X1,
		this->records[i].X2,
		this->records[i].Y1,
		this->records[i].Y2,
		this->records[i].LX,
		this->records[i].LY,
		this->records[i].LR,
		this->records[i].AR

		);

	wcscpy(cpar_csv,spar_csv.c_bstr());
	dtc (cpar_csv);
	fwrite(cpar_csv,wcslen(cpar_csv)*2,1,fc);

	}

	//--------------------------------------------------------------------------

	fclose(ft);
	fclose(fc);

	return 0;
}

int TMeas::LoadData()
{

	TCHAR dir[1024];
	TCHAR tdir[1024];

	wcscpy (dir, SysConfMgr->GetCurBase());
	if (!DirectoryExists(dir))  CreateDirectory(dir,0);

	//wcscat(dir,L"\\");
	wcscat(dir,this->name_place.c_bstr());
	if (!DirectoryExists(dir))  CreateDirectory(dir,0);

	wcscat(dir,L"\\");
	wcscat(dir,this->name_drill.c_bstr());
	if (!DirectoryExists(dir))  CreateDirectory(dir,0);

	WideString s(L"");
	s.printf(L"Измерение_%d", this->num);

	wcscat(dir,L"\\");
	wcscat(dir,s.c_bstr());
	if (!DirectoryExists(dir))  CreateDirectory(dir,0);

	wcscat(dir,L"\\");
	wcscat(dir,s.c_bstr());

	wcscpy(tdir,dir);
	wcscat(dir,L".gms");

	FILE* f = _wfopen(dir,L"rb");

	if (f == NULL) {

		return -1;
	}

	fread(&pnum, sizeof(pnum),1,f);
	fread(&dnum, sizeof(dnum),1,f);
	fread(&num, sizeof(num),1,f);
	fread(&create_time, sizeof(create_time),1,f);
	fread(&finalize_time, sizeof(finalize_time),1,f);
	fread(&finalized, sizeof(finalized),1,f);
	fread(&records, sizeof(records),1,f);
	fclose(f);


	this->DataToTable();


	return 0;
}

int TMeas::ImportDepthTable(TMeas* m)
{
	if (m == NULL) {

		return -1;
	}

	for (int i = 0; i < records_cnt; i++) {

		this->records[i].depth = m->records[i].depth;
		this->records[i].tuberr = m->records[i].tuberr;

	}

	return 0;
}


bool TMeas::DeleteDir(WideString DirName)
{
	TSearchRec sr;
	if (DirName.Length())
	{
		 if (!FindFirst(DirName+"\\*.*",faAnyFile,sr))
		 {
			 do
			  {
				  if (!(sr.Name=="." || sr.Name==".."))
				  {// это удалять не надо
				   if (((sr.Attr & faDirectory) == faDirectory ) || (sr.Attr == faDirectory))// найдена папка
					{
						FileSetAttr(DirName+""+sr.Name, faDirectory );// сброс всяких read-only
						DeleteDir(DirName+""+sr.Name);//рекурсивно удаляем содержимое
						RemoveDir(DirName + ""+sr.Name);// удаляем теперь уже пустую папку
					}
					else// иначе найден файл
					{
						FileSetAttr(DirName+""+sr.Name, 0);// сброс всяких read-only
						DeleteFile(DirName+""+sr.Name);// удаляем файл
					}
				  }
			  }

			 while (!FindNext(sr));// ищем опять, пока не найдем все
		 }
		 FindClose(sr);
	 }
	RemoveDir(DirName);

return true;
}

extern double excel_export_rate;

WideString TMeas::dts(double d)
{
	WideString s(L"");
	TCHAR t[1024];
	//s.printf(L"%.2f", d);

	swprintf(t, L"%.2f", d);

	for (int i = 0; i < wcslen(t); i++)
	{
		if (t[i] == '.')
		{
			t[i] = ',';
		}
	}

	s = t;

	return s;
}

int TMeas::Excel (int par)
{

	TCHAR tdir[1024];

	wcscpy(tdir,SysConfMgr->GetCurBase());
	if (!DirectoryExists(tdir))  CreateDirectory(tdir,0);

	//wcscat(dir,L"\\");
	wcscat(tdir,this->name_place.c_bstr());
	if (!DirectoryExists(tdir))  CreateDirectory(tdir,0);

	wcscat(tdir,L"\\");
	wcscat(tdir,this->name_drill.c_bstr());
	if (!DirectoryExists(tdir))  CreateDirectory(tdir,0);

	WideString s(L"");
	s.printf(L"Измерение_%d", this->num);

	wcscat(tdir,L"\\");
	wcscat(tdir,s.c_bstr());
	if (!DirectoryExists(tdir))  CreateDirectory(tdir,0);

	wcscat(tdir,L"\\");
	wcscat(tdir,s.c_bstr());

	wcscat(tdir,L".xlsx");

	TCHAR cmd[1024];
	wcscpy(cmd,L"del ");
	wcscat(cmd,L"\"");
	wcscat(cmd,tdir);
	wcscat(cmd,L"\"");

	if (par)
	{
		system((char*)cmd);
	}


//Создаем экземпляр таблицы

		if (par)
		{
			if (OpenExcelReportTable(1) == false)
			{
				MessageBox(NULL,L"Не удалось создать таблицу!  Проверьте наличие установленного MS Excell.",L"ОШИБКА",0);
				return -1;
			}
		}

		if (par)
		{
			if (OpenExcelReportSheet(name, 1) == false)
			{
				MessageBox(NULL,L"Не удалось создать лист таблицы!",L"ОШИБКА",0);
				return -1;
			}
		}
		else
		{
			if (OpenExcelReportSheet(name, num) == false)
			{
				MessageBox(NULL,L"Не удалось создать лист таблицы!",L"ОШИБКА",0);
				return -1;
			}
		}


	if (par)
	{
	 Form_excel_progress->StartShow(records_cnt);
	}


	int irow = 1;
	int icol = 1;

	//if (par)

	WriteExcelReportCell(irow,1,L"Площадка:"); WriteExcelReportCell(irow,2,this->name_place); irow++;
	WriteExcelReportCell(irow,1,L"Скважина:"); WriteExcelReportCell(irow,2,name_drill); irow++;

	if (type_drill == 0)
	{
		WriteExcelReportCell(irow,1,L"Направление:"); WriteExcelReportCell(irow,2,L"ГОРИЗОНТАЛЬНАЯ"); irow++;
	}
	else
	{
		WriteExcelReportCell(irow,1,L"Направление:"); WriteExcelReportCell(irow,2,L"ВЕРТИКАЛЬНАЯ"); irow++;
	}

	if (single_way == 0)
	{
		WriteExcelReportCell(irow,1,L"Один проход:"); WriteExcelReportCell(irow,2,L"НЕТ"); irow++;
	}
	else
	{
		WriteExcelReportCell(irow,1,L"Один проход:"); WriteExcelReportCell(irow,2,L"ДА"); irow++;
	}

	WriteExcelReportCell(irow,1,L"Измерение:"); WriteExcelReportCell(irow,2,name); irow++;

	s = FormatDateTime(L"dd-mm-yyyy hh:nn:ss",create_time);
	WriteExcelReportCell(irow,1,L"Создано:"); WriteExcelReportCell(irow,2,s); irow++;

	if ((double) finalize_time > 0.0)
	{
		s = FormatDateTime(L"dd-mm-yyyy hh:nn:ss",finalize_time);
	}
	else
	{
		s = "НЕТ";
	}

	WriteExcelReportCell(irow,1,L"Завершено:"); WriteExcelReportCell(irow,2,s); irow++;

	icol = 1;

	WriteExcelReportCell(irow,icol,L"Глубина"); icol++;
	WriteExcelReportCell(irow,icol,L"Погр. тр."); icol++;
	WriteExcelReportCell(irow,icol,L"X1"); icol++;
	WriteExcelReportCell(irow,icol,L"X2"); icol++;
	WriteExcelReportCell(irow,icol,L"Y1"); icol++;
	WriteExcelReportCell(irow,icol,L"Y2"); icol++;
	WriteExcelReportCell(irow,icol,L"LX"); icol++;
	WriteExcelReportCell(irow,icol,L"LY"); icol++;
	WriteExcelReportCell(irow,icol,L"LR"); icol++;
	WriteExcelReportCell(irow,icol,L"AR"); icol++;

	irow++;

	meas_record* r = NULL;
	double per = 0;
	WideString sbar(L"");

	for (int i = 0; i < records_cnt; i++)
	{
		r = &this->records[i];
		icol = 1;
		WriteExcelReportCell(irow,icol,dts(r->depth)); icol++;
		WriteExcelReportCell(irow,icol,dts(r->tuberr)); icol++;
		WriteExcelReportCell(irow,icol,dts(r->X1)); icol++;
		WriteExcelReportCell(irow,icol,dts(r->X2)); icol++;
		WriteExcelReportCell(irow,icol,dts(r->Y1)); icol++;
		WriteExcelReportCell(irow,icol,dts(r->Y2)); icol++;
		WriteExcelReportCell(irow,icol,dts(r->LX)); icol++;
		WriteExcelReportCell(irow,icol,dts(r->LY)); icol++;
		WriteExcelReportCell(irow,icol,dts(r->LR)); icol++;
		WriteExcelReportCell(irow,icol,dts(r->AR)); icol++;
		irow++;

		per = ((i+1)/records_cnt)*100;
		excel_export_rate = per;

		if (par)
		{
			Form_excel_progress->UpdateShow(i);
		}
		else
		{
			Form_excel_progress->UpdateShow(i+(records_cnt*(num-1)));
        }
	}

	if (par)
	{
		SaveExcelReport(tdir);
		Form_excel_progress->StopShow();
	}

	excel_export_rate = 0.;

	return 0;
}


int TMeas::Sort (int par)
{
	memcpy(records_sort,records,sizeof(records));

	if (par<0) {

		return -1;
	}

	meas_record mtmp;
	double min;

	for (int i=0; i < records_cnt; i++) {

		min = records_sort[i].LX;

		for (int j = i; j < records_cnt; j++) {

			if (records_sort[j].LX<min) {

				min = records_sort[j].LX;
				memcpy(&mtmp,&records_sort[i],sizeof(mtmp));
				memcpy(&records_sort[i],&records_sort[j],sizeof(mtmp));
				memcpy(&records_sort[j],&mtmp,sizeof(mtmp));
			}

		}

	}

	return 0;
}

int TMeas::ConvertDataUTF16(void)
{

	TCHAR dir[1024];

	wcscpy(dir,SysConfMgr->GetCurBase());
	if (!DirectoryExists(dir))  CreateDirectory(dir,0);

	wcscat(dir,this->name_place.c_bstr());
	if (!DirectoryExists(dir))  CreateDirectory(dir,0);

	wcscat(dir,L"\\");
	wcscat(dir,this->name_drill.c_bstr());
	if (!DirectoryExists(dir))  CreateDirectory(dir,0);

	WideString s(L"");
	s.printf(L"\\Измерение_%d", this->num);

	wcscat(dir,s.c_bstr());
    wcscat(dir,s.c_bstr());

	wcscat(dir,L".txt");

	ConvertTextFile_UTF16LEBOM (dir);

	return 0;
}

int TMeas::isMeasTxtUTF8(TCHAR* path)
{
	FILE* f = _wfopen (path, L"rb");

	char buf[10];
	int res = 0;

	if (fread(buf,10,1,f))
	{
		 if (buf[0] != 0xCC) {

			res = -1;

		 }
	}
	else
	{
		res = -2;
	}

	fclose(f);

	return res;
}

int TMeas::AcceptDataFileRecord(int dir, double d, double x, double y)
{
	 int res = -1;

    /* auto adjust the records cnt using step 0.5 */
	int rec_num_calc = (d/0.5) + 1;

	if (this->records_cnt < rec_num_calc)
	{
		records_cnt = rec_num_calc;

		for (int i = 1; i < records_cnt; i++)
		{
			if (records[i].depth != (records[i-1].depth + 0.5))
			{
				records[i].depth = (records[i-1].depth + 0.5);
			}
		}
	}
	/* end of records cnt auto adjust */


	 for (int i =0; i < records_cnt; i++)
	 {
		  if (records[i].depth == d)
		  {
			 if (dir == 0)
			 {
				records[i].X1 = x;
				records[i].Y1 = y;
			 }
			 else
			 {
				records[i].X2 = x;
				records[i].Y2 = y;
			 }

			 res = 0;

             break;
		  }

	 }

	 return res;
}

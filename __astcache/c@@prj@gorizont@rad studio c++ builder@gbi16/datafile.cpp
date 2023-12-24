//---------------------------------------------------------------------------

#pragma hdrstop

#include "DataFile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


TDataFile::TDataFile()
{
	file = NULL;
	cbuf_idx = 0;
	memset(cbuf,0,1024);
	str_idx = 0;
	memset(str,0,1024);
	pars_en = false;

	memset(data_file_meas_set, 0, sizeof(data_file_meas_set));
	data_file_meas_set_idx= 0;
	data_file_meas_set_idx_cur = 0;
	data_file_meas_ptr_cur = NULL;
}

TDataFile::~TDataFile()
{

}

int TDataFile::OpenFile(TCHAR* tdir)
{
	int res = 0;

	if ((file = _wfopen(tdir,L"rb")) == NULL) res = -1; //file not opened

	return res;
}

int TDataFile::CheckFile(void)
{
	if (file == NULL) return -1; //file not opened

	//check utf8 ebom signature
	char sign [] = {0xef, 0xbb, 0xbf};
	BYTE sdata [3];

	if (!fread(sdata,3,1,file)) return -2; //file read fail

	if ((sdata[0] != 0xef)||(sdata[1] != 0xbb)||(sdata[2] != 0xbf))
	{
		return -3; //wrong file signature
	}

	//check file format (header string)  [Объект; Место; Дата; Направление; Позиция; X; Y;]
	GetStr();

	// do not pars the header
	pars_en = false;

	int r = 0;

	GetWord();
	if (wcsstr(tword,L"Объект") == NULL)  return -4;

	GetWord();
	if (wcsstr(tword,L"Место") == NULL)  return -4;

	GetWord();
	if (wcsstr(tword,L"Дата") == NULL)  return -4;

	GetWord();
	if (wcsstr(tword,L"Направление") == NULL)  return -4;

	GetWord();
	if (wcsstr(tword,L"Позиция") == NULL)  return -4;

	GetWord();
	if (wcsstr(tword,L"X") == NULL) return -4;

	GetWord();
	if (wcsstr(tword,L"Y") == NULL)  return -4;



	return 0;
}

int TDataFile::GetStr(void)
{
    int res = -1;
	cbuf_idx = 0;
	tbuf_idx = 0;
	tword_idx = 0;
	pars_pos = w_undef;
	tbuf_pars_idx = 0;
	pars_str_res = 0;

	memset(cbuf,0,1024);
	memset(tbuf,0,sizeof(tbuf));
	memset(tword,0,sizeof(tword));

	char c;

	while (fread(&c,1,1,file))
	{
		cbuf[cbuf_idx++] = c;

		if ((cbuf[cbuf_idx-2] == '\r') && (cbuf[cbuf_idx-1] == '\n'))
		{
			res = 0;
			break;
		}
   }


   memset(tbuf,0,sizeof(tbuf));
   tbuf_idx = 0;

   ConvertStrUTF8ToUtf16 (cbuf, tbuf, cbuf_idx);

   tbuf_idx = wcslen(tbuf);

   return res;
}

void TDataFile::Close(void)
{
	fclose(file);
	cbuf_idx = 0;
	memset(cbuf,0,1024);
	str_idx = 0;
	memset(str,0,1024);
}

int TDataFile::GetWord(void)
{

   if (tword_idx == -1) return -1; //the string is not meas

   TCHAR tsym = 0;
   bool wcor = false;

   memset(tword,0,sizeof(tword));
   tword_idx = 0;

   while (tbuf_pars_idx<tbuf_idx)
   {
	  tsym =tbuf [tbuf_pars_idx++];

	  if (tsym == (TCHAR)';')
	  {
		  wcor = true;
		  break;
	  }
	  else
	  {
		  tword [tword_idx++] = tsym;
      }
   }

   if (wcor == true)
   {
		 pars_pos++;

		 if (pars_pos == 1)
		 {
			 pars_str_res += GetSpar();
			 data_file_record.place = s_par;
		 }

		 if (pars_pos == 2)
		 {
			 pars_str_res += GetSpar();
			 data_file_record.drill = s_par;
		 }

		 if (pars_pos == 3)
		 {
			 pars_str_res += GetTpar();
			 data_file_record.time = t_par;
		 }

		 if (pars_pos == 4)
		 {
			 pars_str_res += GetSpar();
			 data_file_record.dir = s_par;
		 }

		 if (pars_pos == 5)
		 {
			 pars_str_res += GetDpar();
			 data_file_record.level = d_par;
		 }

		 if (pars_pos == 6)
		 {
			 pars_str_res += GetDpar();
			 data_file_record.X = d_par;
		 }

		 if (pars_pos == 7)
		 {
			 pars_str_res += GetDpar();
			 data_file_record.Y = d_par;

			 //if the complete res is 0 accept the record
			 if (pars_str_res == 0)
			 {
				 AcceptDaTaFileMeasRecord();
			 }
			 else
			 {
				 Sleep(1);
			 }
		 }

   }
   else
   {
	   return -1; //str pars problem
   }

   return 0;
}

 int TDataFile::GetSpar(void)
 {
	if (pars_en == false) return -1; //the string is not meas

	TCHAR tc[1024];
	memset(tc,0,1024);

	int j = 0;

	//remove spaces
	for (int i = 0; i < wcslen(tword); i++)
	{
		if (tword[i] != ' ')
		{
		   tc [j++] = tword [i];
		}
	}

	s_par = tc;
	return 0;
 }

 int TDataFile::GetTpar(void)
 {

	if (pars_en == false) return -1; //the string is not meas

	TFormatSettings FS;
	FS.DateSeparator = '.';
	FS.ShortDateFormat = "dd.mm.yyyy";
	FS.LongTimeFormat = "hh:nn:ss";
	FS.TimeSeparator = ':';

	try
	{
		t_par = StrToDateTime((UnicodeString)tword, FS);
	}
	catch(...)
	{
		t_par = 0;
		return -1; //bad date_time format
	}

	return 0;
}

int TDataFile::GetDpar(void)
 {

	if (pars_en == false) return -1; //the string is not meas

	TCHAR tc[1024];
	memset(tc,0,1024);

	int j = 0;

	//filter digit symbols
	for (int i = 0; i < wcslen(tword); i++)
	{
		if (
			(tword[i] == '0')||(tword[i] == '1')||(tword[i] == '2')||
			(tword[i] == '3')||(tword[i] == '4')||(tword[i] == '5')||
			(tword[i] == '6')||(tword[i] == '7')||(tword[i] == '8')||
			(tword[i] == '9')||(tword[i] == '.')
		)
		{
		   tc [j++] = tword [i];
		}
	}

	WideString ws;
	ws = tc;

	if (ws == L"")
	{
		d_par = 0;
	}
	else
	{
		try
		{
			d_par = ws.ToDouble();
		}
		catch (...)
		{
			d_par = 0;
			return -1;
		}
	}
	return 0;
}

int TDataFile::AcceptDaTaFileRecord(void)
{


	return 0;
}

/*
Find the meas in the meas set with the presented place, drill, time
return if the operatin is success  0 / not success -1
set the  data_file_meas_set_idx_cur if the meas is found/created
*/
int TDataFile::GetDaTaFileMeasIdx(void)
{
	int res = -1; //if the meas is not found/created
	data_file_meas_type* m;
	bool exist = false;


	for (int i = 0; i < data_file_meas_set_idx; i++)
	{

		m = data_file_meas_set [i];

		if (
			(data_file_record.place == m->place)&&
			(data_file_record.drill == m->drill)&&
			(data_file_record.time == m->time)
		   )
		{
			exist = true;
			data_file_meas_set_idx_cur = i;
			res = 0;
			break;
		}
	}

	//if not exist in the list
	if (exist == false)
	{
		m = new data_file_meas_type;

		memset(m,0,sizeof(data_file_meas_type));

		m->place = data_file_record.place;
		m->drill = data_file_record.drill;
		m->time = data_file_record.time;

		data_file_meas_set [data_file_meas_set_idx] = m;
		data_file_meas_set_idx_cur = data_file_meas_set_idx;
		data_file_meas_set_idx++;

		res = 0;
	}

	return res;
}

int TDataFile::AcceptDaTaFileMeasRecord(void)
{

	if (pars_en == false) return -1;

	int res = 0;

	int itmp = 0;

	if (GetDaTaFileMeasIdx() == 0)
	{
		data_file_meas_type* m = data_file_meas_set [data_file_meas_set_idx-1];

		if (m != NULL)
		{
			if (m->record_cnt < DATA_FILE_MEAS_CNT_MAX)
			{
				m->record [m->record_cnt] = data_file_record;
				m->record_cnt++;
				itmp =  m->record_cnt;
			}
			else
			{
				res = -1;  //overflow
			}
		}
		else
		{
            res = -1; //bad meas ptr
        }

	}

	return res;
}

int TDataFile::ParsDaTaFile(TCHAR* tdir)
{
   int res = -1;

   memset(data_file_meas_set, 0, sizeof(data_file_meas_set));
   data_file_meas_set_idx = 0;
   data_file_meas_set_idx_cur = 0;

   pars_en = false;

   if (OpenFile(tdir) == 0)
   {
	  if (CheckFile() == 0)
	  {
		  res = 0;

		  pars_en = true;

		  while (GetStr() == 0)
		  {
			  while (GetWord() ==0)
			  {

			  }
		  }
	  }
   }


   int i = 0;
   data_file_meas_type* m = NULL;

   while (data_file_meas_set[i] != NULL)
   {
	  m = data_file_meas_set[i];
      i++;
   }


   return res;
}

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
	char sdata [3];

	if (!fread(sdata,3,1,file)) return -2; //file read fail

	for (int i = 0; i < 3; i++)
	{
		if (sign[i] != sdata[i]) return -3; //wrong file signature
	}

	//check file format (header string)  [Объект; Место; Дата; Направление; Позиция; X; Y;]
	char head [] = "Объект; Место; Дата; Направление; Позиция; X; Y;\r\n";

	GetStr();

	for (int i = 0; i < strlen(head); i++)
	{
		if (head[i] != cbuf[i]) return -4; //wrong header string
	}

	return 0;
}

int TDataFile::GetStr(void)
{
    int res = -1;
	cbuf_idx = 0;
	tbuf_idx = 0;

	memset(cbuf,0,1024);

	char c;

	while (fread(&c,1,1,file))
	{
		cbuf[cbuf_idx] = c;  cbuf_idx++;


		if (((BYTE)c != 0xd0)&&((BYTE)c != 0xd1))
		{
			tbuf [tbuf_idx] = ConvertSmbFromUTF8 (c) ;  tbuf_idx++;
		}

		if ((cbuf[cbuf_idx-2] == '\r') && (cbuf[cbuf_idx-1] == '\n'))
		{
			res = 0;
			break;
		}
   }

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

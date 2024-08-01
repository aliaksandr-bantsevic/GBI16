//---------------------------------------------------------------------------

#pragma hdrstop

#include "utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


void utils_ShowMessage(WideString s)
{
   Application->MessageBoxW(s.c_bstr(),L"ВНИМАНИЕ!", 0);
}

AnsiString utils_int_to_str(int i, int d)
{
	AnsiString s;

	switch(d)
	{
		case 3:
			s.printf("%03d",i);
		break;

		case 6:
			s.printf("%06d",i);
		break;

		default:
		s.printf("%d",i);
	}

	return s;

}

bool utils_confirm_operation_save_adjust(void)
{

	return (Application->MessageBoxW(L"Вы хотите сохранить настройки?",L"ВНИМАНИЕ!",1) == IDOK);

}

int utils_set_tree_node_view(TTreeNode* node, int node_type, int node_state)
{
	if (node == NULL) {

		return -1; //wrong node pointer
	}

	if (node_type > SYSTEM_ICON_TYPES_WIDE-1) {

		return -2; //wrong node type
	}

	if (node_state > SYSTEM_ICON_STATES_WIDE-1) {

		return -3; //wrong node state
	}

	node_type%= SYSTEM_ICON_TYPES_WIDE;
	node_state%= SYSTEM_ICON_STATES_WIDE;

	int idx = 0;
	idx+= node_type * SYSTEM_ICON_STATES_WIDE;
	idx+= node_state;

	node->ImageIndex = idx;
    node->SelectedIndex = idx;

    return 0;
}

/*
  CRC16 library
*/

static const WORD crc_16_table[256] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

WORD Calc_CRC(WORD start, WORD stop, int n,BYTE *b)
{
  WORD sum;int i;
  sum=start;
  for(i=0;i<n;i++) sum=(sum<<8)^crc_16_table[((sum>>8)^b[i])&0xff];
  return sum^stop;
}

WORD Calc_CRC16(int n,BYTE *b)  { return Calc_CRC(0,0,n,b);}
WORD Calc_CITT16(int n,BYTE *b) { return Calc_CRC(0xffff,0,n,b);}
WORD Calc_CSum(int n,BYTE *b) { WORD res;int i;res=0;for(i=0;i<n;i++) res+=b[i];return res;}

//-----------------------------------------------------------------------------
WORD CRC16CCITT(WORD n,BYTE *b) // Calc_CITT16
{
  WORD crc=0xFFFF;
  while (n--) crc=(crc<<8)^crc_16_table[(crc>>8)^*b++];
  return crc;
}
//-----------------------------------------------------------------------------

int CreateTextFile_UTF16LEBOM (TCHAR* pszFilePath)
{

	int res = 0;

	if (!::PathFileExists(pszFilePath))
	{
		 WORD wBOM = 0xFEFF;
		 DWORD NumberOfBytesWritten;

		 HANDLE hFile = ::CreateFile(pszFilePath,
									 GENERIC_WRITE,
									 0,
									 NULL,
									 CREATE_NEW,
									 FILE_ATTRIBUTE_NORMAL,
									 NULL);

		 if (hFile != INVALID_HANDLE_VALUE)
		 {
			 ::WriteFile(hFile, &wBOM, sizeof(WORD), &NumberOfBytesWritten, NULL);
			 ::CloseHandle(hFile);
		 }
		 else
		 {
			 res = -1; //Не удалось создать файл
         }
	}
	else
	{
		res = -2; //Файл уже существует
    }

    return res;
}

int ConvertTextFile_UTF16LEBOM (TCHAR* pszFilePath)
{

	//int isutf8 = CheckTextFile_UTF16LEBOM (pszFilePath); //check if it is already utf16

	/*
	if (isutf8 != -1) {

		return -2; //file is not exist or not utf8
	}
    */

	TCHAR oldpath [1024];
	wcscpy(oldpath, pszFilePath);
	wcscat(oldpath,L".utf8");
	::RenameFile(pszFilePath, oldpath);

	BYTE smb;
	TCHAR SMB;

	FILE* fold = _wfopen(oldpath, L"rb");


	if (CreateTextFile_UTF16LEBOM (pszFilePath) == 0)
	{

		FILE* fnew = _wfopen(pszFilePath, L"wb");

		while(fread(&smb,1,1,fold) )
		{
				SMB = ConvertSmbFrom1251(smb);
				fwrite(&SMB,2,1,fnew);
		}

        fclose(fold);
		fclose(fnew);
		return 0;
	}
	else
	{
		fclose(fold);
		return -1;
    }
}


WideString sutf16(L"АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя");
TCHAR  tcutf16[65];

TCHAR ConvertSmbFrom1251 (char smb)
{
	TCHAR SMB = 0;

	wcscpy(tcutf16, sutf16.c_bstr());
	BYTE code = smb;

	if (code >191) {

		SMB = (TCHAR)tcutf16[code - 192];
	}
	else
	{
		SMB = (TCHAR)(smb);
    }

	return SMB;
}


TCHAR ConvertSmbFromUTF8 (char smb)
{
	TCHAR SMB = 0;

	wcscpy(tcutf16, sutf16.c_bstr());
	BYTE code = smb;

	if ((code >=0x90)&&(code <=0xbf))
	{

		SMB = (TCHAR)tcutf16[code - 0x90];
	}
	else if ((code >=0x80)&&(code <=0x8f))
		 {

			SMB = (TCHAR)tcutf16[code - 0x80 + 48];
		 }
		 else if (code == 0x22)
			  {
				SMB = (TCHAR)'"';
			  }
			  else
			  {
				SMB = (TCHAR)(smb);
			  }

	return SMB;
}

int CheckTextFile_UTF16LEBOM (TCHAR* pszFilePath)
{

	int res = 0;
	TCHAR* sign = L"[SYSTEM]";
	TCHAR  tcread [8];

	if (::PathFileExists(pszFilePath))
	{
		 WORD wBOM = 0;//0xFEFF;
		 DWORD NumberOfBytesWritten;

		 HANDLE hFile = ::CreateFile(pszFilePath,
									 GENERIC_READ,
									 0,
									 NULL,
									 OPEN_EXISTING,
									 FILE_ATTRIBUTE_NORMAL,
									 NULL);

		 if (hFile != INVALID_HANDLE_VALUE)
		 {
			 ::ReadFile(hFile, &tcread, wcslen(sign)*2, &NumberOfBytesWritten, NULL);
			 ::CloseHandle(hFile);

			 if (wcsstr(tcread, sign) == 0) {

				res = -1; //Не UTF16
			 }
		 }
		 else
		 {
			 res = -3; //Файл не открывается
		 }


	}
	else
	{
		res = -2; //Файл не существует
    }

	return res;
}


int ConvertStrUTF8ToUtf16 (char* strU8, TCHAR* strU16, int U8_len)
{
	int i = 0;
    unsigned long uni;
	int todo = 0;
	bool error = false;
	unsigned char ch = 0;

	unsigned long unicode [1024];
	int uidx = 0;

    memset(&unicode, 0, sizeof(unicode));

	while (i < U8_len)
	{
		ch = strU8[i++];

		if (ch <= 0x7F)
		{
			uni = ch;
			todo = 0;
		}
		else if (ch <= 0xBF)
		{
			return -1; //throw std::logic_error("not a UTF-8 string");
		}
		else if (ch <= 0xDF)
		{
			uni = ch&0x1F;
			todo = 1;
		}
		else if (ch <= 0xEF)
		{
			uni = ch&0x0F;
			todo = 2;
		}
		else if (ch <= 0xF7)
		{
			uni = ch&0x07;
			todo = 3;
		}
		else
		{
			return -1; //throw std::logic_error("not a UTF-8 string");
		}

		for (int j = 0; j < todo; ++j)
		{
			if (i == U8_len)
				return -1; //throw std::logic_error("not a UTF-8 string");
			unsigned char ch1 = strU8[i++];
			if (ch1 < 0x80 || ch1 > 0xBF)
				return -1; //throw std::logic_error("not a UTF-8 string");
			uni <<= 6;
			uni += ch1 & 0x3F;
		}

		if (uni >= 0xD800 && uni <= 0xDFFF)
			return -1; //throw std::logic_error("not a UTF-8 string");
		if (uni > 0x10FFFF)
			return -1; //throw std::logic_error("not a UTF-8 string");

		unicode[uidx++] = uni;
	}

	TCHAR utf16[1024];
    memset(&utf16, 0, sizeof(utf16));
	int utf16_idx = 0;

	for (int i = 0; i < uidx; ++i)
	{
		unsigned long uni1 = unicode[i];

		if (uni1 <= 0xFFFF)
        {
			utf16 [utf16_idx++] = (TCHAR)uni1;
		}
		else
		{
			uni1 -= 0x10000;
			utf16 [utf16_idx++] = (TCHAR)((uni1 >> 10) + 0xD800);
			utf16 [utf16_idx++] = (TCHAR)((uni1 & 0x3FF) + 0xDC00);
        }
	}

	wcscpy(strU16, utf16);

	return 0;
}


int ConvertStrUTF8ToUtf16 (TCHAR* strU16, char* strU8, int U16_len)
{

	TCHAR t16;

	//System::UnicodeToUtf8(strU8, strU16, strlen(strU16));

	return 0;
}

String UnicodeToUTF8____(const WideString& str)
{
	char*     pElementText;
	int    iTextLen;

	// wide char to multi char
	iTextLen = WideCharToMultiByte( CP_UTF8,
		 0,
		 str.c_bstr(),
		 -1,
		 NULL,
		 0,
		 NULL,
		 NULL );
   pElementText = new char[iTextLen + 1];
   memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
   ::WideCharToMultiByte( CP_UTF8,
		 0,
		 str.c_bstr(),
		 -1,
		 pElementText,
		 iTextLen,
		 NULL,
		 NULL );
	String strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}

/*
String UnicodeToUTF8__(const WideString& str)
{
	char*     pElementText;
	int    iTextLen;

	// wide char to multi char
	iTextLen = WideCharToMultiByte( CP_UTF8,
		 0,
		 str.c_bstr(),
		 -1,
		 NULL,
		 0,
		 NULL,
		 NULL );
   pElementText = new char[iTextLen + 1];
   memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
   ::WideCharToMultiByte( CP_UTF8,
		 0,
		 str.c_bstr(),
		 -1,
		 pElementText,
		 iTextLen,
		 NULL,
		 NULL );
	String strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}
*/

int ConvertStrUnicodeToUtf8(WideString& ws, char* cstr)
{
	//String s("");
	TCHAR tws [1024];
	wcscpy(tws, ws.c_bstr());
	int iwlen = wcslen(tws);
	char cws[1024];
	memset(cws, 0, sizeof(cws));
	int j = 0;


	for(int i = 0; i < iwlen; i++)
	{
		wchar_t wc = tws[i];

		if ( 0 <= wc && wc <= 0x7f )
		{
			cws [j] = (char)wc; j++;
			Sleep(1);
		}
		else if ( 0x80 <= wc && wc <= 0x7ff )
		{
			cws [j] = ( 0xc0 | (wc >> 6)); j++;
			cws [j] = ( 0x80 | (wc & 0x3f)); j++;
		}
		else if ( 0x800 <= wc && wc <= 0xffff )
		{
			cws [j] = ( 0xe0 | (wc >> 12)); j++;
			cws [j] = ( 0x80 | ((wc >> 6) & 0x3f)); j++;
			cws [j] = ( 0x80 | (wc & 0x3f)); j++;
		}
		else if ( 0x10000 <= wc && wc <= 0x1fffff )
		{
			cws [j] = ( 0xf0 | (wc >> 18)); j++;
			cws [j] = ( 0x80 | ((wc >> 12) & 0x3f)); j++;;
			cws [j] = ( 0x80 | ((wc >> 6) & 0x3f)); j++;
			cws [j] = ( 0x80 | (wc & 0x3f)); j++;
		}
		else if ( 0x200000 <= wc && wc <= 0x3ffffff )
		{
			cws [j] = ( 0xf8 | (wc >> 24));  j++;
			cws [j] = ( 0x80 | ((wc >> 18) & 0x3f)); j++;
			cws [j] = ( 0x80 | ((wc >> 12) & 0x3f)); j++;
			cws [j] = ( 0x80 | ((wc >> 6) & 0x3f)); j++;
			cws [j] = ( 0x80 | (wc & 0x3f)); j++;
		}
		else if ( 0x4000000 <= wc && wc <= 0x7fffffff )
		{
			cws [j] = ( 0xfc | (wc >> 30)); j++;
			cws [j] = ( 0x80 | ((wc >> 24) & 0x3f)); j++;
			cws [j] = ( 0x80 | ((wc >> 18) & 0x3f)); j++;
			cws [j] = ( 0x80 | ((wc >> 12) & 0x3f)); j++;
			cws [j] = ( 0x80 | ((wc >> 6) & 0x3f)); j++;
			cws [j] = ( 0x80 | (wc & 0x3f)); j++;
		}

		Sleep(1);
	}

	//s = "";
	//s += cws;

	strcpy(cstr, (const char*) cws);

	return 0;
}





/*
	Examples
*/

/*

std::wstring utf8_to_utf16(const std::string& utf8)
{
	std::vector<unsigned long> unicode;
	size_t i = 0;
	while (i < utf8.size())
	{
		unsigned long uni;
		size_t todo;
		bool error = false;
		unsigned char ch = utf8[i++];
		if (ch <= 0x7F)
		{
			uni = ch;
			todo = 0;
		}
		else if (ch <= 0xBF)
		{
			throw std::logic_error("not a UTF-8 string");
		}
		else if (ch <= 0xDF)
		{
			uni = ch&0x1F;
			todo = 1;
		}
		else if (ch <= 0xEF)
		{
			uni = ch&0x0F;
			todo = 2;
		}
		else if (ch <= 0xF7)
		{
			uni = ch&0x07;
			todo = 3;
		}
		else
		{
			throw std::logic_error("not a UTF-8 string");
		}
		for (size_t j = 0; j < todo; ++j)
		{
			if (i == utf8.size())
				throw std::logic_error("not a UTF-8 string");
			unsigned char ch = utf8[i++];
			if (ch < 0x80 || ch > 0xBF)
				throw std::logic_error("not a UTF-8 string");
			uni <<= 6;
			uni += ch & 0x3F;
		}
		if (uni >= 0xD800 && uni <= 0xDFFF)
			throw std::logic_error("not a UTF-8 string");
		if (uni > 0x10FFFF)
			throw std::logic_error("not a UTF-8 string");
		unicode.push_back(uni);
	}
	std::wstring utf16;
	for (size_t i = 0; i < unicode.size(); ++i)
	{
		unsigned long uni = unicode[i];
		if (uni <= 0xFFFF)
        {
            utf16 += (wchar_t)uni;
        }
        else
        {
            uni -= 0x10000;
            utf16 += (wchar_t)((uni >> 10) + 0xD800);
            utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00);
        }
    }
    return utf16;
}



*/

/*

wstring UTF8toUnicode(const string& s)
{
    wstring ws;
    wchar_t wc;
    for( int i = 0;i < s.length(); )
    {
        char c = s[i];
        if ( (c & 0x80) == 0 )
        {
            wc = c;
            ++i;
        }
        else if ( (c & 0xE0) == 0xC0 )
        {
            wc = (s[i] & 0x1F) << 6;
            wc |= (s[i+1] & 0x3F);
            i += 2;
        }
        else if ( (c & 0xF0) == 0xE0 )
        {
            wc = (s[i] & 0xF) << 12;
            wc |= (s[i+1] & 0x3F) << 6;
            wc |= (s[i+2] & 0x3F);
            i += 3;
        }
        else if ( (c & 0xF8) == 0xF0 )
        {
            wc = (s[i] & 0x7) << 18;
            wc |= (s[i+1] & 0x3F) << 12;
            wc |= (s[i+2] & 0x3F) << 6;
            wc |= (s[i+3] & 0x3F);
            i += 4;
        }
        else if ( (c & 0xFC) == 0xF8 )
        {
            wc = (s[i] & 0x3) << 24;
            wc |= (s[i] & 0x3F) << 18;
            wc |= (s[i] & 0x3F) << 12;
            wc |= (s[i] & 0x3F) << 6;
            wc |= (s[i] & 0x3F);
            i += 5;
        }
        else if ( (c & 0xFE) == 0xFC )
        {
            wc = (s[i] & 0x1) << 30;
            wc |= (s[i] & 0x3F) << 24;
            wc |= (s[i] & 0x3F) << 18;
            wc |= (s[i] & 0x3F) << 12;
            wc |= (s[i] & 0x3F) << 6;
            wc |= (s[i] & 0x3F);
            i += 6;
        }
        ws += wc;
    }
    return ws;
}

string UnicodeToUTF8( const wstring& ws )
{
	string s;
	for( int i = 0;i < ws.size(); ++i )
	{
		wchar_t wc = ws[i];
		if ( 0 <= wc && wc <= 0x7f )
		{
			s += (char)wc;
		}
		else if ( 0x80 <= wc && wc <= 0x7ff )
		{
			s += ( 0xc0 | (wc >> 6));
			s += ( 0x80 | (wc & 0x3f));
		}
		else if ( 0x800 <= wc && wc <= 0xffff )
		{
			s += ( 0xe0 | (wc >> 12));
			s += ( 0x80 | ((wc >> 6) & 0x3f));
			s += ( 0x80 | (wc & 0x3f));
		}
		else if ( 0x10000 <= wc && wc <= 0x1fffff )
		{
			s += ( 0xf0 | (wc >> 18));
			s += ( 0x80 | ((wc >> 12) & 0x3f));
			s += ( 0x80 | ((wc >> 6) & 0x3f));
			s += ( 0x80 | (wc & 0x3f));
		}
		else if ( 0x200000 <= wc && wc <= 0x3ffffff )
		{
			s += ( 0xf8 | (wc >> 24));
			s += ( 0x80 | ((wc >> 18) & 0x3f));
			s += ( 0x80 | ((wc >> 12) & 0x3f));
			s += ( 0x80 | ((wc >> 6) & 0x3f));
			s += ( 0x80 | (wc & 0x3f));
		}
		else if ( 0x4000000 <= wc && wc <= 0x7fffffff )
		{
			s += ( 0xfc | (wc >> 30));
			s += ( 0x80 | ((wc >> 24) & 0x3f));
			s += ( 0x80 | ((wc >> 18) & 0x3f));
			s += ( 0x80 | ((wc >> 12) & 0x3f));
			s += ( 0x80 | ((wc >> 6) & 0x3f));
			s += ( 0x80 | (wc & 0x3f));
		}
	}
	return s;
}

*/

int CutSpaces(TCHAR* tstr, int len)
{

	TCHAR tbuf [1024];
	bool start = false;
	memset(tbuf, 0, 1024);

	for (int i = wcslen(tstr); i > 0; i--)
	{
		//if (tstr[i] == ' ')
		if (tstr[i] == 0x0020)
		{
			tstr[i] = 0;
		}
		else if (tstr[i] != 0)
		{
			break;
		}
	}


	int j = 0;

	for (int i = 0; i < 1024; i++)
	{
		if ((start == true)&&(tstr[i] == 0))
		{
			break;
		}

		if (((tstr[i] != ' ')&&(tstr[i] != 0))||(start == true))
		{
		   tbuf[j] = tstr[i]; j++;
		   start = true;
		}
	}

    wcscpy(tstr, tbuf);
	
	return 0;
}

int CutSpacesEdit(TEdit* edit)
{
	TCHAR tbuf [1024];
	wcscpy(tbuf, edit->Text.c_str());
	CutSpaces(tbuf, wcslen(tbuf));
	//wcscpy(edit->Text.c_str(), tbuf);

	WideString ws(L"");

	ws += tbuf;
	edit->Text = ws;

	//CutSpaces(edit->Text.c_str(), wcslen(edit->Text.c_str()));

	return 0;
}

int ArrangeToDigitFloatEdit(TEdit* edit)
{
	TCHAR tbuf [1024];
	TCHAR tbuf_done [1024];

	wcscpy(tbuf, edit->Text.c_str());

	for (int i = 0; i < 1024; i++)
	{
		tbuf_done [i] = 0;
	}

	int j = 0;

	for (int i = 0; i < wcslen(tbuf); i++)
	{
		if (is_double_digit_smb(tbuf[i]))
		{
			tbuf_done[j] = tbuf[i]; j++;
		}
	}

	WideString ws(L"");
	ws += tbuf_done;
	edit->Text = ws;
	edit->SelStart = j;

	return 0;
}

bool is_double_digit_smb(TCHAR smb)
{
	if (
			(smb == '.')||
			(smb == '1')||
			(smb == '2')||
			(smb == '3')||
			(smb == '4')||
			(smb == '5')||
			(smb == '6')||
			(smb == '7')||
			(smb == '8')||
			(smb == '9')||
			(smb == '0')
	   )
	{
		return true;
	}
	else
	{
		return false;
    }
}

WideString IntToWstr (int i)
{
	WideString s("");

	s.printf(L"%03d", i);

	return s;
}

WideString DblToWstr (double d)
{
	WideString s("");

	s.printf(L"%.3f", d);

	return s;

}

WideString DblToEdit_1 (double* d, TEdit* e)
{
	WideString s("");
	s.printf(L"%.1f", *d);
	e->Text = s;

	return s;
}


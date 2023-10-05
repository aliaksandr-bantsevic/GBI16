//---------------------------------------------------------------------------

#ifndef DataFileH
#define DataFileH
//---------------------------------------------------------------------------
#endif

#include "defs.h"

typedef struct {

	WideString 	place;
	WideString 	drill;
	TDateTime   tcreated;
	int 		dir;
	double      level;
	double      X;
    double      Y;

} data_file_record;


class TDataFile {

public:

	TDataFile();
	~TDataFile();

public:

	FILE* file;
	char cbuf[1024];
	int cbuf_idx;
	char str[1024];
	int str_idx;
	TCHAR tbuf[1024];
    int tbuf_idx;

public:

	int OpenFile(TCHAR* tdir);
	int CheckFile(void);
	void Close(void);
    int GetStr();
};

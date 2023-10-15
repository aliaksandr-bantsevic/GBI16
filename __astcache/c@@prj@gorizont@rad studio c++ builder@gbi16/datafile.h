//---------------------------------------------------------------------------

#ifndef DataFileH
#define DataFileH
//---------------------------------------------------------------------------
#endif

#include "defs.h"

#define DATA_FILE_RECORD_CNT_MAX 	1000
#define DATA_FILE_MEAS_CNT_MAX 		100

typedef struct {

    int         idx;
	WideString 	place;
	WideString 	drill;
	TDateTime   time;
	WideString 	dir;
	double      level;
	double      X;
	double      Y;

} data_file_record_type;


typedef struct {

	WideString 						place;
	WideString 						drill;
	TDateTime   					time;
	int         					record_cnt;
	data_file_record_type record 	[DATA_FILE_RECORD_CNT_MAX];

} data_file_meas_type;


/*
Объект; Место; Дата; Направление; Позиция; X; Y;
*/

#define 	w_undef 	0
#define 	w_place     1
#define 	w_drill     2
#define 	w_date      3
#define 	w_direction 4
#define 	w_position  5
#define 	w_X         6
#define 	w_Y         7


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
	int pars_pos;
	TCHAR tword[1024];
	int tword_idx;
	int tbuf_pars_idx;
    int pars_str_res;
	data_file_record_type data_file_record;
	data_file_meas_type* data_file_meas_set[DATA_FILE_MEAS_CNT_MAX];
	int data_file_meas_set_idx;
	int data_file_meas_set_idx_cur;
	data_file_meas_type* data_file_meas_ptr_cur;

public:

	int OpenFile(TCHAR* tdir);
	int CheckFile(void);
	void Close(void);
	int GetStr();
	int GetWord(void);
	int GetSpar(void);
	int GetTpar(void);
	int GetDpar(void);
	int AcceptDaTaFileRecord();
	int GetDaTaFileMeasIdx(void);
    int AcceptDaTaFileMeasRecord(void);
	int ParsDaTaFile(TCHAR* tdir);

	double d_par;
	TDateTime t_par;
	WideString s_par;

    bool pars_en;

};

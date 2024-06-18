//---------------------------------------------------------------------------

#ifndef ProfileH
#define ProfileH
//---------------------------------------------------------------------------

#include <vcl.h>
#include "defs.h"

#define MAX_PROFILE_LEN 100

typedef struct {

	int cnt;
	double dep [MAX_PROFILE_LEN];

} profile_type;


class TProfile
{
	public:

	TProfile();
	TProfile(int cnt);
	~TProfile();

	profile_type profile;


	public:

	void copy(profile_type* p);
	void clear();
	void to_table(TListView* list);
	int add(double d);
	int exist(double d, int except);
	int update(int idx, double d);
	int remove(int idx);
	int sort(void);
	int copy(TProfile* p);

};


#endif

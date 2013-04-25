#ifndef __ZPROFILER__
#define __ZPROFILER__

#include <time.h>
#include <string>
#include <vector>

using std::string;
using std::pair;
using std::vector;

#define _Z_OK_ 0
#define _Z_PROFILE_NOT_EXIST_ 1
#define _Z_PROFILE_ALREADY_EXIST_ 2
#define _Z_PROFILE_ALREADY_COMPLETED_ 3
#define _Z_PROFILE_ALREADY_CONTINUED_ 4
#define _Z_PROFILE_ALREADY_STOPED_ 5


const char PROFILE_AUTOGEN_KEY[] = "AUTOGEN_";

class ZProfiler
{
	struct ZTimePoint{
		string Name;
		struct timespec Start, End;
		struct timespec StartWait, EndWait;
		double WaitTime;
		bool Lock, WaitLock;

		ZTimePoint();
	};

	vector< ZTimePoint > Profile;
	int PointCounter;

	string IntToString(int Num);
	vector< ZTimePoint >::iterator Find(string& Name);
	vector< ZTimePoint >::iterator GetLast();


public:
	ZProfiler();

	int Add(string Name);
	int Add();

	int Remove(string Name);

	int End(string Name);
	int End();
	int EndLast();

	int Stop(string Name);
	int StopLast();

	int Continue(string Name);
	int ContinueLast();

	int Clear();

	double GetFullTime(int *Error = NULL);
	double GetPointTime(string Name, int *Error = NULL);
	double GetPointTimeLast(int *Error = NULL);

	void PrintInfo();
	~ZProfiler();
};

#endif // __ZPROFILER__

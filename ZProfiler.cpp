#include "ZProfiler.hpp"

#include <time.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <math.h>

using std::cout;
using std::endl;
using std::ios;
using std::setprecision;
using std::string;


ZProfiler::ZTimePoint::ZTimePoint()
{
	Lock = false;
	WaitLock = false;
	WaitTime = 0;
}


string ZProfiler::IntToString(int Num)
{
	int dev = 1, tmp;
	string Result = "";

	for(int i = 1; ;i++)
	{
		dev*=10;
		if(Num%dev==Num)
		{
			for(int j = 0; j<i; j++)
			{
				dev/=10;
				tmp = Num%dev;
				Num/=dev;
				Result+=Num+48;
				Num = tmp;
			}
			return Result;
		}
	}

	return Result;
}

vector< ZProfiler::ZTimePoint >::iterator ZProfiler::Find(string& Name)
{
	size_t tmp;

	tmp = Profile.size();
	for(size_t i = 0; i<tmp; i++)
	{
		if(Profile[i].Name == Name)
		{
			return Profile.begin()+i;
		}
	}
	return Profile.end();
}

vector< ZProfiler::ZTimePoint >::iterator ZProfiler::GetLast()
{
	return --Profile.end();
}


ZProfiler::ZProfiler()
{
	PointCounter = -1;
	Profile.reserve(8);
}

int ZProfiler::Add(string Name)
{
	vector< ZTimePoint >::iterator it;
	ZTimePoint add;

	it = Find(Name);
	if(it==Profile.end())
	{
		Profile.push_back(add);
		it = GetLast();
		it->Name = Name;
		clock_gettime(CLOCK_REALTIME, &it->End);
		clock_gettime(CLOCK_REALTIME, &it->Start);
		return _Z_OK_;
	}
	return _Z_PROFILE_ALREADY_EXIST_;
}

int ZProfiler::Add()
{
	string Key;

	while(true)
	{
		Key = PROFILE_AUTOGEN_KEY+IntToString(++PointCounter);
		if(Find(Key)==Profile.end())
		{
			break;
		}
	}

	return Add(Key);
}

int ZProfiler::Remove(string Name)
{
	vector< ZTimePoint >::iterator it;
	ZTimePoint add;

	it = Find(Name);
	if(it!=Profile.end())
	{
		Profile.erase(it);
		return _Z_OK_;
	}
	return _Z_PROFILE_NOT_EXIST_;
}

int ZProfiler::End(string Name)
{
	vector< ZTimePoint >::iterator it;
	struct timespec tmp;
	clock_gettime(CLOCK_REALTIME, &tmp);

	it = Find(Name);
	if(it!=Profile.end())
	{
		if(!it->Lock)
		{
		    if(it->WaitLock)
            {
                Continue(it->Name);
            }
			clock_gettime(CLOCK_REALTIME, &it->End);
			it->End = tmp;
			it->Lock = true;
			return _Z_OK_;
		}
		else
		{
			return _Z_PROFILE_ALREADY_COMPLETED_;
		}
	}
	else
	{
		return _Z_PROFILE_NOT_EXIST_;
	}
}

int ZProfiler::End()
{
	vector< ZTimePoint >::iterator it;

	for(it = Profile.begin(); it!=Profile.end(); it++)
	{
		End(it->Name);
	}
	return _Z_OK_;
}

int ZProfiler::EndLast()
{
	return End(GetLast()->Name);
}

int ZProfiler::Stop(string Name)
{
	vector< ZTimePoint >::iterator it;
	struct timespec tmp;
	clock_gettime(CLOCK_REALTIME, &tmp);

	it = Find(Name);
	if(it!=Profile.end())
	{
		if(!it->WaitLock && !it->Lock)
		{
			it->WaitLock = true;
			it->StartWait = tmp;
			return _Z_OK_;
		}
		else
		{
			return (it->Lock ? _Z_PROFILE_ALREADY_COMPLETED_ : _Z_PROFILE_ALREADY_STOPED_);
		}
	}
	else
	{
		return _Z_PROFILE_NOT_EXIST_;
	}
}

int ZProfiler::StopLast()
{
	return Stop(GetLast()->Name);
}

int ZProfiler::Continue(string Name)
{
	vector< ZTimePoint >::iterator it;

	it = Find(Name);
	if(it!=Profile.end())
	{
		if(it->WaitLock && !it->Lock)
		{
			clock_gettime(CLOCK_REALTIME, &it->EndWait);
			it->WaitLock = false;
			it->WaitTime+=(double)((it->EndWait.tv_sec - it->StartWait.tv_sec) + 1.e-9*(it->EndWait.tv_nsec - it->StartWait.tv_nsec));
			return _Z_OK_;
		}
		else
		{
			return (it->Lock ? _Z_PROFILE_ALREADY_COMPLETED_ : _Z_PROFILE_ALREADY_CONTINUED_);
		}
	}
	else
	{
		return _Z_PROFILE_NOT_EXIST_;
	}
}

int ZProfiler::ContinueLast()
{
	return Continue(GetLast()->Name);
}

int ZProfiler::Clear()
{
	PointCounter = -1;
	Profile.clear();

	return _Z_OK_;
}

double ZProfiler::GetFullTime(int *Error)
{
	vector< ZTimePoint >::iterator it;

	double Summ = 0;

	for(it = Profile.begin(); it!=Profile.end(); it++)
	{
		Summ+= (double)((it->End.tv_sec - it->Start.tv_sec) + 1.e-9*(it->End.tv_nsec - it->Start.tv_nsec)) - it->WaitTime;
	}

	return Summ;
}

double ZProfiler::GetPointTime(string Name, int *Error)
{
	vector< ZTimePoint >::iterator it;

	it = Find(Name);
	if(it!=Profile.end())
	{
		if(Error!=0)
		{
			*Error = _Z_OK_;
		}
		return (double)((it->End.tv_sec - it->Start.tv_sec) + 1.e-9*(it->End.tv_nsec - it->Start.tv_nsec)) - it->WaitTime;
	}
	else
	{
		if(Error!=0)
		{
			*Error = _Z_PROFILE_NOT_EXIST_;
		}
		return 0;
	}
}

double ZProfiler::GetPointTimeLast(int *Error)
{
	return GetPointTime(GetLast()->Name, Error);
}

void ZProfiler::PrintInfo()
{
	vector< ZTimePoint >::iterator it;

	double Summ = 0, Time = 0;

	for(it = Profile.begin(); it!=Profile.end(); it++)
	{
		Summ += (double)((it->End.tv_sec - it->Start.tv_sec) + 1.e-9*(it->End.tv_nsec - it->Start.tv_nsec)) - it->WaitTime;
	}

	cout.setf(ios::fixed);
	cout << ">>>>FULL TIME: " << setprecision(9) << Summ << endl;

	for(it = Profile.begin(); it!=Profile.end(); it++)
	{
		Time = (double)((it->End.tv_sec - it->Start.tv_sec) + 1.e-9*(it->End.tv_nsec - it->Start.tv_nsec)) - it->WaitTime;

		cout << it->Name << " : " << setprecision(9) << Time << "\t" << setprecision(3) << Time/Summ*100 << " %" << endl;
	}
}

ZProfiler::~ZProfiler()
{
	Profile.clear();
}

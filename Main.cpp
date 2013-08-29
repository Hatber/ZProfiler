#include "ZProfiler.hpp"

#include<iostream>
#include <unistd.h>

int main()
{
	ZProfiler PF;
	int Error = -1;

	PF.Add("First");

	std::cout << PF.Stop("First");
	std::cout << PF.Stop("First");

	//Some cod

	PF.Add("AUTOGEN_5");
	PF.EndLast(); //End Second

	for(int i = 0; i<10; i++)
	{
		PF.Add();
		PF.EndLast();
	}
	std::cout << PF.Continue("First");
	std::cout << PF.Continue("First");
	PF.End("First");
	std::cout << PF.Continue("First");

	PF.PrintInfo();

	std::cout << PF.GetFullTime(&Error) << "\t" << PF.GetPointTime("First", &Error) << "  ";
	std::cout << Error << std::endl;

	std::cout << "=================" << std::endl;

	PF.Clear();
	PF.PrintInfo();

	//==================
	ZProfiler Profiler;

	Profiler.Add("Allocate");
    sleep(1);
    Profiler.End("Allocate");

    Profiler.Add("Init"); Profiler.Stop("Init");
    Profiler.Add("Cut");  Profiler.Stop("Cut");
    for(size_t i = 0; i<1; i++) {
        Profiler.Continue("Init");
        sleep(1);
        Profiler.Stop("Init");

        Profiler.Continue("Cut");
        sleep(1);
        Profiler.Stop("Cut");
    }
    Profiler.End("Init");

    Profiler.Add("Calc Border Size");
    sleep(1);
    Profiler.Stop("Calc Border Size");

    Profiler.Add("MemoryTest");
    for(size_t j = 0; j<4; j++)
    {
        sleep(1);
        Profiler.Stop("MemoryTest");

        for(size_t i = 0; i<1; i++) {
            Profiler.Continue("Cut");
            sleep(1);
            Profiler.Stop("Cut");
        }

        Profiler.Continue("Calc Border Size");
        sleep(1);
        Profiler.Stop("Calc Border Size");

        Profiler.Continue("MemoryTest");
    }

    Profiler.End();
	Profiler.PrintInfo();

	return 0;
	}

#include "ZProfiler.hpp"

#include<iostream>

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
	std::cout << Error;

	std::cout << "=================" << std::endl;

	PF.Clear();
	PF.PrintInfo();

	return 0;
}

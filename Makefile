all: ZProfilerTest clean

ZProfilerTest: Main.o ZProfiler.o
	g++ *.o -o ZProfilerTest -lrt

Main.o: Main.cpp
	g++ -c Main.cpp -o Main.o

ZProfile.o: ZProfiler.hpp ZProfiler.cpp
	g++ -c ZProfiler.cpp -o ZProfiler.o

clean:
	rm -f *.o

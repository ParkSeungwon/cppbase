#include"stopwatch.h"
using namespace std;

void StopWatch::start()
{
	start_ = chrono::system_clock::now();
}

unsigned long StopWatch::stop()
{
	end_ = chrono::system_clock::now();
	return (end_ - start_).count();
}


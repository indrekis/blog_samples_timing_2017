// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include <ctime>
#include <cmath>
#include <limits>
#include <thread>
#include <chrono>

#ifdef  _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#if defined __linux__ || defined __CYGWIN__
#include <unistd.h>
#include <time.h>
#include <string.h>
#endif 
using std::cout;
using std::cerr;
using std::endl;

template<class D>
inline long long to_us(const D& d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

int main() {
	cout << "CLOCKS_PER_SEC value: " << CLOCKS_PER_SEC << endl;
	cout << "clock_t size: " 		 << sizeof(clock_t) << endl; //-V128
	double oveflow_seconds = std::numeric_limits<clock_t>::max() / CLOCKS_PER_SEC;
	cout << "Overflows in: " << oveflow_seconds << " seconds, or " << oveflow_seconds/60 << " minutes or " 
		 << oveflow_seconds / (60*60*24) << " days" << endl;
	
	cout << endl;
	cout << "high_resolution_clock tick duration: " << std::chrono::high_resolution_clock::period::num
		 << '/' << std::chrono::high_resolution_clock::period::den
		 << " seconds" << endl;

#ifdef  _WIN32
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency); 
	cout << "Win32 QueryPerformanceCounter frequency: " << Frequency.QuadPart << " Hz" << endl;
	cout << "\tTick length: " << 1000000.0/Frequency.QuadPart << " us" << endl;
#endif

#if  _POSIX_C_SOURCE >= 199309L
	auto print_clock_res = [](clockid_t id)
	{
		timespec resolution;
		int err = clock_getres(id, &resolution);
		if(err==-1)
		{
			auto errno_copy = errno;
			cerr << "Error: " << errno_copy << ", " << strerror(errno_copy) << endl;		
		}else{
			double res = resolution.tv_sec * pow(10, 9) + resolution.tv_nsec;
			cout << " resolution: " << res << " ns or " << resolution.tv_sec << " seconds, " 
				<< resolution.tv_nsec << " nanoseconds" << endl;
		}
	};
	#ifdef CLOCK_MONOTONIC_RAW
	cout << "CLOCK_MONOTONIC_RAW";
	print_clock_res(CLOCK_MONOTONIC_RAW);
	#endif 
	#ifdef CLOCK_MONOTONIC
	cout << "CLOCK_MONOTONIC";
	print_clock_res(CLOCK_MONOTONIC);	
	#endif 
#endif 
		 
	return 0;
}

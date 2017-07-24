// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include <ctime>
#include <cmath>
#include <limits>
#include <thread>
#include <chrono>
#include <iomanip>
#include <vector>
#include <algorithm>

//! Нижче часто ігнорую коди помилок -- саму можливість помилки, по суті.
//! Взагалі, це погано, але лінь же ж...

using std::cout;
using std::endl;
using std::setw;
using std::vector;

// std::atomic_thread_fence(std::memory_order_seq_cst);

#if !defined USE_CHRONO && !defined USE_CLOCK && !defined USE_MSWIN_PERFORMANCE_COUNTERS && !defined USE_LINUX_WALL_TIMERS 
#error "Define exactly one macro of USE_CHRONO, USE_CLOCK, USE_MSWIN_PERFORMANCE_COUNTERS, USE_LINUX_WALL_TIMERS"
//#define USE_CHRONO
//#define USE_CLOCK
//#define USE_MSWIN_PERFORMANCE_COUNTERS
//#define USE_LINUX_WALL_TIMERS
#endif

#ifdef USE_CHRONO

template<class D>
inline long long to_us(const D& d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

long long measured_delay(long long delay_tm)
{
	decltype(std::chrono::high_resolution_clock::now()) tick1, tick2;
	if(delay_tm == -1) // No delay at all
	{
		tick1 = std::chrono::high_resolution_clock::now();
		tick2 = std::chrono::high_resolution_clock::now(); //-V656
	}else{
		tick1 = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(std::chrono::microseconds( delay_tm ));
		tick2 = std::chrono::high_resolution_clock::now();		
	}
	return to_us(tick2 - tick1);
}
#elif defined USE_CLOCK
template<class D>
inline long long to_us(const D& d) {
    return static_cast<double>(d) * 1000000 / CLOCKS_PER_SEC;
}
long long measured_delay(long long delay_tm)
{
	clock_t tick1, tick2;
	if(delay_tm == -1) // No delay at all
	{
		tick1 = std::clock();
        //std::atomic_thread_fence(std::memory_order_seq_cst);
		tick2 = std::clock(); //-V656
	}else{
		tick1 = std::clock();
        //std::atomic_thread_fence(std::memory_order_seq_cst);
		std::this_thread::sleep_for(std::chrono::microseconds( delay_tm ));
        //std::atomic_thread_fence(std::memory_order_seq_cst);
		tick2 = std::clock();		
	}
	return to_us(tick2 - tick1);
}
#elif defined USE_MSWIN_PERFORMANCE_COUNTERS
#include <windows.h>
template<class D>
inline long long to_us(const D& d) {
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency); 
    return d * 1000000 / Frequency.QuadPart;
}
long long measured_delay(long long delay_tm)
{
	LARGE_INTEGER tick1, tick2;
	if(delay_tm == -1) // No delay at all
	{
		QueryPerformanceCounter(&tick1);
		QueryPerformanceCounter(&tick2);
	}else{
		QueryPerformanceCounter(&tick1);
		std::this_thread::sleep_for(std::chrono::microseconds( delay_tm ));
		QueryPerformanceCounter(&tick2);
	}
	return to_us(tick2.QuadPart - tick1.QuadPart);
}

#elif defined USE_LINUX_WALL_TIMERS
#include <unistd.h>
#include <time.h>

long long measured_delay(long long delay_tm)
{
	timespec tick1, tick2;
	if(delay_tm == -1) // No delay at all
	{
		clock_gettime(CLOCK_MONOTONIC, &tick1);
		clock_gettime(CLOCK_MONOTONIC, &tick2);
	}else{
		clock_gettime(CLOCK_MONOTONIC, &tick1);
		std::this_thread::sleep_for(std::chrono::microseconds( delay_tm ));
		clock_gettime(CLOCK_MONOTONIC, &tick2);
	}

	double interval = (tick2.tv_sec - tick1.tv_sec) * pow(10, 9) + (tick2.tv_nsec - tick1.tv_nsec);
	interval /= 1000;
	
	return interval;
}
#else
#error "Wrong timing decive"	
#endif

int main() {	
	const int w = 10;
	const int max_exp = 7;
	const int repeats = 20;

	vector<long long> delays;
	
	delays.push_back(-1); // No delay
	delays.push_back(0); // Zero delay;
	unsigned long delay = 1; // Other delays
	for(int i = 0; i<max_exp; ++i)
	{
		delays.push_back(delay);
		delay *= 10;
	}		

	cout << "-1 means no delay call at all" << endl;
	cout << setw(w) << "Delay:";
	for(auto x: delays)
	{
		cout << setw(w) << x;
	}
	cout << endl;
	

	vector<double> average_results;
	vector<double> dispersion_results;
	vector<int> dispersion_results_percent;
	vector< vector<double> > results_table(delays.size());
	
	for(int i = 0; i<repeats; ++i)
	{		
		cout << setw(w) << i;
		for(auto i=delays.begin(); i!=delays.end(); ++i)
		{
			auto delay = measured_delay( *i );
            results_table.at( std::distance(delays.begin(),i) )
                            .push_back(static_cast<double>(delay));
			cout << setw(w) << delay;
		}
		cout << endl;
		
	}
	cout << endl;
	
	for(auto &v: results_table)
	{
		average_results.push_back(0);
		for(auto x: v)
		{
			average_results.back() += x;
		}
		average_results.back() /= v.size();
		
		dispersion_results.push_back(0);
		for(auto x: v)
		{
			dispersion_results.back() += std::pow( average_results.back() - x , 2);
		}
		dispersion_results.back() /= v.size();
		dispersion_results.back() = std::sqrt(dispersion_results.back());		
		if(average_results.back()>1E-10)
            dispersion_results_percent.push_back(
                        static_cast<int>(dispersion_results.back()/average_results.back()*100) );
		else
			dispersion_results_percent.push_back(-1);
	}
	
	cout << endl;
	cout << std::fixed << std::setprecision(1);
	cout << setw(w) << "Average:";
	for(auto x: average_results)
	{
		cout << setw(w) << x;
	}
	cout << endl;
	cout << setw(w) << "Min:";
	for(auto &v: results_table)
	{
		cout << setw(w) << *std::min_element(v.begin(), v.end());	
	}
	cout << endl;
	cout << setw(w) << "Max:";
	for(auto &v: results_table)
	{
		cout << setw(w) << *std::max_element(v.begin(), v.end());	
	}
	cout << endl;
	cout << setw(w) << "Std_var:";
	for(auto &x: dispersion_results)
	{
		cout << setw(w) << x;	
	}
	cout << endl;
	cout << setw(w) << "Std_var%:";
	for(auto &x: dispersion_results_percent)
	{
		cout << setw(w) << x;	
	}
	cout << endl;

	return 0;
}

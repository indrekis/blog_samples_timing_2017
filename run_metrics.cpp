// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <limits>
#include <thread>
#include <chrono>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>

struct run_metrics_t
{
	constexpr static uint64_t not_found_val = static_cast<uint64_t>(-1);
	uint64_t proc_time_total = not_found_val;
	uint64_t proc_time_sys = not_found_val;
	uint64_t proc_time_user = not_found_val;

	uint64_t thread_time_total = not_found_val;
	uint64_t thread_time_sys = not_found_val;
	uint64_t thread_time_user = not_found_val;
	
	uint64_t process_soft_page_faults = not_found_val;
	uint64_t process_hard_page_faults = not_found_val;
	uint64_t process_total_page_faults = not_found_val;
	
	uint64_t process_voluntary_context_switches = not_found_val;
	uint64_t process_involuntary_context_switches = not_found_val; 
	uint64_t process_total_context_switches = not_found_val; 
	
	uint64_t process_max_resident_size = not_found_val; 	
	
	uint64_t thread_soft_page_faults = not_found_val;
	uint64_t thread_hard_page_faults = not_found_val;
	uint64_t thread_total_page_faults = not_found_val;
	
	uint64_t thread_voluntary_context_switches = not_found_val;
	uint64_t thread_involuntary_context_switches = not_found_val; 
	uint64_t thread_total_context_switches = not_found_val; 
	
	uint64_t thread_max_resident_size = not_found_val; 
	
	void print_item_or_none(std::ostream& os, const std::string& msg, uint64_t val) const
	{
		const int w = 35;
		os << std::left;
		if(val == static_cast<uint64_t>(-1))
			os << std::setw(w) << msg << "not_found" << '\n';		
		else
			os << std::setw(w) << msg << val << '\n';		
	}
	void print(std::ostream& os) const
	{
		print_item_or_none(os, "Processor time total: ",  proc_time_total);
		print_item_or_none(os, "Processor time sys: ",  proc_time_sys);
		print_item_or_none(os, "Processor time user: ",  proc_time_user);

		print_item_or_none(os, "Thread time total: ",  thread_time_total);
		print_item_or_none(os, "Thread time sys: ",  thread_time_sys);
		print_item_or_none(os, "Thread time user: ",  thread_time_user);

		print_item_or_none(os, "Total process page faults: ",  process_total_page_faults);
		print_item_or_none(os, "Soft process page faults: ",  process_soft_page_faults);
		print_item_or_none(os, "Hard process page faults: ",  process_hard_page_faults);
	
		print_item_or_none(os, "Total process context switches: ",  process_total_context_switches);
		print_item_or_none(os, "Voluntary process context switches: ",  process_voluntary_context_switches);
		print_item_or_none(os, "Involuntary process context switches: ",  process_involuntary_context_switches);
		
		print_item_or_none(os, "Maximal process resident size: ",  process_max_resident_size);
		
		print_item_or_none(os, "Total thread page faults: ",  thread_total_page_faults);
		print_item_or_none(os, "Soft thread page faults: ",  thread_soft_page_faults);
		print_item_or_none(os, "Hard thread page faults: ",  thread_hard_page_faults);

		print_item_or_none(os, "Total thread context switches: ",  thread_total_context_switches);
		print_item_or_none(os, "Voluntary thread context switches: ",  thread_voluntary_context_switches);
		print_item_or_none(os, "Involuntary thread context switches: ",  thread_involuntary_context_switches);
				
		print_item_or_none(os, "Maximal thread resident size: ",  thread_max_resident_size);
		os << std::flush;
		
	}

	void read_metrics();

	static uint64_t sub_if_present(const uint64_t& a, const uint64_t& b)
	{
		if( a!= not_found_val && b != not_found_val )
			return a-b;
		else
			return not_found_val;
	}
	run_metrics_t& operator-=(const run_metrics_t& b);
	
	void init();
	void clear()
	{
		proc_time_total = not_found_val;
		proc_time_sys = not_found_val;
		proc_time_user = not_found_val;
	
		thread_time_total = not_found_val;
		thread_time_sys = not_found_val;
		thread_time_user = not_found_val;
		
		process_soft_page_faults = not_found_val;
		process_hard_page_faults = not_found_val;
		process_total_page_faults = not_found_val;
		
		process_voluntary_context_switches = not_found_val;
		process_involuntary_context_switches = not_found_val; 
		process_total_context_switches = not_found_val; 
		
		process_max_resident_size = not_found_val; 	
		
		thread_soft_page_faults = not_found_val;
		thread_hard_page_faults = not_found_val;
		thread_total_page_faults = not_found_val;
		
		thread_voluntary_context_switches = not_found_val;
		thread_involuntary_context_switches = not_found_val; 
		thread_total_context_switches = not_found_val; 
		
		thread_max_resident_size = not_found_val; 				
	}
	~run_metrics_t();
#if defined _POSIX_C_SOURCE
#elif defined _WIN32
//private:
#endif	
};

run_metrics_t operator-(run_metrics_t a, const run_metrics_t& b)
{
	return a-=b;
}


run_metrics_t& run_metrics_t::operator-=(const run_metrics_t& b)
{
	proc_time_total = sub_if_present(proc_time_total , b.proc_time_total);
	proc_time_sys = sub_if_present(proc_time_sys , b.proc_time_sys);
	proc_time_user = sub_if_present(proc_time_user , b.proc_time_user);

	thread_time_total = sub_if_present(thread_time_total , b.thread_time_total);
	thread_time_sys = sub_if_present(thread_time_sys , b.thread_time_sys);
	thread_time_user = sub_if_present(thread_time_user , b.thread_time_user);
	
	process_soft_page_faults = sub_if_present(process_soft_page_faults , b.process_soft_page_faults);
	process_hard_page_faults = sub_if_present(process_hard_page_faults , b.process_hard_page_faults);
	process_total_page_faults = sub_if_present(process_total_page_faults , b.process_total_page_faults);
	
	process_voluntary_context_switches = sub_if_present(process_voluntary_context_switches , b.process_voluntary_context_switches);
	process_involuntary_context_switches = sub_if_present(process_involuntary_context_switches , b.process_involuntary_context_switches); 
	process_total_context_switches = sub_if_present(process_total_context_switches , b.process_total_context_switches); 
	
	process_max_resident_size = sub_if_present(process_max_resident_size , b.process_max_resident_size); 	
	
	thread_soft_page_faults = sub_if_present(thread_soft_page_faults , b.thread_soft_page_faults);
	thread_hard_page_faults = sub_if_present(thread_hard_page_faults , b.thread_hard_page_faults);
	thread_total_page_faults = sub_if_present(thread_total_page_faults , b.thread_total_page_faults);
	
	thread_voluntary_context_switches = sub_if_present(thread_voluntary_context_switches , b.thread_voluntary_context_switches);
	thread_involuntary_context_switches = sub_if_present(thread_involuntary_context_switches , b.thread_involuntary_context_switches); 
	thread_total_context_switches = sub_if_present(thread_total_context_switches , b.thread_total_context_switches); 
	
	thread_max_resident_size = sub_if_present(thread_max_resident_size , b.thread_max_resident_size); 	
	
	return *this;
}

#define DEBUG_OR_INTERESTING_PRING	

#if defined _POSIX_C_SOURCE
//! Тут би пасувало ще перевірки на не-Лінукс і не Cygwin системи зробити
//! але це потім.
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <sys/times.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/unistd.h>

#ifndef RUSAGE_THREAD
#include <pthread.h>
#endif


uint64_t to_us(const timeval& t){
	return t.tv_sec * 1000000 + t.tv_usec;
}

uint64_t to_us(const timespec & t){
	return t.tv_sec * 1000000 + t.tv_nsec / 1000;
}



void run_metrics_t::init()
{
}

run_metrics_t::~run_metrics_t()
{
}

void run_metrics_t::read_metrics()
{
	clear(); // All set to not_found_val -- no need to set later
	int err;
	run_metrics_t m;
	tms proc_times;
	rusage usage_proc; 
	times(&proc_times); // Process times
	getrusage(RUSAGE_SELF, &usage_proc); 
#ifdef 	RUSAGE_THREAD
	rusage usage_thread;
	getrusage(RUSAGE_THREAD, &usage_thread); // Thread times -- needs _GNU_SOURCE 
#else 	
	clockid_t cid;
	timespec thread_times;
	err = pthread_getcpuclockid(pthread_self(), &cid);
	if(err == -1){
		std::cerr << "Error obtaining thread clock id" << std::endl;
	}
	err = clock_gettime(cid, &thread_times);
#endif 
//==============Metrice=read===================================
//=Processing==================================================

//!=Thread times ===================
#ifdef 	RUSAGE_THREAD
	thread_time_user = to_us(usage_thread.ru_utime);
	thread_time_sys  = to_us(usage_thread.ru_stime);
	thread_time_total= thread_time_user + thread_time_sys;
#else
	if(err == -1){
		std::cerr << "Error reading time" << std::endl;
	}
	if(err != -1)
		thread_time_total = to_us(thread_times);
#endif 	
//!=Process times ===================	
	long ticks_per_second = sysconf(_SC_CLK_TCK);
	//! We are not interested in child times -- tested process should not fork!
	
	proc_time_sys = static_cast<long>( 1000000.0 * proc_times.tms_stime / ticks_per_second);
	proc_time_user = static_cast<long>( 1000000.0 * proc_times.tms_utime / ticks_per_second);
	proc_time_total = proc_time_sys + proc_time_user;
#ifdef DEBUG_OR_INTERESTING_PRING	
	std::cerr << "Times proc time: " << proc_time_total << ", rusage: " 
			<< to_us(usage_proc.ru_utime) + to_us(usage_proc.ru_stime) 
		    //  << (usage_proc.ru_utime.tv_sec + usage_proc.ru_stime.tv_sec)* 1000000 
		    //    + usage_proc.ru_utime.tv_usec + usage_proc.ru_stime.tv_usec 
			<< std::endl;
#endif 
//!=Thread switches and memory ===================
#ifdef 	RUSAGE_THREAD
	thread_soft_page_faults = usage_thread.ru_minflt;
	thread_hard_page_faults = usage_thread.ru_majflt;
	thread_total_page_faults = thread_soft_page_faults + thread_hard_page_faults;
	
	thread_voluntary_context_switches = usage_thread.ru_nvcsw;
	thread_involuntary_context_switches = usage_thread.ru_nivcsw; 
	thread_total_context_switches = thread_voluntary_context_switches + thread_involuntary_context_switches; 
	
	thread_max_resident_size = usage_thread.ru_maxrss * 1024; 
#else
	// Not found...
#endif 	
//!=Process swithces and memory ===================
	process_soft_page_faults = usage_proc.ru_minflt;
	process_hard_page_faults = usage_proc.ru_majflt;
	process_total_page_faults = process_soft_page_faults + process_hard_page_faults;

	process_max_resident_size = usage_proc.ru_maxrss; 
	
	process_voluntary_context_switches = usage_proc.ru_nvcsw;
	process_involuntary_context_switches = usage_proc.ru_nivcsw; 
	process_total_context_switches = process_voluntary_context_switches + process_involuntary_context_switches; 
	

}


#elif defined _WIN32
#include <windows.h>
#include <psapi.h>
#include <pdh.h>

uint64_t to_us(const FILETIME & t){ //! Unit -- 100ns
	return ( (static_cast<uint64_t>(t.dwHighDateTime) << 32) + t.dwLowDateTime)/10; //-V112
}

void run_metrics_t::init()
{
}

run_metrics_t::~run_metrics_t()
{
}

void run_metrics_t::read_metrics()
{
	
	FILETIME creation_time, exit_time, kernel_time, user_time;
	FILETIME thread_creation_time, thread_exit_time, thread_kernel_time, thread_user_time;
	    
	//=Ready to=collect====================================
	int err_proc = GetProcessTimes(GetCurrentProcess(), 
								&creation_time, &exit_time,
                                &kernel_time, &user_time);

	int err_thread = GetThreadTimes(GetCurrentThread(), 
								&thread_creation_time, &thread_exit_time,
                                &thread_kernel_time, &thread_user_time);

								
	
	proc_time_user = to_us(user_time);
	proc_time_sys  = to_us(kernel_time);
	proc_time_total= proc_time_user + proc_time_sys;								

	thread_time_user = to_us(thread_user_time);
	thread_time_sys  = to_us(thread_kernel_time);
	thread_time_total= thread_time_user + thread_time_sys;		

	PROCESS_MEMORY_COUNTERS meminfo;
	GetProcessMemoryInfo( GetCurrentProcess( ), &meminfo, sizeof(meminfo) );
	process_max_resident_size = meminfo.PeakWorkingSetSize;		
	process_total_page_faults = meminfo.PageFaultCount;
}
#else
#error "Unknown OS"	
#endif

using namespace std;
int main()
{
    srand(static_cast<unsigned>(time(0)));
	volatile long t;
	run_metrics_t metrics1, metrics2;	
	metrics1.read_metrics();
	
	for(int i=0;i<100000; ++i)
	{
		t = rand();
		std::this_thread::sleep_for(std::chrono::microseconds( 1000 ));
	}
	
	metrics2.read_metrics();
	cout << endl;
	metrics1.print(cout);
	cout << "-------------------------------------------------\n";
	metrics2.print(cout);
	cout << "-------------------------------------------------\n";
	(metrics2-metrics1).print(cout);
	
	return 0;
	
}

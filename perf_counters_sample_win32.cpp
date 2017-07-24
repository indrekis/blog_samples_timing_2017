// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#if defined  _WIN32 && defined __MINGW__
#define _WIN32_WINNT 0x0600
#endif

#include <windows.h>
#include <psapi.h>
#include <pdh.h>

#include <iostream>
#include <chrono>
#include <thread>

//! Пасувало б тут RAII до Query використати. 
//! Але вирішив пожертвувати коректністю заради простоти.
using namespace std;
int main()
{	
    srand(static_cast<unsigned>(time(0)));
	volatile long t;
	
    HQUERY Query = NULL;
    PDH_STATUS Status = PdhOpenQuery(NULL, 0, &Query);
    HCOUNTER Counter;
    CHAR CounterPathBuffer[PDH_MAX_COUNTER_PATH] = "\\Thread(_Total/_Total)\\Context Switches/sec";
    if (Status != ERROR_SUCCESS)
    {
       cout << "PdhOpenQuery failed with status: " << hex <<  Status << dec << endl;
    }
    
	Status = PdhAddCounter(Query, CounterPathBuffer, 0, &Counter);
    if (Status != ERROR_SUCCESS)
    {
        cout << "PdhAddCounter failed with status: " << hex << Status << dec << endl;
    }
	
	Status = PdhCollectQueryData(Query);
    if (Status != ERROR_SUCCESS)
    {
        cout << "\nPdhCollectQueryData failed with: "  << hex <<  Status << dec << endl;
    }

	for(int i=0;i<100000; ++i)
	{
		t = rand();
		std::this_thread::sleep_for(std::chrono::microseconds( 1000 ));
	}
	
    Status = PdhCollectQueryData(Query);
    if (Status != ERROR_SUCCESS)
    {
        cout << "PdhCollectQueryData failed with: "  << hex <<  Status << dec << endl;
    }
    DWORD CounterType;
    PDH_FMT_COUNTERVALUE DisplayValue;
    Status = PdhGetFormattedCounterValue(Counter,
                                         PDH_FMT_DOUBLE,
                                         &CounterType,
                                         &DisplayValue);
    cout << "Counter: " << DisplayValue.doubleValue << endl;
	
    if (Query)
    {
       PdhCloseQuery(Query);
    }	
}

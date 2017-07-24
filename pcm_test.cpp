#include <iostream>

#include "cpucounters.h"

using namespace std;
int main() {
    PCM * m = PCM::getInstance();
    PCM::ErrorCode returnResult = m->program();
	
    switch (returnResult)
    {
        case PCM::Success:
            break;
        case PCM::MSRAccessDenied:
            cerr << "MSRAccessDenied." << endl;
            exit(EXIT_FAILURE);
        case PCM::PMUBusy:
            cerr << "Looks like Performance Monitoring Unit is occupied by other application" << endl;
            cerr << "Reset it? (y/n)" << endl;
            char yn;
            std::cin >> yn;
            if ('y' == yn)
            {
                m->resetPMU();
                cerr << "PMU configuration has been reset. Try to rerun the program again." << endl;
            }
            exit(EXIT_FAILURE);
        default:
            cerr << "Unknown error." << endl;
            exit(EXIT_FAILURE);
    }
/*	
    if (returnResult != PCM::Success) {
        std::cerr << "Intel's PCM couldn't start" << std::endl;
        std::cerr << "Error code: " << returnResult << std::endl;
        exit(1);
    }
*/

    SystemCounterState before_sstate = getSystemCounterState();


    /* Begin of code for testing */
    double x = 1;
    for(size_t i = 0; i<100000000; ++i)
    {
        x += std::sqrt(x);
    }
    /* End of code for testing */


    SystemCounterState after_sstate = getSystemCounterState();

	std::cout << fixed << left;
	int w = 35;
    std::cout << setw(w) << "Core cycles:" << getCycles(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) << "Average retired instructions:" << getExecUsage(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) << "Retired instructions:" << getInstructionsRetired(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "Invariant TSC:" << getInvariantTSC(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "Referen clock cycles:" << getRefCycles(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "Instructions per clock cycle:" << getIPC(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "Average core frequency:" << getAverageFrequency(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "Average relative core frequency:" << getRelativeFrequency(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "Core cycles lost due to L3 cache misses:" << getCyclesLostDueL3CacheMisses(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "Core cycles lost due to L2 cache misses:" << getCyclesLostDueL2CacheMisses(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "Core cycles lost due to L2 cache misses:" << getCyclesLostDueL2CacheMisses(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "L2 cache hit ratio:" << getL2CacheHitRatio(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "L2 cache hits:" << getL2CacheHits(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "L2 cache misses:" << getL2CacheMisses(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "L3 cache hit ratio:" << getL3CacheHitRatio(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "L3 cache hits:" << getL3CacheHits(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "L3 cache misses:" << getL3CacheMisses(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "L3 cache occupancy before:" << getL3CacheOccupancy(before_sstate) << std::endl;
    std::cout << setw(w) <<  "L3 cache occupancy after:" << getL3CacheOccupancy(after_sstate) << std::endl;
	
    std::cout << setw(w) <<  "SMI interrupts:" << getSMICount(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "Local Memory Bandwidth:" << getLocalMemoryBW(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "Remote Memory Bandwidth:" << getRemoteMemoryBW(before_sstate,after_sstate) << std::endl;
	
    std::cout << setw(w) <<  "Bytes read from RAM:" << getBytesReadFromMC(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "Bytes written to RAM:" << getBytesWrittenToMC(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "I/O Bytes:" << getIORequestBytesFromMC(before_sstate,after_sstate) << std::endl;

    std::cout << setw(w) <<  "Consumed by CPU energy (Joules):" << getConsumedJoules(before_sstate,after_sstate) << std::endl;
    std::cout << setw(w) <<  "Consumed by DRAM energy (Joules):" << getDRAMConsumedJoules(before_sstate,after_sstate) << std::endl;
	
	m->cleanup();
}




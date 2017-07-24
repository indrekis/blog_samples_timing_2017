#include <papi.h>

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <map>
#include <random>
#include <algorithm>

using namespace std;

int main()
{
    std::random_device rd;
    std::mt19937 rand_g(rd());
	
	// See http://icl.cs.utk.edu/projects/papi/wiki/PAPIC:Preset_Event_Definitions
	
    int ret = PAPI_library_init(PAPI_VER_CURRENT);
    
    if (ret != PAPI_VER_CURRENT) {
        cout << "PAPI library init error!" << endl;
        exit(1);
    }
	
	// ret = PAPI_multiplex_init();
    // if (ret != PAPI_OK) {
        // cout << "PAPI multiplexing init error!" << endl;
        // exit(1);
    // }	
	
    vector<int> Events {
					PAPI_L1_DCM, PAPI_L1_ICM, PAPI_L2_DCM, PAPI_L2_ICM, PAPI_L3_DCM, PAPI_L3_ICM,
					PAPI_L1_TCM, PAPI_L2_TCM, PAPI_L3_TCM,
					PAPI_TLB_DM, PAPI_TLB_IM, PAPI_TLB_TL, 
					PAPI_PRF_DM, 
					PAPI_STL_ICY, PAPI_FUL_ICY, PAPI_STL_CCY, PAPI_FUL_CCY, 
					PAPI_BR_CN, PAPI_BR_TKN, PAPI_BR_NTK, PAPI_BR_MSP, PAPI_BR_PRC,
					PAPI_TOT_IIS, PAPI_TOT_INS, PAPI_INT_INS, PAPI_FP_INS,
					PAPI_LD_INS, PAPI_SR_INS, PAPI_BR_INS, 
					PAPI_VEC_INS,
					PAPI_RES_STL, PAPI_FP_STAL,
					PAPI_HW_INT, 
					PAPI_LST_INS, PAPI_SYC_INS,
					PAPI_FML_INS, PAPI_FAD_INS, PAPI_FDV_INS, PAPI_FSQ_INS, PAPI_FNV_INS, PAPI_FP_OPS,
					PAPI_VEC_SP, PAPI_VEC_DP,PAPI_TOT_INS,PAPI_TOT_CYC
					};
	map<int, PAPI_event_info_t> events_info;
	for(int event: Events)
	{
		PAPI_event_info_t info;
		if ( (ret = PAPI_get_event_info(event, &info)) == PAPI_OK)
		{
			events_info[event] = info;
		}else{
			cout << "Error obtaining event info, code: " << ret << ", " <<  PAPI_strerror(ret) << endl;
		}		
	}
	vector<int> SupportedEvents;
	
	for(int event: Events)
	{
		if(events_info[event].count > 0)
			SupportedEvents.push_back(event);
	}
	cout << "Supported " << SupportedEvents.size() << " of " << Events.size() << " events" << endl;
	cout << "Hardware countes: " << PAPI_num_counters() << endl;
    vector<long_long> values(SupportedEvents.size());
	//cout << "Maximal events: " << PAPI_MAX_PRESET_EVENTS << endl;
	//! Hardware counters are toooo limited, so -- this trick.
	std::shuffle(SupportedEvents.begin(), SupportedEvents.end(), rand_g);	
	
    size_t NUM_EVENTS = min(static_cast<size_t>(PAPI_num_counters()), SupportedEvents.size());
    
    if ( (ret = PAPI_start_counters(SupportedEvents.data(), NUM_EVENTS)) != PAPI_OK)
    {
      cout << "Error starting counters, code: " << ret << ", " <<     PAPI_strerror(ret) << endl;
      return 1;
    }

     /* Begin of code for testing */
	double x = 1;
	for(size_t i = 0; i<100000000; ++i)
	{
		x += std::sqrt(x);
	}
	/* End of code for testing */

    if ( (ret = PAPI_read_counters(values.data(), NUM_EVENTS)) != PAPI_OK)
    {
      cout << "Err  code: " << ret << ", " <<     PAPI_strerror(ret) << endl;
      return 1;
    }

	char EventCodeStr[PAPI_MAX_STR_LEN];
	for(size_t i = 0; i<NUM_EVENTS; ++i)
	{
		cout << "Event code " << hex << SupportedEvents[i] << dec;
		if (PAPI_event_code_to_name(SupportedEvents[i], EventCodeStr) == PAPI_OK)
		{
			cout << ", " << EventCodeStr;
		}
		cout << ": " << values[i];
		PAPI_event_info_t info;
		if (PAPI_get_event_info(SupportedEvents[i], &info) == PAPI_OK)
		{
			cout << "\t" << info.long_descr;
		}else{
			cout << "\t" << "No description";
		}
		cout << endl;		
	}
}
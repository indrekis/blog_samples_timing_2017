cmake_minimum_required(VERSION 3.3)

set(CMAKE_VERBOSE_MAKEFILE ON)

project(timings)

set(CMAKE_CXX_STANDARD 11)

# For FindPAPI.cmake module
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}")

# https://stackoverflow.com/questions/2368811/how-to-set-warning-level-in-cmake
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /wd4503")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

add_executable(run_metrics run_metrics.cpp)
IF(WIN32)
TARGET_LINK_LIBRARIES(run_metrics psapi)
ENDIF(WIN32)

add_executable(diagnose_sys diagnose_sys.cpp)
IF(UNIX)
target_link_libraries(diagnose_sys rt)
ENDIF()

add_executable(clock clock.cpp)

FIND_PACKAGE( Boost COMPONENTS chrono system )
if(Boost_FOUND)
	add_executable(boost_clocks boost_clocks.cpp)
	INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
	target_link_libraries( boost_clocks ${Boost_LIBRARIES} ) 
endif()
 
IF(WIN32 AND NOT CYGWIN)
add_executable(browse_perf_counters_win32 browse_perf_counters_win32.cpp )
add_executable(perf_counters_sample_win32 perf_counters_sample_win32.cpp )

TARGET_LINK_LIBRARIES(browse_perf_counters_win32 psapi pdh)
TARGET_LINK_LIBRARIES(perf_counters_sample_win32 psapi pdh)
ENDIF()

find_package(PAPI)

if(PAPI_FOUND)
    include_directories(${PAPI_INCLUDE_DIRS})
	add_executable(papi_test  papi_test.cpp )
    target_link_libraries(papi_test ${PAPI_LIBRARIES})
endif()

# ==Different implementations of check_timeouts===================
add_executable(check_timeouts_chrono check_timeouts.cpp)
target_compile_definitions(check_timeouts_chrono PRIVATE USE_CHRONO)

add_executable(check_timeouts_clock check_timeouts.cpp)
target_compile_definitions(check_timeouts_clock PRIVATE USE_CLOCK)

IF(WIN32)
add_executable(check_timeouts_win32_perf_cnt check_timeouts.cpp)
target_compile_definitions(check_timeouts_win32_perf_cnt PRIVATE USE_MSWIN_PERFORMANCE_COUNTERS)
ENDIF()

# CYGWIN is included in UNIX
# Should I use if(UNIX AND NOT APPLE)?
IF(UNIX)
add_executable(check_timeouts_linux_wall_timers check_timeouts.cpp)
target_compile_definitions(check_timeouts_linux_wall_timers PRIVATE USE_LINUX_WALL_TIMERS)
target_link_libraries(check_timeouts_linux_wall_timers rt)
ENDIF()

// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include <ctime>
#include <cmath>
#include <limits>

using std::cout;
using std::endl;

int main() {	
    std::clock_t start = std::clock();

    /* Begin of code for testing */
	double x = 1;
	for(size_t i = 0; i<100000000; ++i)
	{
		x += std::sqrt(x);
	}
	/* End of code for testing */

    double duration = ( std::clock() - start ) / static_cast<double>(CLOCKS_PER_SEC);

    cout << "Time: " << duration << " Seconds " << endl;
	
	return 0;
}


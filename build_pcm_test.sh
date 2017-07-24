#!/bin/sh

git clone https://github.com/opcm/pcm
cd pcm
make
cd ..
g++ -std=c++11 -I./pcm pcm/cpucounters.o pcm/pci.o pcm/msr.o pcm/client_bw.o  -lpthread  pcm_test.cpp -o pcm_test
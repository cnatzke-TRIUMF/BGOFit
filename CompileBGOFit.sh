#!/bin/bash 

echo "::: Building BGOFit ..."
g++ bgo_fit.cxx -std=c++0x `root-config --cflags --libs` -lTreePlayer -lMathMore -lSpectrum -lMinuit -lPyROOT -o BGOFit
echo "::: Building BGOFit ... [DONE]"


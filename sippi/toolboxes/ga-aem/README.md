# GA-AEM forward
This folder contains a SIPPI forward wrapper that allow calling the GA-AEM EM forward modeling code (https://github.com/GeoscienceAustralia/ga-aem) using SIPPI.

## Install ga-aem from github

    git clone git@github.com:GeoscienceAustralia/ga-aem.git

On Linux will need the following libraries installed:

    sudo apt-get install build-essential
    sudo apt-get install libfftw3-dev
    sudo apt-get install libopenmpi-dev


Then you can install ga-aem using

    cd ga-aem/makefiles
    export cxx=g++
    export mpicxx=mpiCC
    export cxxflags='-std=c++11 -O3 -Wall'
    export exedir='../bin/'
    ./run_make.sh



On Windows you should  add libfftw3-3.dll to ga-aem/matlab/bin/x64

    cd ga-aem
    cp third_party/fftw3.2.2.dlls/64bit/libfftw3-3.dll matlab/bin/x64/.

# Matlab path
Add path to the following directories in Matlab:

    addpath ga-aem-folder/matlab/gatdaem1d_functions
    addpath ga-aem-folder/matlab/bin/x64
    addpath ga-aem-folder/matlab/bin/linux
    


### Compile issues 
Please checkout https://github.com/GeoscienceAustralia/ga-aem for details on installation.



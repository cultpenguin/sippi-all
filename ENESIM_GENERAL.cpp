//Copyright (c) 2015 I-GIS (www.i-gis.dk) and Solid Earth Geophysics, Niels Bohr Institute (http://imgp.nbi.ku.dk)
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <iomanip>      // std::setprecision
#include <algorithm>    // std::random_shuffle std::remove_if
#include <list>

#include "ENESIM_GENERAL.h"
#include "IO.h"
#include "Utility.h"
#include "Coords3D.h"

/**
* @brief Constructors from a configuration file
*/
MPS::ENESIM_GENERAL::ENESIM_GENERAL(const std::string& configurationFile) : MPS::ENESIM(){
	initialize(configurationFile);
}

/**
* @brief Destructors
*/
MPS::ENESIM_GENERAL::~ENESIM_GENERAL(void) {
	_shuffleEntropyFactor=4;
}

/**
* @brief Initialize the simulation from a configuration file
* @param configurationFile configuration file name
*/
void MPS::ENESIM_GENERAL::initialize(const std::string& configurationFile) {
	//Reading configuration file
	_readConfigurations(configurationFile);

	//Reading data from files
	_readDataFromFiles();

	//Checking the TI array dimensions
	_tiDimX = (int)_TI[0][0].size();
	_tiDimY = (int)_TI[0].size();
	_tiDimZ = (int)_TI.size();

	//Define a random path to loop through TI cell
	_tiPath.resize(_tiDimX * _tiDimY * _tiDimZ);
	_initilizePath(_tiDimX, _tiDimY, _tiDimZ, _tiPath);
	if (_shuffleTiPath) {
		std::random_shuffle ( _tiPath.begin(), _tiPath.end() );
	}

	//Define multi threading parameters
	_threads.resize(_numberOfThreads - 1);
	_jobDone = false;

	if(_debugMode > -1 ) {
		std::cout << "Number of threads: " << _numberOfThreads << std::endl;
		std::cout << "Conditional points: " << _maxNeighbours << std::endl;
		std::cout << "Max iterations: " << _maxIterations << std::endl;
		std::cout << "SG: " << _sgDimX << " " << _sgDimY << " " << _sgDimZ << std::endl;
		std::cout << "TI: " << _tiFilename << " " << _tiDimX << " " << _tiDimY << " " << _tiDimZ << " " << _TI[0][0][0]<< std::endl;
	}
}

/**
* @brief Start the simulation
* Virtual function implemented from MPSAlgorithm
*/
void MPS::ENESIM_GENERAL::startSimulation(void) {
	//Call parent function
	MPS::MPSAlgorithm::startSimulation();
}

/**
* @brief MPS dsim simulation algorithm main function
* @param sgIdxX index X of a node inside the simulation grind
* @param sgIdxY index Y of a node inside the simulation grind
* @param sgIdxZ index Z of a node inside the simulation grind
* @param level multigrid level
* @return found node's value
*/
float MPS::ENESIM_GENERAL::_simulate(const int& sgIdxX, const int& sgIdxY, const int& sgIdxZ, const int& level) {
	// return _getRealizationFromCpdfTiEnesimMetropolis(sgIdxX, sgIdxY, sgIdxZ, _sgIterations[sgIdxZ][sgIdxY][sgIdxX]);
	return _getRealizationFromCpdfTiEnesim(sgIdxX, sgIdxY, sgIdxZ, _sgIterations[sgIdxZ][sgIdxY][sgIdxX]);

}

//Copyright (c) 2015 I-GIS (www.i-gis.dk) and Solid Earth Geophysics, Niels Bohr Institute (http://imgp.nbi.ku.dk)
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include "ENESIM.h"

namespace MPS {
	class ENESIM_GENERAL;
}

/**
* @brief An implementation of the Direct Sampling algorithm
*/
class MPS::ENESIM_GENERAL :
	public MPS::ENESIM
{
protected:
	/**
	* @brief MPS ENESIM simulation algorithm main function
	* @param sgIdxX index X of a node inside the simulation grind
	* @param sgIdxY index Y of a node inside the simulation grind
	* @param sgIdxZ index Z of a node inside the simulation grind
	* @param level level of the current grid
	* @return found node's value
	*/
	virtual float _simulate(const int& sgIdxX, const int& sgIdxY, const int& sgIdxZ, const int& level);

public:
	/**
	* @brief Constructors from a configuration file
	*/
	ENESIM_GENERAL(const std::string& configurationFile);
	/**
	* @brief Initialize the simulation from a configuration file
	* @param configurationFile configuration file name
	*/
	virtual void initialize(const std::string& configurationFile);
	/**
	* @brief Start the simulation
	* Virtual function implemented from MPSAlgorithm
	*/
	virtual void startSimulation(void);
	/**
	* @brief Destructors
	*/
	virtual ~ENESIM_GENERAL(void);
};

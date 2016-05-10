// (c) 2016 I-GIS (www.i-gis.dk) and Solid Earth Geophysics, Niels Bohr Institute (http://imgp.nbi.ku.dk)
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with this program (COPYING.LESSER).  If not, see <http://www.gnu.org/licenses/>.
//
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

// (c) 2015-2016 I-GIS (www.i-gis.dk) and Solid Earth Geophysics, Niels Bohr Institute (http://imgp.nbi.ku.dk)
//
//    This file is part of MPSlib.
//
//    MPSlib is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    MPSlib is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with MPSlib (COPYING.LESSER).  If not, see <http://www.gnu.org/licenses/>.
//
#pragma once

#include <iostream>
#include <map>

#include "MPSAlgorithm.h"

namespace MPS {
	class ENESIM;
	class Coords3D;
}

/**
* @brief An implementation of the ENESIM algorithm
*/
class MPS::ENESIM :
	public MPS::MPSAlgorithm
{
private:

protected:
	/**
	* @brief maximum number of counts setting up conditional pdf
	*/
	int _nMaxCountCpdf;

	/**
	* @brief Select whether to use Metropolis style soft data conditioning
	*/
	int _MetropolisSoftData = 0;
	//_MetropolisSoftData = 1;



	/**
	* @brief Read configuration file
	* @param fileName configuration filename
	*/
	void _readConfigurations(const std::string& fileName);


	/**
	* @brief Compute cpdf from training image using ENESIM type
	* @param x coordinate X of the current node, pdf1
	* @param y coordinate Y of the current node, pdf2 (map must contain ALL possible outcome values)
	* @param map with conditional pdf
	* @param z coordinate Z of the current node
	* @return true if found a value
	*/
	bool _getCpdfTiEnesim(const int& sgIdxX, const int& sgIdxY, const int& sgIdxZ, std::map<float, float>& cPDF);

	/**
	* @brief Compbine two PDFs assuming independence
	* @param probability density 1, pdf1
	* @param probability density 2, pdf2
	* @return true if succesfull, and update pdf is available in pdf1
	*/
	bool _combinePdf(std::map<float, float>& cPdf, std::map<float, float>& softPdf);


	/**
	* @brief Compute realization using cpdf obtained using ENESIM and softdata
	* @param sgIdxX coordinate X of the current node
	* @param sgIdxY coordinate Y of the current node
	* @param sgIdxZ coordinate Z of the current node
	* @param iterationCnt Iterations counter
	* @return simulated value
	*/
	float _getRealizationFromCpdfTiEnesim(const int& sgIdxX, const int& sgIdxY, const int& sgIdxZ, float& iterationCnt);

	/**
	* @brief Compute realization using cpdf obtained using ENESIM and Metropolis with soft data
	* @param sgIdxX coordinate X of the current node
	* @param sgIdxY coordinate Y of the current node
	* @param sgIdxZ coordinate Z of the current node
	* @param iterationCnt Iterations counter
	* @return simulated value
	*/
	float _getRealizationFromCpdfTiEnesimMetropolis(const int& sgIdxX, const int& sgIdxY, const int& sgIdxZ, float& iterationCnt);
	
public:
	/**
	* @brief Constructors default and empty parameters
	*/
	ENESIM(void);
	/**
	* @brief Destructors
	*/
	virtual ~ENESIM(void);

};

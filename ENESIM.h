//Copyright (c) 2015 I-GIS (www.i-gis.dk) and Solid Earth Geophysics, Niels Bohr Institute (http://imgp.nbi.ku.dk)
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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

//Copyright (c) 2015 I-GIS (www.i-gis.dk) and Solid Earth Geophysics, Niels Bohr Institute (http://imgp.nbi.ku.dk)
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "MPSAlgorithm.h"
#include "Coords4D.h"
#include "Utility.h"

namespace igisSIM {
	class SNESIM;
	class Coords3D;
}

/**
* @brief Abstrat class of SNESIM implementation
*/
class igisSIM::SNESIM :
	public igisSIM::MPSAlgorithm
{
protected:

	/**
	* @brief Class using to sort by 3D distance element in a list
	*/
	class TemplateSorter {
		int _templateSizeX;
		int _templateSizeY;
		int _templateSizeZ;
	public:
		TemplateSorter(const int& templateSizeX, const int& templateSizeY, const int& templateSizeZ) : _templateSizeZ(templateSizeZ), _templateSizeY(templateSizeY), _templateSizeX(templateSizeX) {}
		bool operator()(const int& idx1, const int& idx2) const {
			int idx1X, idx1Y, idx1Z, idx2X, idx2Y, idx2Z;
			int templateCenterX = (int)floor(_templateSizeX / 2);
			int templateCenterY = (int)floor(_templateSizeY / 2);
			int templateCenterZ = (int)floor(_templateSizeZ / 2);	
			igisSIM::utility::oneDTo3D(idx1, _templateSizeX, _templateSizeY, idx1X, idx1Y, idx1Z);
			igisSIM::utility::oneDTo3D(idx2, _templateSizeX, _templateSizeY, idx2X, idx2Y, idx2Z);
			return (pow(idx1X - templateCenterX, 2) + pow(idx1Y - templateCenterY, 2) + pow(idx1Z - templateCenterZ, 2)) < (pow(idx2X - templateCenterX, 2) + pow(idx2Y - templateCenterY, 2) + pow(idx2Z - templateCenterZ, 2));
		}
	};	

	/**
	* @brief Read configuration file
	* @param fileName configuration filename
	*/
	void _readConfigurations(const std::string& fileName);

	/**
	* @brief Construct templatefaces and sort them around template center
	*/
	void _constructTemplateFaces(void);

	/**
	* @brief Getting a node value by calculating the cummulatie probability distribution function
	* @param conditionalPoints list of all found conditional points
	* @param x coordinate X of the current node
	* @param y coordinate Y of the current node
	* @param z coordinate Z of the current node
	*/
	float _cpdf(std::map<float, int>& conditionalPoints, const int& x, const int& y, const int& z);

	/**
	* @brief patterns dictionary
	*/
	std::vector<std::map<std::vector<float>, int>> _patternsDictionary;

	/**
	* @brief template size X
	*/
	int _templateSizeX;

	/**
	* @brief template size y
	*/
	int _templateSizeY;

	/**
	* @brief template size z
	*/
	int _templateSizeZ;	

	/**
	* @brief Min node count allowed
	*/
	int _minNodeCount;

	/**
	* @brief List of available faces in the template 
	*/
	std::vector<igisSIM::Coords3D> _templateFaces;
public:
	/**
	* @brief Constructors default and empty parameters
	*/
	SNESIM(void);
	/**
	* @brief Destructors
	*/
	virtual ~SNESIM(void);
};


//Copyright (c) 2015 I-GIS (www.i-gis.dk) and Solid Earth Geophysics, Niels Bohr Institute (http://imgp.nbi.ku.dk)
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#include <iostream>
#include "Coords4D.h"

/**
* @brief Constructors
*
* Default value is (0, 0, 0, NaN)
*/
igisSIM::Coords4D::Coords4D(void) : Coords3D() {
	_v = std::numeric_limits<float>::quiet_NaN();
	//Emtpy constructor
}

/**
* @brief Constructors from x, y, z and value 
* @param x x value
*	@param y y value
* @param z z value
* @param v v value default is NaN
*/
igisSIM::Coords4D::Coords4D(const int& x, const int& y, const int& z, const float& v) : Coords3D(x, y, z){
	_v = v;
}

/**
* @brief Constructors from a coords3D and a value 
* @param coord3D a coord3D
* @param v v value default is NaN
*/
igisSIM::Coords4D::Coords4D(const igisSIM::Coords3D coord3D, const float& v) : Coords3D(coord3D.getX(), coord3D.getY(), coord3D.getZ()){
	_v = v;
}

/**
* @brief Destructors
*/
igisSIM::Coords4D::~Coords4D(void) {
}

namespace igisSIM {
	/**
	* @brief Compare two coordinates
	*
	* @param lhs lef hand value
	* @param rhs right hand value
	*
	* @return true if (x, y, z, v) are equals
	*/
	bool operator==(const igisSIM::Coords4D& lhs, const igisSIM::Coords4D& rhs){
		return (lhs.getX() == rhs.getX() && lhs.getY() == rhs.getY() && lhs.getZ() == rhs.getZ() && lhs.getValue() == rhs.getValue());
	}

	/**
	* @brief Compare two coordinates
	*
	* @param lhs lef hand value
	* @param rhs right hand value
	*
	* @return true if (x, y, z, v) are inferior
	*/
	bool operator<(const igisSIM::Coords4D& lhs, const igisSIM::Coords4D& rhs){
		std::cout << "when this is call ?" << std::endl;
		return (lhs.getX() < rhs.getX() && lhs.getY() < rhs.getY() && lhs.getZ() < rhs.getZ() && lhs.getValue() < rhs.getValue());
	}
}

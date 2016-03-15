//Copyright (c) 2015 I-GIS (www.i-gis.dk) and Solid Earth Geophysics, Niels Bohr Institute (http://imgp.nbi.ku.dk)
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "Coords3D.h"

/**
* @brief Constructors
*
* Default value is (0, 0, 0)
*/
MPS::Coords3D::Coords3D(void) {
}

/**
* @brief Constructors from x, y, z
* @param x x value
* @param y y value
* @param z z value
*/
MPS::Coords3D::Coords3D(const int& x, const int& y, const int& z) {
	_x = x;
	_y = y;
	_z = z;
}

/**
* @brief Destructors
*/
MPS::Coords3D::~Coords3D(void) {
}

namespace MPS {
	/**
	* @brief Compare two coordinates
	*
	* @param lhs lef hand value
	* @param rhs right hand value
	*
	* @return true if (x, y, z) are equals
	*/
	bool operator==(const MPS::Coords3D& lhs, const MPS::Coords3D& rhs){
		return (lhs.getX() == rhs.getX() && lhs.getY() == rhs.getY() && lhs.getZ() == rhs.getZ());
	}
}

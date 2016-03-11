//Copyright (c) 2015 I-GIS (www.i-gis.dk) and Solid Earth Geophysics, Niels Bohr Institute (http://imgp.nbi.ku.dk)
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <limits>
#include "Coords3D.h"

namespace igisSIM {
	class Coords4D;
}

/**
* @brief A 4D coordinates used to keep a 3D coordinate x, y, z and a value v
*/
class igisSIM::Coords4D: public igisSIM::Coords3D
{
private:
	/**
	* @brief value
	*/
	float _v;
public:
	/**
	* @brief Constructors
	*
	* Default value is (0, 0, 0, NaN)
	*/
	Coords4D(void);

	/**
	* @brief Constructors from x, y, z and value 
	* @param x x value
	*	@param y y value
	* @param z z value
	* @param v v value default is NaN
	*/
	Coords4D(const int& x, const int& y, const int& z, const float& v=std::numeric_limits<float>::quiet_NaN());

	/**
	* @brief Constructors from a coords3D and a value 
	* @param coord3D a coord3D
	* @param v v value default is NaN
	*/
	Coords4D(const igisSIM::Coords3D coord3D, const float& v=std::numeric_limits<float>::quiet_NaN());

	/**
	* @brief Destructors
	*/
	~Coords4D(void);

	/**
	* @brief Getter v Value
	* @return x value
	*/
	inline float getValue(void) const {return _v;}

	/**
	* @brief Setter v Value
	* @param v v value
	*/
	inline void setValue(const float& v) {_v = v;}
};

namespace igisSIM {
	/**
	* @brief Compare two coordinates
	*
	* @param lhs lef hand value
	* @param rhs right hand value
	*
	* @return true if (x, y, z, v) are equals
	*/
	bool operator==(const igisSIM::Coords4D& lhs, const igisSIM::Coords4D& rhs);

	/**
	* @brief Compare two coordinates
	*
	* @param lhs lef hand value
	* @param rhs right hand value
	*
	* @return true if (x, y, z, v) are inferior
	*/
	bool operator<(const igisSIM::Coords4D& lhs, const igisSIM::Coords4D& rhs);
}


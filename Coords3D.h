//Copyright (c) 2015 I-GIS (www.i-gis.dk) and Solid Earth Geophysics, Niels Bohr Institute (http://imgp.nbi.ku.dk)
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

namespace igisSIM {
	class Coords3D;
}


/**
* @brief A 3D coordinates used for indexing in a 3D grid
*/
class igisSIM::Coords3D
{
private:
	/**
	* @brief x index
	*/
	int _x;

	/**
	* @brief y index
	*/
	int _y;

	/**
	* @brief z index
	*/
	int _z;
public:
	/**
	* @brief Constructors
	*
	* Default value is (0, 0, 0)
	*/
	Coords3D(void);

	/**
	* @brief Constructors from x, y, z
	* @param x x value
	* @param y y value
	* @param z z value
	*/
	Coords3D(const int& x, const int& y, const int& z);

	/**
	* @brief Destructors
	*/
	~Coords3D(void);

	/**
	* @brief Getter X value
	* @return x value
	*/
	inline int getX(void) const {return _x;}

	/**
	* @brief Getter Y value
	* @return y value
	*/
	inline int getY(void) const {return _y;}

	/**
	* @brief Getter Z value
	* @return z value
	*/
	inline int getZ(void) const {return _z;}

	/**
	* @brief Setter X value
	* @param x x value
	*/
	inline void setX(const int& x) {_x = x;}

	/**
	* @brief Setter Y value
	* @param y y value
	*/
	inline void setY(const int& y) {_y = y;}

	/**
	* @brief Setter Z value
	* @param z z value
	*/
	inline void setZ(const int& z) {_z = z;}
};

namespace igisSIM {
	/**
	* @brief Compare two coordinates
	*
	* @param lhs lef hand value
	* @param rhs right hand value
	*
	* @return true if (x, y, z) are equals
	*/
	bool operator==(const igisSIM::Coords3D& lhs, const igisSIM::Coords3D& rhs);
}


//Copyright (c) 2015 I-GIS (www.i-gis.dk) and Solid Earth Geophysics, Niels Bohr Institute (http://imgp.nbi.ku.dk)
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <vector>
#include <string>

namespace MPS {
	/**
	* @brief Utility package which contains utility functions used in simulation algorithms
	*
	* Functions used to do data conversion, checking ...
	*
	*/
	namespace utility {
		/**
		* @brief Check if a value is NAN
		* 
		* @param x value to check
		*
		* @return true if the input value is NAN
		*/
		bool is_nan(const float &x); 

		/**
		* @brief Convert 1D index to 3D index
		* 
		* @param oneDIndex index of the 1D array
		* @param dimX dimension X of the 3D array 
		* @param dimY dimension Y of the 3D array
		* @param idxX output index X
		* @param idxY output index Y
		* @param idxZ output index Z
		*/
		void oneDTo3D(const int& oneDIndex, const int& dimX, const int& dimY, int& idxX, int& idxY, int& idxZ);

		/**
		* @brief Convert 3D index to 1D index
		* 
		* @param idxX index X of the 3D array
		* @param idxY index Y of the 3D array
		* @param idxZ index Z of the 3D arary
		* @param dimX dimension X of the 3D array 
		* @param dimY dimension Y of the 3D array
		* @param oneDIndex output index of the 1D array
		*/
		void treeDto1D(const int& idxX, const int& idxY, const int& idxZ, const int& dimX, const int& dimY, int& oneDIndex);

		/**
		* @brief Convert seconds to hour minute second
		* 
		* @param seconds seconds
		* @param hour output hour
		* @param minute output minute
		* @param second output second
		*/
		void secondsToHrMnSec(const int& seconds, int& hour, int& minute, int& second);

		/**
		* @brief Limit a value, could be clamp it or wrap around
		* 
		* @param aValue input value
		* @param maxValue the maximum value that aValue could have
		* @param wrapAround if true then if aValue is greater than maxValue, it will be modulo with the maxValue. If false the aValue will be clamp to maxValue
		*/
		void limitValue(int& aValue, const int& maxValue, const bool& wrapAround=true);

		/**
		* @brief Get a random value from a voxel grid
		* 
		* @param ti the 3D grid
		* @param tiDimX grid size X
		* @param tiDimY grid size Y
		* @param tiDimZ grid size Z
		*
		* @return a random value from the grid
		*/
		float getRandomValue(const std::vector<std::vector<std::vector<float>>>& ti, const int& tiDimX, const int& tiDimY, const int& tiDimZ);

		/**
		* @brief Get a file extension
		* 
		* @param filename the file name
		*
		* @return filename extension
		*/
		std::string getExtension(const std::string& filename);
	}
}

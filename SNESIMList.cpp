//Copyright (c) 2015 I-GIS (www.i-gis.dk) and Solid Earth Geophysics, Niels Bohr Institute (http://imgp.nbi.ku.dk)
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <iomanip>      // std::setprecision
#include <cctype>       // isspace
#include <algorithm>    // std::random_shuffle std::remove_if
#include <list>

#include "SNESIMList.h"
#include "IO.h"
#include "Coords3D.h"

/**
* @brief Constructors from a configuration file
*/
igisSIM::SNESIMList::SNESIMList(const std::string& configurationFile) : igisSIM::SNESIM(){
	initialize(configurationFile);
}

/**
* @brief Destructors
*/
igisSIM::SNESIMList::~SNESIMList(void) {

}

/**
* @brief Initialize the simulation from a configuration file
* @param configurationFile configuration file name
*/
void igisSIM::SNESIMList::initialize(const std::string& configurationFile) {

	//Reading configuration file
	_readConfigurations(configurationFile);

	//Reading data from files
	_readDataFromFiles();

	//Checking the TI array dimensions
	_tiDimX = (int)_TI[0][0].size();
	_tiDimY = (int)_TI[0].size();
	_tiDimZ = (int)_TI.size();

	//Building template structure
	_constructTemplateFaces();

	//Scanning the TI and build the search dictionary
	//Building a multi spaces search dictionary
	int offset = 1;
	_patternsDictionary.resize(_totalGridsLevel + 1);
	for (int level=_totalGridsLevel; level>=0; level--) {
		//For each space level from coarse to fine 
		offset = int(std::pow(2, level));		
		if (_debugMode > -1) {
		  std::cout << "level: " << level << " offset: " << offset << std::endl;
		}
		int tiX, tiY, tiZ;
		int deltaX, deltaY, deltaZ;
		bool isValidKey = false;
		std::vector<float> key;
		int pixelsCnt = 0;
		for (int z=0; z<_tiDimZ; z+=1) {
			for (int y=0; y<_tiDimY; y+=1) {
				for (int x=0; x<_tiDimX; x+=1) {
					//For each pixel
					pixelsCnt ++;
					isValidKey = false;
					key.clear();
					for (unsigned int i=0; i<_templateFaces.size(); i++) {
						deltaX = offset * _templateFaces[i].getX();
						deltaY = offset * _templateFaces[i].getY();
						deltaZ = offset * _templateFaces[i].getZ();
						tiX = x + deltaX;
						tiY = y + deltaY;
						tiZ = z + deltaZ;
						if ((tiX < 0 || tiX >= _tiDimX) || (tiY < 0 || tiY >= _tiDimY) || (tiZ < 0 || tiZ >= _tiDimZ) || igisSIM::utility::is_nan(_TI[tiZ][tiY][tiX])) {
							//Out of bound or NaN value then just ignore this key
							isValidKey = false;
							break;
							//key.push_back(std::numeric_limits<float>::quiet_NaN());
							//isValidKey = true;
						}
						else { 
							key.push_back(_TI[tiZ][tiY][tiX]);
							//Check key only when no overflow indices and no NaN value
							isValidKey = true;
						}
					}

					if (isValidKey) {
						//add new key if not found and increase counter if found
						auto itr = _patternsDictionary[level].find(key);
						if (itr != _patternsDictionary[level].end()) { //found 
							//Increase found counter;
							itr->second ++;
						} else { //not found
							//Add a new key inside the dictionary with counter equal to 1
							_patternsDictionary[level].insert ( std::pair<std::vector<float>,int>(key, 1) );
						}
					}
				}
			}	
		}
		if (_debugMode > -1) {
		  std::cout << "Total pixel: " << pixelsCnt << std::endl;
		  //Check out dictionary
		  std::cout << "Dictionary info: " << std::endl;
		  std::cout << "Level: " << level << std::endl;
		  std::cout << "Total elements: " << _patternsDictionary[level].size() << std::endl;
		}
		////Showing the dictionary to debug ...
		//for(auto iter = _patternsDictionary[level].begin(); iter != _patternsDictionary[level].end(); ++iter) {
		//	std::vector<float> key = iter->first;
		//	for (int i=0; i<key.size(); i++)
		//	std::cout << key[i] << " ";
		//	std::cout << " : " << iter->second << std::endl << std::endl;
		//}
	}	
}

/**
* @brief Start the simulation
* Virtual function implemented from MPSAlgorithm
*/
void igisSIM::SNESIMList::startSimulation(void) {
	//Call parent function
	igisSIM::MPSAlgorithm::startSimulation();
}

/**
* @brief MPS dsim simulation algorithm main function
* @param sgIdxX index X of a node inside the simulation grind
* @param sgIdxY index Y of a node inside the simulation grind
* @param sgIdxZ index Z of a node inside the simulation grind
* @param level multigrid level
* @return found node's value
*/
float igisSIM::SNESIMList::_simulate(const int& sgIdxX, const int& sgIdxY, const int& sgIdxZ, const int& level) {
	//Initialize with node's value
	float foundValue = _sg[sgIdxZ][sgIdxY][sgIdxX];
	//If have NaN value then doing the simulation ...
	if (igisSIM::utility::is_nan(_sg[sgIdxZ][sgIdxY][sgIdxX])) {
		int offset = int(std::pow(2, level));			
		int sgX, sgY, sgZ;
		int deltaX, deltaY, deltaZ;
		foundValue = std::numeric_limits<float>::quiet_NaN();
		int maxConditionalPoints = -1, conditionPointsUsedCnt = 0;
		//Initialize a value
		std::vector<float> nodeTemplate;
		//Building a template based on the neighbor points
		for (unsigned int i=1; i<_templateFaces.size(); i++) { //For all the set of templates available except the first one at the template center 	
			//For each template faces
			deltaX = offset * _templateFaces[i].getX();
			deltaY = offset * _templateFaces[i].getY();
			deltaZ = offset * _templateFaces[i].getZ();
			sgX = sgIdxX + deltaX;
			sgY = sgIdxY + deltaY;
			sgZ = sgIdxZ + deltaZ;
			if (!(sgX < 0 || sgX >= _sgDimX) && !(sgY < 0 || sgY >= _sgDimY) && !(sgZ < 0 || sgZ >= _sgDimZ)) { 
				//not overflow
				if (!igisSIM::utility::is_nan(_sg[sgZ][sgY][sgX])) {
					nodeTemplate.push_back(_sg[sgZ][sgY][sgX]);
				} else { //NaN value
					nodeTemplate.push_back(std::numeric_limits<float>::quiet_NaN());
				}
			} else nodeTemplate.push_back(std::numeric_limits<float>::quiet_NaN());
		}

		//Adding the first value to complete the template
		std::vector<float> dictionaryTemplate;
		bool perfectMatched = false;
		int currentLevel = 0, maxLevel = 0;
		int sumCounters = 0;
		//Searching the template inside the dictionary
		std::map<float, int> conditionalPoints;
		//std::vector<std::pair<float, int>> conditionalPoints;
		for(std::map<std::vector<float>,int>::iterator iter = _patternsDictionary[level].begin(); iter != _patternsDictionary[level].end(); ++iter) {
			dictionaryTemplate = iter->first;
			//Compare the fullTemplate with the one in dictionary
			//Could be done parallele here
			sumCounters = iter->second;
			conditionPointsUsedCnt = 0;
			//maxLevel = 0;
			for(std::string::size_type i = 0; i < nodeTemplate.size(); ++i) {
				if ((dictionaryTemplate[i + 1] == nodeTemplate[i]) && (!igisSIM::utility::is_nan(nodeTemplate[i]))) { //Is a matched
					currentLevel = i;
					//Template found so go to higher level node
					if (currentLevel > maxLevel) { 	
						maxLevel = currentLevel;
						//Restart counter at only maximum level
						//sumCounters = iter->second;
					} 
					//else if (currentLevel == maxLevel) {
					//	//Adding the counter to the sum counters
					//	sumCounters += iter->second;
					//}
					conditionPointsUsedCnt ++;
				}
			}

			//Only keep the highest conditional points used
			if (conditionPointsUsedCnt > maxConditionalPoints && currentLevel == maxLevel) {
				//maxLevel = currentLevel;
				//Clearing the current conditional points
				conditionalPoints.clear();
				//Adding points to the list
				conditionalPoints.insert ( std::pair<float, int>(dictionaryTemplate[0], sumCounters) );
				maxConditionalPoints = conditionPointsUsedCnt;
			} else if(conditionPointsUsedCnt == maxConditionalPoints) {
				auto itr = conditionalPoints.find(dictionaryTemplate[0]);
				if (itr != conditionalPoints.end()) { //found 
					//Increase counter if is the same facie value
					conditionalPoints[dictionaryTemplate[0]] += sumCounters; 
				} else {
					//Adding new facie points to the list
					conditionalPoints.insert ( std::pair<float, int>(dictionaryTemplate[0], sumCounters) );
				}
			}

			////Perfect match then stop searching TODO: Need to check about this, speedup but could be wrong mathematically because the lack of cpdf
			//if (conditionPointsUsedCnt == (dictionaryTemplate.size() - 1)) perfectMatched = true;
			//if (perfectMatched) break;
		}

		//Get the value from cpdf
		foundValue = _cpdf(conditionalPoints, sgIdxX, sgIdxY, sgIdxZ);
	}
	return foundValue;
}

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

#include "SNESIMTree.h"
#include "IO.h"
#include "Coords3D.h"

/**
* @brief Constructors from a configuration file
*/
igisSIM::SNESIMTree::SNESIMTree(const std::string& configurationFile) : igisSIM::SNESIM(){
	initialize(configurationFile);
}

/**
* @brief Destructors
*/
igisSIM::SNESIMTree::~SNESIMTree(void) {

}


/**
* @brief Initialize the simulation from a configuration file
* @param configurationFile configuration file name
*/
void igisSIM::SNESIMTree::initialize(const std::string& configurationFile) {
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

	//Scanning the TI and build the search tree
	//Building a multi spaces search tree
	int offset = 1;

	//Resize search tree by level for multigrid level
	_searchTree.resize(_totalGridsLevel + 1);
	if (_debugMode > -1) {
		std::cout << "TI size (X,Y,Z): " << _tiDimX << " " << _tiDimY << " " << _tiDimZ << std::endl;
	}

	for (int level=_totalGridsLevel; level>=0; level--) {
		//For each space level from coarse to fine 
		offset = int(std::pow(2, level));		
		if (_debugMode > -1) {
		  std::cout << "level: " << level << " offset: " << offset << std::endl;
		}
		
		int tiX, tiY, tiZ;
		int deltaX, deltaY, deltaZ;
		int nodeCnt = 0;
		bool foundExistingValue = false;
		int foundIdx = 0;
		int totalNodes = _tiDimX * _tiDimY * _tiDimZ;
		int lastProgress = 0;
		//Put the current node as the root node
		std::vector<TreeNode>* currentTreeNode = &_searchTree[level]; 

		for (int z=0; z<_tiDimZ; z+=1) {
			for (int y=0; y<_tiDimY; y+=1) {
				for (int x=0; x<_tiDimX; x+=1) {
					//For each pixel
					nodeCnt ++;
					if (_debugMode > -1) {
						//Doing the progression
						//Print progression on screen
						int progress = (int)((nodeCnt / (float)totalNodes) * 100);
						if ((progress % 10) == 0 && progress != lastProgress) { //Report every 10%
							lastProgress = progress;
							std::cout << "Building search tree at level: " << level << " Progression (%): " << progress << std::endl;
						}
					}
					//Reset current node to root node
					currentTreeNode = &_searchTree[level]; 
					for (unsigned int i=0; i<_templateFaces.size(); i++) { 
						//Go deeper in the pattern template or to a higher level node
						deltaX = offset * _templateFaces[i].getX();
						deltaY = offset * _templateFaces[i].getY();
						deltaZ = offset * _templateFaces[i].getZ();
						tiX = x + deltaX;
						tiY = y + deltaY;
						tiZ = z + deltaZ;

						foundExistingValue = false;
						foundIdx = 0;
						//Checking of NaN value
						if ((tiX < 0 || tiX >= _tiDimX) || (tiY < 0 || tiY >= _tiDimY) || (tiZ < 0 || tiZ >= _tiDimZ) || igisSIM::utility::is_nan(_TI[tiZ][tiY][tiX])) { //Out of bound or nan
							break; //Ignore border stop here
						} else {
							//Searching the TI cell value inside the current node
							for (unsigned int j=0; j<currentTreeNode->size(); j++) {
								if(_TI[tiZ][tiY][tiX] == currentTreeNode->operator[](j).value) { 
									//Existing value so increase the counter 
									foundExistingValue = true;
									currentTreeNode->operator[](j).counter = currentTreeNode->operator[](j).counter + 1;
									foundIdx = j;
									break;
								}
							}

							//If value is not found then add a new value in the node
							if (!foundExistingValue) {
								TreeNode aTreeNode;
								aTreeNode.counter = 1;
								aTreeNode.value = _TI[tiZ][tiY][tiX];
								aTreeNode.level = i;
								currentTreeNode->push_back(aTreeNode);
								foundIdx = (int)currentTreeNode->size() - 1;
							}
							//Switching the current node to the children
							currentTreeNode = &(currentTreeNode->operator[](foundIdx).children);
						} 
					}
				}
			}	
		}
		if (_debugMode > -1) {
		  std::cout << "Finish building search tree" << std::endl;
		}
		//std::cout << "Total nodes: " << nodeCnt << std::endl;
		//Check out dictionary
		//std::cout << "Dictionary info: " << std::endl;
		//std::cout << "Level: " << level << std::endl;
		////Showing the search tree for debugging
		//std::list<std::vector<TreeNode>*> nodesToCheck;
		//nodesToCheck.push_back(&_searchTree[level]); //Put the root node in the list node to be checked
		////Looping through all the node from top to bottom
		//while(nodesToCheck.size() > 0) {
		//	currentTreeNode = nodesToCheck.back();
		//	nodesToCheck.pop_back();
		//	//Showing the current node value and counter
		//	for (int i=0; i<currentTreeNode->size(); i++) {
		//		std::cout << currentTreeNode->operator[](i).level << " " << currentTreeNode->operator[](i).value << " " << currentTreeNode->operator[](i).counter << std::endl; 
		//		//Adding the children node to the list node to be checked
		//		nodesToCheck.push_front(&(currentTreeNode->operator[](i).children));
		//	}
		//	//std::cout << "list size: " << nodesToCheck.size() << std::endl;
		//}
	}	
}

/**
* @brief Start the simulation
* Virtual function implemented from MPSAlgorithm
*/
void igisSIM::SNESIMTree::startSimulation(void) {
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
float igisSIM::SNESIMTree::_simulate(const int& sgIdxX, const int& sgIdxY, const int& sgIdxZ, const int& level) {
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
		std::vector<float> aPartialTemplate;
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
					aPartialTemplate.push_back(_sg[sgZ][sgY][sgX]);
				} else { //NaN value
					aPartialTemplate.push_back(std::numeric_limits<float>::quiet_NaN());
				}
			} else aPartialTemplate.push_back(std::numeric_limits<float>::quiet_NaN());
		}

		//for (unsigned int i=0; i<aPartialTemplate.size(); i++) {
		//	std::cout << aPartialTemplate[i] << " " ;
		//}
		//std::cout << std::endl;

		//Going through the search tree and get the value of the current template
		std::vector<TreeNode>* currentTreeNode;
		std::list<std::vector<TreeNode>*> nodesToCheck;
		std::map<float, int> conditionalPoints;
		int sumCounters = 0;
		int currentLevel = 0, maxLevel = 0;

		//For all possible values of root tree
		for (unsigned int j=0; j<_searchTree[level].size(); j++) {
			conditionPointsUsedCnt = 0;
			maxLevel = 0;
			sumCounters = _searchTree[level][j].counter;
			nodesToCheck.clear();
			nodesToCheck.push_back(&_searchTree[level][j].children); //Initialize at children in first level
			//Looping through all the node from top to bottom
			while(nodesToCheck.size() > 0) {
				currentTreeNode = nodesToCheck.back();
				nodesToCheck.pop_back();
				//Showing the current node value and counter
				for (unsigned int i=0; i<currentTreeNode->size(); i++) {
					if (igisSIM::utility::is_nan(aPartialTemplate[currentTreeNode->operator[](i).level - 1])) {
						//If the template value is non defined then just go to children
						nodesToCheck.push_front(&(currentTreeNode->operator[](i).children));
					} else if (currentTreeNode->operator[](i).value == aPartialTemplate[currentTreeNode->operator[](i).level - 1]) {
						currentLevel = currentTreeNode->operator[](i).level;
						//Template found so go to higher level node
						if (currentLevel > maxLevel) { 	
							maxLevel = currentLevel;
							//Restart counter at only maximum level
							sumCounters = currentTreeNode->operator[](i).counter;
							conditionPointsUsedCnt ++;
						} else if (currentLevel == maxLevel) {
							//Adding the counter to the sum counters
							sumCounters += currentTreeNode->operator[](i).counter;
						}						

						//Only continue to the children node if the current node counter is big enough or if the number of conditional points used is smaller than a given limit 
						if(currentTreeNode->operator[](i).counter > _minNodeCount && (conditionPointsUsedCnt < _maxCondData || _maxCondData == -1)) {
							//Adding the children node to the list node to be checked
							nodesToCheck.push_front(&(currentTreeNode->operator[](i).children));
						}
					}
				}
			}
			//std::cout << _searchTree[level][j].value << " " << sumCounters << " " << maxLevel << std::endl;
			//finish searching for a value, now do the sum
			if (conditionPointsUsedCnt > maxConditionalPoints) {
				conditionalPoints.clear();
				conditionalPoints.insert ( std::pair<float, int>(_searchTree[level][j].value, sumCounters) );
				maxConditionalPoints = conditionPointsUsedCnt;
				//foundValue = _searchTree[level][j].value;
			} else if(conditionPointsUsedCnt == maxConditionalPoints && conditionPointsUsedCnt != 0) {
				conditionalPoints.insert ( std::pair<float, int>(_searchTree[level][j].value, sumCounters) );
			}
		}

		//Get the value from cpdf
		foundValue = _cpdf(conditionalPoints, sgIdxX, sgIdxY, sgIdxZ);
		//std::cout << std::endl;
	}
	return foundValue;
}

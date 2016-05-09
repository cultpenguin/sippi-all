//Copyright (c) 2015 I-GIS (www.i-gis.dk) and Solid Earth Geophysics, Niels Bohr Institute (http://imgp.nbi.ku.dk)
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <iomanip>      // std::setprecision
#include <cctype>       // isspace
#include <algorithm>    // std::previousMatchedCntuffle std::remove_if
#include <map>

#include "ENESIM.h"
#include "Utility.h"
#include "IO.h"
#include "Coords3D.h"

/**
* @brief Constructors
*/
MPS::ENESIM::ENESIM(void) : MPS::MPSAlgorithm(){
	// Multiple grids are not used in ENESIM type algorithms
	_totalGridsLevel=0;
	_nMaxCountCpdf=1;
}

/**
* @brief Destructors
*/
MPS::ENESIM::~ENESIM(void) {

}

/**
* @brief Read configuration file
* @param fileName configuration filename
*/
void MPS::ENESIM::_readConfigurations(const std::string& fileName) {
	std::ifstream file(fileName);
	std::string str;
	std::stringstream ss;
	std::string s;
	std::vector<std::string> data;
	// Process each line
	// Number of realizations
	_readLineConfiguration(file, ss, data, s, str);
	_realizationNumbers = stoi(data[1]);
	// Initial value
	_readLineConfiguration(file, ss, data, s, str);
	_seed = stof(data[1]);
	// Maximum number of counts for seeting up the conditional pdf
	_readLineConfiguration(file, ss, data, s, str);
	_nMaxCountCpdf =  stoi(data[1]);
	// Maximum neighbours
	_readLineConfiguration(file, ss, data, s, str);
	_maxNeighbours = stoi(data[1]);
	// Maximum iterations
	_readLineConfiguration(file, ss, data, s, str);
	_maxIterations = stoi(data[1]);
	// Simulation Grid size X
	_readLineConfiguration(file, ss, data, s, str);
	_sgDimX = stoi(data[1]);
	// Simulation Grid size Y
	_readLineConfiguration(file, ss, data, s, str);
	_sgDimY = stoi(data[1]);
	// Simulation Grid size Z
	_readLineConfiguration(file, ss, data, s, str);
	_sgDimZ = stoi(data[1]);
	// Simulation Grid World min X
	_readLineConfiguration(file, ss, data, s, str);
	_sgWorldMinX = stof(data[1]);
	// Simulation Grid World min Y
	_readLineConfiguration(file, ss, data, s, str);
	_sgWorldMinY = stof(data[1]);
	// Simulation Grid World min Z
	_readLineConfiguration(file, ss, data, s, str);
	_sgWorldMinZ = stof(data[1]);
	// Simulation Grid Cell Size X
	_readLineConfiguration(file, ss, data, s, str);
	_sgCellSizeX = stof(data[1]);
	// Simulation Grid Cell Size Y
	_readLineConfiguration(file, ss, data, s, str);
	_sgCellSizeY = stof(data[1]);
	// Simulation Grid Cell Size Z
	_readLineConfiguration(file, ss, data, s, str);
	_sgCellSizeZ = stof(data[1]);
	// TI filename
	_readLineConfiguration(file, ss, data, s, str);
	data[1].erase(std::remove_if(data[1].begin(), data[1].end(), [] (char x){return std::isspace(x); }), data[1].end()); //Removing spaces
	_tiFilename = data[1];
	// Output directory
	_readLineConfiguration(file, ss, data, s, str);
	data[1].erase(std::remove_if(data[1].begin(), data[1].end(), [] (char x){return std::isspace(x); }), data[1].end()); //Removing spaces
	_outputDirectory = data[1];
	// Shuffle SGPATH
	_readLineConfiguration(file, ss, data, s, str);
	_shuffleSgPath = (stoi(data[1]));
	// Shuffle Entropy Factor
	//_readLineConfiguration(file, ss, data, s, str);
	//_shuffleEntropyFactor = (stoi(data[1]));
	_shuffleEntropyFactor = 4;
	// Shuffle TI Path
	_readLineConfiguration(file, ss, data, s, str);
	_shuffleTiPath = (stoi(data[1]) != 0);
	// Hard data
	_readLineConfiguration(file, ss, data, s, str);
	data[1].erase(std::remove_if(data[1].begin(), data[1].end(), [] (char x){return std::isspace(x); }), data[1].end()); //Removing spaces
	_hardDataFileNames = data[1];
	// Hard data search radius
	_readLineConfiguration(file, ss, data, s, str);
	_hdSearchRadius = stof(data[1]);
	// Softdata categories
	if(_readLineConfiguration(file, ss, data, s, str)) {
		// std::cout << data.size() << " " << data.empty() << std::endl;
		ss.clear();
		ss << data[1];
		while (getline(ss, s, ';')) {
			//Parsing categories
			if(!s.empty()) {
				_softDataCategories.push_back(stof(s));
			}
		}
	}
	// Softdata filenames
	if(_readLineConfiguration(file, ss, data, s, str)) {
		ss.clear();
		ss << data[1];
		while (getline(ss, s, ';')) {
			//Parsing categories
			if(!s.empty()) {
				s.erase(std::remove_if(s.begin(), s.end(), [] (char x){return std::isspace(x); }), s.end()); //Removing spaces
				_softDataFileNames.push_back(s);
			}
		}
		// Resize softdata grids
		_softDataGrids.resize(_softDataFileNames.size());
	}
	// Number of threads
	_readLineConfiguration(file, ss, data, s, str);
	_numberOfThreads = stoi(data[1]);
	// DEBUG MODE
	_readLineConfiguration(file, ss, data, s, str);
	_debugMode = stoi(data[1]);
}

/**
* @brief Compute cpdf from training image using ENESIM type
* @param x coordinate X of the current node
* @param y coordinate Y of the current node
* @param z coordinate Z of the current node
* @param map with conditional pdf
* @return true if found a value
*/

bool MPS::ENESIM::_getCpdfTiEnesim(const int& sgIdxX, const int& sgIdxY, const int& sgIdxZ, std::map<float, float>& cPdf) {
	int maxCpdfCount = _nMaxCountCpdf;

	// map containing the count of conditional data values
	std::map<float, int> conditionalCount;

	int CpdfCount = 0;
	float valueFromTI;
	float LC_dist_min = RAND_MAX;
	float LC_dist_threshold=1;
	int TI_idxX, TI_idxY, TI_idxZ;



	//Do simulation only for NaN value

	//Seaching for neighbours to get vector V and L
	//Doing a circular seach ATM ...

	std::vector<MPS::Coords3D> L;
	std::vector<float> V;
	_circularSearch(sgIdxX, sgIdxY, sgIdxZ, _sg, _maxNeighbours, -1, L, V);


	// The training image path is shifted such that a random start location is chosen
	int ti_shift;
	ti_shift = (std::rand() % (int)(_tiDimX*_tiDimY*_tiDimZ));
	std::rotate(_tiPath.begin(), _tiPath.begin() + ti_shift, _tiPath.end());

	// At this point V represents the conditional value(s), and L realtive position(s) oF of the contitional values(s).
	// In the folloing the training image should be scanned for all matches of V,L
	// to find the local conditional pdf f(m_i ~ m_c)

	// Loop over training image to find a close match to L,V

	// Set number of count in Cpdf to zero
	CpdfCount = 0;
	float V_ti; // template Value in TI
	V_ti=-1;
	float V_center_ti; // value of the central node in the TI
	V_center_ti=-1;


	float L_dist;  // sum of realtive distance
	float LC_dist; // distance of L,V to value in TI
	for (unsigned int i_ti_path=0; i_ti_path<_tiPath.size(); i_ti_path++) {
		MPS::utility::oneDTo3D(_tiPath[i_ti_path], _tiDimX, _tiDimY, TI_idxX, TI_idxY, TI_idxZ);

		if (_debugMode > 3) {
			std::cout << "  " << "i_ti_path=" << i_ti_path<< " ===";
			std::cout << "  POSITION IN TI: TI_idxX,TI_idxY,TI_idxZ=";
			std::cout << "  " << TI_idxX;
			std::cout << "  " << TI_idxY;
			std::cout << "  " << TI_idxZ << std::endl;
		}

		V_center_ti = _TI[TI_idxZ][TI_idxY][TI_idxX];

		// FIND OUT IF WE HAVE A MATCH AT THE CURRENT LOCATION!
		int TI_x, TI_y, TI_z;
		int matchedCnt = 0; //, previousMatchedCnt = 0;
		matchedCnt = 0;

		LC_dist=0;
		for (unsigned int i=0; i<L.size(); i++) {
			//For each pixel relatively to the current pixel based on vector L
			TI_x = TI_idxX + L[i].getX();
			TI_y = TI_idxY + L[i].getY();
			TI_z = TI_idxZ + L[i].getZ();
			L_dist = abs(L[i].getX()) + abs(L[i].getY()) + abs(L[i].getZ());

			// CHECK IF WE ARE INSIDE BOUNDS!!
			if((TI_x >= 0 && TI_x < _tiDimX) && (TI_y >= 0 && TI_y < _tiDimY) && (TI_z >= 0 && TI_z < _tiDimZ)) {
				V_ti = _TI[TI_z][TI_y][TI_x];

				if (V_ti!=V[i]) {
					// Unless we have a perfect match, a penalty distance of 1 is added.
					LC_dist=LC_dist+1;
				}
			} else {
				// ARE OUT OF BOUNDS
				LC_dist = LC_dist+1;
			}
		}

		// WHAT IS THE VALUE OF AT THE CENTER NOTE IN THE TT RIGHT NOW?

		// Check if current L,T in TI match conditional observations better
		if (LC_dist<LC_dist_min) {
			// We have a new MIN distance
			LC_dist_min=LC_dist;
			valueFromTI = V_center_ti;

		}

		// Add a count to the Cpdf if the current node match L,V according to some threshold..
		if (LC_dist<LC_dist_threshold) {
			CpdfCount++;

			// Update conditionalCount Counter (from which the local cPdf can be computed)
			if (conditionalCount.find(V_center_ti) == conditionalCount.end()) {
				// Then we've encountered the word for a first time.
				// Is this slow?
				conditionalCount[V_center_ti] = 1; // Initialize it to	 1.
			} else {
				// Then we've already seen it before..
				conditionalCount[V_center_ti]++; // Just increment it.
			}

			if (_debugMode > 2) {
				std::cout << "Perfect Match  i_ti_path=" << i_ti_path << ", V_center_ti=" << V_center_ti << std::endl;
			}
		}

		if (maxCpdfCount <= CpdfCount ) {
			if (_debugMode > 1) {
				std::cout << "MaxCpdfCount Reached i_ti_path=" << i_ti_path << " - CpdfCount=" << CpdfCount << std::endl;
			}
			break;
		}


		// Stop looking if we have reached the maximum number of allowed iterations in TI
		if (i_ti_path>_maxIterations) {
			if (_debugMode > 1) {
				std::cout << "Max Ite Reached i_ti_path=" << i_ti_path << " - nmax_ite=" << _maxIterations;
				std::cout << "LC_dist=" << LC_dist << std::endl;
			}
			break;
		}

		if (_debugMode > 3) {
			int a;
			std::cin >> a;
		}


	} // END SCAN OF TI FOR CPDF


	// assign minimum distance tp temporary grid 1
	if (_debugMode>1) {
		_tg1[sgIdxZ][sgIdxY][sgIdxX] = LC_dist_min;
		_tg2[sgIdxZ][sgIdxY][sgIdxX] = CpdfCount;
	}


	// CHECK THAT conditionalCount HAS AT LEAST ONE COUNT
	// This may not always be the case when no conditional event has been found!
	if (CpdfCount==0 ) {
		// NO MATCHES HAS BEEN FOUND. ADD THE CURRENT BEST MACTH TO THE conditionalCount
		// std::cout << " CpdfCount " << CpdfCount << std::endl;
		if (conditionalCount.find(valueFromTI) == conditionalCount.end()) {
			// Then we've encountered the word for a first time. Is this slow?
			conditionalCount[valueFromTI] = 1; // Initialize it to	 1.
		} else {
			// Then we've already seen it before..
			conditionalCount[valueFromTI]++; // Just increment it.
		}
	}

	// COMPUTE ConditionalPdf from conditionalCount
	//Loop through all the conditional points to calculate the pdf (probability distribution function)
	//Getting the sum of counter and sort the map using the counter
	float totalCounter = 0;
	for(std::map<float,int>::iterator iter = conditionalCount.begin(); iter != conditionalCount.end(); ++iter) {
		totalCounter += iter->second;
	}

	//std::map<float, float> conditionalPdfFromTi;
	//Looping again in conditionalCount, compute and add the probabilities from TI
	for(std::map<float, int>::iterator iter = conditionalCount.begin(); iter != conditionalCount.end(); ++iter) {
		cPdf.insert(std::pair<float, float>(iter->first, (float)(iter->second) / (float)totalCounter));
	}
	// Now the conditional PDF from the TI is available as conditionalPdfFromTi;

	return true;
}

/**
* @brief Compbine two PDFs assuming independence
* @param probability density 1, pdf1
* @param probability density 2, pdf2
* @return true if succesfull, and update pdf is available in pdf1
*/
bool MPS::ENESIM::_combinePdf(std::map<float, float>& cPdf, std::map<float, float>& softPdf) {

	float key;
	float val_softpdf;
	float val_cpdf;
	float sum_cpdf;
	sum_cpdf=0;
	for(std::map<float,float>::iterator iter = softPdf.begin(); iter != softPdf.end(); ++iter) {
		key=iter->first;
		val_softpdf=softPdf[key];

		if (cPdf.count(key)) {
				// the key is set in both the soft and coniditional pdf.
				val_cpdf=cPdf[key];
				//std::cout << "  key=" << key << " P SO="<< val_softpdf << std::endl;

		} else {
			// the key is not set for the conditional pdf
				val_cpdf=0.00001;
		}
		//std::cout << "sum pdf = " << sum_cpdf << std::endl;
		//std::cout << "CPDF_TI   INDEX=" << iter->first << "  COUNT =" << iter->second << " ->";

		val_cpdf = val_cpdf*val_softpdf;
		sum_cpdf = sum_cpdf + val_cpdf;

		// Update the coniditonal pdf assuming independence of soft data and conditional pdf from TI
		cPdf[key]=val_cpdf;

	}

	// normalize combined pdf
	for(std::map<float,float>::iterator iter = cPdf.begin(); iter != cPdf.end(); ++iter) {
		cPdf[iter->first] = cPdf[iter->first] / sum_cpdf;
		// std::cout << "key=" << iter->first << " P TI="<< conditionalPdfFromTi[iter->first] << std::endl;
	}

	return true;
}

/**
* @brief Compute cpdf from softdata and set the value to the current node
* @param sgIdxX coordinate X of the current node
* @param sgIdxY coordinate Y of the current node
* @param sgIdxZ coordinate Z of the current node
* @param iterationCnt Iterations counter
* @return simulated value
*/
float MPS::ENESIM::_getRealizationFromCpdfTiEnesim(const int& sgIdxX, const int& sgIdxY, const int& sgIdxZ, float& iterationCnt) {

	// Get cPdf from training image using ENESIM style
	std::map<float, float> conditionalPdfFromTi;
	_getCpdfTiEnesim(sgIdxX, sgIdxY,sgIdxZ, conditionalPdfFromTi);

	// Check if any SoftData are available?
	//std::multimap<float, float> softPdf;
	std::map<float, float> softPdf;
	MPS::Coords3D closestCoords;
	if (_getCpdfFromSoftData(sgIdxX, sgIdxY, sgIdxZ, 0, softPdf, closestCoords)) {
		  // Combine with conditional pdf from TI
			_combinePdf(conditionalPdfFromTi,softPdf);
	}

	// sample from conditional pdf
	float simulatedValue;
	simulatedValue = _sampleFromPdf(conditionalPdfFromTi);

	// DONE SIMULATING
	if (_debugMode > 2) {
		// std::cout << "Simulated value="<< simulatedValue << ", LC_dist_min=" << LC_dist_min << " " << std::endl ;
		std::cout << "Simulated value="<< simulatedValue << " " << std::endl ;
	}
	return simulatedValue;
}

/**
* @brief get realization from Cpdf and Soft data using a metropolis sampler when soft data is available
* @param sgIdxX coordinate X of the current node
* @param sgIdxY coordinate Y of the current node
* @param sgIdxZ coordinate Z of the current node
* @param iterationCnt Iterations counter
* @return simulated value
*/
float MPS::ENESIM::_getRealizationFromCpdfTiEnesimMetropolis(const int& sgIdxX, const int& sgIdxY, const int& sgIdxZ, float& iterationCnt) {

	std::map<float, float> conditionalPdfFromTi;
	// Check if any SoftData are available?
	//std::multimap<float, float> softPdf;
	float simulatedValue;
	float randomValue	;
	float pAcc;

	std::map<float, float> softPdf;
	int i=0;;
	int maxIterations = 100;  // decide whether soft or ti cpdf takes preference..
	bool isAccepted = false;
	MPS::Coords3D closestCoords;
	if (_getCpdfFromSoftData(sgIdxX, sgIdxY, sgIdxZ, 0, softPdf, closestCoords)) {

		do {
			// MAKE SURE THAT conditionalPdfFromTi IS OBTAINED INDEPENDENTLTY EACH TIME!!!
			// ESPECIELLY WHEN BASED ON ONLY ONE COUNT

			conditionalPdfFromTi.clear();
			_getCpdfTiEnesim(sgIdxX, sgIdxY,sgIdxZ, conditionalPdfFromTi);
			simulatedValue = _sampleFromPdf(conditionalPdfFromTi);

			randomValue = ((float) rand() / (RAND_MAX));
			pAcc = softPdf[simulatedValue];

			if(_debugMode > 2 ) {
				std::cout << "i= " << i << " maxIterations=" << maxIterations;
				std::cout << "   p_ti = [" <<  conditionalPdfFromTi[0] << "," << conditionalPdfFromTi[1] << "]";
				std::cout << " simval = " << simulatedValue;
				std::cout << "   p_soft = [" <<  softPdf[0] << "," << softPdf[1] << "]";
				std::cout << " - pAcc = " << pAcc << std::endl;
			}

			// accept simulatedValue with probabilty from soft data
			if (randomValue<pAcc) {
					isAccepted=true;
			}
			i++;
		} while ((i<maxIterations)&(!isAccepted));
		if(_debugMode > 2 ) {
			std::cout << " simval = " << simulatedValue << " - randomValue = " << randomValue << std::endl;
			std::cout << "________________________________" << std::endl;
		}


	} else {
		// obtain conditional and generate a realization wihtout soft data
		_getCpdfTiEnesim(sgIdxX, sgIdxY,sgIdxZ, conditionalPdfFromTi);

		// sample from conditional pdf
		simulatedValue = _sampleFromPdf(conditionalPdfFromTi);
  }



	// DONE SIMULATING
	if (_debugMode > 2) {
		// std::cout << "Simulated value="<< simulatedValue << ", LC_dist_min=" << LC_dist_min << " " << std::endl ;
		std::cout << "Simulated value="<< simulatedValue << " " << std::endl ;
	}
	return simulatedValue;
}

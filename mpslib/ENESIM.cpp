// (c) 2015-2016 I-GIS (www.i-gis.dk) and Solid Earth Geophysics, Niels Bohr Institute (http://imgp.nbi.ku.dk)
//
//    This file is part of MPSlib.
//
//    MPSlib is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    MPSlib is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with MPSlib (COPYING.LESSER).  If not, see <http://www.gnu.org/licenses/>.
//
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

	// Check that parameter file exist
	if  (!(MPS::io::file_exist(fileName))) {
		std::cout << "Paremeter file '" << fileName << "' does not exist -> quitting" << std::endl;
		exit(0);
		return;
	}


	std::ifstream file(fileName);
	std::string str;
	std::stringstream ss;
	std::string s;
	std::vector<std::string> data;
	int read_int; // dummy integer

	// Process each line
	// Number of realizations
	_readLineConfiguration(file, ss, data, s, str);
	_realizationNumbers = stoi(data[1]);
	// Initial value
	_readLineConfiguration(file, ss, data, s, str);
	_seed = stof(data[1]);
	// Maximum number of counts for seeting up the conditional pdf
	_readLineConfiguration(file, ss, data, s, str);
	read_int  =  stoi(data[1]);
	if (read_int<0) {
		_nMaxCountCpdf = std::numeric_limits<int>::max();
	} else {
		_nMaxCountCpdf=read_int;
	}

	// Maximum neighbours
	_readLineConfiguration(file, ss, data, s, str);
	_maxNeighbours = stoi(data[1]);
	// Maximum iterations
	_readLineConfiguration(file, ss, data, s, str);
	read_int  =  stoi(data[1]);
	if (read_int<0) {
		_maxIterations = std::numeric_limits<int>::max();
	} else {
		_maxIterations = read_int;
	}
	// Distance Measure and minimum distance
	_readLineConfiguration_mul(file, ss, data, s, str);
	_distance_measure = stoi(data[1]);
	// optional. Template size x - base when n_mulgrid=0
	if (data.size()>2) {
		_distance_threshold = stof(data[2]);
	} else {
		_distance_threshold = 0;
	}
	if (data.size()>3) {
		_distance_power_order = stof(data[3]);
	} else {
		_distance_power_order = 0;
	}
// Maximum search Radius
	_readLineConfiguration(file, ss, data, s, str);
	_maxSearchRadius = stof(data[1]);
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
	_readLineConfiguration_mul(file, ss, data, s, str);
	_shuffleSgPath = (stoi(data[1]));
	if (data.size()>2) {
		// EntropyFactor is the second value, if it exists
		_shuffleEntropyFactor = stof(data[2]);
	} else {
		_shuffleEntropyFactor = 4;
	}
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


	//float h_dist; // distance to conditional event
	//float h_dist_cum; // cumulative distance to conditional event
	//float h_power=0; // distance weight
	int CpdfCount = 0;
	float valueFromTI;
	float LC_dist_min;
	float LC_dist_current;
	float TI_x_min, TI_y_min, TI_z_min;
	int TI_idxX, TI_idxY, TI_idxZ;



	//Do simulation only for NaN value

	//Seaching for neighbours to get vector V and L
	//Doing a circular seach ATM ...

	std::vector<MPS::Coords3D> L;
	std::vector<float> V;
	//_circularSearch(sgIdxX, sgIdxY, sgIdxZ, _sg, _maxNeighbours, -1, L, V);
	_circularSearch(sgIdxX, sgIdxY, sgIdxZ, _sg, _maxNeighbours, _maxSearchRadius, L, V);
		/**
	* @brief maximum number of counts setting up conditional pdf
	*/

	

	if (_debugMode>2) {
		std::cout << "_getCpdfTiEnesim: -- ENESIM TOP --"<< std::endl;
		std::cout << "SGxyx=(" << sgIdxX << "," << sgIdxY <<  "," << sgIdxZ <<")" << std::endl;
		std::cout << "nLxyz=" << L.size() << "(of max "<< _maxNeighbours << ")" << std::endl;
		std::cout << "circularSearch: _maxNeighbours=" << _maxNeighbours << " _maxSearchRadius=" << _maxSearchRadius << std::endl;

	}

	// Compute realtive distance for each conditional data
	std::vector<float> L_dist(L.size());
	for (unsigned int i=0; i<L.size(); i++) {

		if (_debugMode>3) {
			std::cout << "  Lxyx=("<<L[i].getX() << ","<<L[i].getY() <<  ","<<L[i].getZ() << ")"<< " V="<<V[i]<< std::endl;
		}
		if (_distance_power_order!=0) {
			//h_dist = sqrt(L[i].getX()*L[i].getX() + L[i].getY()*L[i].getY() + L[i].getZ()*L[i].getZ());
			L_dist[i] = sqrt(L[i].getX()*L[i].getX() + L[i].getY()*L[i].getY() + L[i].getZ()*L[i].getZ());
			L_dist[i]  = pow(L_dist[i],-1*_distance_power_order) ;

		} else {
			L_dist[i]=1;
		}
	}
 	//for(float n : L_dist) {
//        std::cout << n << '\n';
//			}

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
	//float V_ti; // template Value in TI
	//V_ti=-1;
	float V_center_ti; // value of the central node in the TI
	V_center_ti=-1;

 	int node1DIdx; // Integer to hold 1d index from 3D point

  LC_dist_min = 1e+9;
	//float L_dist;  // sum of realtive distance
	float LC_dist; // distance of L,V to value in TI
	unsigned int i_ti_path;
	for (i_ti_path=0; i_ti_path<_tiPath.size(); i_ti_path++) {
		MPS::utility::oneDTo3D(_tiPath[i_ti_path], _tiDimX, _tiDimY, TI_idxX, TI_idxY, TI_idxZ);


		// Get the centered value in the TI
		V_center_ti = _TI[TI_idxZ][TI_idxY][TI_idxX];

		// Get the distance between the conditional data in TI and SIM grid
		LC_dist = _computeDistanceLV_TI(L, V, TI_idxX, TI_idxY, TI_idxZ, L_dist);
		//LC_dist = _computeDistanceLV_TI(L, V, TI_idxX, TI_idxY, TI_idxZ);


		// Check if current L,T in TI match conditional observations better
		if (LC_dist<LC_dist_min) {
			LC_dist_min=LC_dist;
			//std::cout << "  -->" << LC_dist_min << " " << LC_dist << std::endl;
			// We have a new MIN distance
			valueFromTI = V_center_ti;

			LC_dist_current = LC_dist;
			// keep track of the locaton in TI used for match
			TI_x_min = TI_idxX;
			TI_y_min = TI_idxY;
			TI_z_min = TI_idxZ;

			// Add a count to the Cpdf if the current node match L,V according to some threshold..
			if (LC_dist<=_distance_threshold) {
				CpdfCount++;
				// Update conditionalCount Counter (from which the local cPdf can be computed)
				if (conditionalCount.find(valueFromTI) == conditionalCount.end()) {
					// Then we've encountered the word for a first time.
					// Is this slow?
					conditionalCount[valueFromTI] = 1; // Initialize it to	 1.
				} else {
					// Then we've already seen it before..
					conditionalCount[valueFromTI]++; // Just increment it.
				}
				if (_debugMode > 2) {
					std::cout << "Matching event  i_ti_path=" << i_ti_path << ", V_center_ti=" << valueFromTI << std::endl;
				}
				// MAKE SURE TO RESET LC_dist_min
				LC_dist_min = 1e+9;
			
			
				// WE HAVE A MATCH. 
				// IF COLOCATED
				//    FIND ANY COLOCATED SOFT DATA -> THE DEFAULT AND FASTEST
				// IF NON COLOCATED 
				//    LOOK FOR FOR THE N_c closest NON_COLOCATED DATA
				// NOW, OPTIONALLY LOOK FOR THE SOFT CONDITIONAL DATA, AND COMPUTE THE PROBABILITY OF THE SOFT CONDITIONAL DATA, CONDITINOAL TO THE FOUND MATCH IN THE TI
				// FOR EACH MATCH THIS SHOULD RESULT IN PROBABAILITTY : PROD((f_soft(m_i) == m_i^*)) / f_max
				// THIS probability SHOULD BE RETUREN AS WELL AS THE 
				// OUTPUT SHOULD BE AN ARRAY OR PROBABILITIS, ONE FOR EACH FOUND MATCH OF THE HARD DATA!
				// Then, one can decide elsewhere how to combine these
			
				// 1. FIND CONDITIONAL SOFT DATA (OPTINALLY ONLY 1 COLOCATED SOFT DATA!!)
				// 2. LOOP THROUGH CONDITIONAL SOFT DATA AND COMPUTE THE NORMALIZAED PROBABILITY OF EACH ONE SOFT DATA SET
				//    MULTIPLY THIS TO THE
			

			}

		}


		if (_debugMode > 3) {
			std::cout << "  " << "i_ti_path=" << i_ti_path<< " ===";
			std::cout << "  TI	xyz=(" << TI_idxX << "," << TI_idxY << "," << TI_idxZ << ")";
			std::cout << " - LC_dist=" << LC_dist << " min(LC_dist)=" << LC_dist_current;
			std::cout << std::endl;
		}




		if (maxCpdfCount <= CpdfCount ) {
			if (_debugMode > 2) {
				std::cout << "MaxCpdfCount Reached i_ti_path=" << i_ti_path << " - CpdfCount=" << CpdfCount << std::endl;
			}
			break;
		}


		// Stop looking if we have reached the maximum number of allowed iterations in TI
		if (i_ti_path>_maxIterations) {
			if (_debugMode > 1) {
				std::cout << "Max Ite Reached i_ti_path=" << i_ti_path << " - nmax_ite=" << _maxIterations << std::endl;
			}
			break;
		}



	} // END SCAN OF TI FOR CPDF
  //std::cout << sgIdxX << "," << sgIdxY << "  :h_dist_cum = " << h_dist_cum << "  LC_dist = "<< LC_dist << std::endl;

	



	// assign minimum distance to temporary grid 1
	if (_debugMode>1) {
		_tg1[sgIdxZ][sgIdxY][sgIdxX] = LC_dist_current;
		_tg2[sgIdxZ][sgIdxY][sgIdxX] = CpdfCount;
		MPS::utility::treeDto1D(TI_x_min, TI_y_min, TI_z_min, _tiDimX, _tiDimY, node1DIdx);
		_tg3[sgIdxZ][sgIdxY][sgIdxX] = node1DIdx;
		//_tg4[sgIdxZ][sgIdxY][sgIdxX] = _tiPath[i_ti_path]; // POSITION ON TI
		_tg4[sgIdxZ][sgIdxY][sgIdxX] = i_ti_path;
		_tg5[sgIdxZ][sgIdxY][sgIdxX] = L.size(); // number of used conditional points
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
	// std::cout << "totalCounter=" << totalCounter << std::endl;

	//std::map<float, float> conditionalPdfFromTi;
	//Looping again in conditionalCount, compute and add the probabilities from TI
	for(std::map<float, int>::iterator iter = conditionalCount.begin(); iter != conditionalCount.end(); ++iter) {
		cPdf.insert(std::pair<float, float>(iter->first, (float)(iter->second) / (float)totalCounter));
	}
	// Now the conditional PDF from the TI is available as conditionalPdfFromTi;


	if (_debugMode>2) {
		std::cout << "NsimInTI=" << i_ti_path;
		std::cout << " --CpdfCount = " << CpdfCount;
		std::cout << " valueFromTI = " << valueFromTI;
		std::cout << " LC_dist_current = " << LC_dist_current;

		std::cout << "SGxyx=(" << sgIdxX << "," << sgIdxY <<  "," << sgIdxZ <<")" << std::endl;
		std::cout << "_getCpdfTiEnesim: -- ENESIM END --"<< std::endl;

	}



	return true;
}


/**
* @brief Compute distance between conditional data in TI and template L
* @param TIi_dxX coordinate X of the current node in TI
* @param TIi_dxY coordinate Y of the current node in TI
* @param TIi_dxZ coordinate Z of the current node in TI
* @param L_dist Precompute Distance!
* @return distance
*/
float MPS::ENESIM::_computeDistanceLV_TI(std::vector<MPS::Coords3D>& L, std::vector<float>& V, const int& TI_idxX, const int& TI_idxY, const int& TI_idxZ, std::vector<float>& L_dist) {

	float h_dist=0;
	float h_dist_cum=0;
	float LC_dist;
	float V_ti;
	int TI_x, TI_y, TI_z;

	LC_dist=0;
	//h_dist_cum=0;

	if (L.size()>0) {
		for (unsigned int i=0; i<L.size(); i++) {
			//For each pixel relatively to the current pixel based on vector L
			TI_x = TI_idxX + L[i].getX();
			TI_y = TI_idxY + L[i].getY();
			TI_z = TI_idxZ + L[i].getZ();

		  h_dist = L_dist[i];
			h_dist_cum = h_dist_cum + h_dist;

			// Check wgether the current conditional point
			// is located within the training image limits
			if((TI_x >= 0 && TI_x < _tiDimX) && (TI_y >= 0 && TI_y < _tiDimY) && (TI_z >= 0 && TI_z < _tiDimZ)) {
				V_ti = _TI[TI_z][TI_y][TI_x];

				if (_distance_measure==1) {
					// Discrete measure: no matching pixel means added distance of 1
					if (V_ti!=V[i]) {
						// add a distance of 1, if case of no matching pixels
						LC_dist=LC_dist+1*h_dist;
					}
				}	else if (_distance_measure==2){
					LC_dist = LC_dist + ((V_ti-V[i])*(V_ti-V[i]))/(256*256)*h_dist;
				}
			} else {
				// The conditioning location of the point to compare to is located outside
				// the traning image, and will be treated not a match
				if (_distance_measure==1) {
					LC_dist = LC_dist+1*h_dist;
				} else if (_distance_measure==2) {
					LC_dist = 1e+9*h_dist;
				}
			}
		}
		if (_distance_measure==1) {
			// Normaize LC_dist
			//LC_dist = LC_dist / h_dist_cum;
		} else if (_distance_measure==2) {
			LC_dist = LC_dist / L.size();
		}
	} else {
		// No conditional points --> distance zero
		LC_dist=0;
	}

	return LC_dist;
}

/**
* @brief Compute distance between conditional data in TI and template L.
* @brief Much slower than using Precomputed distance
* @param TIi_dxX coordinate X of the current node in TI
* @param TIi_dxY coordinate Y of the current node in TI
* @param TIi_dxZ coordinate Z of the current node in TI
* @return distance
*/
float MPS::ENESIM::_computeDistanceLV_TI(std::vector<MPS::Coords3D>& L, std::vector<float>& V, const int& TI_idxX, const int& TI_idxY, const int& TI_idxZ) {

	float h_dist;
	float h_dist_cum;
	float LC_dist;
	float V_ti;
	int TI_x, TI_y, TI_z;

	LC_dist=0;
	h_dist_cum=0;

	if (L.size()>0) {
		for (unsigned int i=0; i<L.size(); i++) {
			//For each pixel relatively to the current pixel based on vector L
			TI_x = TI_idxX + L[i].getX();
			TI_y = TI_idxY + L[i].getY();
			TI_z = TI_idxZ + L[i].getZ();
			if (_distance_power_order!=0) {
				h_dist = sqrt(L[i].getX()*L[i].getX() + L[i].getY()*L[i].getY() + L[i].getZ()*L[i].getZ());
				h_dist = pow(h_dist,-1*_distance_power_order) ;
			} else {
				h_dist=1;
			}
			h_dist_cum = h_dist_cum + h_dist;

			// Check wgether the current conditional point
			// is located within the training image limits
			if((TI_x >= 0 && TI_x < _tiDimX) && (TI_y >= 0 && TI_y < _tiDimY) && (TI_z >= 0 && TI_z < _tiDimZ)) {
				V_ti = _TI[TI_z][TI_y][TI_x];

				if (_distance_measure==1) {
					// Discrete measure: no matching picel means added distance of 1
					if (V_ti!=V[i]) {
						// add a distance of 1, if case of no matching pixels
						LC_dist=LC_dist+1*h_dist;
					}
				}	else if (_distance_measure==2){
					LC_dist = LC_dist + ((V_ti-V[i])*(V_ti-V[i]))*h_dist;
				}
			} else {
				// The conditioning location of the point to compare to is located outside
				// the traning image, and will be treated not a match
				if (_distance_measure==1) {
					LC_dist = LC_dist+1*h_dist;
				} else if (_distance_measure==2) {
					LC_dist = 1000000*h_dist;
				}
			}
		}
		if (_distance_measure==1) {
			// Normaize LC_dist
			LC_dist = LC_dist / h_dist_cum;
		}
	} else {
		// No conditional points --> distance zero
		LC_dist=0;
	}

	return LC_dist;
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
float MPS::ENESIM::_getRealizationFromCpdfTiEnesimRejection(const int& sgIdxX, const int& sgIdxY, const int& sgIdxZ, float& iterationCnt) {

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



/**
* @brief get realization from Cpdf and Soft data using a metropolis sampler when soft data is available
* @param sgIdxX coordinate X of the current node
* @param sgIdxY coordinate Y of the current node
* @param sgIdxZ coordinate Z of the current node
* @param iterationCnt Iterations counter
* @return simulated value
*/
float MPS::ENESIM::_getRealizationFromCpdfTiEnesimRejectionNonCo(const int& sgIdxX, const int& sgIdxY, const int& sgIdxZ, float& iterationCnt) {


	std::map<float, float> conditionalPdfFromTi;
	// Check if any SoftData are available?
	//std::multimap<float, float> softPdf;
	float simulatedValue;
	float randomValue;
	float pAcc;

	std::map<float, float> softPdf;
	int i = 0;;
	int maxIterations = 100;  // decide whether soft or ti cpdf takes preference..
	bool isAccepted = false;


	//
	//  START: NONCO
	//
	std::vector<MPS::Coords3D> L;
	std::vector<float> V;

	//int maxSearchRadius_soft = _maxSearchRadius;
	//int maxNeighbours_soft = _maxNeighbours;
	int maxSearchRadius_soft = 10;
	int maxNeighbours_soft = 2;
	int sdX, sdY, sdZ;

	std::cout << "At _sg location [" << sgIdxX << "," << sgIdxY << "," << sgIdxZ << "]" << std::endl;


	if (!_softDataGrids.empty()) {
		L.clear();
		_circularSearch(sgIdxX, sgIdxY, sgIdxZ, _softDataGrids[0], maxNeighbours_soft, maxSearchRadius_soft, L, V);
		
		if (L.size() > 0) {
			std::cout << " relpos=(" << L[i].getX() << "," << L[i].getY() << "," << L[i].getY() << ")  - ";
			std::cout << "Found L.size()=" << L.size() << "conditional data using _maxNeighbours=" << maxNeighbours_soft << "  _maxSearchRadius=" << maxSearchRadius_soft << std::endl;
			for (unsigned int i = 0; i < L.size(); i++) {

				// find coordinate and value of conditional soft data

				sdX = sgIdxX + L[i].getX();
				sdY = sgIdxY + L[i].getY();
				sdZ = sgIdxZ + L[i].getZ();

				if (sdX >= 0 && sdX < _sgDimX && sdY >= 0 && sdY < _sgDimY && sdZ >= 0 && sdZ < _sgDimZ) {
					// get value at soft position in TI
					//float sdValTI = _sg[sdZ][sdY][sdX];
					std::cout << "Soft pos in TI=(" << sdX << "," << sdY << "," << sdZ << ")";
					//std::cout << " V=" << sdValTI;
					std::cout << " relpos=(" << L[i].getX() << "," << L[i].getY() << "," << L[i].getY() << ")";
					std::cout << " center=(" << sgIdxX << "," << sgIdxY << "," << sgIdxZ << ")" << std::endl;

				}

			}
		}
		


	}

	

	//
	// END: NONCO
	//

	i = 0;
	MPS::Coords3D closestCoords;
	if (_getCpdfFromSoftData(sgIdxX, sgIdxY, sgIdxZ, 0, softPdf, closestCoords)) {

		do {
			// MAKE SURE THAT conditionalPdfFromTi IS OBTAINED INDEPENDENTLTY EACH TIME!!!
			// ESPECIELLY WHEN BASED ON ONLY ONE COUNT

			conditionalPdfFromTi.clear();
			_getCpdfTiEnesim(sgIdxX, sgIdxY, sgIdxZ, conditionalPdfFromTi);
			simulatedValue = _sampleFromPdf(conditionalPdfFromTi);

			randomValue = ((float)rand() / (RAND_MAX));
			pAcc = softPdf[simulatedValue];

			if (_debugMode > -2) {
				std::cout << "i= " << i << " maxIterations=" << maxIterations;
				std::cout << "   p_ti = [" << conditionalPdfFromTi[0] << "," << conditionalPdfFromTi[1] << "]";
				std::cout << " simval = " << simulatedValue;
				std::cout << "   p_soft = [" << softPdf[0] << "," << softPdf[1] << "]";
				std::cout << " - r=" << randomValue << "pAcc = " << pAcc << std::endl;
			}

			// accept simulatedValue with probabilty from soft data
			if (randomValue<pAcc) {
				isAccepted = true;
			}
			i++;
		} while ((i<maxIterations)&(!isAccepted));
		if (_debugMode > 2) {
			std::cout << " simval = " << simulatedValue << " - randomValue = " << randomValue << std::endl;
			std::cout << "________________________________" << std::endl;
		}


	}
	else {
		// obtain conditional and generate a realization wihtout soft data
		_getCpdfTiEnesim(sgIdxX, sgIdxY, sgIdxZ, conditionalPdfFromTi);

		// sample from conditional pdf
		simulatedValue = _sampleFromPdf(conditionalPdfFromTi);
	}



	// DONE SIMULATING
	if (_debugMode > 2) {
		// std::cout << "Simulated value="<< simulatedValue << ", LC_dist_min=" << LC_dist_min << " " << std::endl ;
		std::cout << "Simulated value=" << simulatedValue << " " << std::endl;
	}
	return simulatedValue;
}



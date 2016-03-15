#include <iostream>
#include "ENESIM_GENERAL.h"

int main(int argc, char* argv[]) {

  std::string parameterFile;

  if (argc>1) {
    parameterFile = argv[1];
  } else {
    parameterFile = "mps_enesim.txt";
  }

  MPS::ENESIM_GENERAL aSimulation(parameterFile);

  aSimulation.startSimulation();

  return 0;
}

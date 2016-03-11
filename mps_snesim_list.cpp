#include <iostream>
#include "SNESIMList.h"
//#include "IO.h"

int main(int argc, char* argv[]) {

  std::string parameterFile;

  if (argc>1) {
    parameterFile = argv[1];
  } else {
    parameterFile = "mps_snesim.txt";
  }

  igisSIM::SNESIMList aSimulation(parameterFile);
  aSimulation.startSimulation();

  return 0;
}

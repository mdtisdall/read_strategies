#include "single_value.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <chrono>

int main(void) {
  size_t numFiles = 256;

  std::vector<std::ifstream*> inputFiles(numFiles);

  for(size_t i = 0; i < numFiles; i++) {
    std::stringstream sstream;
    sstream << "inputfile_" << i;
    inputFiles[i] =
      new std::ifstream(sstream.str(), std::ios::binary | std::ios::in);
    
    if(!inputFiles[i]->good()) {
      std::cerr << "Could not open file: " << sstream.str() << std::endl;
      exit(1);
    }
  }
 
  auto start = std::chrono::steady_clock::now(); 
  single_value<double>(inputFiles);
  auto end = std::chrono::steady_clock::now();  
  auto diff = end - start;
  std::cout
    << std::chrono::duration <double, std::milli> (diff).count()
    << " ms" << std::endl;
}

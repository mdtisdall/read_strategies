#include "single_value_cppstyle.h"
#include "single_value_cstyle.h"
#include "single_value_cstyle_fadvise.h"
#include "single_value_cstyle_buffered.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <chrono>

typedef float testtype;

int main(void) {
  size_t numFiles = 256;

  std::vector<std::string> inputFilePaths(numFiles);

  for(size_t i = 0; i < numFiles; i++) {
    std::stringstream sstream;
    sstream << "inputfile_" << i;
    inputFilePaths[i] = sstream.str();  
  }
 
  auto start = std::chrono::steady_clock::now(); 
//single_value_cppstyle<testtype>(inputFilePaths);
//single_value_cstyle<testtype>(inputFilePaths);
//single_value_cstyle_fadvise<testtype>(inputFilePaths);
single_value_cstyle_buffered<testtype>(inputFilePaths, 4 * 1024);
  auto end = std::chrono::steady_clock::now();  
  auto diff = end - start;
  std::cout
    << std::chrono::duration <double, std::milli> (diff).count()
    << " ms" << std::endl;
}

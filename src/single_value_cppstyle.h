#ifndef single_value_cppstyle_h
#define single_value_cppstyle_h

#include <iostream>
#include <fstream>
#include <vector>

template <typename T>
inline void single_value_cppstyle(std::vector<std::string> inputFilePaths) {
  size_t numFiles = inputFilePaths.size();
  std::vector<std::ifstream*> inputFiles(numFiles);
  std::vector<T> totalValues(numFiles);

  for(size_t i = 0; i < numFiles; i++) {
    totalValues[i] = 0; 
  }


  for(size_t i = 0; i < numFiles; i++) {
    inputFiles[i] =
      new std::ifstream(inputFilePaths[i], std::ios::binary | std::ios::in);

    if(!inputFiles[i]->good()) {
      std::cerr << "Could not open file: " << inputFilePaths[i] << std::endl;
      exit(1);
    }
  }

  T processingBuffer[numFiles];
  bool done = false;
  size_t counter = 0;

  while(true) {
    for(size_t i = 0; i < numFiles; i++) {
      inputFiles[i]->read((char*) (processingBuffer + i), sizeof(T));

      if(inputFiles[i]->eof()) {
        done = true;
        break; 
      }
    }
    
    counter++;
    
    if(counter % 1000 == 0) {
      std::cout << "read " << counter/1000 << "k lines" << std::endl;
    }
    
    if(done) {
      break; 
    }

    // process data here
    for(size_t i = 0; i < numFiles; i++) {
      totalValues[i] += processingBuffer[i];
    }
  }

  std::cout << "Results:" << std::endl;
  for(size_t i = 0; i < numFiles; i++) {
    std::cout << totalValues[i] << std::endl;
  }
  
}

#endif

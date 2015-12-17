#ifndef single_value_h
#define single_value_h

#include <iostream>
#include <fstream>
#include <vector>

template <typename T>
inline void single_value(std::vector<std::ifstream*> inputFiles) {
  size_t numFiles = inputFiles.size();
  T processingBuffer[numFiles];
  bool done = false;

  while(true) {
    for(size_t i = 0; i < numFiles; i++) {
      inputFiles[i]->read((char*) (processingBuffer + i), sizeof(T));
    }
    
    if(inputFiles[0]->eof()) {
      break; 
    }

    // process data here
  }
}

#endif

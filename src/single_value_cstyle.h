#ifndef single_value_cstyle_h
#define single_value_cstyle_h

#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>

template <typename T>
inline void single_value_cstyle(std::vector<std::string> inputFilePaths) {
  size_t numFiles = inputFilePaths.size();
  std::vector<int> inputFiles(numFiles);
  std::vector<T> totalValues(numFiles);

  for(size_t i = 0; i < numFiles; i++) {
    totalValues[i] = 0; 
  }
  
  for(size_t i = 0; i < numFiles; i++) {
    inputFiles[i] = open(inputFilePaths[i].c_str(), O_RDONLY);

    if(-1 == inputFiles[i]) {
      std::cerr << "Could not open file: " << inputFilePaths[i] << std::endl;
      exit(1);
    }
  }

  T processingBuffer[numFiles];
  bool done = false;
  size_t counter = 0; 

  while(true) {
    size_t readSize;

    for(size_t i = 0; i < numFiles; i++) {
      readSize = read(inputFiles[i], processingBuffer + i, sizeof(T));
 
      if(readSize != sizeof(T)) {
        done = true;
        break; 
      }
    }

    counter++;
    
    if(counter % 1000 == 0) {
      std::cout << "read " << counter / 1000 << "k lines" << std::endl;
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

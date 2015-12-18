#ifndef single_value_cstyle_buffered_h
#define single_value_cstyle_buffered_h

#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>

template <typename T>
inline void single_value_cstyle_buffered(
  std::vector<std::string> inputFilePaths,
  size_t buffersize) {
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

  size_t bufferSizeInBytes = buffersize * sizeof(T);

  T ioBuffer[numFiles * bufferSizeInBytes];
  T processingBuffer[numFiles];
  bool done = false;
  size_t counter = 0; 

  while(true) {
    size_t readSize;
    size_t readElements = buffersize;

    T* readHead = ioBuffer;

    for(size_t i = 0; i < numFiles; i++) {
      readSize = read(inputFiles[i], readHead, bufferSizeInBytes);

      if(0 == readSize) {
        readElements = 0; 
        done = true;
        std::cout << "readSize == 0 on file " << i << std::endl;
        break;
      }
      
      readHead += buffersize;
    } 
   
    if(readSize != bufferSizeInBytes &&
       readSize != 0) {
        done = true;
        readElements = readSize / sizeof(T);
    }
    
    for(size_t procIndex = 0; procIndex < readElements; procIndex++) {
      for(unsigned int i = 0; i < numFiles; i++) {
        processingBuffer[i] = ioBuffer[i * buffersize + procIndex];
      }

      counter++;
      
      if(counter % 1000 == 0) {
        std::cout << "read " << counter / 1000 << "k lines" << std::endl;
      }

      // process data here
      for(size_t i = 0; i < numFiles; i++) {
        totalValues[i] += processingBuffer[i];
      }
    }

    if(done) {
      break; 
    }
  }

  std::cout << "Results:" << std::endl;
  for(size_t i = 0; i < numFiles; i++) {
    std::cout << totalValues[i] << std::endl;
  }
}

#endif

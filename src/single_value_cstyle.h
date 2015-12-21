#ifndef single_value_cstyle_h
#define single_value_cstyle_h

#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>

template <typename T>
class single_value_cstyle {
  public:
    single_value_cstyle(
      std::vector<std::string> inputFilePaths) :
      numFiles(inputFilePaths.size()),
      inputFiles(numFiles)
    {

      for(size_t i = 0; i < numFiles; i++) {
        inputFiles[i] = open(inputFilePaths[i].c_str(), O_RDONLY);
    
        if(-1 == inputFiles[i]) {
          std::cerr << "Could not open file: " << inputFilePaths[i] << std::endl;
          exit(1);
        }
      }
    }

    inline bool read(T *buffer) const {
      for(size_t i = 0; i < numFiles; i++) {
        if(::read(inputFiles[i], buffer + i, sizeof(T)) != sizeof(T)) {
          return false; 
        }
      }

      return true;
    }

  protected:
    size_t numFiles;
    std::vector<int> inputFiles;
    
};

#endif

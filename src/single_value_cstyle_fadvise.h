#ifndef single_value_cstyle_fadvise_h
#define single_value_cstyle_fadvise_h

#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>

template <typename T>
class single_value_cstyle_fadvise {
  public:
    single_value_cstyle_fadvise(
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

        posix_fadvise(inputFiles[i], 0, 0, POSIX_FADV_SEQUENTIAL | POSIX_FADV_NOREUSE);
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

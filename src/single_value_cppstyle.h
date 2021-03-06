#ifndef single_value_cppstyle_h
#define single_value_cppstyle_h

#include <iostream>
#include <fstream>
#include <vector>

template <typename T>
class single_value_cppstyle {
  public:
    single_value_cppstyle(
      std::vector<std::string> inputFilePaths) :
      numFiles(inputFilePaths.size()),
      inputFiles(numFiles)
    {

      for(size_t i = 0; i < numFiles; i++) {
        inputFiles[i] =
          new std::ifstream(inputFilePaths[i], std::ios::binary | std::ios::in);
    
        if(!inputFiles[i]->good()) {
          std::cerr << "Could not open file: " << inputFilePaths[i] << std::endl;
          exit(1);
        }
      }
    }

    inline bool read(T *buffer) const {
      for(size_t i = 0; i < numFiles; i++) {
        inputFiles[i]->read((char*) (buffer + i), sizeof(T));
        if(inputFiles[i]->eof()) {
          return false; 
        }
      }

      return true;
    }

  protected:
    size_t numFiles;
    std::vector<std::ifstream*> inputFiles;
    
};
#endif

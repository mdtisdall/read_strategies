#ifndef single_value_cppstyle_manual_buffer_h
#define single_value_cppstyle_manual_buffer_h

#include <iostream>
#include <fstream>
#include <vector>

template <typename T>
class single_value_cppstyle_manual_buffer {
  public:
    single_value_cppstyle_manual_buffer(
      std::vector<std::string> inputFilePaths,
      size_t buffersize = (8 * 1024)) :
      numFiles(inputFilePaths.size()),
      buffersize(buffersize),
      inputFiles(numFiles),
      ioBuffers(numFiles)
    {

      for(size_t i = 0; i < numFiles; i++) {
        inputFiles[i] =
          new std::ifstream();
        ioBuffers[i].resize(buffersize);
        inputFiles[i]->rdbuf()->pubsetbuf(
          (char*) (ioBuffers[i].data()), buffersize * sizeof(T));
        inputFiles[i]->open(inputFilePaths[i], std::ios::binary | std::ios::in);
    
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
    size_t buffersize;
    std::vector<std::ifstream*> inputFiles;
    std::vector< std::vector<T> > ioBuffers;
    
};
#endif

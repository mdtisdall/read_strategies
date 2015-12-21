#ifndef buffered_cstyle_h
#define buffered_cstyle_h

#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>

template <typename T>
class buffered_cstyle {
  public:
    buffered_cstyle(
      std::vector<std::string> inputFilePaths,
      size_t buffersize = (8 * 1024)) :
      numFiles(inputFilePaths.size()),
      buffersize(buffersize),
      inputFiles(numFiles),
      bufferSizeInBytes(buffersize * sizeof(T)),
      ioBuffer(numFiles * bufferSizeInBytes),
      readCounter(0),
      maxReadCounter(0),
      eof(false)
    {

      for(size_t i = 0; i < numFiles; i++) {
        inputFiles[i] = open(inputFilePaths[i].c_str(), O_RDONLY);
    
        if(-1 == inputFiles[i]) {
          std::cerr << "Could not open file: " << inputFilePaths[i] << std::endl;
          exit(1);
        }
      }
    }

    bool read(T *buffer) {
      if(eof) {
        return false;
      }

      if(readCounter == maxReadCounter) {
        T* writeHead = ioBuffer.data();
        size_t readSize;

        for(size_t i = 0; i < numFiles; i++) {
          readSize = ::read(inputFiles[i], writeHead, bufferSizeInBytes);
    
          if(0 == readSize) {
            eof = true; 
            return false;
          }
          
          writeHead += buffersize;
        }

        readCounter = 0;
        maxReadCounter = readSize / sizeof(T);
      }

      for(size_t i = 0; i < numFiles; i++) {
        buffer[i] = ioBuffer[i * buffersize + readCounter];
      }

      readCounter++;

      return true;
    }

  protected:
    size_t numFiles;
    size_t buffersize;
    std::vector<int> inputFiles;
    size_t bufferSizeInBytes;
    std::vector<T> ioBuffer;    
    size_t readCounter;    
    size_t maxReadCounter;    
    bool eof;    
};




#endif

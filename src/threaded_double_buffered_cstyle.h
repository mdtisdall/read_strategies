#ifndef threaded_double_buffered_cstyle_h
#define threaded_double_buffered_cstyle_h

#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>

template <typename T>
class threaded_double_buffered_cstyle {
  public:
    threaded_double_buffered_cstyle(
      std::vector<std::string> inputFilePaths,
      size_t buffersize = (8 * 1024)):
      numFiles(inputFilePaths.size()),
      buffersize(buffersize),
      inputFiles(numFiles),
      bufferSizeInBytes(buffersize * sizeof(T)),
      readCounter(1),
      maxReadCounter(1),
      frontEmptyFlag(true)
    {
      for(size_t i = 0; i < 2; i ++) {
         buffers[i] = new std::vector<T>(buffersize * numFiles); 
      }

      frontBuffer = buffers[0]->data();

      for(size_t i = 0; i < numFiles; i++) {
        inputFiles[i] = open(inputFilePaths[i].c_str(), O_RDONLY);
    
        if(-1 == inputFiles[i]) {
          std::cerr << "Could not open file: " << inputFilePaths[i] << std::endl;
          exit(1);
        }
      }

      readerThread = new std::thread(&threaded_double_buffered_cstyle::file_loop, this);
    }

    ~threaded_double_buffered_cstyle() {
      delete buffers[0];
      buffers[0] = NULL;

      { 
        std::unique_lock<std::mutex> locker(bufferMutex);
        delete buffers[1];
        buffers[1] = NULL;
      }

      frontBufferIsEmpty.notify_one();
      readerThread->join();
      delete readerThread;
    }

    bool read(T *buffer) {
      if(0 == maxReadCounter) {
        return false; 
      }

      if(readCounter == maxReadCounter) {
        //std::cout << "About to wait for front buffer to be full" << std::endl;
        std::unique_lock<std::mutex> locker(bufferMutex);
        frontEmptyFlag = true;
        frontBufferIsEmpty.notify_one();
       
        // can't always trust that wait was signaled if it return
        while(frontEmptyFlag) { 
          if(0 == maxReadCounter) {
            return false; 
          }

          frontBufferIsFull.wait(locker);
        }
      }

      for(size_t i = 0; i < numFiles; i++) {
        buffer[i] = frontBuffer[i * buffersize + readCounter];
      }

      readCounter++;

      return true;
    }

    void file_loop() {
      while(0 != maxReadCounter) { 
        // fill the back buffer
        T* writeHead = buffers[1]->data();
        size_t readSize;

        for(size_t i = 0; i < numFiles; i++) {
          readSize = ::read(inputFiles[i], writeHead, bufferSizeInBytes);

          if(0 == readSize) {
            break;
          }
          
          writeHead += buffersize;
        }

        //std::cout << "About to wait for front buffer to be empty" << std::endl;

        { 
          std::unique_lock<std::mutex> locker(bufferMutex);
          // can't always trust that wait was signaled if it return
          while(!frontEmptyFlag) {
            frontBufferIsEmpty.wait(locker);
          }        
          
          std::vector<T>* temp = buffers[0];
          buffers[0] = buffers[1];
          buffers[1] = temp;

          frontBuffer = buffers[0]->data();

          readCounter = 0;
          maxReadCounter = readSize / sizeof(T);
          frontEmptyFlag = false;

          frontBufferIsFull.notify_one();
        }
      }
    }


  protected:
    size_t numFiles; 
    size_t readCounter;
    size_t maxReadCounter;
    size_t buffersize;
    size_t bufferSizeInBytes;
    bool frontEmptyFlag; 
    std::vector<int> inputFiles;
    std::vector<T>* buffers[2];
    T* frontBuffer;
    std::mutex bufferFlagMutex;
    std::mutex bufferMutex;
    std::condition_variable frontBufferIsEmpty;
    std::condition_variable frontBufferIsFull;
    std::thread *readerThread; 
};

#endif

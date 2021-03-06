#include "single_value_cppstyle.h"
#include "single_value_cppstyle_manual_buffer.h"
#include "single_value_cstyle.h"
#include "single_value_cstyle_fadvise.h"
#include "buffered_cstyle.h"
#include "buffered_cstyle_fadvise.h"
#include "threaded_double_buffered_cstyle.h"

#include <tclap/CmdLine.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <chrono>

typedef float T;

template <typename strategyT>
strategyT* init_reader(std::vector<std::string> inputFilePaths) {
  return new strategyT(inputFilePaths);
}

template <typename dataT, typename strategyT>
void process_data(std::vector<std::string> inputFilePaths) {
  auto *reader = new strategyT(inputFilePaths);
  size_t numFiles = inputFilePaths.size();
  dataT processingBuffer[numFiles];
  dataT totalValues[numFiles];

  for(size_t i = 0; i < numFiles; i++) {
    totalValues[i] = 0; 
  }

  size_t counter = 0;

  auto start = std::chrono::steady_clock::now(); 
  while(reader->read(processingBuffer)) { 
/*
    counter++;
   
    if(counter % 1000 == 0) {
      std::cout << "read " << counter/1000 << "k lines" << std::endl;
    }
*/    
    // process data here
    for(size_t i = 0; i < numFiles; i++) {
      totalValues[i] += processingBuffer[i];
    }
  }

  auto end = std::chrono::steady_clock::now();  
  auto diff = end - start;

  delete reader;
  
  std::cout << "Result: " << totalValues[0] << " ";

  {
    bool good = true;
    for(size_t i = 1; i < numFiles; i++) {
      if(totalValues[i] != totalValues[0]) {
        good = false;
      }
    }

    if(good) {
      std::cout << "ok.";
    }
    else {
      std::cout << "mismatch."; 
    }

    std::cout << std::endl;
  }
/*
  for(size_t i = 0; i < numFiles; i++) {
    std::cout << totalValues[i] << std::endl;
  }
*/
  std::cout
    << std::chrono::duration <double, std::milli> (diff).count()
    << " ms" << std::endl;
}

int main(int argc, char** argv) {
  size_t numFiles = 256;
  std::vector<std::string> inputFilePaths(numFiles);

  bool doCStyle = false;
  bool doCStyleFadvise = false;
  bool doCStyleBuffered = false;
  bool doCStyleBufferedFadvise = false;
  bool doCStyleThreadedBuffered = false;
  bool doCPPStyle = false;


  try {
		TCLAP::CmdLine cmd(
      "Test different startegies for linear, parallel file access",
      ' ', "0.0.1");


		TCLAP::ValueArg<std::string> testFilePathArg("i", "input-path", "path containing input files to read", true, "./", "file path ending in /", cmd);
		TCLAP::SwitchArg cstyleArg("", "c-style", "use read(2)", false);
		TCLAP::SwitchArg cstylefadviseArg("", "c-style-fadvise",
      "use read(2) with fadvise", false);
		TCLAP::SwitchArg cstylebufferedArg("", "c-style-buffered",
      "use read(2) with manual buffering", false);
		TCLAP::SwitchArg cstylebufferedfadviseArg("", "c-style-buffered-fadvise",
      "use read(2) with manual buffering and fadvise", false);
		TCLAP::SwitchArg cstylethreadedbufferedArg("", "c-style-threaded-buffered",
      "use read(2) with threaded double buffering", false);
		TCLAP::SwitchArg cppstyleArg("", "cpp-style", "use ifstream.read()", false);
   
    std::vector<TCLAP::Arg*> xorArgList;
    xorArgList.push_back(&cstyleArg);
    xorArgList.push_back(&cstylefadviseArg);
    xorArgList.push_back(&cstylebufferedArg);
    xorArgList.push_back(&cstylebufferedfadviseArg);
    xorArgList.push_back(&cstylethreadedbufferedArg);
    xorArgList.push_back(&cppstyleArg);
    
    cmd.xorAdd(xorArgList);
		cmd.parse(argc, argv);

    doCStyle = cstyleArg.getValue();
    doCStyleFadvise = cstylefadviseArg.getValue();
    doCStyleBuffered = cstylebufferedArg.getValue();
    doCStyleBufferedFadvise = cstylebufferedfadviseArg.getValue();
    doCStyleThreadedBuffered = cstylethreadedbufferedArg.getValue();
    doCPPStyle = cppstyleArg.getValue();

    std::string testFilePath = testFilePathArg.getValue(); 
    if(testFilePath.back() != '/') {
      testFilePath = testFilePath + std::string("/"); 
    }

    for(size_t i = 0; i < numFiles; i++) {
      std::stringstream sstream;
      sstream << "inputfile_" << i;
      inputFilePaths[i] = testFilePath + sstream.str();  
    }
	} 	catch (TCLAP::ArgException &e) {
		std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
		exit(1);
	}


  
  if(doCStyle) {
    process_data<T, single_value_cstyle<T> >(inputFilePaths);
  }
  if(doCStyleFadvise) {
    process_data<T, single_value_cstyle_fadvise<T> >(inputFilePaths);
  }
  if(doCStyleBuffered) {
    process_data<T, buffered_cstyle<T> >(inputFilePaths);
  }
  if(doCStyleBufferedFadvise) {
    process_data<T, buffered_cstyle_fadvise<T> >(inputFilePaths);
  }
  if(doCStyleThreadedBuffered) {
    process_data<T, threaded_double_buffered_cstyle<T> >(inputFilePaths);
  }
  if(doCPPStyle) {
    process_data<T, single_value_cppstyle<T> >(inputFilePaths);
  }
}

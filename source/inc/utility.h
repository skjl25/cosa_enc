/*
utility header for the cosa_enc

The MIT License (MIT)

Copyright (c) 2014-2017 Suk Kyu Lee <skjl25@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef UTILITY_H_
#define UTILITY_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <fstream>
#include <vector>
#include <stdint.h>
#define WINDOWS 1
#define LINUX 0

#if WINDOWS
#include "time.h"
#endif

using namespace std;

class Utility {
public:
  ofstream myfile;
  FILE* pFile;
  //struct timeval tv;
#if LINUX
  double start_time, end_time, elapsed_time;
#endif

#if WINDOWS
  clock_t start_time;
  clock_t end_time;
  double elapsed_time;
#endif

  void startTimer();
  void getElapsedTime();
  void getElapsedTime(char* debugMsg);

  //double array->Uint character array //255.0->255 double val is within the
  //unsigned char range
  char* convertDoubleToUintChar1D(double** returnArray2, int width1,
    int height1);
  char* convertDoubleToUintChar1D(double* returnArray2, int arrayLength);

  //char array->Double array
  double** convertChar1DToDouble2D(char* orgArray, int width, int height);

  int* convertCharToUintInt1D(char* orgArray, int arrayLength);
  void convertCharToUintInt1DExt(char* orgArray, int* convertArray,
    int arrayLength);

  //Getting the avg and standard deviation of set of arrays in various forms
  double getMean(double** org_img, int width, int height);
  double getMean(double* org_img, int width, int height);
  double getStd(double** org_img, int width, int height);
  double getStd(double* org_img, int width, int height);

  void changeFileNameInOrderedSequence(char* dst_file_name, int img_seq,
    char* loc_folder, char* name_file_header,
    char* type_ext);

  //Writing File functions
  void initWritingFile(char* fileLocation);
  void closeFile();

  //C++ function template sample
  template <class T>
  T Add(T a, T b) {
    return a + b;
  }

  template<class T> void writeToFile(T data) {
    myfile << data << endl;
  }

  template<class T> T** memset2DArray(int width, int height) {
    T** resultArray = new T*[width];
    for (int i = 0; i < width; i++) {
      resultArray[i] = new T[height];
    }
    return resultArray;
  }

  template<class T> double* convertArrayToDouble1D(T* orgArray, int width,
    int height) {
    double* resultArray = new double[width*height];

    if (sizeof(T) == sizeof(char)) {
      int index = 0;
      for (int i = 0; i < width*height; i++) {
        uint8_t temp = (uint8_t)orgArray[i];
        resultArray[i] = temp*1.0;
        index++;
      }
    }
    if (sizeof(T) == sizeof(short)) {
      int index = 0;
      for (int i = 0; i < width*height; i++) {
        int temp = orgArray[i];
        resultArray[i] = temp*1.0;
        index++;
      }
    }
    return resultArray;
  }

  template<class T> char* convertVectorToChar1D(vector<T> returnArray2,
    int arrayLength) {
    if (sizeof(T) == sizeof(int)) {
      int index = 0;
      char* tempArray = new char[arrayLength*sizeof(int)];

      for (int i = 0; i < arrayLength; i = i++) {
        memcpy(&tempArray[index], &returnArray2[i], sizeof(int));
        index = index + sizeof(int);
      }
      return tempArray;
    }
    if (sizeof(T) == sizeof(double)) {
      int index = 0;
      char* tempArray = new char[arrayLength*sizeof(double)];

      for (int i = 0; i < arrayLength; i = i++) {
        memcpy(&tempArray[index], &returnArray2[i], sizeof(double));
        index = index + sizeof(double);
      }
      return tempArray;
    }
  }

  template<class T> char* convertArrayToChar1D(T* orgArray, int arrayLength) {
    if (sizeof(T) == sizeof(int)) {
      char* returnArray = new char[arrayLength];
      for (int i = 0; i < arrayLength; i++) {
        returnArray[i] = (uint8_t)orgArray[i];
      }
      return returnArray;
    }
    if (sizeof(T) == sizeof(double)) {
      int index = 0;
      char* tempArray = new char[arrayLength*sizeof(double)];

      for (int i = 0; i < arrayLength; i = i++) {
        memcpy(&tempArray[index], &orgArray[i], sizeof(double));
        index = index + sizeof(double);
      }
      return tempArray;
    }
  }
  void saveDatatoDatFile(char* integratedArray, int size, char* filelocation);

  Utility();
  virtual ~Utility();
};
#endif /* UTILITY_H_ */

/*
utility code for the cosa_enc

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

#include "../inc/utility.h"
Utility::Utility() {}
#if LINUX
void utility::startTimer() {
  gettimeofday(&tv, NULL);
  startTime = tv.tv_sec + (tv.tv_usec / 1000000.0);
}

void utility::getElapsedTime() {
  gettimeofday(&tv, NULL);
  endTime = tv.tv_sec + (tv.tv_usec / 1000000.0);
  elapsedTime = 1000.0*(endTime - startTime);
  printf("Elapsed time is %.0lf ms\n", elapsedTime);
}
void utility::getElapsedTime(char* debugMsg) {
  gettimeofday(&tv, NULL);
  endTime = tv.tv_sec + (tv.tv_usec / 1000000.0);
  elapsedTime = 1000.0*(endTime - startTime);
  printf("%s Elapsed time is %.0lf ms\n", debugMsg, elapsedTime);
}
#endif

#if WINDOWS
void Utility::startTimer() {
  start_time = clock();
}

void Utility::getElapsedTime() {
  end_time = clock();
  elapsed_time = (double)(end_time - start_time);
  printf("Elapsed time is %f ms\n", elapsed_time);
}
void Utility::getElapsedTime(char* debug_msg) {
  clock_t endTime = clock();
  elapsed_time = (double)(endTime - start_time);
  printf("%s Elapsed time is %f ms\n", debug_msg, elapsed_time);
}
#endif

double Utility::getMean(double** org_img, int width, int height) {
  double mean = 0;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      mean = mean + org_img[i][j];
    }
  }
  return mean / (width*height);
}

double Utility::getMean(double* org_img, int width, int height) {
  double mean = 0;
  for (int i = 0; i < width*height; i++) {
    mean = mean + org_img[i];
  }
  return mean / (width*height);
}

double Utility::getStd(double** org_img, int width, int height) {
  double mean = 0;
  double sum = 0;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      mean = mean + org_img[i][j];
      sum = sum + (org_img[i][j])*(org_img[i][j]);
    }
  }

  mean = mean / (width*height);
  sum = (sum / (width*height)) - mean*mean;
  sum = sqrt(sum);
  return sum;
}

double Utility::getStd(double* org_img, int width, int height) {
  double mean = 0;
  double sum = 0;
  for (int i = 0; i < width*height; i++) {
    mean = mean + org_img[i];
    sum = sum + (org_img[i])*(org_img[i]);
  }

  mean = mean / (width*height);
  sum = (sum / (width*height)) - mean*mean;
  sum = sqrt(sum);
  return sum;
}

double** Utility::convertChar1DToDouble2D(char* orgArray, int width,
  int height) {
  double** resultArray = memset2DArray<double>(width, height);

  int index = 0;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      uint8_t temp = (uint8_t)orgArray[j*width + i];
      resultArray[i][j] = temp*1.0;
      index++;
    }
  }
  return resultArray;
}

char* Utility::convertDoubleToUintChar1D(double* returnArray2, int arrayLength) {
  int index = 0;
  char* tempArray = new char[arrayLength*sizeof(short)];

  for (int i = 0; i < arrayLength; i++) {
    short tempVal = 0;
    //    		printf("%f\n",returnArray2[i][j]);
    if (returnArray2[i] > 255.0)
      tempVal = (short)255;
    else if (returnArray2[i] < 0.0)
      tempVal = (short)0;
    else
      tempVal = (short)returnArray2[i];
    memcpy(&tempArray[index], &tempVal, sizeof(short));

    index++;
  }
  return tempArray;
}

int* Utility::convertCharToUintInt1D(char* orgArray, int arrayLength) {
  //	int* returnArray=new int[arrayLength];
  int* returnArray = (int*)malloc(sizeof(int)*arrayLength);
  for (int i = 0; i < arrayLength; i++) {
    uint8_t temp = (uint8_t)orgArray[i];
    returnArray[i] = temp;
  }
  return returnArray;
}

void Utility::convertCharToUintInt1DExt(char* orgArray, int* convertArray,
  int arrayLength) {
  //	int* returnArray=new int[arrayLength];
  //	int* returnArray=(int*)malloc(sizeof(int)*arrayLength);
  for (int i = 0; i < arrayLength; i++) {
    uint8_t temp = (uint8_t)orgArray[i];
    convertArray[i] = temp;
  }
  //	return returnArray;
}

char* Utility::convertDoubleToUintChar1D(double** returnArray2, int width,
  int height) {
  int index = 0;
  int Width = width;
  int Height = height;
  char* tempArray = new char[Width*Height*sizeof(short)];

  for (int j = 0; j < Height; j++) {
    for (int i = 0; i<Width; i++) {
      short tempVal = 0;
      //    		printf("%f\n",returnArray2[i][j]);
      if (returnArray2[i][j]>255.0)
        tempVal = (short)255;
      else if (returnArray2[i][j] < 0.0)
        tempVal = (short)0;
      else
        tempVal = (short)returnArray2[i][j];
      memcpy(&tempArray[index], &tempVal, sizeof(short));
      index++;
    }
  }
  return tempArray;
}

void Utility::changeFileNameInOrderedSequence(char* dst_file_name, int img_seq,
  char* loc_folder,
  char* name_file_header,
  char* type_ext) {
  char* FileName = new char[100];
  if (img_seq < 10) {
    sprintf(FileName, "%s_0000%d", name_file_header, img_seq);
  }
  if (img_seq >= 10 && img_seq < 100) {
    sprintf(FileName, "%s_000%d", name_file_header, img_seq);
  }
  if (img_seq >= 100 && img_seq < 1000) {
    sprintf(FileName, "%s_00%d", name_file_header, img_seq);
  }
  if (img_seq >= 1000 && img_seq < 10000) {
    sprintf(FileName, "%s_0%d", name_file_header, img_seq);
  }
  if (img_seq >= 10000 && img_seq < 100000) {
    sprintf(FileName, "%s_%d", name_file_header, img_seq);
  }
  sprintf(dst_file_name, "%s%s.%s", loc_folder, FileName, type_ext);
  delete[] FileName;
}
void Utility::initWritingFile(char* fileLocation) {
  //		pFile=fopen(fileLocation,"w");
  myfile.open(fileLocation);
}

void Utility::closeFile() {
  //		pFile=fopen(fileLocation,"w");
  myfile.close();
}

void Utility::saveDatatoDatFile(char* integratedArray, int size,
  char* filelocation) {
  char* fileName = new char[100];
  sprintf(fileName, "%s.dat", filelocation);
  initWritingFile(fileName);
  for (int i = 0; i < size; i++) {
    //		writeToFile(integratedArray[i]);
    fprintf(pFile, "%c", integratedArray[i]);
  }
  delete[] fileName;
}

Utility::~Utility() {}
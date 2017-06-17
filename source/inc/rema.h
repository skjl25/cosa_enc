/*
rema header for the cosa_enc

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

#ifndef REMA_H_
#define REMA_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <stdint.h>
#include "../../../utility/inc/utility.h"
#include "../../../utility/inc/typedef.h"

using namespace std;

#define PI 3.14159265

class ReMA {
public:

  int* encoded_depth_data;
  int* encoded_visible_data;
  int* encoded_reduced_visible_data;
  int* encoded_merge_visible_depth_data;

  int* decoded_reduced_visible_data;
  int* decoded_visible_data;
  int* reconstructed_visible_data;

  //Need to be more specific whether depth of visible
  int* decodedBlock;
  int  autoViewVal;
  int* decodedData;

  ReMA(int width, int height);

  //DWT implementation
  void rema_visible_encode(double* dwtOrgImage);
  void rema_visible_encode(int* dwtOrgImage);
  void rema_visible_decode();

  int* getLowFreqArrayExt(double* dwtOrgImage, int width, int height);

  template<class T> int* getLowFreqArray(T* dwtOrgImage, int width, int height) {
    int* lowPassFilter = new int[(width*height) / 2];
    //creating low freq
    int j = 0;
    for (int i = 0; i < width*height; i = i + 2) {
      lowPassFilter[j] = ((int)dwtOrgImage[i] + (int)dwtOrgImage[i + 1]) / 2;
      j++;
    }
    return lowPassFilter;
  }

  template<class T> int* getHighFreqArray(T* dwtOrgImage, int* lowPassFilter,
    int width, int height) {
    int* highPassFilter = new int[(width*height) / 2];
    //creating low freq
    //creating high freq
    int j = 0;
    for (int i = 0; i < width*height; i = i + 2) {
      highPassFilter[j] = (int)dwtOrgImage[i] - (int)lowPassFilter[j];
      j++;
    }
    return highPassFilter;
  }

  int* getEncodedDWTExt(double* dwtOrgImage, int width, int height);
  void getReplicateBlocks(int compressedIndicator);
  int* getDWTBlock(int* oneMergeFilter, int width, int height,
    int compressedIndicator);

  void rema_depth_decode_2();
  void rema_depth_encode_2(int* realDepthVal1D);

  void mergeVisibleDepthData();
  void extractVisibleDepthData(char* receivedData);

  inline void setImageheight(int cameraheight) { height = cameraheight; };
  inline void setImagewidth(int camerawidth) { width = camerawidth; };
  inline void setEncodedVisibleData() { encoded_visible_data = new int[width*height]; };
  inline void setReducedVisibleData() { encoded_reduced_visible_data = new int[width*height]; };
  inline void setEncodedDepthData() { encoded_depth_data = new int[(width*height) / 4]; };
  inline void setDecodedBlock() { decodedBlock = new int[height*width]; };
  inline void setEncodedMergeVisibleDepthData() { encoded_merge_visible_depth_data = new int[(width*height) / 2]; };
  inline void setDecodedVisibleData() { decoded_visible_data = new int[width*height]; };
  inline void setReconstructedVisibleData() { reconstructed_visible_data = new int[width*height]; };
  inline void setDecodedReducedVisibleData() { decoded_reduced_visible_data = new int[width*height]; };
  inline int getImageheight() { return height; };
  inline int getImagewidth() { return width; };

  virtual ~ReMA();

private:
  uint32_t width;
  uint32_t height;
};

#endif /* CVIDEO_H_ */

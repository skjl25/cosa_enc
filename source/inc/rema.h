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
#include "utility.h"
#include "typedef.h"

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
  void_t rema_visible_encode(double* dwtOrgImage);
  void_t rema_visible_encode(int* dwtOrgImage);
  void_t rema_visible_decode();

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
  void_t getReplicateBlocks(int compressedIndicator);
  int* getDWTBlock(int* oneMergeFilter, int width, int height,
    int compressedIndicator);

  void_t rema_depth_decode_2();
  void_t rema_depth_encode_2(int* realDepthVal1D);

  void_t mergeVisibleDepthData();
  void_t extractVisibleDepthData(char* receivedData);

  inline void_t setImageheight(int cameraheight) { height = cameraheight; };
  inline void_t setImagewidth(int camerawidth) { width = camerawidth; };
  inline void_t setEncodedVisibleData() { encoded_visible_data = new int[width*height]; };
  inline void_t setReducedVisibleData() { encoded_reduced_visible_data = new int[width*height]; };
  inline void_t setEncodedDepthData() { encoded_depth_data = new int[(width*height) / 4]; };
  inline void_t setDecodedBlock() { decodedBlock = new int[height*width]; };
  inline void_t setEncodedMergeVisibleDepthData() { encoded_merge_visible_depth_data = new int[(width*height) / 2]; };
  inline void_t setDecodedVisibleData() { decoded_visible_data = new int[width*height]; };
  inline void_t setReconstructedVisibleData() { reconstructed_visible_data = new int[width*height]; };
  inline void_t setDecodedReducedVisibleData() { decoded_reduced_visible_data = new int[width*height]; };
  inline int getImageheight() { return height; };
  inline int getImagewidth() { return width; };

  virtual ~ReMA();

private:
  uint32_t width;
  uint32_t height;
};

#endif /* CVIDEO_H_ */
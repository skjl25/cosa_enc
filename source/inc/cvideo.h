/*
cvideo header for the cosa_enc

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


#ifndef CVIDEO_H_
#define CVIDEO_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "global.h"

using namespace std;

class mb {
public:
  //0 if it is not ROI
  //1 if it is ROI
  int ROIFlag;
  double_t** macroBPixel;
  double_t deliveryRate;
  char* orgImage;
};

class cvideo {
public:

  mb* macroBHolder;

  cvideo();
  cvideo(int Height, int Width, int macroBlockSize, double_t nonROIDeliver,
    double_t ROIDeliver);

  void setMacroBHolder();
  double_t* getOneIntegratedArray(double_t** inputArray);

  void setMacroBPixel(double_t** double_tOrgImage);
  void getDCTArray();
  double_t* getZigZagArray(int macroBlockID);
  double_t** setMacroBlocksToOriginalImage();
  void getInverseZigZagArray(double_t** inputArray);

  void setMacroBPixel_withROI(double_t** double_tOrgImage, vector<int> vw);

  void getIDCTArray();

  // frome here

  void quantization(short** matrix);
  void dc_encoding(short** mat, int** intmat, int* preblock);
  void ac_encoding(short** mat, int** intmat, int* preblock);
  void zigzag(short* block);
  void ac_decoding(short** mat, int** intmat, int* Ipreblock);
  int* IacHuff(int data);
  short symb2(short sym2, short cat);               // calculate the original data from symbol-2
  void dc_decoding(short** mat, int** intmat, int* Ipreblock);                 // function doing dc decoding;
  short symbol2(short diff);
  int count(short run, short cat);
  short achuff(short run, short cat);
  short dchuff(short cat);
  short cat(short diff);
  short IdcHuff(int data);
  void iquantization(short** mat);
  void izigzag(short* block);

  //////////////////////////////////////////////////////

  char* convertToGrayScale(char* origImg, int width, int height);
  void getZigZagArray_with_ROI(double_t* realReturnArray, int macroBlockID);
  void getInverseZigZagArray_withROI(vector< vector<double_t> > inputArray, vector<int> vw);
  double_t* getOneIntegratedArray_withROI(double_t** inputArray, vector<int> vw);

  inline int getNumberOfRow() { return numberOfRow; };
  inline int getNumberOfColumn() { return numberOfColumn; };
  inline int getNumberOfMacroBlocks() { return numberOfMacroBlocks; };
  inline int getMacroBlockWidth() { return macroBlockWidth; };
  inline int getMacroBlockHeight() { return macroBlockHeight; };
  inline int getImageHeight() { return imageHeight; };
  inline int getImageWidth() { return imageWidth; };
  inline double_t getDeliveryRate() { return deliveryRate; };
  inline double_t getROIDeliveryRate() { return ROIdeliveryRate; };
  inline int getLengthOfOneMacroString() { return lengthOfOneMacroString; };
  inline int getROILengthOfOneMacroString() { return ROIlengthOfOneMacroString; };

  inline void setNumberOfRow() { numberOfRow = imageHeight / macroBlockHeight; };
  inline void setNumberOfColumn() { numberOfColumn = imageWidth / macroBlockWidth; };
  inline void setNumberOfMacroBlocks() { numberOfMacroBlocks = numberOfRow*numberOfColumn; };
  inline void setMacroBlockWidth(int mbwidth) { macroBlockWidth = mbwidth; };
  inline void setMacroBlockHeight(int mbwidth) { macroBlockHeight = mbwidth; };
  inline void setImageHeight(int imageHeight1) { imageHeight = imageHeight1; };
  inline void setImageWidth(int imageWidth1) { imageWidth = imageWidth1; };
  inline void setDeliveryRate(double_t dR) { deliveryRate = dR; };
  inline void setROIDeliveryRate(double_t dR) { ROIdeliveryRate = dR; };
  inline void setLengthOfOneMacroString() { lengthOfOneMacroString = (int)(macroBlockHeight * macroBlockWidth * deliveryRate); };
  inline void setLengthOfOneMacroString_withROI() { ROIlengthOfOneMacroString = (int)(macroBlockHeight * macroBlockWidth * 1.0); };

  void setMacroBPixel_withROI_ext(IplImage* grayImage, vector<int> vw);
  vector<double_t> getZigZagArray_with_ROI_ext();

  virtual ~cvideo();
  void init_cvideo();

private:
  int numberOfRow;//for now for 256 macro blocks
  int numberOfColumn;//for now for 256 macro blocks
  int numberOfMacroBlocks;//temp
  int macroBlockWidth;
  int macroBlockHeight;
  int lengthOfOneMacroString;
  int ROIlengthOfOneMacroString;
  int imageWidth;
  int imageHeight;
  double_t deliveryRate;
  double_t ROIdeliveryRate;
};
vector<int> FindImportantRegion(IplImage* inputArray, int macroBlockWidth, int macroBlockHeight);

#endif /* CVIDEO_H_ */

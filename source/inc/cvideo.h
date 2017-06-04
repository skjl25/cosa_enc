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
#include "utility.h"
#include "global.h"
#include "typedef.h"

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

  void_t setMacroBHolder();
  double_t* getOneIntegratedArray(double_t** inputArray);

  void_t setMacroBPixel(double_t** double_tOrgImage);
  void_t getDCTArray();
  double_t* getZigZagArray(int macroBlockID);
  double_t** setMacroBlocksToOriginalImage();
  void_t getInverseZigZagArray(double_t** inputArray);

  void_t setMacroBPixel_withROI(double_t** double_tOrgImage, vector<int> vw);

  void_t getIDCTArray();

  // frome here

  void_t quantization(short** matrix);
  void_t dc_encoding(short** mat, int** intmat, int* preblock);
  void_t ac_encoding(short** mat, int** intmat, int* preblock);
  void_t zigzag(short* block);
  void_t ac_decoding(short** mat, int** intmat, int* Ipreblock);
  int* IacHuff(int data);
  short symb2(short sym2, short cat);               // calculate the original data from symbol-2
  void_t dc_decoding(short** mat, int** intmat, int* Ipreblock);                 // function doing dc decoding;
  short symbol2(short diff);
  int count(short run, short cat);
  short achuff(short run, short cat);
  short dchuff(short cat);
  short cat(short diff);
  short IdcHuff(int data);
  void_t iquantization(short** mat);
  void_t izigzag(short* block);

  //////////////////////////////////////////////////////

  char* convertToGrayScale(char* origImg, int width, int height);
  void_t getZigZagArray_with_ROI(double_t* realReturnArray, int macroBlockID);
  void_t getInverseZigZagArray_withROI(vector< vector<double_t> > inputArray, vector<int> vw);
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

  inline void_t setNumberOfRow() { numberOfRow = imageHeight / macroBlockHeight; };
  inline void_t setNumberOfColumn() { numberOfColumn = imageWidth / macroBlockWidth; };
  inline void_t setNumberOfMacroBlocks() { numberOfMacroBlocks = numberOfRow*numberOfColumn; };
  inline void_t setMacroBlockWidth(int mbwidth) { macroBlockWidth = mbwidth; };
  inline void_t setMacroBlockHeight(int mbwidth) { macroBlockHeight = mbwidth; };
  inline void_t setImageHeight(int imageHeight1) { imageHeight = imageHeight1; };
  inline void_t setImageWidth(int imageWidth1) { imageWidth = imageWidth1; };
  inline void_t setDeliveryRate(double_t dR) { deliveryRate = dR; };
  inline void_t setROIDeliveryRate(double_t dR) { ROIdeliveryRate = dR; };
  inline void_t setLengthOfOneMacroString() { lengthOfOneMacroString = (int)(macroBlockHeight * macroBlockWidth * deliveryRate); };
  inline void_t setLengthOfOneMacroString_withROI() { ROIlengthOfOneMacroString = (int)(macroBlockHeight * macroBlockWidth * 1.0); };

  void_t setMacroBPixel_withROI_ext(IplImage* grayImage, vector<int> vw);
  vector<double_t> getZigZagArray_with_ROI_ext();

  virtual ~cvideo();
  void_t init_cvideo();

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

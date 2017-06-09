/*
image_tools header for the cosa_enc

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

#ifndef IMG_TOOLS_H_
#define IMG_TOOLS_H_

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "typedef.h"
#define MAX_1                   (255*255)

using namespace std;

class ImageTools {
public:
  void_t saveSequenceOfIplImages(char* filelocation, int num_sequence,
    IplImage* foregroundImg);
  void_t convertToGrayScale(char* origImg, char* grayG2ImgChar, uint32_t width,
    uint32_t height);
  //IplImage* convertCharGrayArrayToIplImage(char* orgArray, int height, int width);
  IplImage* convertColorArrayToIplImage(char* orgArray, uint32_t height,
    uint32_t width);
  IplImage* convertColorArrayToIplImage_ext(char* orgArray, uint32_t height,
    uint32_t width);
  void_t convertIplImageToCharArray(char* returnArray, IplImage* orgImage,
    uint32_t height, uint32_t width);
  void_t convertColorIplImageToCharArray(char* returnArray, IplImage* orgImage,
    uint32_t height, uint32_t width);

  double_t compute_euclidean_dist(int x1, int x2, int y1, int y2);

  IplImage* getSynthesizedImage_gray(IplImage* leftIm, IplImage* rightIm,
    int* dispMapMat, double_t alpha);
  IplImage* getSynthesizedImage_rgb(IplImage* leftIm, IplImage* rightIm,
    int** dispMapMat, double_t alpha);

  //void_t save_pgm(string, char*, unsigned int, unsigned int);
  char* load_pgm(string, unsigned int&, unsigned int&);
  void_t save_ppm(string, char*, uint32_t, uint32_t);
  char* load_ppm(string, unsigned int&, unsigned int&);
  void_t save_ppm_unsigned(string filename, unsigned char* data,
    uint32_t width, uint32_t height);
  void_t extractVideoSequenceToImg(char* movieFileLocation,
    char* saveFileLocation, char* saveFileType);
  double_t get_image_psnr(uint8_t *frame1, uint8_t *frame2, uint32_t width,
    uint32_t height);
  int compute_ipl_img_avg(IplImage* data);
  double_t compute_ipl_img_sdv(IplImage* data);

  template<class T> IplImage* convertGrayArrayToIplImage(T* orgArray, int width,
    int height) {
    IplImage* returnImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
    if (sizeof(T) == sizeof(int) || sizeof(T) == sizeof(short)) {
      int index = 0;
      for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
          int pixel = orgArray[index];
          ((uchar*)(returnImg->imageData + i*returnImg->widthStep))[j] = pixel;
          index++;
        }
      }
    }
    if (sizeof(T) == sizeof(char)) {
      int index = 0;
      for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
          char pixel = orgArray[index];
          ((uchar*)(returnImg->imageData + i*returnImg->widthStep))[j] = pixel;
          index++;
        }
      }
    }
    return returnImg;
  }
};
#endif /* IMG_TOOLS_ */

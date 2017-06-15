/*
Main for the cosa_enc

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

#include "../inc/global.h"
#include "../inc/cvideo.h"
#include "../inc/dct.h"
#include "../inc/enc_config.h"
#include "../inc/dec_config.h"
#include "../inc/cosa_common.h"

#pragma warning(disable:4819)
using namespace std;

#define RUN_INFINITE 1
void_t main() {
  Utility util;
  ImageTools image_tool;

  IplImage* ipl_rec_img = 0;
  IplImage* ipl_gray_img = 0;
  IplImage* ipl_org_img = 0;

  int dct_output[max_mb_size *max_mb_size];
  int zigzag_array[max_mb_size *max_mb_size];
  int izigzag_array[max_mb_size *max_mb_size];

  int** enc_mb = 0;
  int** dec_mb = 0;

  quantize_param qp_param;
  encoder_param enc_param;
  picture_param pic_param;

  ipl_org_img = cvLoadImage(input_file_name, 1);
  ipl_gray_img = cvCreateImage(cvSize(ipl_org_img->width, ipl_org_img->height), IPL_DEPTH_8U, 1);
  ipl_rec_img = cvCreateImage(cvSize(ipl_org_img->width, ipl_org_img->height), IPL_DEPTH_8U, 1);
  cvCvtColor(ipl_org_img, ipl_gray_img, CV_RGB2GRAY);

#if SAVE_IMAGE
  cvSaveImage(output_org_file_name, ipl_gray_img);
#endif

  uint32_t org_img_w = ipl_org_img->width;
  uint32_t org_img_h = ipl_org_img->height;
  uint32_t num_mb = (org_img_h / mb_size)*(org_img_w / mb_size);
  uint32_t mb_length = mb_size * mb_size;

  enc_mb = util.memset2DArray<int>(num_mb, mb_length);
  dec_mb = util.memset2DArray<int>(num_mb, mb_length);

  init_encoder(ipl_gray_img, enc_mb, &enc_param, &qp_param, &pic_param, mb_size);

#if RUN_INFINITE
  while(1){
#endif
  util.startTimer();

  for (uint32_t i = 0; i < num_mb; i++) {
    transform_img(dct_output, enc_mb[i], enc_param.tu_size);
    get_zigzag_array(zigzag_array, dct_output);

    get_quantize_parameter(zigzag_array, &qp_param, enc_param.tu_size);
    quantize(zigzag_array, qp_param, enc_param.tu_size);

    enc_param.blk_size = get_size_of_mb_block(zigzag_array, enc_param.tu_size);

    //printf("%d\n", enc_param.blk_size);

    ///////////////////////////////////////////////////////////////////////////

#if printf_en
    for (int k = 0; k < mb_size; k++) {
      for (int l = 0; l < mb_size; l++) {
        printf("%d ", zigzag_array[l + k*mb_size]);
      }
      printf("\n");
    }
    printf("---------------------\n");


    int cnt_freq_coef_array[max_mb_size *max_mb_size];
    int cnt_freq_coef_array_temp[max_mb_size *max_mb_size];
    int max_coef = 0;
    int max_coef_idx = 0;
    int most_freq_coef = 0;

    for (int j = 0; j < mb_size*mb_size; j++) {
      cnt_freq_coef_array[j] = zigzag_array[j];
      cnt_freq_coef_array_temp[j] = 0;
      max_coef = 0;
    }

    max_coef = 0;
    for (int j = 0; j < mb_size*mb_size; j++) {
      if (cnt_freq_coef_array[j] < 255) {
        for (int k = j + 1; k < mb_size*mb_size; k++) {
          if (cnt_freq_coef_array[j] == cnt_freq_coef_array[k]) {
            cnt_freq_coef_array[k] = 1000;
            cnt_freq_coef_array_temp[j] = cnt_freq_coef_array_temp[j] + 1;
          }
        }
        if (max_coef < cnt_freq_coef_array_temp[j]) {
          max_coef = cnt_freq_coef_array_temp[j];
          max_coef_idx = j;
        }
      }
    }

    most_freq_coef=zigzag_array[max_coef_idx];

    for(int k=0;k<mb_size;k++) {
      for (int l = 0; l < mb_size;l++) {
        printf("%d ",cnt_freq_coef_array[l+k*mb_size]);
      }
      printf("\n");
    }
    printf("---------------------\n");

    for (int k = 0; k < mb_size; k++) {
      for (int l = 0; l < mb_size; l++) {
        printf("%d ", cnt_freq_coef_array_temp[l + k*mb_size]);
      }
      printf("\n");
    }
    printf("most frequent coef : %d\n", zigzag_array[max_coef_idx]);
    getchar();

    for (int k = 0; k < mb_size; k++) {
      for (int l = 0; l < mb_size; l++) {
        printf("%d ", zigzag_array[l + k*mb_size]-most_freq_coef);
      }
      printf("\n");
    }
    getchar();
#endif

    ///////////////////////////////////////////////////////////////////////////


    //Maybe add secondary transformation to place more coefficients to the left top to prevent futher degradation
    //Utilize 4x4 as a tx for roi

    dequantize(zigzag_array, qp_param, enc_param.tu_size);
    get_izigzag_array(izigzag_array, zigzag_array);
    inv_transform_img(dec_mb[i], (int*)izigzag_array, enc_param.tu_size);
  }

  util.getElapsedTime();

  recon_picture_data(ipl_rec_img, dec_mb, enc_param.tu_size);

  double_t psnr = image_tool.get_image_psnr((uint8_t*)ipl_rec_img->imageData,
                                            (uint8_t*)ipl_gray_img->imageData,
                                            ipl_gray_img->width,
                                            ipl_gray_img->height);
  printf("PSNR is %f\n", psnr);

#if SAVE_IMAGE
  cvSaveImage("./output/fdct_result.pgm", ipl_rec_img);
#endif

#if RUN_INFINITE
  }
#endif
  getchar();

#if 0
  ///////////////
  IplImage* depthCVImage;
  IplImage* foregroundImg;
  int imageSequence = 0;
  int globalCounter = 130;
  cvideo encoder;
  encoder.init_cvideo();

  vector< vector<double_t> > vec;

  int threshold = encoder.getMacroBlockWidth()*encoder.getMacroBlockHeight()*encoder.getDeliveryRate();
  zigzaggedArrays = util.memset2DArray<double_t>(encoder.getNumberOfMacroBlocks(), threshold);
  gray_ipl_image = cvCreateImage(cvSize(Width, Height), IPL_DEPTH_8U, 1);

  while (1) {
    if (globalCounter == 45) {
      exit(1);
    }
    char* fileName2 = new char[100];
    sprintf(fileName2, "C:/test_set/input/foreground_mask_%d.pgm", globalCounter);
    foregroundImg = cvLoadImage(fileName2, 1);

    char* fileName = new char[100];
    sprintf(fileName, "C:/test_set/input/original_%d.pgm", globalCounter);
    color_ipl_image = cvLoadImage(fileName, 1);

    cvCvtColor(color_ipl_image, gray_ipl_image, CV_RGB2GRAY);

    //cvShowImage("depthShow", foregroundImg);
    //cvShowImage("colorCVImg", gray_ipl_image);
    //	cvWaitKey(1);

    /////////////////////////-for saving the file////--start
    //	globalCounter++;
    //	printf("Saved %d images\n",globalCounter);
    /////////////////////////-for saving the file////--end

    vector<int> ROIIndex = FindImportantRegion(foregroundImg, 4, 4);

    //for converting color to gray image
    //	save_pgm("/home/sklee25/Desktop/test.pgm",orgImage,Width,Height);

    encoder.setMacroBPixel_withROI_ext(gray_ipl_image, ROIIndex);

    encoder.getDCTArray();

    std::vector<double_t> dataVector;

    for (int i = 0; i < encoder.getNumberOfMacroBlocks(); i++) {
      encoder.getZigZagArray_with_ROI(zigzaggedArrays[i], i);
      if (encoder.macroBHolder[i].ROIFlag == 0) {
        int threshold = (int)(encoder.getMacroBlockWidth() * encoder.getMacroBlockHeight() * encoder.getDeliveryRate());
        for (int j = 0; j < threshold; j++) {
          dataVector.push_back(zigzaggedArrays[i][j]);
        }
      } else {
        int threshold = (int)(encoder.getMacroBlockWidth() * encoder.getMacroBlockHeight() * encoder.getROIDeliveryRate());
        for (int j = 0; j < threshold; j++) {
          dataVector.push_back(zigzaggedArrays[i][j]);
        }
      }
    }

    //for testing reducing the data size
    ///////////////////////////////////////////////////////////////////
    double_t min = *min_element(dataVector.begin(), dataVector.end());
    double_t max = *max_element(dataVector.begin(), dataVector.end());
    double_t reduceRatio = (abs(min) + max) / 255;
    double_t intermediateVal = abs(min) / reduceRatio;

    //	cout << "Min val is " << min << endl;
    //	cout << "Max val is " <<  max << endl;
    //	cout << "Reduce ratio val is " <<  reduceRatio << endl;
    //	cout << "Intermediate val is " <<  intermediateVal << endl;
    //	cout << "Min new Val " <<  min/reduceRatio+intermediateVal << endl;
    //	cout << "Max new Val " <<  max/reduceRatio+intermediateVal << endl;

    //	getchar();
    std::vector<int> quantized_data;

    //Creating the reduced data -start
    for (int i = 0; i < dataVector.size(); i++) {
      quantized_data.push_back(int(dataVector[i] / reduceRatio + intermediateVal));
    }
    //	double newmin=*min_element(testVector.begin(),testVector.end());
    //	double newmax=*max_element(testVector.begin(),testVector.end());
    //	cout << "Min new Val " <<  newmin << endl;
    //	cout << "Max new Val " <<  newmax << endl;
    //	getchar();
    //Creating the reduced data -end

    //decompress the reduced data -start
    vector<double_t>dataVectorTest;
    for (int i = 0; i < quantized_data.size(); i++) {
      double_t restoredVal = (quantized_data[i] - intermediateVal)*reduceRatio;
      dataVectorTest.push_back(restoredVal);
    }

    ////////////////////////////////////////////////////////

    cvideo decoder;
    decoder.setImageHeight(Height);
    decoder.setImageWidth(Width);
    decoder.setMacroBlockHeight(macroBlockSize);
    decoder.setMacroBlockWidth(macroBlockSize);
    decoder.setDeliveryRate(nonROIDeliver);
    decoder.setROIDeliveryRate(ROIDeliver);

    decoder.setNumberOfColumn();
    decoder.setNumberOfRow();
    decoder.setNumberOfMacroBlocks();

    decoder.setLengthOfOneMacroString();
    decoder.setLengthOfOneMacroString_withROI();
    decoder.setMacroBHolder();

    int receivedVectorSize = ROIIndex.size() + dataVectorTest.size();

    double_t** zigzaggedArraysReceiver = new double_t*[decoder.getNumberOfMacroBlocks()];

    for (int i = 0; i < decoder.getNumberOfMacroBlocks(); i++) {
      if (std::find(ROIIndex.begin(), ROIIndex.end(), i) != ROIIndex.end()) {
        zigzaggedArraysReceiver[i] = new double_t[(int)(decoder.getMacroBlockWidth() * decoder.getMacroBlockHeight())];
      } else {
        int size = (int)(decoder.getMacroBlockWidth() * decoder.getMacroBlockHeight()*nonROIDeliver);
        zigzaggedArraysReceiver[i] = new double_t[size];
      }
    }
    int indexVectorReceiver = 0;
    for (int i = 0; i < decoder.getNumberOfMacroBlocks(); i++) {
      if (std::find(ROIIndex.begin(), ROIIndex.end(), i) != ROIIndex.end()) {
        vector<double_t> row;
        for (int j = 0; j < (decoder.getMacroBlockWidth() * decoder.getMacroBlockHeight()*decoder.getROIDeliveryRate()); j++) {
          //					zigzaggedArraysReceiver[i][j]=dataVector[indexVectorReceiver];
          row.push_back(dataVector[indexVectorReceiver]);
          indexVectorReceiver++;
        }
        vec.push_back(row);
      } else {
        int threshold = (int)(decoder.getMacroBlockWidth() * decoder.getMacroBlockHeight()*nonROIDeliver);
        vector<double_t> row;
        for (int j = 0; j < threshold; j++) {
          double_t vectorVal = dataVector[indexVectorReceiver];
          //					zigzaggedArraysReceiver[i][j]=vectorVal;
          row.push_back(vectorVal);
          indexVectorReceiver++;
        }
        vec.push_back(row);
      }
    }

    decoder.getInverseZigZagArray_withROI(vec, ROIIndex);
    //	decoder.getInverseZigZagArray_withROI(zigzaggedArrays,ROIIndex);
    //decoder.getInverseZigZagArray(macroStrings);
    //	decoder.getInverseZigZagArray_withROI(macroStrings,ROIIndex);

    decoder.getIDCTArray();

    double_t** test = decoder.setMacroBlocksToOriginalImage();
    char* finalImage = 0;

    finalImage = util.convertDoubleToUintChar1D(test, decoder.getImageWidth(), decoder.getImageHeight());

    char* file_name_output = new char[100];
    sprintf(file_name_output, "C:/test_set/output/result_%f_%d.pgm", nonROIDeliver, globalCounter);
    ImageTools ip;
    IplImage* ipl_final = ip.convertGrayArrayToIplImage(finalImage, 640, 480);
    cvSaveImage(file_name_output, ipl_final);
    cout << "Done with DCT-Cut" << endl;
    globalCounter++;
    delete[] finalImage;
    vector< vector<double_t> >().swap(vec);
    vector<double_t>().swap(dataVector);
    vector<double_t>().swap(dataVector);
    vector<int>().swap(ROIIndex);
  }
  cvWaitKey(2);
#endif
}
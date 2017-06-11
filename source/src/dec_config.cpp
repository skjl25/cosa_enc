/*
decoder configuration code for the cosa_enc

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

#include "../inc/dec_config.h"

void_t recon_picture_data(IplImage* ipl_dec_img, int** dec_mb) {
  uint32_t mb_idx = 0;
  uint32_t dst_offset = ipl_dec_img->widthStep;
  char* dec_img = ipl_dec_img->imageData;

  for (int j = 0; j < ipl_dec_img->height; j += mb_size) {
    for (int i = 0; i < ipl_dec_img->width; i += mb_size) {
      for (int k = 0; k < mb_size; k++) {
        for (int l = 0; l < mb_size; l++) {
          dec_img[(dst_offset * (j + k)) + (i + l)] = (char)dec_mb[mb_idx][(k * mb_size) + l];
        }
      }
      mb_idx++;
    }
  }
}


void_t inv_transform_img(int* dst, int* src, int shift_1st, int shift_2nd,
                         int tu_size) {
  int coef_inv [max_mb_size *max_mb_size];

  if(tu_size == 4) {
    get_idct4_hevc(coef_inv, src, shift_1st, mb_size);
    get_idct4_hevc(dst, coef_inv, shift_2nd, mb_size);
  }
  else if (tu_size == 8) {
    get_idct8_hevc(coef_inv, src, shift_1st, mb_size);
    get_idct8_hevc(dst, coef_inv, shift_2nd, mb_size);
  }
  else if (tu_size == 16) {
    get_idct16_hevc(coef_inv, src, shift_1st, mb_size);
    get_idct16_hevc(dst, coef_inv, shift_2nd, mb_size);
  }
  else if (tu_size == 32) {
    get_idct32_hevc(coef_inv, src, shift_1st, mb_size);
    get_idct32_hevc(dst, coef_inv, shift_2nd, mb_size);
  }

}

void_t dequantize(int* src, quantize_param qp_param, int tu_size) {
  for (int j = 0; j < mb_size*mb_size; j++) {
    double_t restoredVal = (double_t)(src[j] - qp_param.intermediate_val)*qp_param.reduce_ratio;
    //dataVectorTest.push_back(restoredVal);
    src[j] = (int)restoredVal;
  }
}

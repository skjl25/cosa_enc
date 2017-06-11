/*
encoder configuration for the cosa_enc

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

#include "../inc/enc_config.h"

void_t read_picture_data(IplImage* ipl_src_img, int** enc_mb) {
  uint32_t mb_idx = 0;
  uint32_t src_offset = ipl_src_img->widthStep;
  char* src_img = ipl_src_img->imageData;

  for (int j = 0; j < ipl_src_img->height; j += mb_size) {
    for (int i = 0; i < ipl_src_img->width; i += mb_size) {
      for (int k = 0; k < mb_size; k++) {
        for (int l = 0; l < mb_size; l++) {
          enc_mb[mb_idx][k * mb_size + l] = src_img[(src_offset * (j + k)) + (i + l)];
        }
      }
      mb_idx++;
    }
  }
}

void_t transform_img(int* dst, int* src, int shift_1st, int shift_2nd, int tu_size) {
  //int* coef = (int*)malloc(sizeof(int) * mb_size * mb_size);
  int coef [max_mb_size *max_mb_size];

  if(tu_size == 4) {
    get_dct4_hevc(coef, src, shift_1st, mb_size);
    get_dct4_hevc(dst, coef, shift_2nd, mb_size);
  }
  else if (tu_size == 8) {
    get_dct8_hevc(coef, src, shift_1st, mb_size);
    get_dct8_hevc(dst, coef, shift_2nd, mb_size);
  }
  else if (tu_size == 16) {
    get_dct16_hevc(coef, src, shift_1st, mb_size);
    get_dct16_hevc(dst, coef, shift_2nd, mb_size);
  }
  else if (tu_size == 32) {
    get_dct32_hevc(coef, src, shift_1st, mb_size);
    get_dct32_hevc(dst, coef, shift_2nd, mb_size);
  }
}

void_t get_quantize_parameter(int* src, quantize_param* qp_param){
  int min = find_min(src, mb_size);
  int max = find_max(src, mb_size);

  qp_param->reduce_ratio = (double_t)(abs(min) + max) / 255;
  qp_param->intermediate_val = abs(min) / qp_param->reduce_ratio;
}

void_t quantize(int* src, quantize_param qp_param, int tu_size) {
  for (int j = 0; j < mb_size*mb_size; j++) {
    src[j] = (int(src[j] / qp_param.reduce_ratio + qp_param.intermediate_val));
  }
}

int* encode_blk() {
  return 0;
}

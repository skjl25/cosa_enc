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

void set_picture_data(IplImage* ipl_src_img, encoder_param* enc_param, 
					  picture_param* pic_param) {
  uint32_t mb_idx = 0;
  uint32_t src_offset = pic_param->org_img_width;
  char* src_img = ipl_src_img->imageData;

  for (int j = 0; j < pic_param->org_img_height; j += enc_param->tu_size) {
    for (int i = 0; i < pic_param->org_img_width; i += enc_param->tu_size) {
      for (int k = 0; k < enc_param->tu_size; k++) {
        for (int l = 0; l < enc_param->tu_size; l++) {
          enc_param->src_data[mb_idx][k * enc_param->tu_size + l] =
                                      src_img[(src_offset * (j + k)) + (i + l)];
        }
      }
      mb_idx++;
    }
  }
}

void transform_img(int* dst, int* src, int tu_size) {
  //int* coef = (int*)malloc(sizeof(int) * mb_size * mb_size);
  int coef[max_mb_size *max_mb_size];
  int shift_1st = 0;
  int shift_2nd = 0;

  if (tu_size == 4) {
    shift_1st = 1 + X265_DEPTH - 8;
    shift_2nd = 8;
    get_dct4_hevc(coef, src, shift_1st, tu_size);
    get_dct4_hevc(dst, coef, shift_2nd, tu_size);
  } else if (tu_size == 8) {
    shift_1st = 2 + X265_DEPTH - 8;
    shift_2nd = 9;
    get_dct8_hevc(coef, src, shift_1st, tu_size);
    get_dct8_hevc(dst, coef, shift_2nd, tu_size);
  } else if (tu_size == 16) {
    shift_1st = 3 + X265_DEPTH - 8;
    shift_2nd = 10;
    get_dct16_hevc(coef, src, shift_1st, tu_size);
    get_dct16_hevc(dst, coef, shift_2nd, tu_size);
  } else if (tu_size == 32) {
    shift_1st = 4 + X265_DEPTH - 8;
    shift_2nd = 11;
    get_dct32_hevc(coef, src, shift_1st, tu_size);
    get_dct32_hevc(dst, coef, shift_2nd, tu_size);
  }
}

inline void get_quantize_parameter(int* src, quantize_param* qp_param, int tu_size) {
  int min = find_min(src, tu_size);
  int max = find_max(src, tu_size);

  qp_param->reduce_ratio = (double_t)(abs(min) + max) / 255;
  qp_param->intermediate_val = abs(min) / qp_param->reduce_ratio;
}

inline void quantize_data(int* src, quantize_param* qp_param, int tu_size) {
  for (int j = 0; j < tu_size*tu_size; j++) {
    src[j] = (int(src[j] / qp_param->reduce_ratio + qp_param->intermediate_val));
  }
}

void quantize(int* src, encoder_param* enc_param, quantize_param* qp_param) {
  int tu_size = enc_param->tu_size;
  get_quantize_parameter(src, qp_param, tu_size);
  quantize_data(src, qp_param, tu_size);
  enc_param->blk_size = get_size_of_mb_block(src, tu_size);
}

void set_picture_parameter(IplImage* src_img, picture_param* pic_param) {
  pic_param->org_img_height = src_img->height;
  pic_param->org_img_width  = src_img->width;
}

void set_picture_parameter(yuv_video* src_img, picture_param* pic_param) {
	pic_param->org_img_height = src_img->height;
	pic_param->org_img_width = src_img->width;
}


void init_encoder_param(picture_param* pic_param, encoder_param* enc_param,
                        int tu_size) {
  Utility util;
  enc_param->blk_size = 0;
  enc_param->tu_size = tu_size;
  enc_param->num_mb = (pic_param->org_img_height / tu_size)*
                      (pic_param->org_img_width / tu_size);
  enc_param->mb_length = tu_size * tu_size;
  enc_param->roi_flag = util.memset1DArray<int>(enc_param->mb_length);
  enc_param->src_data = util.memset2DArray<int>(enc_param->num_mb,
                                                enc_param->mb_length);
  enc_param->enc_data = util.memset2DArray<int>(enc_param->num_mb,
									        	enc_param->tu_size*enc_param->tu_size);
  enc_param->qp_param = util.memset1DArray<quantize_param>(enc_param->num_mb);
  enc_param->delivery_ratio = DELIVERY_RATIO_NON_ROI;

  for (int i = 0; i < enc_param->num_mb; i++) {
	  init_qp_param(&enc_param->qp_param[i]);
  }
}

void init_encoder(IplImage* src_img, encoder_param* enc_param,
                  picture_param* pic_param, int tu_size) {
	set_picture_parameter(src_img, pic_param);
	init_encoder_param(pic_param, enc_param, tu_size);
}

void init_encoder(yuv_video* src_img, encoder_param* enc_param,
				  picture_param* pic_param, int tu_size) {
	set_picture_parameter(src_img, pic_param);
	init_encoder_param(pic_param, enc_param, tu_size);
}


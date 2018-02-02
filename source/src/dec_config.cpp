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

void recon_picture_data(IplImage* ipl_dec_img, decoder_param* dec_param) {
  uint32_t mb_idx = 0;
  uint32_t dst_offset = ipl_dec_img->widthStep;
  char* dec_img = ipl_dec_img->imageData;

  for (int j = 0; j < ipl_dec_img->height; j += dec_param->tu_size) {
    for (int i = 0; i < ipl_dec_img->width; i += dec_param->tu_size) {
      for (int k = 0; k < dec_param->tu_size; k++) {
        for (int l = 0; l < dec_param->tu_size; l++) {
          dec_img[(dst_offset * (j + k)) + (i + l)] = 
            (char)dec_param->recon_data[mb_idx][(k * dec_param->tu_size) + l];
        }
      }
      mb_idx++;
    }
  }
}


void inv_transform_img(int* dst, int* src, int tu_size) {
  int coef_inv[max_mb_size *max_mb_size];

  int shift_1st = 7;
  int shift_2nd = 12 - (X265_DEPTH - 8);;

  if (tu_size == 4) {
    get_idct4_hevc(coef_inv, src, shift_1st, tu_size);
    get_idct4_hevc(dst, coef_inv, shift_2nd, tu_size);
  } else if (tu_size == 8) {
    get_idct8_hevc(coef_inv, src, shift_1st, tu_size);
    get_idct8_hevc(dst, coef_inv, shift_2nd, tu_size);
  } else if (tu_size == 16) {
    get_idct16_hevc(coef_inv, src, shift_1st, tu_size);
    get_idct16_hevc(dst, coef_inv, shift_2nd, tu_size);
  } else if (tu_size == 32) {
    get_idct32_hevc(coef_inv, src, shift_1st, tu_size);
    get_idct32_hevc(dst, coef_inv, shift_2nd, tu_size);
  }

}

inline void dequantize_data(int* src, quantize_param qp_param, int tu_size) {
  for (int j = 0; j < tu_size*tu_size; j++) {
    src[j] = (int)(src[j] - qp_param.intermediate_val)*qp_param.reduce_ratio;
  }
}

void dequantize(int* src, quantize_param qp_param, int tu_size) {
  dequantize_data(src, qp_param, tu_size);
}

void init_decoder_param(picture_param* pic_param, decoder_param* dec_param,
                        int tu_size) {
  Utility util;
  dec_param->blk_size = 0;
  dec_param->tu_size = tu_size;
  dec_param->num_mb = (pic_param->org_img_height / tu_size)*
                      (pic_param->org_img_width / tu_size);
  dec_param->mb_length = tu_size * tu_size;
  dec_param->roi_flag = util.memset1DArray<int>(dec_param->mb_length);

  dec_param->recon_data = util.memset2DArray<int>(dec_param->num_mb,
                                                dec_param->mb_length);
  dec_param->dec_data = util.memset2DArray<int>(dec_param->num_mb,
										   	    dec_param->mb_length);

  dec_param->qp_param = util.memset1DArray<quantize_param>(dec_param->num_mb);

  for (int i = 0; i < dec_param->num_mb; i++) {
	  init_qp_param(&dec_param->qp_param[i]);
  }

}

void init_decoder(decoder_param* dec_param, picture_param* pic_param, 
				  int tu_size) {
  init_decoder_param(pic_param, dec_param, tu_size);
}


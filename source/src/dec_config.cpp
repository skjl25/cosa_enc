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


void_t inv_transform_img(int* dst, int* src, int shift_1st, int shift_2nd, int tu_size) {
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

void_t dequantize(int* src, double_t reduce_ratio, double_t intermediate_val, int tu_size) {
  for (int j = 0; j < mb_size*mb_size; j++) {
    double_t restoredVal = (double_t)(src[j] - intermediate_val)*reduce_ratio;
    //dataVectorTest.push_back(restoredVal);
    src[j] = (int)restoredVal;
  }
}

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

void_t quantize(int* src, double_t reduce_ratio, double_t intermediate_val, int tu_size) {
  for (int j = 0; j < mb_size*mb_size; j++) {
    src[j] = (int(src[j] / reduce_ratio + intermediate_val));
  }
}

int* encode_blk() {
  return 0;
}

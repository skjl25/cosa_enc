#ifndef DEC_CONIFG_H_
#define DEC_CONIFG_H_

#include "global.h"
#include "typedef.h"
#include "../inc/dct.h"


void_t recon_picture_data(IplImage* ipl_src_img, int** enc_mb);
void_t inv_transform_img(int* dst, int* src, int shift_1st, int shift_2nd, int tu_size);
void_t dequantize(int* src, double_t reduce_ratio, double_t intermediate_val, int tu_size);

#endif /* DEC_CONIFG_H_ */
/*
cosa_common header for the cosa_enc

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


#ifndef COSA_COMMON_H_
#define COSA_COMMON_H_

#include "global.h"
#include "../../../utility/inc/typedef.h"
#include "../../../utility/inc/utility.h"
#include "../../../utility/inc/img_tools.h"

struct quantize_param{
  double_t reduce_ratio;
  double_t intermediate_val;
};

struct encoder_param{
  int tu_size;
  int blk_size;
  int** src_data;
  int** enc_data;
  int num_mb;
  int mb_length;
  int* roi_flag;
  double delivery_ratio;
  quantize_param* qp_param;
};

struct decoder_param{
  int tu_size;
  int blk_size;
  int** dec_data;
  int** recon_data;
  int num_mb;
  int mb_length;
  int* roi_flag;
  double delivery_ratio;
  quantize_param* qp_param;
  //quantize_param qp_param;
};

struct picture_param{
  int org_img_width;
  int org_img_height;
};


int find_max(int* src, int blk_size);
int find_min(int* src, int blk_size);

int check_non_zero_all_block(int* src, int blk_size);
int check_non_zero_quarter_block(int* src, int blk_size);
int check_non_zero_half_block(int* src, int blk_size);
int check_zero_by_partition_block(int* src, int blk_size);
int get_size_of_mb_block(int* src, int blk_size);
void init_qp_param(quantize_param* qp_param);


#endif /* COSA_COMMON_H_ */
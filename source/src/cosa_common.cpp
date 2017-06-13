/*
cosa_common for the cosa_enc

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

#include "../inc/cosa_common.h"

int find_max(int* src, int blk_size) {
  int max = src[0];
  for (int i = 0; i < blk_size*blk_size; i++) {
    if (src[i] > max) {
      max = src[i];
    }
  }
  return max;
}

int find_min(int* src, int blk_size) {
  int min = src[0];
  for (int i = 0; i < blk_size*blk_size; i++) {
    if (src[i] < min) {
      min = src[i];
    }
  }
  return min;
}

int check_non_zero_all_block(int* src, int blk_size) {
  int non_zero_flag = 0;

  for (int j = 0; j < blk_size*blk_size; j++) {
    if (src[j] != 0) {
      non_zero_flag = 1;
      return non_zero_flag;
    }
  }
  return non_zero_flag;

}

// check if the 3/4 of the block is 0 coefficients
int check_non_zero_quarter_block(int* src, int blk_size) {
  int non_zero_flag = 0;

  for (int j = blk_size - 1; j < blk_size*blk_size; j++) {
    if (src[j] != 0) {
      non_zero_flag = 1;
      return non_zero_flag;
    }
  }
  return non_zero_flag;
}

int check_non_zero_half_block(int* src, int blk_size) {
  int non_zero_flag = 0;

  for (int j = (blk_size * 2) - 1; j < blk_size*blk_size; j++) {
    if (src[j] != 0) {
      non_zero_flag = 1;
      return non_zero_flag;
    }
  }
  return non_zero_flag;

}

// return val is 0 : all of the block is zero
// return val is 1 : two thirds of the block is zero
// retrun val is 2 : half of the block is zero
// return val is 3 : all coefficients in the block is non-zero
int check_zero_by_partition_block(int* src, int blk_size) {
  int non_zero_flag = check_non_zero_all_block(src, mb_size);

  if (non_zero_flag == 0) {
    return 0;
  } else {
    int non_zero_one_quarter_flag = check_non_zero_quarter_block(src, mb_size);
    if (non_zero_one_quarter_flag == 0) {
      return 1;
    } else {
      int non_zero_half_flag = check_non_zero_half_block(src, mb_size);

      if (non_zero_half_flag == 0) {
        return 2;
      } else {
        return 3;
      }
    }
  }
}

int get_size_of_mb_block(int* src, int blk_size) {
  int non_zero_flag_val = 0;
  non_zero_flag_val = check_zero_by_partition_block(src, blk_size);

  if (non_zero_flag_val == 0) {
    return 0;
  } else if (non_zero_flag_val == 1) {
    return blk_size;
  } else if (non_zero_flag_val == 2) {
    return (blk_size*blk_size) / 2;
  } else {
    return blk_size*blk_size;
  }

}
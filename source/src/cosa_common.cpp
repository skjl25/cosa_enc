#include "../inc/cosa_common.h"

int find_max(int* src, int blk_size) {
  int max = src[0];
  for(int i=0; i < blk_size*blk_size; i++) {
    if (src[i] > max) {
        max = src[i];
    }
  }
  return max;
}


int find_min(int* src, int blk_size) {
  int min = src[0];
  for(int i=0; i < blk_size*blk_size; i++) {
    if (src[i] < min) {
        min = src[i];
    }
  }
  return min;
}

int non_zero_check(int* src, int blk_size) {
  int non_zero_flag = 0;

  for (int j = 0; j < blk_size*blk_size; j++) {
    if (src[j] != 0) {
      non_zero_flag = 1;
      return non_zero_flag;
    }
  }
  return non_zero_flag;

}


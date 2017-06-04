#ifndef COSA_COMMON_H_
#define COSA_COMMON_H_

#include "global.h"
#include "typedef.h"



int non_zero_check(int* src, int blk_size);
int find_max(int* src, int blk_size);
int find_min(int* src, int blk_size);

#endif /* COSA_COMMON_H_ */
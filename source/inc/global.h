/*
global header for the cosa_enc

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

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <assert.h>
#include <limits>
#include "../../../utility/inc/utility.h"
#include "../../../utility/inc/img_tools.h"
#include "../../../utility/inc/typedef.h"


#define ROUND( a )            ( ( (a) < 0 ) ? (int) ( (a) - 0.5 ) : \
                      		      (int) ( (a) + 0.5 ) )

#define image_sequence          130

#define input_file_name "./input/1920x1024_test.jpg"

#define output_org_file_name "./output/fdct_org.pgm"

// #define input_file_name "./input/fhd_test_10.jpg"
// #define input_file_name "./input/fhd_test_9.jpg"
// #define input_file_name "./input/fhd_test_8.jpg"
// #define input_file_name "./input/fhd_test_7.jpg"
// #define input_file_name "./input/fhd_test_6.jpg"
// #define input_file_name "./input/fhd_test_5.jpg"
// #define input_file_name "./input/fhd_test_4.jpg"
// #define input_file_name "./input/fhd_test_3.jpg"
// #define input_file_name "./input/fhd_test_2.jpg"
// #define input_file_name "./input/fhd_test.jpg"
// #define input_file_name "./input/original_100.pgm"


#define IMG_WIDTH               1920
#define IMG_HEIGHT              1024
#define X265_DEPTH              8
//#define X265_DEPTH 10
//#define X265_DEPTH 12

#define PI                      3.14159265358979
#define SAVE_IMAGE              1
#define printf_en               0

#define mb_size                 4
#define min_mb_size             4
#define max_mb_size             32

#define DELIVERY_RATIO_NON_ROI  1.0
#define DELIVERY_RATIO_ROI      1.0

#endif /* GLOBAL_H_ */

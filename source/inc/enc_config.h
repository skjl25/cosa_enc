/*
encoder configuration header for the cosa_enc

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

#ifndef ENC_CONIFG_H_
#define ENC_CONIFG_H_

#include "global.h"
#include "../../../utility/inc/typedef.h"
#include "../../../utility/inc/utility.h"

#include "dct.h"
#include "cosa_common.h"

void set_picture_data(IplImage* ipl_src_img, int** enc_mb, int tu_size);

void transform_img(int* dst, int* src, int tu_size);

void get_quantize_parameter(int* src, quantize_param* qp_param, int tu_size);
void quantize(int* src, quantize_param* qp_param, encoder_param* enc_param);

void init_encoder(IplImage* src_img, encoder_param* enc_param,
                    quantize_param* qp_param, picture_param* pic_param,
                    int tu_size);

#endif /* ENC_CONIFG_H_ */
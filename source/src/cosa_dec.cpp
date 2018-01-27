/*
Main for the cosa_enc

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

#include "../inc/cosa_dec.h"

int izigzag_array[max_mb_size *max_mb_size];


void decode_picture(decoder_param* dec_param) {
	for (int i = 0; i < dec_param->num_mb; i++) {
		//Maybe add secondary transformation to place more coefficients to the left top to prevent futher degradation
		//Utilize 4x4 as a tx for roi
		dequantize(dec_param->dec_data[i], dec_param->qp_param[i], dec_param->tu_size);
		set_inverse_scan_oder(izigzag_array, dec_param->dec_data[i]);
		inv_transform_img(dec_param->recon_data[i], (int*)izigzag_array, dec_param->tu_size);
	}
}

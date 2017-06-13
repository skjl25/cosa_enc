/*
transformation code for the cosa_enc

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

#include "../inc/dct.h"

//struct zigzag_4x4 {
//  int row;
//  int col;
//} zigzag_4x4[4 * 4] = {
//  { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 },
//  { 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 },
//  { 2, 0 }, { 2, 1 }, { 2, 2 }, { 2, 3 },
//  { 3, 0 }, { 3, 1 }, { 3, 2 }, { 3, 3 }
//};

//0, 0    1, 0     2, 0     3, 0
//0, 1    1, 1     2, 1     3, 1
//0, 2    1, 2    2, 2      3, 2
//0, 3     1, 3    2, 3     3, 3
struct zigzag_4x4 {
  int row;
  int col;
} zigzag_4x4[4 * 4] = {
  { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 },
  { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 },
  { 0, 2 }, { 1, 2 }, { 2, 2 }, { 3, 2 },
  { 0, 3 }, { 1, 3 }, { 2, 3 }, { 3, 3 }
};

struct zigzag_8x8 {
  int row;
  int col;
} zigzag_8x8[8 * 8] =
{
  { 0, 0 },
  { 0, 1 }, { 1, 0 },
  { 2, 0 }, { 1, 1 }, { 0, 2 },
  { 0, 3 }, { 1, 2 }, { 2, 1 }, { 3, 0 },
  { 4, 0 }, { 3, 1 }, { 2, 2 }, { 1, 3 }, { 0, 4 },
  { 0, 5 }, { 1, 4 }, { 2, 3 }, { 3, 2 }, { 4, 1 }, { 5, 0 },
  { 6, 0 }, { 5, 1 }, { 4, 2 }, { 3, 3 }, { 2, 4 }, { 1, 5 }, { 0, 6 },
  { 0, 7 }, { 1, 6 }, { 2, 5 }, { 3, 4 }, { 4, 3 }, { 5, 2 }, { 6, 1 }, { 7, 0 },
  { 7, 1 }, { 6, 2 }, { 5, 3 }, { 4, 4 }, { 3, 5 }, { 2, 6 }, { 1, 7 },
  { 2, 7 }, { 3, 6 }, { 4, 5 }, { 5, 4 }, { 6, 3 }, { 7, 2 },
  { 7, 3 }, { 6, 4 }, { 5, 5 }, { 4, 6 }, { 3, 7 },
  { 4, 7 }, { 5, 6 }, { 6, 5 }, { 7, 4 },
  { 7, 5 }, { 6, 6 }, { 5, 7 },
  { 6, 7 }, { 7, 6 },
  { 7, 7 }
};

void_t izigzag_4x4_1d(int16_t* raw_input_data, int* output_data, int* Quantum_1d) {
  int i;
  int row;
  int col;
  int N = mb_size;

  for (i = 0; i < (N * N); i++) {
    if (mb_size == 4) {
      row = zigzag_4x4[i].row;
      col = zigzag_4x4[i].col;
    } else {
      row = zigzag_8x8[i].row;
      col = zigzag_8x8[i].col;
    }
    output_data[row*N + col] = raw_input_data[i] * Quantum_1d[row*N + col];
  }
}

void_t zigzag_4x4_1d(double* output_data, int16_t* result_out_put_data, int* Quantum_1d) {
  int i;
  int row;
  int col;
  double_t result;
  int N = mb_size;

  for (i = 0; i < (N * N); i++) {
    if (mb_size == 4) {
      row = zigzag_4x4[i].row;
      col = zigzag_4x4[i].col;
    } else {
      row = zigzag_8x8[i].row;
      col = zigzag_8x8[i].col;
    }

    result = output_data[row*N + col] / Quantum_1d[row*N + col];
    result_out_put_data[i] = ROUND(result);
  }
}

void_t izigzag_4x4_2d(int16_t* raw_input_data, int** output_data, int** Quantum_2d) {
  int i;
  int row;
  int col;
  int N = mb_size;

  for (i = 0; i < (N * N); i++) {
    row = zigzag_4x4[i].row;
    col = zigzag_4x4[i].col;
    //row = zigzag_8x8[i].row;
    //col = zigzag_8x8[i].col;
    output_data[row][col] = raw_input_data[i] * Quantum_2d[row][col];
  }
}

void_t zigzag_4x4_2d(int** output_data, int16_t* result_out_put_data, int** Quantum_2d) {
  int i;
  int row;
  int col;
  double_t result;
  int N = mb_size;

  for (i = 0; i < (N * N); i++) {
    row = zigzag_4x4[i].row;
    col = zigzag_4x4[i].col;
    //row = zigzag_8x8[i].row;
    //col = zigzag_8x8[i].col;
    result = output_data[row][col] / Quantum_2d[row][col];
    result_out_put_data[i] = ROUND(result);
  }
}

void_t get_dct4_hevc(int *dst, int *src, int shift, int line) {
  int j;
  int E[2], O[2];
  int add = (shift > 0) ? (1 << (shift - 1)) : 0;

  for (j = 0; j < line; j++) {
    /* E and O */
    E[0] = src[0] + src[3];
    O[0] = src[0] - src[3];
    E[1] = src[1] + src[2];
    O[1] = src[1] - src[2];

    dst[0] = (g_aiT4[TRANSFORM_FORWARD][0][0] * E[0] + g_aiT4[TRANSFORM_FORWARD][0][1] * E[1] + add) >> shift;
    dst[2 * line] = (g_aiT4[TRANSFORM_FORWARD][2][0] * E[0] + g_aiT4[TRANSFORM_FORWARD][2][1] * E[1] + add) >> shift;
    dst[line] = (g_aiT4[TRANSFORM_FORWARD][1][0] * O[0] + g_aiT4[TRANSFORM_FORWARD][1][1] * O[1] + add) >> shift;
    dst[3 * line] = (g_aiT4[TRANSFORM_FORWARD][3][0] * O[0] + g_aiT4[TRANSFORM_FORWARD][3][1] * O[1] + add) >> shift;
    src += 4;
    dst++;
  }
}

/** 4x4 inverse transform implemented using partial butterfly structure (1D)
*  \param src   input data (transform coefficients)
*  \param dst   output data (residual)
*  \param shift specifies right shift after 1D transform
*  \param line
*  \param outputMinimum  minimum for clipping
*  \param outputMaximum  maximum for clipping
//*/

//maxLog2TrDynamicRange = 1;
// const TCoeff clipMinimum = -(1 << maxLog2TrDynamicRange);
// const TCoeff clipMaximum = (1 << maxLog2TrDynamicRange) - 1;

void_t get_idct4_hevc(int *dst, int *src, int shift, int line) {
  int j;
  int E[2], O[2];
  int add = (shift > 0) ? (1 << (shift - 1)) : 0;

  for (j = 0; j < line; j++) {
    /* Utilizing symmetry properties to the maximum to minimize the number of multiplications */
    O[0] = g_aiT4[TRANSFORM_INVERSE][1][0] * src[line] + g_aiT4[TRANSFORM_INVERSE][3][0] * src[3 * line];
    O[1] = g_aiT4[TRANSFORM_INVERSE][1][1] * src[line] + g_aiT4[TRANSFORM_INVERSE][3][1] * src[3 * line];
    E[0] = g_aiT4[TRANSFORM_INVERSE][0][0] * src[0] + g_aiT4[TRANSFORM_INVERSE][2][0] * src[2 * line];
    E[1] = g_aiT4[TRANSFORM_INVERSE][0][1] * src[0] + g_aiT4[TRANSFORM_INVERSE][2][1] * src[2 * line];

    /* Combining even and odd terms at each hierarchy levels to calculate the final spatial domain vector */
    dst[0] = (int)Clip3(-32768, 32767, (E[0] + O[0] + add) >> shift);
    dst[1] = (int)Clip3(-32768, 32767, (E[1] + O[1] + add) >> shift);
    dst[2] = (int)Clip3(-32768, 32767, (E[1] - O[1] + add) >> shift);
    dst[3] = (int)Clip3(-32768, 32767, (E[0] - O[0] + add) >> shift);

    src++;
    dst += 4;
  }
}

void_t get_dct8_hevc(int* dst, int* src, uint32_t shift, uint32_t line) {
  uint32_t j, k;
  int E[4], O[4];
  int EE[2], EO[2];
  int add = (shift > 0) ? (1 << (shift - 1)) : 0;

  for (j = 0; j < line; j++) {
    /* E and O*/
    for (k = 0; k < 4; k++) {
      E[k] = src[k] + src[7 - k];
      O[k] = src[k] - src[7 - k];
    }

    /* EE and EO */
    EE[0] = E[0] + E[3];
    EO[0] = E[0] - E[3];
    EE[1] = E[1] + E[2];
    EO[1] = E[1] - E[2];

    dst[0] = (g_aiT8[TRANSFORM_FORWARD][0][0] * EE[0] +
      g_aiT8[TRANSFORM_FORWARD][0][1] * EE[1] + add) >> shift;
    dst[4 * line] = (g_aiT8[TRANSFORM_FORWARD][4][0] * EE[0] +
      g_aiT8[TRANSFORM_FORWARD][4][1] * EE[1] + add) >> shift;
    dst[2 * line] = (g_aiT8[TRANSFORM_FORWARD][2][0] * EO[0] +
      g_aiT8[TRANSFORM_FORWARD][2][1] * EO[1] + add) >> shift;
    dst[6 * line] = (g_aiT8[TRANSFORM_FORWARD][6][0] * EO[0] +
      g_aiT8[TRANSFORM_FORWARD][6][1] * EO[1] + add) >> shift;

    dst[line] = (g_aiT8[TRANSFORM_FORWARD][1][0] * O[0] +
      g_aiT8[TRANSFORM_FORWARD][1][1] * O[1] +
      g_aiT8[TRANSFORM_FORWARD][1][2] * O[2] +
      g_aiT8[TRANSFORM_FORWARD][1][3] * O[3] + add) >> shift;
    dst[3 * line] = (g_aiT8[TRANSFORM_FORWARD][3][0] * O[0] +
      g_aiT8[TRANSFORM_FORWARD][3][1] * O[1] +
      g_aiT8[TRANSFORM_FORWARD][3][2] * O[2] +
      g_aiT8[TRANSFORM_FORWARD][3][3] * O[3] + add) >> shift;
    dst[5 * line] = (g_aiT8[TRANSFORM_FORWARD][5][0] * O[0] +
      g_aiT8[TRANSFORM_FORWARD][5][1] * O[1] +
      g_aiT8[TRANSFORM_FORWARD][5][2] * O[2] +
      g_aiT8[TRANSFORM_FORWARD][5][3] * O[3] + add) >> shift;
    dst[7 * line] = (g_aiT8[TRANSFORM_FORWARD][7][0] * O[0] +
      g_aiT8[TRANSFORM_FORWARD][7][1] * O[1] +
      g_aiT8[TRANSFORM_FORWARD][7][2] * O[2] +
      g_aiT8[TRANSFORM_FORWARD][7][3] * O[3] + add) >> shift;

    src += 8;
    dst++;
  }
}
void_t get_idct8_hevc(int* dst, int* src, uint32_t shift, uint32_t line) {
  uint32_t j, k;
  int E[4], O[4];
  int EE[2], EO[2];
  //int add = 1 << (shift - 1);
  int add = (shift > 0) ? (1 << (shift - 1)) : 0;

  for (j = 0; j < line; j++) {
    /* Utilizing symmetry properties to the maximum to minimize the number of multiplications */
    for (k = 0; k < 4; k++) {
      O[k] = g_aiT8[TRANSFORM_INVERSE][1][k] * src[line] +
        g_aiT8[TRANSFORM_INVERSE][3][k] * src[3 * line] +
        g_aiT8[TRANSFORM_INVERSE][5][k] * src[5 * line] +
        g_aiT8[TRANSFORM_INVERSE][7][k] * src[7 * line];
    }

    EO[0] = g_aiT8[TRANSFORM_INVERSE][2][0] * src[2 * line] +
      g_aiT8[TRANSFORM_INVERSE][6][0] * src[6 * line];
    EO[1] = g_aiT8[TRANSFORM_INVERSE][2][1] * src[2 * line] +
      g_aiT8[TRANSFORM_INVERSE][6][1] * src[6 * line];
    EE[0] = g_aiT8[TRANSFORM_INVERSE][0][0] * src[0] +
      g_aiT8[TRANSFORM_INVERSE][4][0] * src[4 * line];
    EE[1] = g_aiT8[TRANSFORM_INVERSE][0][1] * src[0] +
      g_aiT8[TRANSFORM_INVERSE][4][1] * src[4 * line];

    /* Combining even and odd terms at each hierarchy levels to calculate the final spatial domain vector */
    E[0] = EE[0] + EO[0];
    E[3] = EE[0] - EO[0];
    E[1] = EE[1] + EO[1];
    E[2] = EE[1] - EO[1];
    for (k = 0; k < 4; k++) {
      dst[k] = (int)Clip3(-32768, 32767, (E[k] + O[k] + add) >> shift);
      dst[k + 4] = (int)Clip3(-32768, 32767, (E[3 - k] - O[3 - k] + add) >> shift);
    }

    src++;
    dst += 8;
  }
}

void_t get_dct16_hevc(int* dst, int* src, int shift, int line) {
  int j, k;
  int E[8], O[8];
  int EE[4], EO[4];
  int EEE[2], EEO[2];
  int add = 1 << (shift - 1);

  for (j = 0; j < line; j++) {
    /* E and O */
    for (k = 0; k < 8; k++) {
      E[k] = src[k] + src[15 - k];
      O[k] = src[k] - src[15 - k];
    }

    /* EE and EO */
    for (k = 0; k < 4; k++) {
      EE[k] = E[k] + E[7 - k];
      EO[k] = E[k] - E[7 - k];
    }

    /* EEE and EEO */
    EEE[0] = EE[0] + EE[3];
    EEO[0] = EE[0] - EE[3];
    EEE[1] = EE[1] + EE[2];
    EEO[1] = EE[1] - EE[2];

    dst[0] = (int16_t)((g_aiT16[TRANSFORM_FORWARD][0][0] * EEE[0] +
      g_aiT16[TRANSFORM_FORWARD][0][1] * EEE[1] + add) >> shift);
    dst[8 * line] = (int16_t)((g_aiT16[TRANSFORM_FORWARD][8][0] * EEE[0] +
      g_aiT16[TRANSFORM_FORWARD][8][1] * EEE[1] + add) >> shift);
    dst[4 * line] = (int16_t)((g_aiT16[TRANSFORM_FORWARD][4][0] * EEO[0] +
      g_aiT16[TRANSFORM_FORWARD][4][1] * EEO[1] + add) >> shift);
    dst[12 * line] = (int16_t)((g_aiT16[TRANSFORM_FORWARD][12][0] * EEO[0] +
      g_aiT16[TRANSFORM_FORWARD][12][1] * EEO[1] + add) >> shift);

    for (k = 2; k < 16; k += 4) {
      dst[k * line] = (int16_t)((g_aiT16[TRANSFORM_FORWARD][k][0] * EO[0] +
        g_aiT16[TRANSFORM_FORWARD][k][1] * EO[1] +
        g_aiT16[TRANSFORM_FORWARD][k][2] * EO[2] +
        g_aiT16[TRANSFORM_FORWARD][k][3] * EO[3] +
        add) >> shift);
    }

    for (k = 1; k < 16; k += 2) {
      dst[k * line] = (int16_t)((g_aiT16[TRANSFORM_FORWARD][k][0] * O[0] +
        g_aiT16[TRANSFORM_FORWARD][k][1] * O[1] +
        g_aiT16[TRANSFORM_FORWARD][k][2] * O[2] +
        g_aiT16[TRANSFORM_FORWARD][k][3] * O[3] +
        g_aiT16[TRANSFORM_FORWARD][k][4] * O[4] +
        g_aiT16[TRANSFORM_FORWARD][k][5] * O[5] +
        g_aiT16[TRANSFORM_FORWARD][k][6] * O[6] +
        g_aiT16[TRANSFORM_FORWARD][k][7] * O[7] + add) >> shift);
    }

    src += 16;
    dst++;
  }
}

void_t get_idct16_hevc(int* dst, int* src, int shift, int line) {
  int j, k;
  int E[8], O[8];
  int EE[4], EO[4];
  int EEE[2], EEO[2];
  int add = 1 << (shift - 1);

  for (j = 0; j < line; j++) {
    /* Utilizing symmetry properties to the maximum to minimize the number of multiplications */
    for (k = 0; k < 8; k++) {
      O[k] = g_aiT16[TRANSFORM_INVERSE][1][k] * src[line] +
        g_aiT16[TRANSFORM_INVERSE][3][k] * src[3 * line] +
        g_aiT16[TRANSFORM_INVERSE][5][k] * src[5 * line] +
        g_aiT16[TRANSFORM_INVERSE][7][k] * src[7 * line] +
        g_aiT16[TRANSFORM_INVERSE][9][k] * src[9 * line] +
        g_aiT16[TRANSFORM_INVERSE][11][k] * src[11 * line] +
        g_aiT16[TRANSFORM_INVERSE][13][k] * src[13 * line] +
        g_aiT16[TRANSFORM_INVERSE][15][k] * src[15 * line];
    }

    for (k = 0; k < 4; k++) {
      EO[k] = g_aiT16[TRANSFORM_INVERSE][2][k] * src[2 * line] +
        g_aiT16[TRANSFORM_INVERSE][6][k] * src[6 * line] +
        g_aiT16[TRANSFORM_INVERSE][10][k] * src[10 * line] +
        g_aiT16[TRANSFORM_INVERSE][14][k] * src[14 * line];
    }

    EEO[0] = g_aiT16[TRANSFORM_INVERSE][4][0] * src[4 * line] +
      g_aiT16[TRANSFORM_INVERSE][12][0] * src[12 * line];
    EEE[0] = g_aiT16[TRANSFORM_INVERSE][0][0] * src[0] +
      g_aiT16[TRANSFORM_INVERSE][8][0] * src[8 * line];
    EEO[1] = g_aiT16[TRANSFORM_INVERSE][4][1] * src[4 * line] +
      g_aiT16[TRANSFORM_INVERSE][12][1] * src[12 * line];
    EEE[1] = g_aiT16[TRANSFORM_INVERSE][0][1] * src[0] +
      g_aiT16[TRANSFORM_INVERSE][8][1] * src[8 * line];

    /* Combining even and odd terms at each hierarchy levels to calculate the final spatial domain vector */
    for (k = 0; k < 2; k++) {
      EE[k] = EEE[k] + EEO[k];
      EE[k + 2] = EEE[1 - k] - EEO[1 - k];
    }

    for (k = 0; k < 4; k++) {
      E[k] = EE[k] + EO[k];
      E[k + 4] = EE[3 - k] - EO[3 - k];
    }

    for (k = 0; k < 8; k++) {
      dst[k] = (int)Clip3(-32768, 32767, (E[k] + O[k] + add) >> shift);
      dst[k + 8] = (int)Clip3(-32768, 32767, (E[7 - k] - O[7 - k] +
        add) >> shift);
    }

    src++;
    dst += 16;
  }
}

void_t get_dct32_hevc(int *dst, int *src, uint32_t shift, uint32_t line) {
  int j, k;
  int E[16], O[16];
  int EE[8], EO[8];
  int EEE[4], EEO[4];
  int EEEE[2], EEEO[2];
  int add = (shift > 0) ? (1 << (shift - 1)) : 0;

  for (j = 0; j < line; j++) {
    /* E and O*/
    for (k = 0; k < 16; k++) {
      E[k] = src[k] + src[31 - k];
      O[k] = src[k] - src[31 - k];
    }
    /* EE and EO */
    for (k = 0; k < 8; k++) {
      EE[k] = E[k] + E[15 - k];
      EO[k] = E[k] - E[15 - k];
    }
    /* EEE and EEO */
    for (k = 0; k < 4; k++) {
      EEE[k] = EE[k] + EE[7 - k];
      EEO[k] = EE[k] - EE[7 - k];
    }
    /* EEEE and EEEO */
    EEEE[0] = EEE[0] + EEE[3];
    EEEO[0] = EEE[0] - EEE[3];
    EEEE[1] = EEE[1] + EEE[2];
    EEEO[1] = EEE[1] - EEE[2];

    dst[0] = (g_aiT32[TRANSFORM_FORWARD][0][0] * EEEE[0] +
      g_aiT32[TRANSFORM_FORWARD][0][1] * EEEE[1] + add) >> shift;
    dst[16 * line] = (g_aiT32[TRANSFORM_FORWARD][16][0] * EEEE[0] +
      g_aiT32[TRANSFORM_FORWARD][16][1] * EEEE[1] + add) >> shift;
    dst[8 * line] = (g_aiT32[TRANSFORM_FORWARD][8][0] * EEEO[0] +
      g_aiT32[TRANSFORM_FORWARD][8][1] * EEEO[1] + add) >> shift;
    dst[24 * line] = (g_aiT32[TRANSFORM_FORWARD][24][0] * EEEO[0] +
      g_aiT32[TRANSFORM_FORWARD][24][1] * EEEO[1] + add) >> shift;

    for (k = 4; k < 32; k += 8) {
      dst[k*line] = (g_aiT32[TRANSFORM_FORWARD][k][0] * EEO[0] +
        g_aiT32[TRANSFORM_FORWARD][k][1] * EEO[1] +
        g_aiT32[TRANSFORM_FORWARD][k][2] * EEO[2] +
        g_aiT32[TRANSFORM_FORWARD][k][3] * EEO[3] + add) >> shift;
    }
    for (k = 2; k < 32; k += 4) {
      dst[k*line] = (g_aiT32[TRANSFORM_FORWARD][k][0] * EO[0] +
        g_aiT32[TRANSFORM_FORWARD][k][1] * EO[1] +
        g_aiT32[TRANSFORM_FORWARD][k][2] * EO[2] +
        g_aiT32[TRANSFORM_FORWARD][k][3] * EO[3] +
        g_aiT32[TRANSFORM_FORWARD][k][4] * EO[4] +
        g_aiT32[TRANSFORM_FORWARD][k][5] * EO[5] +
        g_aiT32[TRANSFORM_FORWARD][k][6] * EO[6] +
        g_aiT32[TRANSFORM_FORWARD][k][7] * EO[7] + add) >> shift;
    }
    for (k = 1; k < 32; k += 2) {
      dst[k*line] = (g_aiT32[TRANSFORM_FORWARD][k][0] * O[0] +
        g_aiT32[TRANSFORM_FORWARD][k][1] * O[1] +
        g_aiT32[TRANSFORM_FORWARD][k][2] * O[2] +
        g_aiT32[TRANSFORM_FORWARD][k][3] * O[3] +
        g_aiT32[TRANSFORM_FORWARD][k][4] * O[4] +
        g_aiT32[TRANSFORM_FORWARD][k][5] * O[5] +
        g_aiT32[TRANSFORM_FORWARD][k][6] * O[6] +
        g_aiT32[TRANSFORM_FORWARD][k][7] * O[7] +
        g_aiT32[TRANSFORM_FORWARD][k][8] * O[8] +
        g_aiT32[TRANSFORM_FORWARD][k][9] * O[9] +
        g_aiT32[TRANSFORM_FORWARD][k][10] * O[10] +
        g_aiT32[TRANSFORM_FORWARD][k][11] * O[11] +
        g_aiT32[TRANSFORM_FORWARD][k][12] * O[12] +
        g_aiT32[TRANSFORM_FORWARD][k][13] * O[13] +
        g_aiT32[TRANSFORM_FORWARD][k][14] * O[14] +
        g_aiT32[TRANSFORM_FORWARD][k][15] * O[15] + add) >> shift;
    }

    src += 32;
    dst++;
  }
}

void_t get_idct32_hevc(int *dst, int *src, uint32_t shift,
  uint32_t line) {
  int j, k;
  int E[16], O[16];
  int EE[8], EO[8];
  int EEE[4], EEO[4];
  int EEEE[2], EEEO[2];
  int add = (shift > 0) ? (1 << (shift - 1)) : 0;

  for (j = 0; j < line; j++) {
    /* Utilizing symmetry properties to the maximum to minimize the number of multiplications */
    for (k = 0; k < 16; k++) {
      O[k] = g_aiT32[TRANSFORM_INVERSE][1][k] * src[line] +
        g_aiT32[TRANSFORM_INVERSE][3][k] * src[3 * line] +
        g_aiT32[TRANSFORM_INVERSE][5][k] * src[5 * line] +
        g_aiT32[TRANSFORM_INVERSE][7][k] * src[7 * line] +
        g_aiT32[TRANSFORM_INVERSE][9][k] * src[9 * line] +
        g_aiT32[TRANSFORM_INVERSE][11][k] * src[11 * line] +
        g_aiT32[TRANSFORM_INVERSE][13][k] * src[13 * line] +
        g_aiT32[TRANSFORM_INVERSE][15][k] * src[15 * line] +
        g_aiT32[TRANSFORM_INVERSE][17][k] * src[17 * line] +
        g_aiT32[TRANSFORM_INVERSE][19][k] * src[19 * line] +
        g_aiT32[TRANSFORM_INVERSE][21][k] * src[21 * line] +
        g_aiT32[TRANSFORM_INVERSE][23][k] * src[23 * line] +
        g_aiT32[TRANSFORM_INVERSE][25][k] * src[25 * line] +
        g_aiT32[TRANSFORM_INVERSE][27][k] * src[27 * line] +
        g_aiT32[TRANSFORM_INVERSE][29][k] * src[29 * line] +
        g_aiT32[TRANSFORM_INVERSE][31][k] * src[31 * line];
    }
    for (k = 0; k < 8; k++) {
      EO[k] = g_aiT32[TRANSFORM_INVERSE][2][k] * src[2 * line] +
        g_aiT32[TRANSFORM_INVERSE][6][k] * src[6 * line] +
        g_aiT32[TRANSFORM_INVERSE][10][k] * src[10 * line] +
        g_aiT32[TRANSFORM_INVERSE][14][k] * src[14 * line] +
        g_aiT32[TRANSFORM_INVERSE][18][k] * src[18 * line] +
        g_aiT32[TRANSFORM_INVERSE][22][k] * src[22 * line] +
        g_aiT32[TRANSFORM_INVERSE][26][k] * src[26 * line] +
        g_aiT32[TRANSFORM_INVERSE][30][k] * src[30 * line];
    }
    for (k = 0; k < 4; k++) {
      EEO[k] = g_aiT32[TRANSFORM_INVERSE][4][k] * src[4 * line] +
        g_aiT32[TRANSFORM_INVERSE][12][k] * src[12 * line] +
        g_aiT32[TRANSFORM_INVERSE][20][k] * src[20 * line] +
        g_aiT32[TRANSFORM_INVERSE][28][k] * src[28 * line];
    }
    EEEO[0] = g_aiT32[TRANSFORM_INVERSE][8][0] * src[8 * line] +
      g_aiT32[TRANSFORM_INVERSE][24][0] * src[24 * line];
    EEEO[1] = g_aiT32[TRANSFORM_INVERSE][8][1] * src[8 * line] +
      g_aiT32[TRANSFORM_INVERSE][24][1] * src[24 * line];
    EEEE[0] = g_aiT32[TRANSFORM_INVERSE][0][0] * src[0] +
      g_aiT32[TRANSFORM_INVERSE][16][0] * src[16 * line];
    EEEE[1] = g_aiT32[TRANSFORM_INVERSE][0][1] * src[0] +
      g_aiT32[TRANSFORM_INVERSE][16][1] * src[16 * line];

    /* Combining even and odd terms at each hierarchy levels to calculate the final spatial domain vector */
    EEE[0] = EEEE[0] + EEEO[0];
    EEE[3] = EEEE[0] - EEEO[0];
    EEE[1] = EEEE[1] + EEEO[1];
    EEE[2] = EEEE[1] - EEEO[1];
    for (k = 0; k < 4; k++) {
      EE[k] = EEE[k] + EEO[k];
      EE[k + 4] = EEE[3 - k] - EEO[3 - k];
    }
    for (k = 0; k < 8; k++) {
      E[k] = EE[k] + EO[k];
      E[k + 8] = EE[7 - k] - EO[7 - k];
    }
    for (k = 0; k < 16; k++) {
      dst[k] = (int)Clip3(-32768, 32767, (E[k] + O[k] + add) >> shift);
      dst[k + 16] = (int)Clip3(-32768, 32767, (E[15 - k] - O[15 - k] + add) >> shift);
    }
    src++;
    dst += 32;
  }
}

#define COEFFS(Cu,Cv,u,v) { \
  if (u == 0) Cu = 1.0 / sqrt(2.0); else Cu = 1.0; \
  if (v == 0) Cv = 1.0 / sqrt(2.0); else Cv = 1.0; \
  }

//Sum of Absolute Difference for block
uint32_t SAD(uint8_t *block, uint8_t* block2, uint32_t x, uint32_t y) {
  uint32_t i;
  uint32_t sum = 0;
  for (i = 0; i < x*y; i += 4) {
    sum += abs(block[i] - block2[i]);
    sum += abs(block[i + 1] - block2[i + 1]);
    sum += abs(block[i + 2] - block2[i + 2]);
    sum += abs(block[i + 3] - block2[i + 3]);
  }

  return sum;
}

void_t get_izigzag_array(int* dst, int* src) {
  uint32_t width = mb_size;
  uint32_t height = mb_size;
  double_t mb_size_delivery_ratio = width*height*DELIVERY_RATIO_NON_ROI;
  uint32_t phase = 0;
  uint32_t count = 0;

  uint32_t x_idx = 0;
  uint32_t y_idx = 0;

  while (count < mb_size_delivery_ratio) {
    dst[x_idx + y_idx*height] = src[count];
    if (phase == 0) {
      y_idx += 1;
      phase = 1;
    } else if (phase == 1) {
      x_idx += 1;
      y_idx -= 1;
      if (x_idx == (width - 1)) {
        phase = 5;
      } else if (y_idx == 0) {
        phase = 2;
      }
    } else if (phase == 2) {
      x_idx += 1;
      phase = 3;
    } else if (phase == 3) {
      x_idx -= 1;
      y_idx += 1;

      if (y_idx == (height - 1)) {
        phase = 4;
      } else if (x_idx == 0) {
        phase = 0;
      }
    } else if (phase == 4) {
      x_idx += 1;
      phase = 1;
    } else if (phase == 5) {
      y_idx += 1;
      phase = 3;
    }

    count += 1;
  }

  while (count < width * height) {
    //		returnArray[x_index][y_index] = 0;
    dst[x_idx + y_idx*mb_size] = 0;

    if (phase == 0) {
      y_idx += 1;
      phase = 1;
    } else if (phase == 1) {
      x_idx += 1;
      y_idx -= 1;

      if (x_idx == (width - 1)) {
        phase = 5;
      } else if (y_idx == 0) {
        phase = 2;
      }
    } else if (phase == 2) {
      x_idx += 1;
      phase = 3;
    } else if (phase == 3) {
      x_idx -= 1;
      y_idx += 1;
      if (y_idx == (height - 1)) {
        phase = 4;
      } else if (x_idx == 0) {
        phase = 0;
      }
    } else if (phase == 4) {
      x_idx += 1;
      phase = 1;
    } else if (phase == 5) {
      y_idx += 1;
      phase = 3;
    }
    count += 1;
  }
}

void_t get_zigzag_array(int* dst, int* src) {
  uint32_t width = mb_size;
  uint32_t height = mb_size;

  uint32_t phase = 0;
  uint32_t count = 0;

  uint32_t x_idx = 0;
  uint32_t y_idx = 0;

  while (count < width * height) {
    dst[count] = src[x_idx + y_idx*height];
    if (phase == 0) {
      y_idx += 1;
      phase = 1;
    } else if (phase == 1) {
      x_idx += 1;
      y_idx -= 1;

      if (x_idx == (width - 1)) {
        phase = 5;
      } else if (y_idx == 0) {
        phase = 2;
      }
    } else if (phase == 2) {
      x_idx += 1;
      phase = 3;
    } else if (phase == 3) {
      x_idx -= 1;
      y_idx += 1;

      if (y_idx == (height - 1)) {
        phase = 4;
      } else if (x_idx == 0) {
        phase = 0;
      }
    } else if (phase == 4) {
      x_idx += 1;
      phase = 1;
    } else if (phase == 5) {
      y_idx += 1;
      phase = 3;
    }
    count += 1;
  }

  int threshold = width * height * DELIVERY_RATIO_NON_ROI;

  for (int i = width*height - 1; i >= threshold; i--) {
    dst[i] = 0;
  }
}
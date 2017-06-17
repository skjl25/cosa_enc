/*
cvideo for the cosa_enc

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

#include "../inc/cvideo.h"

cvideo::cvideo() {
  numberOfRow = 0;//for now for 256 macro blocks
  numberOfColumn = 0;//for now for 256 macro blocks
  numberOfMacroBlocks = 0;//temp
  macroBlockWidth = 0;
  macroBlockHeight = 0;
  imageHeight = 0;
  imageWidth = 0;
  deliveryRate = 0.0;//temp
}

cvideo::cvideo(int Height, int Width, int macroBlockSize,
  double_t nonROIDeliver, double_t ROIDeliver) {
  setImageHeight(Height);
  setImageWidth(Width);
  setMacroBlockHeight(macroBlockSize);
  setMacroBlockWidth(macroBlockSize);
  setDeliveryRate(nonROIDeliver);
  setROIDeliveryRate(ROIDeliver);
  setNumberOfColumn();
  setNumberOfRow();
  setNumberOfMacroBlocks();
  setLengthOfOneMacroString();
  setLengthOfOneMacroString_withROI();
  setMacroBHolder();
}

cvideo::~cvideo() {
  // TODO Auto-generated destructor stub
}

void cvideo::setMacroBHolder() {
  Utility util;
  macroBHolder = new mb[numberOfMacroBlocks];
  for (int i = 0; i < numberOfMacroBlocks; i++) {
    macroBHolder[i].macroBPixel = util.memset2DArray<double_t>(macroBlockWidth, macroBlockHeight);
  }
}

//double_t* readTextFile(char* fileName){
//	double_t* returnArray=new double_t[307200];
//	ifstream in("/home/sklee25/Desktop/string.txt");
//	if(!in){
//		cout << "Cannot open file.";
//		exit (1);
//	}
//	char str[255];
//	int counter=0;
//	while(in){
//		in.getline(str, 255);      // Delimiter defaults to newline
//		returnArray[counter]=atof(str);
//		//	      cout << returnArray[counter] << "  "<<counter<<endl;
//		counter++;
//	} in.close(); return returnArray;
//getchar();
//}

void cvideo::getDCTArray() {
  Utility util;
  int Width = getMacroBlockWidth();
  int Height = getMacroBlockHeight();
  double_t** temporary1 = util.memset2DArray<double_t>(Width, Height);
  Utility timer;
  for (int macroIndex = 0; macroIndex < getNumberOfMacroBlocks(); macroIndex++) {
    double_t cu = 0;
    double_t temporarySum = 0;

    for (int j = 0; j < Height; j++) {
      for (int u = 0; u < Width; u++) {
        temporarySum = 0;

        if (u == 0) {
          cu = 1.0 / sqrt(2.0);
        } else if (u > 0) {
          cu = 1;
        }

        for (int i = 0; i < Width; i++) {
          temporarySum += macroBHolder[macroIndex].macroBPixel[i][j] * cos((((2.0 * (double_t)i) + 1.0) * ((double_t)u * PI)) / (2.0 * (double_t)Width));
        }

        temporary1[u][j] = (sqrt(2.0 / (double_t)Width) * cu * temporarySum);
      }
    }

    for (int i = 0; i < Width; i++) {
      for (int u = 0; u < Height; u++) {
        temporarySum = 0;

        if (u == 0) {
          cu = 1 / sqrt(2.0);
        } else if (u > 0) {
          cu = 1;
        }

        for (int j = 0; j < Height; j++) {
          temporarySum += temporary1[i][j] * cos((((2.0 * (double_t)j) + 1.0) * ((double_t)u * PI)) / (2.0 * (double_t)Height));
        }

        macroBHolder[macroIndex].macroBPixel[i][u] = (sqrt(2.0 / (double_t)Height) * cu * temporarySum);
      }
    }
  }
  delete temporary1;
}

double_t* cvideo::getZigZagArray(int macroBlockID) {
  int Width = getMacroBlockWidth();
  int Height = getMacroBlockHeight();
  double_t* returnArray = new double_t[Width * Height];
  double_t** inputArray = macroBHolder[macroBlockID].macroBPixel;

  int phase = 0;
  int count = 0;

  int x_index = 0;
  int y_index = 0;

  while (count < Width * Height) {
    returnArray[count] = inputArray[x_index][y_index];
    if (phase == 0) {
      y_index += 1;
      phase = 1;
    } else if (phase == 1) {
      x_index += 1;
      y_index -= 1;

      if (x_index == (Width - 1)) {
        phase = 5;
      } else if (y_index == 0) {
        phase = 2;
      }
    } else if (phase == 2) {
      x_index += 1;
      phase = 3;
    } else if (phase == 3) {
      x_index -= 1;
      y_index += 1;

      if (y_index == (Height - 1)) {
        phase = 4;
      } else if (x_index == 0) {
        phase = 0;
      }
    } else if (phase == 4) {
      x_index += 1;
      phase = 1;
    } else if (phase == 5) {
      y_index += 1;
      phase = 3;
    }

    count += 1;
  }

  int threshold = (int)(getMacroBlockWidth() * getMacroBlockHeight() * getDeliveryRate());

  //Previously Implemented
  //	for (int i = 0; i < threshold; i++)
  //	{
  //		returnArray[i] = returnArray[i];
  //	}

  //Modified version
  double_t* realReturnArray = new double_t[threshold];
  for (int i = 0; i < threshold; i++) {
    realReturnArray[i] = returnArray[i];
  }
  return realReturnArray;
}

vector<double_t> cvideo::getZigZagArray_with_ROI_ext() {
  std::vector<double_t> integratedVector;

  int Width = getMacroBlockWidth();
  int Height = getMacroBlockHeight();
  double_t* returnArray = new double_t[Width * Height];

  int phase = 0;
  int count = 0;

  int x_index = 0;
  int y_index = 0;

  for (int macroBlockID = 0; macroBlockID < getNumberOfMacroBlocks(); macroBlockID++) {
    double_t** inputArray = macroBHolder[macroBlockID].macroBPixel;

    while (count < Width * Height) {
      returnArray[count] = inputArray[x_index][y_index];
      if (phase == 0) {
        y_index += 1;
        phase = 1;
      } else if (phase == 1) {
        x_index += 1;
        y_index -= 1;

        if (x_index == (Width - 1)) {
          phase = 5;
        } else if (y_index == 0) {
          phase = 2;
        }
      } else if (phase == 2) {
        x_index += 1;
        phase = 3;
      } else if (phase == 3) {
        x_index -= 1;
        y_index += 1;

        if (y_index == (Height - 1)) {
          phase = 4;
        } else if (x_index == 0) {
          phase = 0;
        }
      } else if (phase == 4) {
        x_index += 1;
        phase = 1;
      } else if (phase == 5) {
        y_index += 1;
        phase = 3;
      }

      count += 1;
    }

    //Since its for ROI it's alway 100=1
    int threshold = 0;
    if (macroBHolder[macroBlockID].ROIFlag == 0) {
      threshold = (int)(getMacroBlockWidth() * getMacroBlockHeight() * getDeliveryRate());
    } else {
      threshold = (int)(getMacroBlockWidth() * getMacroBlockHeight() * getROIDeliveryRate());
    }

    //		double_t* realReturnArray = new double_t[threshold];
    for (int i = 0; i < threshold; i++) {
      //			realReturnArray[i] = returnArray[i];
      integratedVector.push_back(returnArray[i]);
      printf("%f\n", returnArray[i]);
    }
  }
  return integratedVector;
}

void cvideo::getZigZagArray_with_ROI(double_t* realReturnArray, int macroBlockID) {
  int Width = getMacroBlockWidth();
  int Height = getMacroBlockHeight();
  double_t* returnArray = new double_t[Width * Height];
  double_t** inputArray = macroBHolder[macroBlockID].macroBPixel;

  int phase = 0;
  int count = 0;

  int x_index = 0;
  int y_index = 0;

  while (count < Width * Height) {
    returnArray[count] = inputArray[x_index][y_index];
    if (phase == 0) {
      y_index += 1;
      phase = 1;
    } else if (phase == 1) {
      x_index += 1;
      y_index -= 1;

      if (x_index == (Width - 1)) {
        phase = 5;
      } else if (y_index == 0) {
        phase = 2;
      }
    } else if (phase == 2) {
      x_index += 1;
      phase = 3;
    } else if (phase == 3) {
      x_index -= 1;
      y_index += 1;

      if (y_index == (Height - 1)) {
        phase = 4;
      } else if (x_index == 0) {
        phase = 0;
      }
    } else if (phase == 4) {
      x_index += 1;
      phase = 1;
    } else if (phase == 5) {
      y_index += 1;
      phase = 3;
    }

    count += 1;
  }

  //Since its for ROI it's alway 100=1
  int threshold = 0;
  if (macroBHolder[macroBlockID].ROIFlag == 0) {
    threshold = (int)(getMacroBlockWidth() * getMacroBlockHeight() * getDeliveryRate());
  } else {
    threshold = (int)(getMacroBlockWidth() * getMacroBlockHeight() * getROIDeliveryRate());
  }

  //	double_t* realReturnArray = new double_t[threshold];
  for (int i = 0; i < threshold; i++) {
    realReturnArray[i] = returnArray[i];
  }
  delete[] returnArray;
  //	return realReturnArray;
}

double_t* cvideo::getOneIntegratedArray(double_t** inputArray) {
  //informationArray[0] = (double_t)numberOfMacroBlocks;
  //informationArray[1] = (double_t)Width;
  //informationArray[2] = (double_t)Height;
  //informationArray[3] = (double_t)lengthOfOneMacroString;
  //informationArray[4] = (double_t)macroBlockWidth;
  //informationArray[5] = (double_t)macroBlockHeight;
  //informationArray[6] = (double_t)numberOfColumn;
  //informationArray[7] = (double_t)numberOfRow;

  //	int lengthOfReturnArray = informationArray.Length + dataTotalLength;

  int lengthOfReturnArray = getLengthOfOneMacroString()*getNumberOfMacroBlocks();

  double_t* returnArray = new double_t[lengthOfReturnArray];

  for (int i = 0; i < getNumberOfMacroBlocks(); i++)//total number of macro block
  {
    //for non-ROI region blocks
    for (int j = 0; j < lengthOfOneMacroString; j++) {
      returnArray[(i * lengthOfOneMacroString) + j] = inputArray[i][j];
    }
  }
  return returnArray;
}

double_t* cvideo::getOneIntegratedArray_withROI(double_t** inputArray, vector<int> vw) {
  //informationArray[0] = (double_t)numberOfMacroBlocks;
  //informationArray[1] = (double_t)Width;
  //informationArray[2] = (double_t)Height;
  //informationArray[3] = (double_t)lengthOfOneMacroString;
  //informationArray[4] = (double_t)macroBlockWidth;
  //informationArray[5] = (double_t)macroBlockHeight;
  //informationArray[6] = (double_t)numberOfColumn;
  //informationArray[7] = (double_t)numberOfRow;

  //	int lengthOfReturnArray = informationArray.Length + dataTotalLength;

  int lengthOfReturnArray = getLengthOfOneMacroString()*getNumberOfMacroBlocks();

  double_t* returnArray = new double_t[lengthOfReturnArray];

  for (int i = 0; i < getNumberOfMacroBlocks(); i++)//total number of macro block
  {
    if (std::find(vw.begin(), vw.end(), i) != vw.end()) {
      //for ROI region blocks
      for (int j = 0; j < ROIlengthOfOneMacroString; j++) {
        returnArray[(i * ROIlengthOfOneMacroString) + j] = inputArray[i][j];
      }
    } else {
      //for non-ROI region blocks
      for (int j = 0; j < lengthOfOneMacroString; j++) {
        returnArray[(i * lengthOfOneMacroString) + j] = inputArray[i][j];
      }
    }
  }
  return returnArray;
}

void cvideo::getIDCTArray() {
  int Width = getMacroBlockWidth();
  int Height = getMacroBlockHeight();
  Utility util;

  double_t cu = 0;

  double_t temporarySum = 0;

  double_t** temporary1 = util.memset2DArray<double_t>(Width, Height);

  for (int x = 0; x < getNumberOfMacroBlocks(); x++) {
    for (int j = 0; j < Height; j++) {
      for (int i = 0; i < Width; i++) {
        temporarySum = 0;

        for (int u = 0; u < Width; u++) {
          if (u == 0) {
            cu = 1.0 / sqrt(2.0);
          } else {
            cu = 1;
          }

          temporarySum += cu * macroBHolder[x].macroBPixel[u][j] * cos(((2.0 * i + 1) * u * PI) / (2.0 * Width));
        }
        temporary1[i][j] = sqrt(2.0 / (double_t)Width) * temporarySum;
      }
    }

    for (int i = 0; i < Width; i++) {
      for (int j = 0; j < Height; j++) {
        temporarySum = 0;

        for (int u = 0; u < Height; u++) {
          if (u == 0) {
            cu = 1.0 / sqrt(2.0);
          } else {
            cu = 1;
          }

          temporarySum += cu * temporary1[i][u] * cos(((2.0 * j + 1) * u * PI) / (2.0 * Height));
        }
        macroBHolder[x].macroBPixel[i][j] = sqrt(2.0 / (double_t)Height) * temporarySum;
      }
    }
  }
}
void cvideo::getInverseZigZagArray(double_t** inputArray) {
  int Width = getMacroBlockWidth();
  int Height = getMacroBlockHeight();
  int lengthOfOneMacroStringLocal = getLengthOfOneMacroString();

  for (int x = 0; x < getNumberOfMacroBlocks(); x++) {
    int phase = 0;
    int count = 0;

    int x_index = 0;
    int y_index = 0;

    while (count < lengthOfOneMacroStringLocal) {
      macroBHolder[x].macroBPixel[x_index][y_index] = inputArray[x][count];
      if (phase == 0) {
        y_index += 1;
        phase = 1;
      } else if (phase == 1) {
        x_index += 1;
        y_index -= 1;

        if (x_index == (Width - 1)) {
          phase = 5;
        } else if (y_index == 0) {
          phase = 2;
        }
      } else if (phase == 2) {
        x_index += 1;
        phase = 3;
      } else if (phase == 3) {
        x_index -= 1;
        y_index += 1;

        if (y_index == (Height - 1)) {
          phase = 4;
        } else if (x_index == 0) {
          phase = 0;
        }
      } else if (phase == 4) {
        x_index += 1;
        phase = 1;
      } else if (phase == 5) {
        y_index += 1;
        phase = 3;
      }

      count += 1;
    }

    while (count < Width * Height) {
      //		returnArray[x_index][y_index] = 0;
      macroBHolder[x].macroBPixel[x_index][y_index] = 0;

      if (phase == 0) {
        y_index += 1;
        phase = 1;
      } else if (phase == 1) {
        x_index += 1;
        y_index -= 1;

        if (x_index == (Width - 1)) {
          phase = 5;
        } else if (y_index == 0) {
          phase = 2;
        }
      } else if (phase == 2) {
        x_index += 1;
        phase = 3;
      } else if (phase == 3) {
        x_index -= 1;
        y_index += 1;

        if (y_index == (Height - 1)) {
          phase = 4;
        } else if (x_index == 0) {
          phase = 0;
        }
      } else if (phase == 4) {
        x_index += 1;
        phase = 1;
      } else if (phase == 5) {
        y_index += 1;
        phase = 3;
      }

      count += 1;
    }
  }
}

void cvideo::getInverseZigZagArray_withROI(vector< vector<double_t> > inputArray, vector<int> vw) {
  int Width = getMacroBlockWidth();
  int Height = getMacroBlockHeight();
  int lengthOfOneMacroStringLocal = getLengthOfOneMacroString();

  for (int x = 0; x < getNumberOfMacroBlocks(); x++) {
    int phase = 0;
    int count = 0;

    int x_index = 0;
    int y_index = 0;

    //Zigzag ROI
    if (std::find(vw.begin(), vw.end(), x) != vw.end()) {
      while (count < getROILengthOfOneMacroString()) {
        macroBHolder[x].macroBPixel[x_index][y_index] = inputArray[x][count];
        if (phase == 0) {
          y_index += 1;
          phase = 1;
        } else if (phase == 1) {
          x_index += 1;
          y_index -= 1;

          if (x_index == (Width - 1)) {
            phase = 5;
          } else if (y_index == 0) {
            phase = 2;
          }
        } else if (phase == 2) {
          x_index += 1;
          phase = 3;
        } else if (phase == 3) {
          x_index -= 1;
          y_index += 1;

          if (y_index == (Height - 1)) {
            phase = 4;
          } else if (x_index == 0) {
            phase = 0;
          }
        } else if (phase == 4) {
          x_index += 1;
          phase = 1;
        } else if (phase == 5) {
          y_index += 1;
          phase = 3;
        }

        count += 1;
      }

      while (count < Width * Height) {
        //		returnArray[x_index][y_index] = 0;
        macroBHolder[x].macroBPixel[x_index][y_index] = 0;

        if (phase == 0) {
          y_index += 1;
          phase = 1;
        } else if (phase == 1) {
          x_index += 1;
          y_index -= 1;

          if (x_index == (Width - 1)) {
            phase = 5;
          } else if (y_index == 0) {
            phase = 2;
          }
        } else if (phase == 2) {
          x_index += 1;
          phase = 3;
        } else if (phase == 3) {
          x_index -= 1;
          y_index += 1;

          if (y_index == (Height - 1)) {
            phase = 4;
          } else if (x_index == 0) {
            phase = 0;
          }
        } else if (phase == 4) {
          x_index += 1;
          phase = 1;
        } else if (phase == 5) {
          y_index += 1;
          phase = 3;
        }

        count += 1;
      }
    } else {
      while (count < lengthOfOneMacroStringLocal) {
        macroBHolder[x].macroBPixel[x_index][y_index] = inputArray[x][count];
        if (phase == 0) {
          y_index += 1;
          phase = 1;
        } else if (phase == 1) {
          x_index += 1;
          y_index -= 1;

          if (x_index == (Width - 1)) {
            phase = 5;
          } else if (y_index == 0) {
            phase = 2;
          }
        } else if (phase == 2) {
          x_index += 1;
          phase = 3;
        } else if (phase == 3) {
          x_index -= 1;
          y_index += 1;

          if (y_index == (Height - 1)) {
            phase = 4;
          } else if (x_index == 0) {
            phase = 0;
          }
        } else if (phase == 4) {
          x_index += 1;
          phase = 1;
        } else if (phase == 5) {
          y_index += 1;
          phase = 3;
        }

        count += 1;
      }

      while (count < Width * Height) {
        //		returnArray[x_index][y_index] = 0;
        macroBHolder[x].macroBPixel[x_index][y_index] = 0;

        if (phase == 0) {
          y_index += 1;
          phase = 1;
        } else if (phase == 1) {
          x_index += 1;
          y_index -= 1;

          if (x_index == (Width - 1)) {
            phase = 5;
          } else if (y_index == 0) {
            phase = 2;
          }
        } else if (phase == 2) {
          x_index += 1;
          phase = 3;
        } else if (phase == 3) {
          x_index -= 1;
          y_index += 1;

          if (y_index == (Height - 1)) {
            phase = 4;
          } else if (x_index == 0) {
            phase = 0;
          }
        } else if (phase == 4) {
          x_index += 1;
          phase = 1;
        } else if (phase == 5) {
          y_index += 1;
          phase = 3;
        }

        count += 1;
      }
    }
  }
}

double_t** cvideo::setMacroBlocksToOriginalImage() {
  Utility util;
  double_t** returnArray2 = util.memset2DArray<double_t>(getImageWidth(), getImageHeight());

  for (int x = 0; x < getNumberOfColumn(); x++) {
    for (int y = 0; y < getNumberOfRow(); y++) {
      for (int i = 0; i < getMacroBlockWidth(); i++) {
        for (int j = 0; j < getMacroBlockHeight(); j++) {
          int macroBlockIndex = x + (getNumberOfColumn() * y);

          returnArray2[(x * getMacroBlockWidth()) + i][(y * getMacroBlockHeight()) + j] = macroBHolder[macroBlockIndex].macroBPixel[i][j];
        }
      }
    }
  }
  return returnArray2;
}

void cvideo::setMacroBPixel(double_t** double_tOrgImage) {
  for (int x = 0; x < getNumberOfColumn(); x++) {
    for (int y = 0; y < getNumberOfRow(); y++) {
      for (int i = 0; i < getMacroBlockWidth(); i++) {
        for (int j = 0; j < getMacroBlockHeight(); j++) {
          int macroBlockIndex = x + (getNumberOfColumn() * y);
          macroBHolder[macroBlockIndex].macroBPixel[i][j] = double_tOrgImage[(x * getMacroBlockWidth()) + i][(y * getMacroBlockHeight()) + j];
        }
      }
    }
  }
}

void cvideo::setMacroBPixel_withROI(double_t** double_tOrgImage, vector<int> vw) {
  int index = 0;
  int macroBlockIndex = 0;
  for (int x = 0; x < getNumberOfColumn(); x++) {
    for (int y = 0; y < getNumberOfRow(); y++) {
      //Setting up macro block
      macroBlockIndex = x + (getNumberOfColumn() * y);

      //Find ROI macro block and place flag--start
      if (std::find(vw.begin(), vw.end(), macroBlockIndex) != vw.end()) {
        macroBHolder[macroBlockIndex].ROIFlag = 1;
        index++;
        // found it
      } else {
        macroBHolder[macroBlockIndex].ROIFlag = 0;
        // doesn't exist
      }
      //Find ROI macro block and place flag--end

      for (int i = 0; i < getMacroBlockWidth(); i++) {
        for (int j = 0; j < getMacroBlockHeight(); j++) {
          macroBHolder[macroBlockIndex].macroBPixel[i][j] = double_tOrgImage[(x * getMacroBlockWidth()) + i][(y * getMacroBlockHeight()) + j];
        }
      }
    }
  }
}

void cvideo::setMacroBPixel_withROI_ext(IplImage* grayImage, vector<int> vw) {
  int index = 0;
  int macroBlockIndex = 0;
  for (int x = 0; x < getNumberOfColumn(); x++) {
    for (int y = 0; y < getNumberOfRow(); y++) {
      //Setting up macro block
      macroBlockIndex = x + (getNumberOfColumn() * y);

      //Find ROI macro block and place flag--start
      if (std::find(vw.begin(), vw.end(), macroBlockIndex) != vw.end()) {
        macroBHolder[macroBlockIndex].ROIFlag = 1;
        index++;
        // found it
      } else {
        macroBHolder[macroBlockIndex].ROIFlag = 0;
        // doesn't exist
      }
      //Find ROI macro block and place flag--end

      for (int i = 0; i < getMacroBlockWidth(); i++) {
        for (int j = 0; j < getMacroBlockHeight(); j++) {
          //					macroBHolder[macroBlockIndex].macroBPixel[i][j] = double_tOrgImage[(x * getMacroBlockWidth()) + i][(y * getMacroBlockHeight()) + j];
          macroBHolder[macroBlockIndex].macroBPixel[i][j] = ((uchar*)(grayImage->imageData + ((y * getMacroBlockHeight()) + j)*grayImage->widthStep))[((x * getMacroBlockWidth()) + i)];
        }
      }
    }
  }
}

char* cvideo::convertToGrayScale(char* origImg, int width, int height) {
  char* grayG2ImgChar = new char[width*height];
  for (int i = 0; i < width*height; i++) {
    int location = 3 * i + 1;
    grayG2ImgChar[i] = origImg[location];
  }
  return grayG2ImgChar;
}

void cvideo::init_cvideo() {
  //4*4*number of row *number of height=number of Pixels
  //number of row *number of height=number of macro blocks
  //number of row=image height/macro block height
  //number of column=image width/macro block width
  // create macro blocks
  setImageHeight(IMG_HEIGHT);
  setImageWidth(IMG_WIDTH);
  setMacroBlockHeight(mb_size);
  setMacroBlockWidth(mb_size);
  setDeliveryRate(DELIVERY_RATIO_NON_ROI);
  setROIDeliveryRate(DELIVERY_RATIO_ROI);
  setNumberOfColumn();
  setNumberOfRow();
  setNumberOfMacroBlocks();
  setLengthOfOneMacroString();
  setLengthOfOneMacroString_withROI();
  setMacroBHolder();
}

// from here

void cvideo::zigzag(short* block) {
  int i;
  short zig[64];

  zig[0] = block[0];
  zig[1] = block[1];
  zig[2] = block[8];
  zig[3] = block[16];
  zig[4] = block[9];
  zig[5] = block[2];
  zig[6] = block[3];
  zig[7] = block[10];
  zig[8] = block[17];
  zig[9] = block[24];
  zig[10] = block[32];
  zig[11] = block[25];
  zig[12] = block[18];
  zig[13] = block[11];
  zig[14] = block[4];
  zig[15] = block[5];
  zig[16] = block[12];
  zig[17] = block[19];
  zig[18] = block[26];
  zig[19] = block[33];
  zig[20] = block[40];
  zig[21] = block[48];
  zig[22] = block[41];
  zig[23] = block[34];
  zig[24] = block[27];
  zig[25] = block[20];
  zig[26] = block[13];
  zig[27] = block[6];
  zig[28] = block[7];
  zig[29] = block[14];
  zig[30] = block[21];
  zig[31] = block[28];
  zig[32] = block[35];
  zig[33] = block[42];
  zig[34] = block[49];
  zig[35] = block[56];
  zig[36] = block[57];
  zig[37] = block[50];
  zig[38] = block[43];
  zig[39] = block[36];
  zig[40] = block[29];
  zig[41] = block[22];
  zig[42] = block[15];
  zig[43] = block[23];
  zig[44] = block[30];
  zig[45] = block[37];
  zig[46] = block[44];
  zig[47] = block[51];
  zig[48] = block[58];
  zig[49] = block[59];
  zig[50] = block[52];
  zig[51] = block[45];
  zig[52] = block[38];
  zig[53] = block[31];
  zig[54] = block[39];
  zig[55] = block[46];
  zig[56] = block[53];
  zig[57] = block[60];
  zig[58] = block[61];
  zig[59] = block[54];
  zig[60] = block[47];
  zig[61] = block[55];
  zig[62] = block[62];
  zig[63] = block[63];

  for (i = 0; i < 64; i++) {
    block[i] = zig[i];
  }
}

vector<int> FindImportantRegion(IplImage* inputArray, int macroBlockWidth, int macroBlockHeight) {
  std::vector<int> array;
  unsigned int Width = 640;
  unsigned int Height = 480;

  int prevMacroBlockIndex = -1;
  int numberOfRow = Height / macroBlockHeight;
  int numberOfColumn = Width / macroBlockWidth;

  for (int x = 0; x < numberOfColumn; x++) {
    for (int y = 0; y < numberOfRow; y++) {
      for (int i = 0; i < macroBlockWidth; i++) {
        for (int j = 0; j < macroBlockHeight; j++) {
          int macroBlockIndex = x + (numberOfColumn * y);
          //					if (inputArray[(x * macroBlockWidth) + i][(y * macroBlockHeight) + j] == 255.0)
          if (((uchar*)(inputArray->imageData + ((y * macroBlockHeight) + j)*inputArray->widthStep))[((x * macroBlockWidth) + i)] == 255.0) {
            if (prevMacroBlockIndex != macroBlockIndex) {
              //							returnArray[importantMacroBlockCounter] = macroBlockIndex;
              array.push_back(macroBlockIndex);
            }
            prevMacroBlockIndex = macroBlockIndex;
          }
        }
      }
    }
  }
  return array;
}

void cvideo::quantization(short** mat) {
  short quant[64] =
  {
    2048, 2979, 3277, 2048, 1365, 819, 643, 537,
    2731, 2731, 2341, 1725, 1260, 565, 546, 596,
    2341, 2521, 2048, 1365, 819, 575, 475, 585,
    2341, 1927, 1490, 1130, 643, 377, 410, 529,
    1820, 1490, 886, 585, 482, 301, 318, 426,
    1365, 936, 596, 512, 405, 315, 290, 356,
    669, 512, 420, 377, 318, 271, 273, 324,
    455, 356, 345, 334, 293, 328, 318, 331
  };
  zigzag(quant);
  int i;
  for (int j = 0; j < getNumberOfMacroBlocks(); j++) {
    for (i = 0; i < 64; i++) {
      mat[j][i] = (short)(mat[j][i] * quant[i] >> 10);
      if ((mat[j][i] & 0x0001) == 1) {           // test the first bit after the radix point
        mat[j][i] += 2;                       // if it is 1, the value should add 1
      }
      mat[j][i] = mat[j][i] >> 1;
    }
  }
}
short cvideo::cat(short diff)                           // define the category (CAT)
{
  short cate;

  if (diff == 1 || diff == -1)
    cate = 1;
  else if (diff == -3 || diff == -2 || diff == 2 || diff == 3)
    cate = 2;
  else if ((diff >= -7 && diff <= -4) || (diff >= 4 && diff <= 7))
    cate = 3;
  else if ((diff >= -15 && diff <= -8) || (diff >= 8 && diff <= 15))
    cate = 4;
  else if ((diff >= -31 && diff <= -16) || (diff >= 16 && diff <= 31))
    cate = 5;
  else if ((diff >= -63 && diff <= -32) || (diff >= 32 && diff <= 63))
    cate = 6;
  else if ((diff >= -127 && diff <= -64) || (diff >= 64 && diff <= 127))
    cate = 7;
  else if ((diff >= -255 && diff <= -128) || (diff >= 128 && diff <= 255))
    cate = 8;
  else if ((diff >= -511 && diff <= -256) || (diff >= 256 && diff <= 511))
    cate = 9;
  else if ((diff >= -1023 && diff <= -521) || (diff >= 521 && diff <= 1023))
    cate = 10;
  else if ((diff >= -2047 && diff <= -1024) || (diff >= 1024 && diff <= 2047))
    cate = 11;
  else cate = 0;

  return cate;
}
short cvideo::dchuff(short cat)                // find the DC codeword according to DC Huffman coefficients
{
  short dcHuffman[12] =
  {
    0x0, 0x2, 0x3, 0x4, 0x5, 0x6,
    0xE, 0x1E, 0x3E, 0x7E, 0xFE, 0x1FE
  };
  short codedc;

  codedc = dcHuffman[cat];

  return codedc;
}
short cvideo::achuff(short run, short cat)     // find the AC codeword according to AC Huffman coefficients
{
  short acHuffman[16][11] = {
    0x000A, 0x0000, 0x0001, 0x0004, 0x000B, 0x001A, 0x0078, 0x00F8, 0x03F6, 0xFF82, 0xFF83,
    0xFFFF, 0x000C, 0x001B, 0x0079, 0x01F6, 0x07F6, 0xFF84, 0xFF85, 0xFF86, 0xFF87, 0xFF88,
    0xFFFF, 0x001C, 0x00F9, 0x03F7, 0x0FF4, 0xFF89, 0xFF8A, 0xFF8B, 0xFF8C, 0xFF8D, 0xFF8E,
    0xFFFF, 0x003A, 0x01F7, 0x0FF5, 0xFF8F, 0xFF90, 0xFF91, 0xFF92, 0xFF93, 0xFF94, 0xFF95,
    0xFFFF, 0x003B, 0x03F8, 0xFF96, 0xFF97, 0xFF98, 0xFF99, 0xFF9A, 0xFF9B, 0xFF9C, 0xFF9D,
    0xFFFF, 0x007A, 0x07F7, 0xFF9E, 0xFF9F, 0xFFA0, 0xFFA1, 0xFFA2, 0xFFA3, 0xFFA4, 0xFFA5,
    0xFFFF, 0x007B, 0x0FF6, 0xFFA6, 0xFFA7, 0xFFA8, 0xFFA9, 0xFFAA, 0xFFAB, 0xFFAC, 0xFFAD,
    0xFFFF, 0x00FA, 0x0FF7, 0xFFAE, 0xFFAF, 0xFFB0, 0xFFB1, 0xFFB2, 0xFFB3, 0xFFB4, 0xFFB5,
    0xFFFF, 0x01F8, 0x7FC0, 0xFFB6, 0xFFB7, 0xFFB8, 0xFFB9, 0xFFBA, 0xFFBB, 0xFFBC, 0xFFBD,
    0xFFFF, 0x01F9, 0xFFBE, 0xFFBF, 0xFFC0, 0xFFC1, 0xFFC2, 0xFFC3, 0xFFC4, 0xFFC5, 0xFFC6,
    0xFFFF, 0x01FA, 0xFFC7, 0xFFC8, 0xFFC9, 0xFFCA, 0xFFCB, 0xFFCC, 0xFFCD, 0xFFCE, 0xFFCF,
    0xFFFF, 0x03F9, 0xFFD0, 0xFFD1, 0xFFD2, 0xFFD3, 0xFFD4, 0xFFD5, 0xFFD6, 0xFFD7, 0xFFD8,
    0xFFFF, 0x03FA, 0xFFD9, 0xFFDA, 0xFFDB, 0xFFDC, 0xFFDD, 0xFFDE, 0xFFDF, 0xFFE0, 0xFFE1,
    0xFFFF, 0x07F8, 0xFFE2, 0xFFE3, 0xFFE4, 0xFFE5, 0xFFE6, 0xFFE7, 0xFFE8, 0xFFE9, 0xFFEA,
    0xFFFF, 0xFFEB, 0xFFEC, 0xFFED, 0xFFEE, 0xFFEF, 0xFFF0, 0xFFF1, 0xFFF2, 0xFFF3, 0xFFF4,
    0x07F9, 0xFFF5, 0xFFF6, 0xFFF7, 0xFFF8, 0xFFF9, 0xFFFA, 0xFFFB, 0xFFFC, 0xFFFD, 0xFFFE
  };
  short codeac;

  codeac = acHuffman[run][cat];

  return codeac;
}
int cvideo::count(short run, short cat)        // calculate the number of bits
{
  int ACbit[16][11] = {
    4, 2, 2, 3, 4, 5, 7, 8, 10, 16, 16,
    0, 4, 5, 7, 9, 11, 16, 16, 16, 16, 16,
    0, 5, 8, 10, 12, 16, 16, 16, 16, 16, 16,
    0, 6, 9, 12, 16, 16, 16, 16, 16, 16, 16,
    0, 6, 10, 16, 16, 16, 16, 16, 16, 16, 16,
    0, 7, 11, 16, 16, 16, 16, 16, 16, 16, 16,
    0, 7, 12, 16, 16, 16, 16, 16, 16, 16, 16,
    0, 8, 12, 16, 16, 16, 16, 16, 16, 16, 16,
    0, 9, 15, 16, 16, 16, 16, 16, 16, 16, 16,
    0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    0, 10, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    0, 10, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    0, 11, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    11, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16
  };
  int bit;

  bit = ACbit[run][cat];

  return bit;
}
short cvideo::symbol2(short diff)              // calculate symbol-2
{
  short symbol;

  if (diff >= 0) {                      // when DIFF is positive, symbol-2 is the lower order bits of DIFF
    symbol = diff;
  } else {                             // when DIFF is negative, symbol-2 is the lower order bits of DIFF-1
    diff = diff - 1;
    if ((diff & 0x4000) == 0) {     // locate the most significant bit which is 0 for negative DIFF
      symbol = diff & 0x7FFF;
    } else if ((diff & 0x2000) == 0) {
      symbol = diff & 0x3FFF;
    } else if ((diff & 0x1000) == 0) {
      symbol = diff & 0x1FFF;
    } else if ((diff & 0x0800) == 0) {
      symbol = diff & 0x0FFF;
    } else if ((diff & 0x0400) == 0) {
      symbol = diff & 0x07FF;
    } else if ((diff & 0x0200) == 0) {
      symbol = diff & 0x03FF;
    } else if ((diff & 0x0100) == 0) {
      symbol = diff & 0x01FF;
    } else if ((diff & 0x0080) == 0) {
      symbol = diff & 0x00FF;
    } else if ((diff & 0x0040) == 0) {
      symbol = diff & 0x007F;
    } else if ((diff & 0x0020) == 0) {
      symbol = diff & 0x003F;
    } else if ((diff & 0x0010) == 0) {
      symbol = diff & 0x001F;
    } else if ((diff & 0x0008) == 0) {
      symbol = diff & 0x000F;
    } else if ((diff & 0x0004) == 0) {
      symbol = diff & 0x0007;
    } else if ((diff & 0x0002) == 0) {
      symbol = diff & 0x0003;
    } else if ((diff & 0x0001) == 0) {
      symbol = diff & 0x0001;
    } else
      ;
  }
  return symbol;
}
void cvideo::dc_encoding(short** mat, int** intmat, int* preblock)                 // function doing DC encoding
{
  int DCbit[12] =
  {
    2, 3, 3, 3, 3, 3, 4, 5, 6, 7, 8, 9
  };
  short dcsym1, dcsym2;
  short diff, cate;
  int data, bit;
  for (int j = 0; j < getNumberOfMacroBlocks(); j++) {
    diff = mat[j][0] - *preblock;
    cate = cat(diff);                  // calculate CAT
    dcsym1 = dchuff(cate);             // calculate dc-symbol-1
    dcsym2 = symbol2(diff);            // calculate dc-symbol-2

    *preblock = mat[j][0];
    data = (dcsym1 << cate) + dcsym2;  // connect symbol-1 and symbol-2 together
    bit = DCbit[cate];
    bit += cate;                       // calculate number of significance bits
    intmat[j][0] = data << (32 - bit);      // shift the data to the most significant bit, and store in stream[0]
  }
}
void cvideo::ac_encoding(short** mat, int** intmat, int* preblock) {
  short acsym1, acsym2;
  int i, num, data_num;
  short run, cate;
  int data, bit;
  for (int j = 0; j < getNumberOfMacroBlocks(); j++) {
    data_num = 1;
    run = 0;

    for (i = 63; i > 0; i--) { // find EOB
      if (mat[j][i] != 0) {
        num = i + 1;
        break;
      } else if (mat[j][i] == 0 && i == 1) {
        num = 1;
        break;
      } else
        ;
    } // for

    for (i = 1; i < num; i++) {
      if (mat[j][i] != 0) {
        cate = cat(mat[j][i]);                // calculate CAT
        acsym1 = achuff(run, cate);          // calculate ac-symbol-1
        acsym2 = symbol2(mat[j][i]);          // calculate ac-symbol-2
        data = (acsym1 << cate) + acsym2;    // connect symbol-1 and symbol-2 together
        bit = count(run, cate);
        bit += cate;                         // calculate number of significance bits
        intmat[j][data_num] = data << (32 - bit); // shift the data to the most significant bit, and store in stream
        data_num++;                          // number of data
        run = 0;
      } // if
      else if (mat[j][i] == 0 && run == 15) {       // 16 zeros together
        acsym1 = achuff(15, 0);               // RUN = 15, CAT = 0
        acsym2 = 0;
        data = acsym1;
        bit = 11;
        intmat[j][data_num] = data << (32 - bit);
        data_num++;
        run = 0;
      } // else if
      else if (mat[j][i] == 0 && run < 15) {
        run++;
      } else;// else
    } // for

    data = achuff(0, 0);                          // EOB, RUN = 0, CAT = 0
    bit = 4;
    intmat[j][data_num] = data << (32 - bit);         // the last data of current block
  }
}
int* cvideo::IacHuff(int data)                           // calculate RUN and CAT from ac codeword
{
  short acHuffman[16][11] = {
    0x000A, 0x0000, 0x0001, 0x0004, 0x000B, 0x001A, 0x0078, 0x00F8, 0x03F6, 0xFF82, 0xFF83,
    0xFFFF, 0x000C, 0x001B, 0x0079, 0x01F6, 0x07F6, 0xFF84, 0xFF85, 0xFF86, 0xFF87, 0xFF88,
    0xFFFF, 0x001C, 0x00F9, 0x03F7, 0x0FF4, 0xFF89, 0xFF8A, 0xFF8B, 0xFF8C, 0xFF8D, 0xFF8E,
    0xFFFF, 0x003A, 0x01F7, 0x0FF5, 0xFF8F, 0xFF90, 0xFF91, 0xFF92, 0xFF93, 0xFF94, 0xFF95,
    0xFFFF, 0x003B, 0x03F8, 0xFF96, 0xFF97, 0xFF98, 0xFF99, 0xFF9A, 0xFF9B, 0xFF9C, 0xFF9D,
    0xFFFF, 0x007A, 0x07F7, 0xFF9E, 0xFF9F, 0xFFA0, 0xFFA1, 0xFFA2, 0xFFA3, 0xFFA4, 0xFFA5,
    0xFFFF, 0x007B, 0x0FF6, 0xFFA6, 0xFFA7, 0xFFA8, 0xFFA9, 0xFFAA, 0xFFAB, 0xFFAC, 0xFFAD,
    0xFFFF, 0x00FA, 0x0FF7, 0xFFAE, 0xFFAF, 0xFFB0, 0xFFB1, 0xFFB2, 0xFFB3, 0xFFB4, 0xFFB5,
    0xFFFF, 0x01F8, 0x7FC0, 0xFFB6, 0xFFB7, 0xFFB8, 0xFFB9, 0xFFBA, 0xFFBB, 0xFFBC, 0xFFBD,
    0xFFFF, 0x01F9, 0xFFBE, 0xFFBF, 0xFFC0, 0xFFC1, 0xFFC2, 0xFFC3, 0xFFC4, 0xFFC5, 0xFFC6,
    0xFFFF, 0x01FA, 0xFFC7, 0xFFC8, 0xFFC9, 0xFFCA, 0xFFCB, 0xFFCC, 0xFFCD, 0xFFCE, 0xFFCF,
    0xFFFF, 0x03F9, 0xFFD0, 0xFFD1, 0xFFD2, 0xFFD3, 0xFFD4, 0xFFD5, 0xFFD6, 0xFFD7, 0xFFD8,
    0xFFFF, 0x03FA, 0xFFD9, 0xFFDA, 0xFFDB, 0xFFDC, 0xFFDD, 0xFFDE, 0xFFDF, 0xFFE0, 0xFFE1,
    0xFFFF, 0x07F8, 0xFFE2, 0xFFE3, 0xFFE4, 0xFFE5, 0xFFE6, 0xFFE7, 0xFFE8, 0xFFE9, 0xFFEA,
    0xFFFF, 0xFFEB, 0xFFEC, 0xFFED, 0xFFEE, 0xFFEF, 0xFFF0, 0xFFF1, 0xFFF2, 0xFFF3, 0xFFF4,
    0x07F9, 0xFFF5, 0xFFF6, 0xFFF7, 0xFFF8, 0xFFF9, 0xFFFA, 0xFFFB, 0xFFFC, 0xFFFD, 0xFFFE
  };
  int* returnmat = new int[2];
  short ac_run, ac_cat;
  int i, j, flag = 0;
  data = data >> 16;
  data = data & 0x0000FFFF;

  if ((data & 0x8000) == 0) {                   //0...
    ac_run = 0;
    if ((data & 0x4000) == 0) {
      ac_cat = 1;
    } else {
      ac_cat = 2;
    }
  } else {
    if ((data & 0xC000) == 0x8000) {          // 10...
      ac_run = 0;
      if ((data & 0x2000) == 0)
        ac_cat = 3;
      else if ((data & 0x1000) == 0)
        ac_cat = 0;
      else
        ac_cat = 4;
    } else {
      if ((data & 0xE000) == 0xC000) {      // 110...
        if ((data & 0x1000) == 0) {
          ac_run = 1;
          ac_cat = 1;
        } else if ((data & 0x0800) == 0) {
          ac_run = 0;
          ac_cat = 5;
        } else {
          ac_run = 1;
          ac_cat = 2;
        }
      } else {
        if ((data & 0xF000) == 0xE000) {   //1110...
          ac_cat = 1;
          if ((data & 0x0800) == 0)
            ac_run = 2;
          else if ((data & 0x0400) == 0)
            ac_run = 3;
          else
            ac_run = 4;
        } else {
          if ((data & 0xF800) == 0xF000) {  // 1111 0...
            if ((data & 0x0400) == 0) {
              if ((data & 0x0200) == 0) {
                ac_run = 0;
                ac_cat = 6;
              } else {
                ac_run = 1;
                ac_cat = 3;
              }
            } else {
              ac_cat = 1;
              if ((data & 0x0200) == 0)
                ac_run = 5;
              else
                ac_run = 6;
            }
          } else {
            if ((data & 0xFC00) == 0xF800) {  //1111 10...
              if ((data & 0x0200) == 0) {
                if ((data & 0x0100) == 0) {
                  ac_run = 0;
                  ac_cat = 7;
                } else {
                  ac_run = 2;
                  ac_cat = 2;
                }
              } else {
                if ((data & 0x0100) == 0) {
                  ac_run = 7;
                  ac_cat = 1;
                } else if ((data & 0x0080) == 0) {
                  ac_run = 1;
                  ac_cat = 4;
                } else {
                  ac_run = 3;
                  ac_cat = 2;
                }
              }
            } else {
              if ((data & 0xFE00) == 0xFC00) {  //1111 110...
                if ((data & 0x0100) == 0) {
                  ac_cat = 1;
                  if ((data & 0x0080) == 0)
                    ac_run = 8;
                  else
                    ac_run = 9;
                } else {
                  if ((data & 0x0080) == 0) {
                    ac_run = 10;
                    ac_cat = 1;
                  } else if ((data & 0x0040) == 0) {
                    ac_run = 0;
                    ac_cat = 8;
                  } else {
                    ac_run = 2;
                    ac_cat = 3;
                  }
                }
              } else {
                if ((data & 0xFF00) == 0xFE00) {  //1111 1110...
                  if ((data & 0x0080) == 0) {
                    if ((data & 0x0040) == 0) {
                      ac_run = 4;
                      ac_cat = 2;
                    } else {
                      ac_run = 11;
                      ac_cat = 1;
                    }
                  } else {
                    if ((data & 0x0040) == 0) {
                      ac_run = 12;
                      ac_cat = 1;
                    } else if ((data & 0x0020) == 0) {
                      ac_run = 1;
                      ac_cat = 5;
                    } else {
                      ac_run = 5;
                      ac_cat = 2;
                    }
                  }
                } else {
                  if ((data & 0xFF80) == 0xFF00) {  //1111 1111 0...
                    if ((data & 0x0040) == 0) {
                      if ((data & 0x0020) == 0) {
                        ac_run = 13;
                        ac_cat = 1;
                      } else {
                        ac_run = 15;
                        ac_cat = 0;
                      }
                    } else if ((data & 0x0020) == 0) {
                      if ((data & 0x0010) == 0) {
                        ac_run = 2;
                        ac_cat = 4;
                      } else {
                        ac_run = 3;
                        ac_cat = 3;
                      }
                    } else {
                      ac_cat = 2;
                      if ((data & 0x0010) == 0)
                        ac_run = 6;
                      else
                        ac_run = 7;
                    }
                  } else {
                    if ((data & 0x007E) == 0) {
                      ac_run = 8;
                      ac_cat = 2;
                    } else {                       // 16 bit
                      for (i = 0; i < 16; i++) {
                        if (flag == 1) {
                          break;
                        }
                        for (j = 0; j < 11; j++) {
                          if ((short)data == acHuffman[i][j]) {
                            ac_run = i;
                            ac_cat = j;
                            flag = 1;
                            break;
                          }
                        }
                      } // for
                    } // 16 bit
                  } // 0xFF80
                } // 0xFF00
              } // 0xFE00
            } // 0xFC00
          } // 0xF800
        } // 0xF000
      } // 0xE000
    } // 0xC000
  } //0x8000
  returnmat[0] = ac_run;
  returnmat[1] = ac_cat;
  return returnmat;
}
short cvideo::symb2(short sym2, short cat)               // calculate the original data from symbol-2
{
  short code, symbol;

  symbol = sym2 << (16 - cat);
  if ((symbol & 0x8000) == 0x8000) {            // if the most significant bit is 1, the data is positive
    code = sym2;
  } else {                                       // the data is negtive
    code = -1 << cat;
    code = sym2 | code;
    code = code + 1;
  }
  return code;
}

void cvideo::ac_decoding(short** mat, int** intmat, int* preblock) {
  short acsym1, acsym2;
  int i, j, position;
  int data, bit;
  int* returnmat;
  for (j = 0; j < getNumberOfMacroBlocks(); j++) {
    position = 1;

    for (i = 1;; i++) {
      if (intmat[j][i] == 0xA0000000) { // check EOB
        mat[j][position] = 0;
        break;
      } else {
        returnmat = IacHuff(intmat[j][i]); // calculate RUN and CAT from ac codeword
        int ac_run = returnmat[0];
        int ac_cat = returnmat[1];
        for (j = 0; j < ac_run; j++) { // number of zeros
          mat[j][position] = 0;
          position++;
        }
        if (ac_run == 15 && ac_cat == 0) { // ZRL
          mat[j][position] = 0;
          position++;
        } else {
          bit = count(ac_run, ac_cat);
          bit += ac_cat; //  calculate number of significance bits bits
          data = intmat[j][i];
          data = data >> 1;
          data = data & 0x7FFFFFFF;
          data = data >> (32 - bit - 1); // shift the data back to the least significant bit
          acsym1 = data >> ac_cat; // separate symbol-1 and symbol-2
          acsym2 = data - (acsym1 << ac_cat);
          mat[j][position] = symb2(acsym2, ac_cat); //calculate original data from symbol-2
          position++; // current position of data
        }
      }
    } // for

    for (i = position + 1; i < 64; i++) { // after EOB all fill with 0
      mat[j][position] = 0;
    }
  }
}
short cvideo::IdcHuff(int data)                          // calculate CAT from dc codeword
{
  short cate;

  data = data >> 16;
  if ((data & 0x8000) == 0) {                   //first bit is 0: 0,1,2
    if ((data & 0x4000) == 0) {
      cate = 0;
    } else if ((data & 0x2000) == 0) {
      cate = 1;
    } else {
      cate = 2;
    }
  } // 0,1,2
  else {
    if ((data & 0x4000) == 0) {               //first two bits is 10: 3,4
      if ((data & 0x2000) == 0) {
        cate = 3;
      } else {
        cate = 4;
      }
    } // 3,4
    else {
      if ((data & 0x2000) == 0) {           // 110
        cate = 5;
      } else if ((data & 0x1000) == 0) {      // 1110
        cate = 6;
      } else if ((data & 0x0800) == 0) {      // 1111 0
        cate = 7;
      } else if ((data & 0x0400) == 0) {      // 1111 10
        cate = 8;
      } else if ((data & 0x0200) == 0) {      // 1111 110
        cate = 9;
      } else if ((data & 0x0100) == 0) {      // 1111 1110
        cate = 10;
      } else if ((data & 0x0080) == 0) {      // 1111 1111 0
        cate = 11;
      } else;
    } // 5~11
  }
  return cate;
}
void cvideo::dc_decoding(short** mat, int** intmat, int* Ipreblock)                 // function doing dc decoding
{
  int DCbit[12] =
  {
    2, 3, 3, 3, 3, 3, 4, 5, 6, 7, 8, 9
  };
  short dcsym1, dcsym2;
  short diff, cat;
  int data, bit, j;
  for (j = 0; j < getNumberOfMacroBlocks(); j++) {
    cat = IdcHuff(intmat[j][0]); // calculate CAT from dc codeword
    bit = DCbit[cat];
    bit += cat; // calculate number of significance bits bits
    data = intmat[j][0];
    data = data >> 1;
    data = data & 0x7FFFFFFF;
    data = data >> (32 - bit - 1); // shift the data back to the least significant bit
    dcsym1 = data >> cat; // separate symbol-1 and symbol-2
    dcsym2 = data - (dcsym1 << cat);
    diff = symb2(dcsym2, cat); // calculate diff from symbol-2

    mat[j][0] = diff + *Ipreblock; // calculate the original data
    *Ipreblock = mat[j][0];
  }
}
void cvideo::izigzag(short* block) {
  int i;
  short izig[64];

  izig[0] = block[0];
  izig[1] = block[1];
  izig[8] = block[2];
  izig[16] = block[3];
  izig[9] = block[4];
  izig[2] = block[5];
  izig[3] = block[6];
  izig[10] = block[7];
  izig[17] = block[8];
  izig[24] = block[9];
  izig[32] = block[10];
  izig[25] = block[11];
  izig[18] = block[12];
  izig[11] = block[13];
  izig[4] = block[14];
  izig[5] = block[15];
  izig[12] = block[16];
  izig[19] = block[17];
  izig[26] = block[18];
  izig[33] = block[19];
  izig[40] = block[20];
  izig[48] = block[21];
  izig[41] = block[22];
  izig[34] = block[23];
  izig[27] = block[24];
  izig[20] = block[25];
  izig[13] = block[26];
  izig[6] = block[27];
  izig[7] = block[28];
  izig[14] = block[29];
  izig[21] = block[30];
  izig[28] = block[31];
  izig[35] = block[32];
  izig[42] = block[33];
  izig[49] = block[34];
  izig[56] = block[35];
  izig[57] = block[36];
  izig[50] = block[37];
  izig[43] = block[38];
  izig[36] = block[39];
  izig[29] = block[40];
  izig[22] = block[41];
  izig[15] = block[42];
  izig[23] = block[43];
  izig[30] = block[44];
  izig[37] = block[45];
  izig[44] = block[46];
  izig[51] = block[47];
  izig[58] = block[48];
  izig[59] = block[49];
  izig[52] = block[50];
  izig[45] = block[51];
  izig[38] = block[52];
  izig[31] = block[53];
  izig[39] = block[54];
  izig[46] = block[55];
  izig[53] = block[56];
  izig[60] = block[57];
  izig[61] = block[58];
  izig[54] = block[59];
  izig[47] = block[60];
  izig[55] = block[61];
  izig[62] = block[62];
  izig[63] = block[63];

  for (i = 0; i < 64; i++) {
    block[i] = izig[i];
  }
}
void cvideo::iquantization(short** mat) {
  short iquant[64] =
  {
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99
  };
  izigzag(iquant);

  int i;
  for (int j = 0; j < getNumberOfMacroBlocks(); j++) {
    for (i = 0; i < 64; i++) {
      mat[j][i] = mat[j][i] * iquant[i];
    }
  }
}
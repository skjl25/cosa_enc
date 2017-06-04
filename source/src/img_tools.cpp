#include "../inc/img_tools.h"

void_t ImageTools::saveSequenceOfIplImages(char* filelocation, int num_sequence,
  IplImage* foregroundImg) {
  char* fileName2 = new char[100];
  sprintf(fileName2, "%s_%d.pgm", filelocation, num_sequence);
  cvSaveImage(fileName2, foregroundImg);
  delete[] fileName2;
}

void_t ImageTools::convertToGrayScale(char* origImg, char* grayG2ImgChar,
  uint32_t width, uint32_t height) {
  for (uint32_t i = 0; i < width*height; i++) {
    uint32_t location = 3 * i + 1;
    grayG2ImgChar[i] = origImg[location];
  }
}

IplImage* ImageTools::convertColorArrayToIplImage(char* orgArray,
  uint32_t height,
  uint32_t width) {
  IplImage* returnImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
  int index = height*width;
  uint32_t offset = returnImg->widthStep;
  uint32_t num_channels = returnImg->nChannels;
  char* image_data = returnImg->imageData;
  for (uint32_t i = 0; i < height; i++) {
    for (uint32_t j = 0; j < width; j++) {
      int rVal = orgArray[index + 0];
      int gVal = orgArray[index + 1];
      int bVal = orgArray[index + 2];

      ((uchar *)(image_data + i*offset))[j*num_channels + 0] = bVal; // B
      ((uchar *)(image_data + i*offset))[j*num_channels + 1] = gVal; // G
      ((uchar *)(image_data + i*offset))[j*num_channels + 2] = rVal; // R

      index = index + 3;
    }
  }
  return returnImg;
}

IplImage* ImageTools::convertColorArrayToIplImage_ext(char* orgArray,
  uint32_t height,
  uint32_t width) {
  IplImage* returnImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
  int idx = 0;
  uint32_t offset = returnImg->widthStep;
  uint32_t num_channels = returnImg->nChannels;
  char* image_data = returnImg->imageData;
  for (uint32_t i = height - 1; i = 0; i--) {
    for (uint32_t j = 0; j < width; j++) {
      int rVal = orgArray[idx + 0];
      int gVal = orgArray[idx + 1];
      int bVal = orgArray[idx + 2];

      ((uchar *)(image_data + i*offset))[j*num_channels + 0] = bVal; // B
      ((uchar *)(image_data + i*offset))[j*num_channels + 1] = gVal; // G
      ((uchar *)(image_data + i*offset))[j*num_channels + 2] = rVal; // R

      idx = idx + 3;
    }
  }
  return returnImg;
}

void_t ImageTools::convertIplImageToCharArray(char* returnArray,
  IplImage* orgImage, uint32_t height,
  uint32_t width) {
  uint32_t offset = orgImage->widthStep;
  for (uint32_t i = 0; i < height; i++) {
    for (uint32_t j = 0; j < width; j++) {
      returnArray[i*width + j] = ((uchar*)(orgImage->imageData + i*offset))[j];
    }
  }
  //	return returnArray;
}

void_t ImageTools::convertColorIplImageToCharArray(char* returnArray,
  IplImage* orgImage,
  uint32_t height,
  uint32_t width) {
  int index = 0;
  char* image_data = orgImage->imageData;
  uint32_t offset = orgImage->widthStep;
  uint32_t num_channels = orgImage->nChannels;
  for (uint32_t i = 0; i < height; i++) {
    for (uint32_t j = 0; j < width; j++) {
      int rVal = ((uchar *)(image_data + i*offset))[j*num_channels + 2];
      int gVal = ((uchar *)(image_data + i*offset))[j*num_channels + 1];
      int bVal = ((uchar *)(image_data + i*offset))[j*num_channels + 0];

      returnArray[index + 0] = rVal;
      returnArray[index + 1] = gVal;
      returnArray[index + 2] = bVal;

      index = index + 3;
    }
  }
}

double_t ImageTools::compute_ipl_img_sdv(IplImage* data) {
  double_t sum = 0;
  uint32_t offset = data->widthStep;
  for (int i = 0; i < data->height; i++) {
    for (int j = 0; j < data->width; j++) {
      sum = ((uchar*)(data->imageData + i*offset))[j] + sum;
    }
  }

  double_t threshMean = sum / (data->height*data->width);
  double_t threshSdv = 0;
  for (int i = 0; i < data->height; i++) {
    for (int j = 0; j < data->width; j++) {
      threshSdv = pow((((uchar*)(data->imageData + i*offset))[j] - threshMean),
        2) + threshSdv;
    }
  }
  threshSdv = sqrt(threshSdv / (data->height*data->width));
  return threshSdv;
}

int ImageTools::compute_ipl_img_avg(IplImage* data) {
  int sum = 0;
  for (int i = 0; i < data->height; i++) {
    for (int j = 0; j < data->width; j++) {
      sum = ((uchar*)(data->imageData + i*data->widthStep))[j] + sum;
    }
  }

  int threshMean = sum / (data->height*data->width);
  return threshMean;
}

double_t ImageTools::compute_euclidean_dist(int x1, int x2, int y1, int y2) {
  double_t euclidean_dist;

  double_t xSquare = pow(double_t(x1 - x2), 2);
  double_t ySquare = pow(double_t(y1 - y2), 2);
  euclidean_dist = sqrt(xSquare + ySquare);

  return euclidean_dist;
}

IplImage* ImageTools::getSynthesizedImage_gray(IplImage* leftIm,
  IplImage* rightIm,
  int* dispMapMat, double_t alpha) {
  int dispVal = 0;
  IplImage *iirImage = cvCreateImage(cvSize(rightIm->width, rightIm->height),
    IPL_DEPTH_8U, 1);
  IplImage *iImage = cvCreateImage(cvSize(rightIm->width, rightIm->height),
    IPL_DEPTH_8U, 1);

  int dispMapMatIndex = 0;
  for (int i = 0; i < rightIm->height; i++) {
    for (int j = 0; j < rightIm->width; j++) {
      int indexY = dispMapMatIndex / (rightIm->width);
      int indexX = dispMapMatIndex % (rightIm->width);

      int pixel = ((uchar*)(rightIm->imageData + i*rightIm->widthStep))[j];

      dispVal = dispMapMat[i*rightIm->width + j];
      double_t temp_iirX = j + (1 - alpha)*dispVal;
      int iirX = (int)temp_iirX;

      ///////////////////////////////////////////
      if (iirX > rightIm->width || iirX < 0) {
        if (iirX > rightIm->width) {
          iirX = rightIm->width - 1;
        } else {
          iirX = 0;
        }
      }
      ///////////////////////////////////////////
      int iirY = i;
      ((uchar*)(iirImage->imageData + iirY*iirImage->widthStep))[iirX] = pixel;
    }
  }

  for (int i = 0; i < leftIm->height; i++) {
    for (int j = 0; j < leftIm->width; j++) {
      //Method without left+dispVal*alpha
      //			int leftImgPixel=((uchar*)(leftImGray->imageData+i*leftImGray->widthStep))[j];

      //Method with left+dispVal*alpha
      int newX = j + (int)(alpha*dispMapMat[i*rightIm->width + j]);

      ///////////////////////////////////////////
      if (newX > rightIm->width || newX < 0) {
        if (newX > rightIm->width) {
          newX = rightIm->width - 1;
        } else {
          newX = 0;
        }
      }
      ///////////////////////////////////////////

      int leftImgPixel = ((uchar*)(leftIm->imageData + i*leftIm->widthStep))[newX];

      int rightImgPixel = ((uchar*)(iirImage->imageData + i*iirImage->widthStep))[j];

      if (rightImgPixel == 0) {
        ((uchar*)(iImage->imageData + i*iImage->widthStep))[j] = leftImgPixel;
      } else {
        ((uchar*)(iImage->imageData + i*iImage->widthStep))[j] = rightImgPixel;
      }
    }
  }
  cvReleaseImage(&iirImage);

  return iImage;
}

IplImage* ImageTools::getSynthesizedImage_rgb(IplImage* leftIm,
  IplImage* rightIm,
  int** dispMapMat, double_t alpha) {
  int dispVal = 0;
  IplImage *iirCImage = cvCreateImage(cvSize(rightIm->width, rightIm->height),
    IPL_DEPTH_8U, 3);
  IplImage *iilCImage = cvCreateImage(cvSize(rightIm->width, rightIm->height),
    IPL_DEPTH_8U, 3);
  IplImage *iCImage = cvCreateImage(cvSize(rightIm->width, rightIm->height),
    IPL_DEPTH_8U, 3);

#ifdef _GRAY
  for (int i = 0; i < rightImGray->height; i++) {
    for (int j = 0; j < rightImGray->width; j++) {
      int pixel = ((uchar*)(rightImGray->imageData + i*rightImGray->widthStep))[j];

      dispVal = dispMapMat[i][j];
      double_t temp_iirX = j + (1 - alpha)*dispVal;
      int iirX = (int)temp_iirX;

      int iirY = i;
      ((uchar*)(iirImage->imageData + iirY*iirImage->widthStep))[iirX] = pixel;
    }
  }
#endif

  //	getDispMapFromSynthesizedImage(iirImage,rightImGray);

  for (int i = 0; i < rightIm->height; i++) {
    for (int j = 0; j < rightIm->width; j++) {
      int pixelB = ((uchar *)(rightIm->imageData + i*rightIm->widthStep))[j*rightIm->nChannels + 0]; // B
      int pixelG = ((uchar *)(rightIm->imageData + i*rightIm->widthStep))[j*rightIm->nChannels + 1]; // G
      int pixelR = ((uchar *)(rightIm->imageData + i*rightIm->widthStep))[j*rightIm->nChannels + 2]; // R

      dispVal = dispMapMat[i][j];
      double_t temp_iirX = j + (1 - alpha)*dispVal;
      int iirX = (int)temp_iirX;

      int iirY = i;
      ((uchar *)(iirCImage->imageData + iirY*iirCImage->widthStep))[iirX*iirCImage->nChannels + 0] = pixelB; // B
      ((uchar *)(iirCImage->imageData + iirY*iirCImage->widthStep))[iirX*iirCImage->nChannels + 1] = pixelG; // G
      ((uchar *)(iirCImage->imageData + iirY*iirCImage->widthStep))[iirX*iirCImage->nChannels + 2] = pixelR; // R
    }
  }

  for (int i = 0; i < leftIm->height; i++) {
    for (int j = 0; j < leftIm->width; j++) {
      //Method without left+dispVal*alpha
      //				int pixelLB=((uchar *)(leftIm->imageData + i*leftIm->widthStep))[j*leftIm->nChannels + 0]; // B
      //				int pixelLG=((uchar *)(leftIm->imageData + i*leftIm->widthStep))[j*leftIm->nChannels + 1]; // G
      //				int pixelLR=((uchar *)(leftIm->imageData + i*leftIm->widthStep))[j*leftIm->nChannels + 2]; // R

      //Method with left+dispVal*alpha
      int newX = j + (int)(alpha*dispMapMat[i][j]);
      int pixelLB = ((uchar *)(leftIm->imageData + i*leftIm->widthStep))[newX*leftIm->nChannels + 0]; // B
      int pixelLG = ((uchar *)(leftIm->imageData + i*leftIm->widthStep))[newX*leftIm->nChannels + 1]; // G
      int pixelLR = ((uchar *)(leftIm->imageData + i*leftIm->widthStep))[newX*leftIm->nChannels + 2]; // R

      int pixelRB = ((uchar *)(iirCImage->imageData + i*iirCImage->widthStep))[j*iirCImage->nChannels + 0]; // B
      int pixelRG = ((uchar *)(iirCImage->imageData + i*iirCImage->widthStep))[j*iirCImage->nChannels + 1]; // G
      int pixelRR = ((uchar *)(iirCImage->imageData + i*iirCImage->widthStep))[j*iirCImage->nChannels + 2]; // R

      if (pixelRB == 0 && pixelRG == 0 && pixelRR == 0) {
        //					((uchar*)(iImage->imageData+i*iImage->widthStep))[j]=leftImgPixel;
        ((uchar *)(iCImage->imageData + i*iCImage->widthStep))[j*iCImage->nChannels + 0] = pixelLB; // B
        ((uchar *)(iCImage->imageData + i*iCImage->widthStep))[j*iCImage->nChannels + 1] = pixelLG; // G
        ((uchar *)(iCImage->imageData + i*iCImage->widthStep))[j*iCImage->nChannels + 2] = pixelLR; // R
      } else {
        //					((uchar*)(iImage->imageData+i*iImage->widthStep))[j]=rightImgPixel;
        ((uchar *)(iCImage->imageData + i*iCImage->widthStep))[j*iCImage->nChannels + 0] = pixelRB; // B
        ((uchar *)(iCImage->imageData + i*iCImage->widthStep))[j*iCImage->nChannels + 1] = pixelRG; // G
        ((uchar *)(iCImage->imageData + i*iCImage->widthStep))[j*iCImage->nChannels + 2] = pixelRR; // R
      }
    }
  }

  return iCImage;
}

//void_t ImageTools::save_pgm(string filename, char* data, unsigned int width, unsigned int height)
//{
//	FILE* fid = fopen(filename.c_str(), "w");
//	fprintf(fid,"P5\n%u %u\n255\n", width, height);
//	fwrite(data, 1, width*height, fid);
//	fclose(fid);
//}

char* ImageTools::load_pgm(string filename, unsigned int& width,
  unsigned int& height) {
  char * data;
  char tmpc;
  string type;
  int max;
  int at;

  ifstream ins;
  ins.open(filename.c_str(), ios::binary);
  ins >> type;
  ins >> width >> height;
  ins >> max;

  //cout<<endl<<"Type: "<<type;
  //cout<<endl<<"Dimensions: "<<width<<" x "<<height;
  //cout<<endl<<"Max value: "<<max;
  //cout<<endl;

  data = new char[width * height];

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      at = y*width + x;
      ins.read((char*)&tmpc, sizeof(char));
      data[at] = tmpc;
    }
  }

  ins.close();

  return data;
}

void_t ImageTools::save_ppm_unsigned(string filename, unsigned char* data,
  uint32_t width, uint32_t height) {
  FILE* fid = fopen(filename.c_str(), "w");
  fprintf(fid, "P6\n%u %u\n255\n", width, height);
  fwrite(data, 1, width*height * 3, fid);
  fclose(fid);
}

char* ImageTools::load_ppm(string filename, unsigned int& width,
  unsigned int& height) {
  char * data;
  char tmpc;
  string type;
  int max;
  int at;
  string line;
  istringstream istr;
  int i;

  ifstream ins;
  ins.open(filename.c_str(), ios::binary);

  i = 0;
  while (i < 3) {
    getline(ins, line);
    if (line.at(0) != '#') {	//Ignore comments
      if (i == 0) {
        type = line;
      } else if (i == 1) {
        istr.clear();
        istr.str(line);
        istr >> width;
        istr >> height;
      } else if (i == 2) {
        max = atoi(line.c_str());
      }

      i++;
    }
  }

  //cout<<endl<<"Type: "<<type;
  //cout<<endl<<"Dimensions: "<<width<<" x "<<height;
  //cout<<endl<<"Max value: "<<max;
  //cout<<endl;

  data = new char[width * height * 3];

  for (uint32_t y = 0; y < height; y++) {
    for (uint32_t x = 0; x < width; x++) {
      at = y*width * 3 + x * 3;
      ins.read((char*)&tmpc, sizeof(char));
      data[at + 0] = tmpc;
      ins.read((char*)&tmpc, sizeof(char));
      data[at + 1] = tmpc;
      ins.read((char*)&tmpc, sizeof(char));
      data[at + 2] = tmpc;
    }
  }

  ins.close();

  return data;
}

void_t ImageTools::save_ppm(string filename, char* data,
  uint32_t width, uint32_t height) {
  FILE* fid = fopen(filename.c_str(), "w");
  fprintf(fid, "P6\n%u %u\n255\n", width, height);
  fwrite(data, 1, width*height * 3, fid);
  fclose(fid);
}

void_t ImageTools::extractVideoSequenceToImg(char* movieFileLocation,
  char* saveFileLocation,
  char* saveFileType) {
  //char fileName[200];
  //sprintf(fileName, "%s.avi", movieFileLocation);
  printf("%s\n", movieFileLocation);

  CvCapture* capture = 0;
  IplImage* frame = 0;
  capture = cvCaptureFromAVI(movieFileLocation); // read AVI video
  if (!capture) {
    throw "Error when reading video file";
  }

  int counter = 0;
  while (true) {
    char buffer[200];
    sprintf(buffer, "%s_%d.%s", movieFileLocation, counter, saveFileType);

    /*  int cvGrabFrame (CvCapture* capture);
    IplImage* cvRetrieveFrame (CvCapture* capture)*/
    printf("%s\n", buffer);
    frame = cvQueryFrame(capture);
    if (!frame)
      break;
    cvSaveImage(buffer, frame);
    //cvNamedWindow( "w", CV_WINDOW_AUTOSIZE);
    //cvShowImage("w", frame);
    cvWaitKey(1);
    counter++;
  }
}
//Calculates image PSNR value
double_t ImageTools::get_image_psnr(uint8_t* rec_img, uint8_t* org_img,
  uint32_t width, uint32_t height) {
  double_t mse = 0.0;
  double_t mse_temp = 0.0;
  double_t psnr = 0.0;
  uint32_t idx;

  //Calculate MSE
  for (idx = 0; idx < width*height; idx++) {
    mse_temp = abs(rec_img[idx] - org_img[idx]);
    mse += mse_temp*mse_temp;
  }
  mse /= width*height;

  //Avoid_t division by zero
  if (mse == 0) return 99.0;

  //The PSNR
  psnr = 10 * log10(MAX_1 / mse);

  //Thats it.
  return psnr;
}
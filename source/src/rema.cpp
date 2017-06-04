#include "../inc/rema.h"

ReMA::ReMA(int width, int height) {
  setImageheight(height);
  setImagewidth(width);
  setEncodedVisibleData();
  setReducedVisibleData();
  setEncodedDepthData();
  setDecodedBlock();
  setEncodedMergeVisibleDepthData();
  setDecodedVisibleData();
  setReconstructedVisibleData();
  setDecodedReducedVisibleData();
}

ReMA::~ReMA() {
  // TODO Auto-generated destructor stub
}

//For 2x2 matrix
void_t ReMA::mergeVisibleDepthData() {
  int indexForDepthBlock = 0;
  int length = (width*height) / 2;
  for (int i = 0; i < length; i++) {
    if (i >= length / 2) {
      encoded_merge_visible_depth_data[i] = encoded_depth_data[indexForDepthBlock];
      indexForDepthBlock++;
    } else {
      encoded_merge_visible_depth_data[i] = encoded_reduced_visible_data[i];
    }
  }
}

//int* ReMA::getLowFreqArray(int* dwtOrgImage,int width,int height){
//	int* lowPassFilter=new int[(width*height)/2];
//	//creating low freq
//	int j=0;
//	for(int i=0;i<width*height;i=i+2)
//	{
//		lowPassFilter[j]=((int)dwtOrgImage[i]+(int)dwtOrgImage[i+1])/2;
//		j++;
//	}
//	return lowPassFilter;
//}

//int* ReMA::getHighFreqArray(int* dwtOrgImage,int* lowPassFilter, int width,int height){
//	int* highPassFilter=new int[(width*height)/2];
//	//creating low freq
//	//creating high freq
//	int j=0;
//	for(int i=0;i<width*height;i=i+2)
//	{
//		highPassFilter[j]=(int)dwtOrgImage[i]-(int)lowPassFilter[j];
//		j++;
//	}
//	return highPassFilter;
//}

//int* ReMA::getLowFreqArray(double_t* dwtOrgImage,int width,int height){
//	int* lowPassFilter=new int[(width*height)/2];
//	//creating low freq
//	int j=0;
//	for(int i=0;i<width*height;i=i+2)
//	{
//		lowPassFilter[j]=((int)dwtOrgImage[i]+(int)dwtOrgImage[i+1])/2;
//		j++;
//	}
//	return lowPassFilter;
//}

//int* ReMA::getHighFreqArray(double_t* dwtOrgImage,int* lowPassFilter, int width,int height){
//	int* highPassFilter=new int[(width*height)/2];
//	//creating low freq
//	//creating high freq
//	int j=0;
//	for(int i=0;i<width*height;i=i+2)
//	{
//		highPassFilter[j]=(int)dwtOrgImage[i]-(int)lowPassFilter[j];
//		j++;
//	}
//	return highPassFilter;
//}

int* ReMA::getLowFreqArrayExt(double_t* dwtOrgImage, int width, int height) {
  //	int* lowPassFilter=new int[(width*height)/2];
  //	int* highPassFilter=new int[(width*height)/2];
  int* oneMergeFilter = new int[width*height];

  //creating low freq
  int j = 0;
  int index = 0;
  for (int i = 0; i < width*height; i = i + 2) {
    oneMergeFilter[index] = ((int)dwtOrgImage[i] + (int)dwtOrgImage[i + 1]) / 2;
    oneMergeFilter[index + (width*height / 2)] = (int)dwtOrgImage[i] - oneMergeFilter[index];
    j++;
    index++;
  }
  return oneMergeFilter;
}

int* ReMA::getEncodedDWTExt(double_t* dwtOrgImage, int width, int height) {
  int* highPassFilter = new int[(width*height) / 2];
  int* oneMergeFilter = 0;
  int* lowPassFilter = new int[(width*height) / 2];

  oneMergeFilter = new int[width*height];

  lowPassFilter = getLowFreqArray(dwtOrgImage, width, height);
  highPassFilter = getHighFreqArray(dwtOrgImage, lowPassFilter, width, height);

  //merging high and low freq into one
  int j = 0;
  for (int i = 0; i < width*height; i++) {
    if (i < (width*height) / 2) {
      oneMergeFilter[i] = lowPassFilter[i];
    } else {
      oneMergeFilter[i] = highPassFilter[j];
      j++;
    }
  }
  delete[] highPassFilter;
  delete[] lowPassFilter;
  highPassFilter = NULL;
  lowPassFilter = NULL;
  return oneMergeFilter;
}

void_t ReMA::rema_visible_encode(int* dwtOrgImage) {
  int* highPassFilter = NULL;
  int* lowPassFilter = NULL;

  lowPassFilter = getLowFreqArray(dwtOrgImage, width, height);
  highPassFilter = getHighFreqArray(dwtOrgImage, lowPassFilter, width, height);

  //merging high and low freq into one
  int j = 0;
  for (uint32_t i = 0; i < width*height; i++) {
    if (i < (width*height) / 2) {
      encoded_visible_data[i] = lowPassFilter[i];
    } else {
      encoded_visible_data[i] = highPassFilter[j];
      j++;
    }
  }

  for (int i = 0; i < (int)(width*height / 4); i++) {
    int indexY = i / (width / 2);
    int indexX = i % (width / 2);

    //		blockTopLeft[(width*indexY)+indexX]=oneMergeFilter[(width*indexY)+indexX];
    encoded_reduced_visible_data[i] = encoded_visible_data[(width*indexY) + indexX];
  }

  delete[] highPassFilter;
  delete[] lowPassFilter;
  highPassFilter = NULL;
  lowPassFilter = NULL;

  //	return oneMergeFilter;
}
void_t ReMA::rema_visible_encode(double_t* dwtOrgImage) {
  int* highPassFilter = NULL;
  int* lowPassFilter = NULL;

  lowPassFilter = getLowFreqArray(dwtOrgImage, width, height);
  highPassFilter = getHighFreqArray(dwtOrgImage, lowPassFilter, width, height);

  //merging high and low freq into one
  int j = 0;
  for (uint32_t i = 0; i < width*height; i++) {
    if (i < (width*height) / 2) {
      encoded_visible_data[i] = lowPassFilter[i];
    } else {
      encoded_visible_data[i] = highPassFilter[j];
      j++;
    }
  }

  for (int i = 0; i < (int)(width*height / 4); i++) {
    int indexY = i / (width / 2);
    int indexX = i % (width / 2);

    //		blockTopLeft[(width*indexY)+indexX]=oneMergeFilter[(width*indexY)+indexX];
    encoded_reduced_visible_data[i] = encoded_visible_data[(width*indexY) + indexX];
  }

  delete[] highPassFilter;
  delete[] lowPassFilter;
  highPassFilter = NULL;
  lowPassFilter = NULL;
  //	return oneMergeFilter;
}

//int* ReMA::getEncodedDWT(double_t* dwtOrgImage,int width, int height){
//	int* highPassFilter=NULL;
//	int* oneMergeFilter=NULL;
//	int* lowPassFilter=NULL;
//
//	oneMergeFilter=new int[width*height];
//
//	lowPassFilter=getLowFreqArray(dwtOrgImage,width,height);
//	highPassFilter=getHighFreqArray(dwtOrgImage,lowPassFilter,width,height);
//
//	//merging high and low freq into one
//	int j=0;
//	for(int i=0;i<width*height;i++)
//	{
//		if(i<(width*height)/2)
//		{
//			oneMergeFilter[i]=lowPassFilter[i];
//		}
//		else
//		{
//			oneMergeFilter[i]=highPassFilter[j];
//			j++;
//		}
//	}
//	delete[] highPassFilter;
//	delete[] lowPassFilter;
//	highPassFilter=NULL;
//	lowPassFilter=NULL;
//	return oneMergeFilter;
//}
//
//int* ReMA::getEncodedDWT(int* dwtOrgImage,int width, int height){
//	int* highPassFilter=NULL;
//	int* oneMergeFilter=NULL;
//	int* lowPassFilter=NULL;
//
//	oneMergeFilter=new int[width*height];
//
//
//	lowPassFilter=getLowFreqArray(dwtOrgImage,width,height);
//	highPassFilter=getHighFreqArray(dwtOrgImage,lowPassFilter,width,height);
//
//	//merging high and low freq into one
//	int j=0;
//	for(int i=0;i<width*height;i++)
//	{
//		if(i<(width*height)/2)
//		{
//			oneMergeFilter[i]=lowPassFilter[i];
//		}
//		else
//		{
//			oneMergeFilter[i]=highPassFilter[j];
//			j++;
//		}
//	}
//	delete[] highPassFilter;
//	delete[] lowPassFilter;
//	highPassFilter=NULL;
//	lowPassFilter=NULL;
//
//	return oneMergeFilter;
//}

void_t ReMA::extractVisibleDepthData(char* receivedData) {
  int indexT = 0;
  int indexT2 = 0;
  for (uint32_t i = 0; i < (width*height) / 2; i++) {
    uint8_t temp = (uint8_t)receivedData[i];
    if (i < (width*height) / 4) {
      decoded_reduced_visible_data[indexT] = temp;
      indexT++;
    } else {
      encoded_depth_data[indexT2] = temp;
      indexT2++;
    }
  }
}

void_t ReMA::getReplicateBlocks(int compressedIndicator) {
  //	int* oneMergeFilter=new int[width*height];

  for (uint32_t i = 0; i < (width*height / compressedIndicator); i++) {
    int indexY = i / (width / 2);
    int indexX = i % (width / 2);

    //Debugging purpose
    //		printf("i:%d  indexY:%d \n",i,indexY);
    decoded_visible_data[(width*indexY) + indexX] = decoded_reduced_visible_data[i];

    //replication process
    decoded_visible_data[((width*indexY) + indexX) + (width / 2)] = decoded_reduced_visible_data[i];
  }

  //	return oneMergeFilter;
}

void_t ReMA::rema_visible_decode() {
  //	int* dwtRecoveredImage=new int[width*height];

  for (uint32_t i = 0; i < (width*height) / 2; i++) {
    double_t recoverVal1 = decoded_visible_data[i] + (decoded_visible_data[i + (width*height / 2)]);
    double_t recoverVal2 = decoded_visible_data[i] - (decoded_visible_data[i + (width*height / 2)]);
    reconstructed_visible_data[i * 2] = (int)recoverVal1;
    reconstructed_visible_data[(i * 2) + 1] = (int)recoverVal2;
  }
  //	return dwtRecoveredImage;
}

//void_t ReMA::rema_visible_decode(double_t* oneMergeFilter,int width,int height){
//	int* dwtRecoveredImage=new int[width*height];
//
//	for(int i=0;i<(width*height)/2;i++)
//	{
//		double_t recoverVal1=oneMergeFilter[i]+(oneMergeFilter[i+(width*height/2)]);
//		double_t recoverVal2=oneMergeFilter[i]-(oneMergeFilter[i+(width*height/2)]);
//		dwtRecoveredImage[i*2]=recoverVal1;
//		dwtRecoveredImage[(i*2)+1]=recoverVal2;
//	}
//	return dwtRecoveredImage;
//
//}

int* ReMA::getDWTBlock(int* oneMergeFilter, int width, int height, int compressedIndicator) {
  int* blockTopLeft = new int[(width*height) / compressedIndicator];
  Utility testTimer;
  //testTimer.startTimer();

  for (int i = 0; i < (width*height / compressedIndicator); i++) {
    int indexY = i / (width / 2);
    int indexX = i % (width / 2);

    //Debugging purpose
    //		printf("i:%d  indexY:%d \n",i,indexY);
    blockTopLeft[i] = oneMergeFilter[(width*indexY) + indexX];
  }
  return blockTopLeft;
}

void_t ReMA::rema_depth_decode_2() {
  int expValIndex = 0;
  for (uint32_t i = 0; i < (height*width) / 4; i++) {
    int indexY = expValIndex / (width);
    int indexX = expValIndex % (width);
    if (indexY % 2 != 0) {
      //			indexY++;
      expValIndex = width + expValIndex;
      i--;
    } else {
      decodedBlock[width*indexY + indexX] = encoded_depth_data[i];
      decodedBlock[width*indexY + indexX + 1] = encoded_depth_data[i];
      decodedBlock[width*(indexY + 1) + indexX] = encoded_depth_data[i];
      decodedBlock[width*(indexY + 1) + (indexX + 1)] = encoded_depth_data[i];
      expValIndex = expValIndex + 2;
    }
    //				printf("%d %d\n",indexY,indexX);
  }
}

void_t ReMA::rema_depth_encode_2(int* realDepthVal1D) {
  int depthHolder = 0;

  int returnValIndex = 0;

  for (uint32_t i = 0; i < width*height; i = i + 2) {
    int indexY = i / (width);
    int indexX = i % (width);
    if (indexY % 2 != 0) {
      //			indexY++;
      i = i + width - 2;
    } else {
      depthHolder = (int)(realDepthVal1D[i] + realDepthVal1D[i + 1] + realDepthVal1D[i + width] + realDepthVal1D[i + width + 1]) / 4;
      encoded_depth_data[returnValIndex] = depthHolder;
      returnValIndex++;
      //			printf("%d  %d\n",indexY,indexX);
    }
  }
}
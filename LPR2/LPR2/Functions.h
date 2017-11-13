#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

void otsu(Mat Imgs,Mat plates,Mat binPlates,Mat greyImgs){
	int otsuTreshold = 0;
	double myu[256] = { 0 };
	double sigma[256] = { 0 }; 
	double probability[256] = { 0 }; 
	double findOptimum[256] = { 0 };
	double grabOptimum = 0.0;
	int counts[256] = { 0 };



	for (int i = 0; i < plates.rows; i++){
		for (int j = 0; j < plates.cols; j++){
			counts[greyImgs.at<uchar>(i, j)] ++;
		}
	}

	for (int i = 0; i < 256; i++){
		probability[i] = counts[i] / (double)( Imgs.rows * Imgs.cols);
	}

	sigma[0] = probability[0];
	myu[0] = 0;

	for (int i = 1; i < 256; i++){
		sigma[i] = sigma[i - 1] + probability[i];
		myu[i] = myu[i - 1] + i*probability[i];
	}

	for (int i = 0; i < 256; i++){
		findOptimum[i] = pow(255 * sigma[i] - myu[i], 2) / sigma[i] * (1 - sigma[i]);

		if (findOptimum[i] > grabOptimum){
			grabOptimum = findOptimum[i];
			otsuTreshold = i;
		}
	}

	for (int i = 1; i < plates.rows; i++) {
		for (int j = 1; j < plates.cols; j++){
			if (plates.at<uchar>(i, j) > otsuTreshold){
				binPlates.at<uchar>(i, j) = 255;
			}
			else{
				binPlates.at<uchar>(i, j) = 0;
			}
		}
	}
}


void greyscaleConversion(Mat img ,Mat greyImg){
	for (int i = 0; i < img.rows; i++){
		for (int j = 0; j < img.cols * 3; j += 3){
			greyImg.at<uchar>(i, j / 3) = (img.at<uchar>(i, j) + img.at<uchar>(i, j + 1) + img.at<uchar>(i, j + 2)) / 3;
		}
	}

}

void findSobel(Mat img,Mat EnhGreyImg,Mat sobelImg,Mat dilsobelImg,int val){
	int sobel = 0;

	for (int i = 1 ; i < img.rows -1; i++){
		for (int j = 1 ; j < img.cols-1 ; j++){
			int Cx = ((EnhGreyImg.at<uchar>(i - 1, j - 1)*-1) + (EnhGreyImg.at<uchar>(i-1,j)*-2) + (EnhGreyImg.at<uchar>(i - 1, j +1)*-1)
						+ (EnhGreyImg.at<uchar>(i + 1 , j - 1)*1) + (EnhGreyImg.at<uchar>(i + 1, j )*2) + (EnhGreyImg.at<uchar>(i + 1, j + 1)*1));
			
			int Cy = ((EnhGreyImg.at<uchar>(i - 1 , j - 1)*-1) + (EnhGreyImg.at<uchar>(i ,j-1)*-2) + (EnhGreyImg.at<uchar>(i + 1, j -1)*-1)
						+ (EnhGreyImg.at<uchar>(i - 1 , j + 1)*1) + (EnhGreyImg.at<uchar>(i , j + 1 )*2) + (EnhGreyImg.at<uchar>(i + 1, j + 1)*1));

			sobel = abs(Cx)+abs(Cy);
			if (sobel > val){
				sobelImg.at<uchar>(i,j) = 255;
			}
		}
	}


	for (int i = 1; i < img.rows - 1; i++){
		for (int j = 1; j < img.cols - 1; j++){
			for (int ii =-1 ; ii <=1 ; ii++){
				for (int jj = -1 ; jj <=1 ; jj++){
					if (sobelImg.at<uchar>(i+ii,j+jj) == 255)
						dilsobelImg.at<uchar>(i,j) = 255;
				}
			}
		}
	}
}


void imgEnhancing(Mat img,Mat greyImg,Mat EnhGreyImg){
	double prob[256] = { 0 };
	for (int i = 0; i < img.rows; i++){
		for (int j = 0; j < img.cols; j++){
			prob[greyImg.at<uchar>(i, j)]++;
		}
	}
	double accProb = 0;
	for (int i = 0; i < 256; i++){
		prob[i] = prob[i] / (img.rows*img.cols);
		accProb += prob[i];
		prob[i] = floor((255 * accProb)+0.5);
	}

	for (int i = 0; i < img.rows; i++){
		for (int j = 0; j < img.cols; j++){
			EnhGreyImg.at<uchar>(i, j) = (uchar)prob[greyImg.at<uchar>(i, j)];
		}
	}
}

void findEdge(Mat imgs,Mat greyImgs,Mat edgeImgs,int val){
	for (int i = 1; i < imgs.rows - 1; i++){
		for (int j = 1; j < imgs.cols - 1; j++){
			int iLeft = (greyImgs.at<uchar>(i - 1, j - 1) + greyImgs.at<uchar>(i, j - 1) + greyImgs.at<uchar>(i + 1, j - 1))/3;
			int iRight = (greyImgs.at<uchar>(i - 1, j + 1) + greyImgs.at<uchar>(i, j + 1) + greyImgs.at<uchar>(i + 1, j + 1))/3;

			if (abs(iRight - iLeft) > val)
				edgeImgs.at<uchar>(i, j) = 255;
			else
				edgeImgs.at<uchar>(i, j) = 0;
		}
	}
}


void dilateImg(Mat imgs,Mat edgeImgs,Mat dilImgs,int val){
	for (int i = val; i < imgs.rows - val; i++){
		for (int j = val; j < imgs.cols - val; j++){
			for (int ii =-val ; ii <=val ; ii++){
				for (int jj = -val ; jj <=val ; jj++){
					if (edgeImgs.at<uchar>(i+ii,j+jj) == 255)
					dilImgs.at<uchar>(i,j) = 255;
				}
			}
		}
	}
}
#include "stdafx.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>
#include "Functions.h"

using namespace cv;
using namespace std;

void betas(){
	int x = 2, y = 3;
	int result = ((pow(x,3) - 1) / (x - y)) + ((pow(x,2) + 1) / (y - x));
	cout << result;
	system("pause");
}

int main(){
	system("cls");
	Mat img;
	int edge;
	int dilation;
	int plateContours;
	int choice;
	cout<<"1. First Plate"<<endl;
	cout<<"2. Second Plate"<<endl;
	cout<<"3. Third Plate"<<endl;
	cout<<"4. Fourth Plate"<<endl;
	cout<<"5. Fifth Plate"<<endl;
	cout<<"Enter Choice : ";
	cin>>choice;

	switch (choice)
	{
	case 1:
		img = imread("D:\\FYP\\New folder\\WVM757 14.jpg");
		edge = 100;
		dilation = 2;
		plateContours = 75;
		break;
	case 2:
		img = imread("D:\\FYP\\New folder\\BKQ9784 26.jpg");
		edge = 64;
		dilation = 2;
		plateContours = 90;
		break;
	case 3:
		img = imread("D:\\FYP\\New folder\\2015-03-25 Evening 77_Opt 00319.jpg");
		edge = 95;
		dilation = 2;
		plateContours = 80;
		break;
	case 4:
		img = imread("D:\\FYP\\New folder\\2015-03-25 Evening 77_Opt 00446.jpg");
		edge = 120;
		dilation = 3;
		plateContours = 78;
		break;
	case 5:
		img = imread("D:\\FYP\\New folder\\2015-03-25 Evening 77_Opt 00411.jpg");
		edge = 96;
		dilation = 2;
		plateContours = 66;
		break;

	case 6:
		betas();
		break;
	default:
		cout<<"Wrong Input ! "<<endl<<endl;
		system("pause");
		main();
		break;
	}




	Mat greyImg = Mat::zeros(img.size(), CV_8UC1);
	Mat edgeImg = Mat::zeros(img.size(), CV_8UC1);
	Mat EnhGreyImg = Mat::zeros(img.size(), CV_8UC1);
	Mat dilImg = Mat::zeros(img.size(), CV_8UC1);
	Mat sobelImg = Mat::zeros(img.size(), CV_8UC1);
	Mat dilsobelImg = Mat::zeros(img.size(), CV_8UC1);

	greyscaleConversion(img,greyImg);

	imgEnhancing(img, greyImg, EnhGreyImg);

	findEdge(img,greyImg,edgeImg,edge);
	
	findSobel(img,EnhGreyImg,sobelImg,dilsobelImg,400);

	dilateImg(img,edgeImg,dilImg,dilation);

	Mat blob;
	blob = dilImg.clone();
		
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(dilImg,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_NONE,Point(0,0) );

	Mat dst = Mat::zeros(greyImg.size(),CV_8UC3);
	if (!contours.empty() && !hierarchy.empty()){
		int idx = 0 ;
		for (;idx >= 0;idx = hierarchy[idx][0]){
			Scalar color(rand()&255, rand()&255, rand()&255 );
			drawContours(dst,contours,idx,color,CV_FILLED,8,hierarchy);
		}
	}


	Mat plate;
	Rect rect_first;
	Scalar black = CV_RGB(0,0,0);
	unsigned int number_of_blobs = contours.size();
	for (int i = 0;i < number_of_blobs;i++){
		rect_first = boundingRect (contours[i]);

		if (rect_first.width < plateContours){
			drawContours(blob,contours,i,black,CV_FILLED,8,hierarchy);
		}
		else{
			plate = greyImg(rect_first);		
		}
	}

	Mat binPlate = Mat::zeros(plate.size(), CV_8UC1);

	otsu(img,plate,binPlate,greyImg);

	imshow("Plate Number", plate);
	imshow("Plate Number1", sobelImg);
	imshow("Plate Number2", dilImg);


	waitKey();
	return 0;
}
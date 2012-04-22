/*
 * main.cpp
 *
 *  Created on: Aug 28, 2011
 *      Author: Alexander Peller
 *
 *  CNC Convert an Input Image to CNC Data
 *
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.h>

#include <stdio.h>
#include <float.h>
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;


double tick(double t) {
	return (double)cvGetTickCount() - t;
}

int main(int argc, char *argv[])
{

	char c;
	
	namedWindow("Schow",		1);
	namedWindow("Original",		1);

	int			g_zoom				= 21;
	createTrackbar( "Zoom",		"Show",	&g_zoom,			100);

	//Point mean[4];
	//Point2f features[4][5];

	Scalar mulcol[5];
	mulcol[0] = CV_RGB( 255, 0, 0);
	mulcol[1] = CV_RGB( 0, 255, 0);
	mulcol[2] = CV_RGB( 0, 0, 255);
	mulcol[3] = CV_RGB( 255, 255, 0);
	mulcol[4] = CV_RGB( 0, 255, 255);

    vector<vector<Point> > contours;
    vector<Vec4i>  hierarchy;
	
	int an = 20;
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(an*2+1, an*2+1), Point(an, an) );
	
	
	Mat input = imread("kueken.png", 0);
	Mat thresh = Mat::zeros( input.size(), CV_8UC1 );
	Mat morph = Mat::zeros( input.size(), CV_8UC1 );
	
	Mat tmp_contours = Mat::zeros( input.size(), CV_8UC1 );
	
	Mat output = Mat::zeros( input.size(), CV_8UC3 );

	
	
	//cvtColor(input1, filter_image1, CV_RGB2GRAY );
	//GaussianBlur(filter_image1, filter_image1,Size(3,3),9,9);

	threshold(input, thresh, 150, 255, THRESH_BINARY_INV);
	
		thresh.copyTo( tmp_contours);
		findContours( tmp_contours, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		drawContours( output, contours, 0, mulcol[0], 1, 8, hierarchy );
	
	dilate(thresh, morph, element, Point(-1,-1), 1);
	//morphologyEx(thresh, morph , CV_MOP_CLOSE, element, Point(-1,-1), 1);
	
		//threshold(morph, thresh, 150, 255, THRESH_BINARY_INV);
		morph.copyTo(tmp_contours);
		findContours( tmp_contours, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		drawContours( output, contours, 0, mulcol[1], 1, 8, hierarchy );

	for (int i = 0; i < contours[0].size(); i++) {
		circle(output, Point((int)contours[0][i].x,(int)contours[0][i].y), 6, mulcol[2], 3);
	}
			// This is the search for the biggest contours.
			// A contour is detected, if it has a minimum size of 'size':
			/*for(h = 0; h < 2; h++) {
				second = 0;
				size = 200;
				tmpi = 0;

				for(k = 0; k < (int)contours[h].size(); k++ )
				{
					tmpi = contours[h][k].size();
					if (tmpi > size) {

						if (second == 1) {
							hand2_k[h] = hand1_k[h];
						}

						hand1_k[h] = k;
						second = 1;
					}
				}
			}*/

		//vector<Point> hand = contours[(int)floor(h/2)][caphand[h]];

	imshow( "Original", input);
	imshow( "Show", output);
	c = cvWaitKey(3);
	while (c != 27) {
		c = cvWaitKey(3);
	}

		

	return 0;
}

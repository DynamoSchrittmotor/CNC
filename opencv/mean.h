/*
 * mean.h
 *
 *  Created on: Aug 28, 2011
 *      Author: Alexander Peller
 *
 *      The library includes the cropping algorithm of the thesis. First, a
 *      rectangular box is drawn around the contour of the hand. Second, a square
 *      is drawn at the opposite side of the rectangular box that is closest to
 *      the border of the image. Finally, the mean position of the square is
 *      calculated and the radius (the half of the diagonal of the square) around
 *      is calculated. It can be seen in main.cpp that only contour points inside
 *      the circle are calculated.
 *
 */

Point calc_mean(const vector<Point>& hand, int init_k, float& rad, bool print_crop, Mat& out) {

	// A rectangular box is drawn around the contour of the hand.
	RotatedRect rect = minAreaRect(Mat(hand));
	static Point2f center, vtx[4];
	static int i;
	rect.points(vtx);

    static Point big1;
    static Point big2;
    static Point small1;
    static Point small2;

    static int min_length;

    static Point start1;
    static Point start2;

    static int tmp;
    static int start2_ind;

    static Point vector;
    static int vector_length;

    static Point mean;

    // Following conditions evaluate the side of the rectangular box
    // that is closest to the border of the image. Then, the corner positions
    // of the square are calculated.
    if(abs(vtx[0].x - vtx[1].x) + abs(vtx[0].y - vtx[1].y) > abs(vtx[1].x - vtx[2].x) + abs(vtx[1].y - vtx[2].y) ) {
    	big1 = vtx[0];
    	big2 = vtx[1];
    	small1 = vtx[1];
    	small2 = vtx[2];
    } else {
    	big1 = vtx[1];
    	big2 = vtx[2];
    	small1 = vtx[2];
    	small2 = vtx[3];
    }

    min_length = abs(small1.x-small2.x) + abs(small1.y-small2.y);

    if (abs(vtx[0].x - hand[init_k].x) + abs(vtx[0].y - hand[init_k].y) > abs(vtx[2].x - hand[init_k].x) + abs(vtx[2].y - hand[init_k].y) )
    {
    	start1 = vtx[0];
    	big1 = vtx[0];
    	tmp = 0;
    } else {
    	start1 = vtx[2];
    	big1 = vtx[2];
    	tmp = 2;
	}


    if (abs(vtx[1].x - hand[init_k].x) + abs(vtx[1].y - hand[init_k].y) > abs(vtx[3].x - hand[init_k].x) + abs(vtx[3].y - hand[init_k].y) )
    {
    	start2 = vtx[1];
    	start2_ind = 1;
    } else {
    	start2 = vtx[3];
    	start2_ind = 3;
	}

    if (tmp + 1 == start2_ind and tmp + 1 == start2_ind) {
    	if ((tmp - 1) == -1)
    		big2 = vtx[3];
    	else
    		big2 = vtx[1];
    } else
    	big2 = vtx[tmp + 1];

    vector.x = (big2.x - big1.x);
    vector.y = (big2.y - big1.y);

    vector_length = abs(vector.x) + abs(vector.y);

    vector.x = vector.x*min_length/vector_length;
    vector.y = vector.y*min_length/vector_length;

    // The mean position is calculated out of the four
    // corner positions of the square:
    mean.x = (2*start1.x + 2*start2.x + 2*vector.x) / 4;
    mean.y = (2*start1.y + 2*start2.y + 2*vector.y) / 4;

    if (print_crop) {

        for( i = 0; i < 4; i++ ) {
            line(out, vtx[i], vtx[(i+1)%4], Scalar(0, 255, 0), 1, CV_AA);
            circle(out,vtx[i],1,CV_RGB( 255, 0,0 ),i*4);

        }

		line(out, start1, start2, Scalar(255, 255, 255), 1, CV_AA);
		line(out, start2, start2+vector, Scalar(255, 255, 255), 1, CV_AA);
		line(out, start2+vector, start1+vector, Scalar(255, 255, 255), 1, CV_AA);
		line(out, start1+vector, start1, Scalar(255, 255, 255), 1, CV_AA);
		int rad2 = (mean.x - start1.x)*(mean.x - start1.x) + (mean.y - start1.y)*(mean.y - start1.y);

		circle(out,mean,(int)sqrt(rad2),CV_RGB( 255, 255, 255 ),3);
    }

    // The radius is taken to crop useless contour points
    // apart from the 'region of interest' in the main.cpp.
    rad = abs(mean.x - start1.x) + abs(mean.y - start1.y);
    return mean;
}

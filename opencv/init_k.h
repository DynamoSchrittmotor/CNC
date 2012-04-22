/*
 * init_k.h
 *
 *  Created on: Aug 28, 2011
 *      Author: Alexander Peller
 *
 *      A scan is performed over the contour. The start/end point of the contour
 *      is shifted to a point that is close to the boarder of the image. Therefore,
 *      good finger recognition is guaranteed afterwards.
 *
 */

int calc_init_k(const vector<Point>& hand, int WIDTH, int HEIGHT, bool print_initk, Mat& out) {

	static int total;
	static int temp_old;
	static int init_k;
	static int k;

	total = hand.size();
	temp_old = WIDTH/2;

	for (k = 0; k < total; k++) {
		int xtemp;
		int ytemp;

		if (hand[k].x >= WIDTH/2){
			xtemp = hand[k].x - WIDTH/2;
			xtemp = abs(WIDTH/2 - xtemp);}
		else
			xtemp = hand[k].x;

		if (hand[k].y >= HEIGHT/2){
			ytemp = hand[k].y - HEIGHT/2;
			ytemp = abs(HEIGHT/2 - ytemp);}
		else
			ytemp = hand[k].y;

		if (temp_old > xtemp or temp_old > ytemp) {
			//printf("k = %d, x = %d, y = %d\n", k, hand[k].x, hand[k].y );
			init_k = k;
			if (xtemp <= ytemp)
				temp_old = xtemp;
			else
				temp_old = ytemp;
		}

	}

	if (print_initk) {
		circle(out,Point(hand[init_k].x, hand[init_k].y),6,CV_RGB( 255, 255 ,255 ),3);
	}

	return init_k;
}

/*
 * feature.h
 *
 *  Created on: Aug 28, 2011
 *      Author: Alexander Peller
 *
 *      The features are the positions of the fingertips. First, the hull of a finger is
 *      detected. The hull of each finger is separated in a 'left', 'right' and middle
 *      part. Second, the weighted mean algorithm calculates a position that represents
 *      the fingertip position. The weighted mean calculates the shift of the fingertip
 *      in relation to strong shadow, too.
 *
 */

bool paint_feature = false;
bool paint_actual = false;

// This function implements the weighted mean algorithm:
Point2f calc_feature(const Point* boarder_prev,const Point* boarder_l,const Point* boarder_after,int h, int mul, int prev, int l, int after, int max, int& toggle_adjust, Mat& out) {

	static double 	max_avg 		= 0;
	static double 	l_avg 			= 0;
	static double 	register_index 	= 0;

	static int 		min;
	static int 		index;
	static double 	sumx;
	static double 	sumy;
	static double 	sumall;
	static double 	multiply;
	static double 	factor;
	static char		g_text[] 		= "Long long long long long long long long long long string";
	static Point2f 	feature;

	if(toggle_adjust) {
		l_avg = 0;
		max_avg = 0;
		register_index = 0;
		toggle_adjust = 0;
	}

	// The average values for 'l_avg' and 'max_avg' are calculated first.
	// They are used to shift the positions by the detection of strong shadow.
	if (register_index < 100) {
		l_avg += (double)l;
		max_avg += (double)max;
		register_index++;
	} else if (register_index == 100){
		l_avg /=100;
		max_avg /= 100;
		register_index++;
	} else if (register_index < 200) {
		sprintf(g_text, "Finger calibrated! l = %d, max = %f", (int)l_avg, max_avg);
		putText(out, g_text, Point(100,100), FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB( 255, 0,255 ));
		register_index++;
	}

	index = 0;
	sumx = 0;
	sumy = 0;
	sumall = 0;

	if (prev < after)
		min = prev;
	else
		min = after;

	// The shift of the position due to strong shadow is proportional
	// to a factor:
	factor = (max_avg/max)*(l/l_avg);
	factor = pow(factor,5);

	// The weighting in combination with the hull segments is performed here:
	for(int a = prev-min; a < prev; a++) {
		if(paint_feature)
			circle(out,boarder_prev[a],1,CV_RGB( 255, 0, 0 ),2);

		if(h == 0) {
			multiply = factor;
		} else
			multiply = 1;

		sumx += (double)boarder_prev[a].x/((double)min - (double)index)*multiply;
		sumy += (double)boarder_prev[a].y/((double)min - (double)index)*multiply;
		sumall += 1/((double)min - (double)index)*multiply;

		index ++;

	}

	for(int a = 0; a < l; a++) {
		if(paint_feature)
			circle(out,boarder_l[a],1,CV_RGB( 0, 255, 0 ),2);
		sumx += (double)boarder_l[a].x;
		sumy += (double)boarder_l[a].y;
		sumall += 1;
	}

	for(int a = 0; a < min; a++) {
		if(paint_feature)
			circle(out,boarder_after[a],1,CV_RGB( 0, 0, 255 ),2);

		if(h == 0)
			multiply = 1;
		else
			multiply = factor;

		sumx += (double)boarder_after[a].x/((double)a + 1)*multiply;
		sumy += (double)boarder_after[a].y/((double)a + 1)*multiply;
		sumall += 1/((double)a + 1)*multiply;
	}

	sumx = sumx/sumall;
	sumy = sumy/sumall;
	feature.x = sumx;
	feature.y = sumy;

	if(paint_feature)
		circle(out,Point(feature.x,feature.y),6,CV_RGB( 0,0,0 ),3);

	return feature;
}

void zero(int& prev ,int& l, int& max ,int& after, float& integral) {
	prev = 0;
	l = 0;
	max = 0;
	after = 0;
	integral = 0;
}

void detect_features(const vector<Point>& hand, Point2f features[4][5], float angles_raw[4][10000], int g_offs, int h, int mul, int init_k, int* fingercount, int& toggle_adjust, Mat& out){

	static Point boarder_prev[1000];
	static Point boarder_l[1000];
	static Point boarder_after[1000];

	static Point point;

	static int total;
	static int a;
	static int prev;
	static int l,k,n;
	static int max;
	static int after;
	static int finger;
	static float integral;
	static int detect;

	total = hand.size();

	a 		= 1;
	finger 	= 0;

	zero(prev , l, max , after, integral);

	// The state machine starts here to detect the hull of the fingers:
	for(k = 0; k < total; k++) {

		if (k + init_k >= total)
			n = k + init_k - total;
		else
			n = k + init_k;

		point = Point(hand[n].x,hand[n].y);

		if ((int)angles_raw[h][k] <= g_offs and (int)angles_raw[h][k] >= -g_offs and (a == 1 or a == 2)) {

			// Left boarder points are detected here.
			a = 2;

			if (prev < 1000)
				boarder_prev[prev] = point;
			if(paint_actual)
				circle(out,boarder_prev[prev],1,CV_RGB( 255, 0, 0 ),2);
			prev++;
		} else if ((int)angles_raw[h][k] < -g_offs and (a == 2 or a == 3) ) {

			// Middle boarder points are detected here.
			a = 3;
			integral += angles_raw[h][k];

			if ((int)angles_raw[h][k] < max) {
				max = angles_raw[h][k];
			}

			if (l < 1000)
				boarder_l[l] = point;
			if(paint_actual)
				circle(out,boarder_l[l],1,CV_RGB( 0, 255, 0 ),2);
			l++;

		} else if ((int)angles_raw[h][k] <= g_offs and (int)angles_raw[h][k] >= -g_offs and after <= 80 and (a == 3 or a ==4) ) {

			// Right boarder points are detected here.
			a = 4;
			if (after < 1000)
				boarder_after[after] = point;
			if(paint_actual)
				circle(out,boarder_after[after],1,CV_RGB( 0, 0, 255 ),2);
			after++;

		} else if (((int)angles_raw[h][k] > g_offs or (int)angles_raw[h][k] < -g_offs  or after > 80) and a == 4){
			a = 1;

			// The integral sum is the main condition for detecting fingers.
			// Small contours are excluded by setting a threshold to 'prev', 'l' and 'after':
			if (	(int)(integral/(float)mul) < -100 and
					(int)(integral/(float)mul) > -220 and
					prev	> 10	and
					l 		> 10	and
					after 	> 10)
				detect = 1;
			else
				detect = 0;

			if (detect) {

				if (finger < 5) {

					// The positions of the fingertips are calculated next:
					features[h][finger] = calc_feature(
							boarder_prev,
							boarder_l,
							boarder_after,
							h,mul,
							prev,
							l,
							after,
							max,
							toggle_adjust,
							out);

					fingercount[h] += 1;
				}

				finger++;
			} else {
				k = k - after;
			}

			zero(prev , l, max , after, integral);

		} else {
			zero(prev , l, max , after, integral);
		}
	}
}

/*
 * angledif.h
 *
 *  Created on: Aug 28, 2011
 *      Author: Alexander Peller
 *
 *      This library includes the calculation of the look-up table for the
 *      tangential angles difference calculation. Two steps are performed.
 *      Initially (if the first contour is detected), the tangential angles
 *      differences get calculated. After that, the values are just looked up
 *      in the array.
 *
 */

double calc_angledif(int angle_x, int angle_y, int angleold_x, int angleold_y) {

	static int init = 1;

	static double pi = 3.1415;
	static double tmp;
	static double angle;
	static double angleold;
	static double angledif[50][50][50][50];
	static double value = 0;
	static int width = 25;


	if(init == 1) {

		double dyold;
		double dyoldyold;

		double x;
		double y;
		double xold;
		double yold;

		for(int xold_t = -width; xold_t < width; xold_t ++) {
			for(int yold_t = -width; yold_t < width; yold_t ++){
				for(int x_t = -width; x_t < width; x_t ++) {
					for(int y_t = -width; y_t < width; y_t ++){

						x = x_t;
						xold = xold_t;
						y = y_t;
						yold = yold_t;

						if(y == 0)
							y = dyold;
						if(yold == 0)
							yold = dyoldyold;

						angle = atan2(y,x)/pi*180;
						angleold = atan2(yold,xold)/pi*180;

						if (angleold > 0 and angleold < 90){			// 1
							if (angle > -180 and angle < -90) {
								tmp = 180 + angle;
								if (tmp < angleold)
									angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = 360 - angleold + angle;
								else
									angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = angle - angleold;
							} else {
								angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = angle - angleold;
							}

						} else if(angleold > 90 and angleold < 180) {	// 2
							if (angle < 0 and angle > -90) {
								tmp = 180 + angle;
								if (tmp < angleold)
									angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = 360 - angleold + angle;
								else
									angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = angle - angleold;
							} else if (angle < -90 and angle > -180) {
								angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = 360 - angleold + angle;
							} else {
								angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = angle - angleold;
							}

						} else if(angleold < -90 and angleold > -180) { // 3
							if (angle > 0 and angle < 90) {
								tmp = -180 + angle;
								if (tmp > angleold)
									angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = -360 - angleold + angle;
								else
									angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = angle - angleold;
							} else if (angle > 90 and angle < 180) {
								angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = -360 + angle -angleold;
							} else
								angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = angle - angleold;

						} else if(angleold < 0 and angleold > -90) {	// 4
							if (angle > 90 and angle < 180) {
								tmp = -180 + angle;
								if (tmp > angleold)
									angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = -360 - angleold + angle;
								else
									angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = angle - angleold;
							} else {
								angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = angle - angleold;
							}
						} else if (angleold == 0 and angle == 0)
							angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = 0;
						else if (angleold == 180 and angle == 180)
							angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = 0;
						else if (angleold == -180 and angle == -180)
							angledif[(int)(x)+width][(int)(y)+width][(int)(xold)+width][(int)(yold)+width] = 0;

						if(y < 0)
							dyold = -0.01;
						if(y > 0)
							dyold = 0.01;

						if(yold < 0)
							dyoldyold = -0.01;
						if(yold > 0)
							dyoldyold = 0.01;

					}
				}
			}
		}


		init = 0;

	} else {

		if (	angle_x < width and angle_x >= -width and
				angle_y < width and angle_y >= -width and
				angleold_x < width and angleold_x >= -width and
				angleold_y < width and angleold_y >= -width) {

			value = angledif[angle_x+width][angle_y+width][angleold_x+width][angleold_y+width];

		}
	}

	return value;
}

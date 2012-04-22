/*
 * finger.h
 *
 *  Created on: Aug 28, 2011
 *      Author: Alexander Peller
 *
 *      The finger positions of left and right capture device are matched here and
 *      the virtual mouse interface detects motions to control the desktop of an
 *      operating system.
 *
 */

bool paint_finger = true;
bool paint_motion = false;
bool paint_mouse = true;
bool paint_mean = false;

Scalar* init_col(){
	static Scalar mulcol[5];

	mulcol[1] = CV_RGB( 255, 	0, 		0);
	mulcol[2] = CV_RGB( 0, 		255, 	0);
	mulcol[3] = CV_RGB( 0, 		0, 		255);
	mulcol[4] = CV_RGB( 255, 	255, 	0);
	mulcol[5] = CV_RGB( 0, 		255, 	255);

	return mulcol;
}


void mouse(int finger, bool init, int H1, int H2, int screenx, int screeny, Point2f features[4][5], Display* dpy, XEvent& event, Mat& out1, Mat& out2) {
	
	static Scalar* 	mulcol = init_col();
	static char		g_text[] 				= "Long long long long long long long long long long string";
	
	screenx = 1280;
	screeny = 800;
	
	static int mx = screenx/2;
	static int my = screeny/2;
	
	static int paintx;
	static int painty;
	static int painti;

	float lx = features[H1][1].x;
	float ly = features[H1][1].y;

	static float lx_old = features[H1][1].x;
	static float ly_old = features[H1][1].y;

	float lx2 = features[H2][1].x;
	float ly2 = features[H2][1].y;

	static float lx2_old = features[H2][1].x;
	static float ly2_old = features[H2][1].y;

	float disxl = features[H1][1].x - features[H2][1].x;
	float disxr = features[H1][0].x - features[H2][0].x;

	static float disxl_back = features[H1][1].x - features[H2][1].x;
	static float disxr_back = features[H1][0].x - features[H2][0].x;

	static float disxl_old = features[H1][1].x - features[H2][1].x;
	static float disxr_old = features[H1][0].x - features[H2][0].x;

	disxl = (disxl + disxl_old)/2;
	disxr = (disxr + disxr_old)/2;

	float rx = features[H1][0].x;
	float ry = features[H1][0].y;

	static float rx_old = features[H1][0].x;
	static float ry_old = features[H1][0].y;

	float rx2 = features[H2][0].x;
	float ry2 = features[H2][0].y;

	static float rx2_old = features[H2][0].x;
	static float ry2_old = features[H2][0].y;

	static bool left 	= false;
	static bool right 	= false;
	static bool jumpl, jumpr;
	
	static bool enter = false;
	static bool enter2 = false;

	if (init == false) {
		if(finger == 1) {

			float thres = 2.4;
			float thres2 = 1.5;
			float hist = 0.7;

			if (	(abs((lx - lx_old)*(lx - lx_old)+(ly - ly_old)*(ly - ly_old)) > thres or
					abs((lx2 - lx2_old)*(lx2 - lx2_old)+(ly2 - ly2_old)*(ly2 - ly2_old)) > thres) and
					left == false) {
					left = true;
					if(paint_motion)
					circle(out1,Point(lx,ly),10,mulcol[0],3);
			}

			if (	(abs((rx - rx_old)*(rx - rx_old)+(ry - ry_old)*(ry -ry_old)) > thres or
					abs((rx2 - rx2_old)*(rx2 - rx2_old)+(ry2 - ry2_old)*(ry2 -ry2_old)) > thres ) and
					right == false) {
					right = true;
					if(paint_motion)
					circle(out1,Point(rx,ry),10,mulcol[0],3);
			}

			if ((left == true and right == false) or jumpl == true) {

				if (disxl > disxr + thres2 + hist and jumpl == false) {

					jumpl = true;
					enter = true;
					
					sprintf(g_text,"./click -x %d -y %d -f 1", (int)(mx),(int)(my));
					system(g_text);
					
					//sprintf(g_text,"./click -x %d -y %d -p 0 -f 0", (int)(mx),(int)(my));
					//system(g_text);
					
					//sprintf(g_text,"./click -x %d -y %d -p 1 -f 0", (int)(mx),(int)(my));
					//system(g_text);
					
					//XTestFakeButtonEvent(dpy, 1, True,  0);
					//XSync(dpy, 0);

					paintx = lx;
					painty = ly;
					painti = 200;

				}

				if (disxl < disxr + thres2 - hist and jumpl == true) {

					sprintf(g_text,"./click -x %d -y %d -f 3", (int)(mx),(int)(my));
					system(g_text);
					
					//sprintf(g_text,"./click -x %d -y %d -f 1", (int)(mx),(int)(my));
					//system(g_text);
					
					//sprintf(g_text,"./click -x %d -y %d -f 3", (int)(mx),(int)(my));
					//system(g_text);
					
					//XTestFakeButtonEvent(dpy, 1, False,  0);
					//XSync(dpy, 0);

					jumpl = false;
					
					painti = 200;
				}


			} /*else if ((left == false and right == true) or jumpr == true) {

				if (disxr > disxl + thres2 + hist and jumpr == false) {

					jumpr = true;

					sprintf(g_text,"./click -x %d -y %d -p 1 -f 1", (int)(mx),(int)(my));
					system(g_text);
					
					//XTestFakeButtonEvent(dpy, 3, True,  0);
					//XSync(dpy, 0);

				}


				if (disxr < disxl + thres2 - hist and jumpr == true) {

					sprintf(g_text,"./click -x %d -y %d -p 0 -f 1", (int)(mx),(int)(my));
					system(g_text);
					
					//XTestFakeButtonEvent(dpy, 3, False,  0);
					//XSync(dpy, 0);

					jumpr = false;

					right = false;
				}


			}
*/
			if (left == true and right == true){

					if ((lx - lx_old)*(lx - lx_old)+(ly - ly_old)*(ly - ly_old) < 1) {
						mx += ((lx - lx_old)+(rx - rx_old))*2;
						my += ((ly - ly_old)+(ry - ry_old))*2;
					} else {
						mx += ((lx - lx_old)+(rx - rx_old))*4;
						my += ((ly - ly_old)+(ry - ry_old))*4;
					}

					if (mx < 0)
						mx = 0;
					if (mx > screenx)
						mx = screenx;

					if (my < 0)
						my = 0;
					if (my > screeny)
						my = screeny;

				if (disxl < disxr + thres2 - hist and jumpl == true) {
					jumpl == false;
					sprintf(g_text,"./click -x %d -y %d -f 3", (int)(mx),(int)(my));
					system(g_text);
				}
				
				//XTestFakeMotionEvent(dpy, 0, (int)(mx),(int)(my), 0);
					//XSync(dpy, 0);
				
				if (jumpl == true) {
					sprintf(g_text,"./click -x %d -y %d -f 2", (int)(mx),(int)(my));
					system(g_text);
				} else {
					sprintf(g_text,"./click -x %d -y %d -f 0", (int)(mx),(int)(my));
					system(g_text);
				}

					
				/*if (jumpl == true) {
					
					if(enter == true) {
						// down
						sprintf(g_text,"./click -x %d -y %d -f 1", (int)(mx),(int)(my));
						enter == false;
					} else {
						// drag
						sprintf(g_text,"./click -x %d -y %d -f 2", (int)(mx),(int)(my));
						enter2 == true;
					}
				} else {
					
					if(enter2 == true) {
						//sprintf(g_text,"./click -x %d -y %d -f 3", (int)(mx),(int)(my));
						enter2 == false;
					} else {
						sprintf(g_text,"./click -x %d -y %d -f 0", (int)(mx),(int)(my));
					}
				}*/

				//sprintf(g_text,"./click -x %d -y %d -f 0", (int)(mx),(int)(my));
				//system(g_text);
			
				left = false;
				right = false;

			}

			if (painti <= 200 and painti > 0) {
				if(paint_mouse)
					circle(out1,Point(paintx,painty),(210-painti)*(210-painti)/1000,Scalar(painti,painti,painti),1);
				painti -= 10;
			}


			double dif = features[H1][1].x - features[H2][1].x;
			if(paint_mouse) {
				sprintf(g_text, "%2.2f", disxl );
				putText(out1, g_text, Point((int)features[H1][1].x,(int)features[H1][1].y), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0),2);
			}

			dif = features[H1][0].x - features[H2][0].x;
			if(paint_mouse) {
				sprintf(g_text, "%2.2f", disxr );
				putText(out1, g_text, Point((int)features[H1][0].x,(int)features[H1][0].y), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0),2);
			}

		} else {
			if (ry - ry_old > 1) {
				sprintf(g_text,"./click -f 6");
				system(g_text);
				
				//XTestFakeButtonEvent(dpy, 5, True,  0);
				//XTestFakeButtonEvent(dpy, 5, False,  0);
				//XSync(dpy, 0);
			} else if (ry - ry_old < -1) {
				sprintf(g_text,"./click -f 7");
				system(g_text);
				
				//XTestFakeButtonEvent(dpy, 4, True,  0);
				//XTestFakeButtonEvent(dpy, 4, False,  0);
				//XSync(dpy, 0);
			}
		}

	} else {
		jumpl = false;
		jumpr = false;
		left = false;
		right = false;

		sprintf(g_text,"./click -x %d -y %d -f 3", (int)(mx),(int)(my));
		system(g_text);
		//sprintf(g_text,"./click -x %d -y %d -f 3", (int)(mx),(int)(my));
		//system(g_text);
		
		//XTestFakeButtonEvent(dpy, 1, False,  0);
		//XTestFakeButtonEvent(dpy, 3, False,  0);
		//XSync(dpy, 0);
	}

	if (jumpl == false or jumpr == false) {
		disxl_back = (disxl_back + disxl)/2;
		disxr_back = (disxr_back + disxr)/2;
	}

	lx_old = lx;
	ly_old = ly;
	lx2_old = lx2;
	ly2_old = ly2;

	disxl_old = disxl;
	disxr_old = disxr;

	rx_old = rx;
	ry_old = ry;
	rx2_old = rx2;
	ry2_old = ry2;

}

void finger_sync(int caphand[4], int fingercount[4], int screenx, int screeny, Point2f features[4][5], Point mean[4], Display* dpy, XEvent& event, Mat& out1, Mat& out2) {

	static Scalar* mulcol = init_col();

	Scalar red 	= CV_RGB( 255, 0, 0);
	Scalar blue = CV_RGB( 0, 0, 255);
	Scalar col;

	int H1;
	int H2;
	int h = 0;
	int h1 = 0;
	int h2 = 2;
	int iterate = 1;
	bool four = false;
	bool zero = false;
	static bool init;

	if (caphand[0] != -1 and caphand[1] != -1 and caphand[2] != -1 and caphand[3] != -1 ) {
		four = true;
		iterate = 2;
	} else if (caphand[0] == -1 and caphand[1] == -1 and caphand[2] == -1 and caphand[3] == -1 ) {
		zero = true;
	}

	if(!zero)
		for (h = 0; h < iterate; h++) {

			if (h == 0)
				col = red;
			else
				col = blue;

			if (four) {
				int h1min;
				int h2min;
				int disx1, disx2;
				int disy1, disy2;

				h1min = h;

				if (h1min == 0) {

					disx1 = mean[0].x - mean[2].x;
					disy1 = abs(mean[0].y - mean[2].y);

					disx2 = mean[0].x - mean[3].x;
					disy2 = abs(mean[0].y - mean[3].y);

					if ((disy1 < 40 and disy2 < 40) or disy2 == disy1) {
						if (disx1 > 0 and disx2 > 0) {
							if(disx2 > disx1)
								h2min = 2;
							else
								h2min = 3;
						} else {
							if (disx1 > 0)
								h2min = 2;
							else
								h2min = 3;
						}
					} else if (disy1 < disy2) {
						h2min = 2;
					} else if (disy2 < disy1) {
						h2min = 3;
					}

				} else {
					if (h2min == 2)
						h2min = 3;
					else if (h2min == 3)
						h2min = 2;
				}

				if(paint_mean) {
					circle(out1,mean[h1min],6,mulcol[h1min],3);
					circle(out2,mean[h2min],6,mulcol[h1min],3);
				}

				H1 = h1min;
				H2 = h2min;
			} else {
				H1 = h;
				H2 = h + 2;
			}


		if (fingercount[H1] != fingercount[H2] and fingercount[H1] != 0 and fingercount[H2] != 0) {

			if (fingercount[H1] > fingercount[H2]) {
				h1 = H1;
				h2 = H2;
			} else {
				h1 = H2;
				h2 = H1;
			}

			// h1 has always a bigger contour than h2
			// h1 and h2 belong to the same hand/contour

			// get the smaller number of features -> h2

			// calculate the difference:
			int min_dif;
			int dx;
			int dy;

			int the_offs = 0;
			for (int offs = 0; offs <= fingercount[h1]-fingercount[h2]; offs++){

				int dis = 0;
				int dis_dif = 0;

				for (int finger = 0; finger < fingercount[h2]; finger++){
					dx = (int)features[h2][finger].x-(int)features[h1][finger+offs].x;
					dy = (int)features[h2][finger].y-(int)features[h1][finger+offs].y;

					if (finger == 0)
						dis_dif = 0;
					else
						dis_dif += abs(dis - abs(dx) - abs(dy));

					dis = abs(dx) + abs(dy);
				}

				if( offs == 0 ) {
					min_dif = dis_dif;
					the_offs = offs;
				} else {
					if (dis_dif < min_dif){
						min_dif = dis_dif;
						the_offs = offs;
					}
				}
				//printf("dis_dif = %d, min_dif = %d, the_offs = %d\n",dis_dif , min_dif, the_offs);
			}
			//printf("\n");
			if (fingercount[H1] > fingercount[H2]) {
				for (int finger = 0; finger < fingercount[h2]; finger++){
					if(paint_finger) {
						circle(out2,cvPointFrom32f(features[H2][finger]),6,mulcol[finger],3);
						circle(out1,cvPointFrom32f(features[H1][finger+the_offs]),6,mulcol[finger],3);
					}

				}
			} else {
				for (int finger = 0; finger < fingercount[h2]; finger++) {
					if(paint_finger) {
						circle(out2,cvPointFrom32f(features[H2][finger+the_offs]),6,mulcol[finger],3);
						circle(out1,cvPointFrom32f(features[H1][finger]),6,mulcol[finger],3);
					}
				}
			}
		} else if (fingercount[H1] == fingercount[H2] and fingercount[H1] != 0){

			for (int finger = 0; finger < fingercount[H1]; finger++){
				if(paint_finger){
					circle(out2,Point((int)features[H2][finger].x, (int)features[H2][finger].y),6,mulcol[finger],3);
					circle(out1,Point((int)features[H1][finger].x, (int)features[H1][finger].y),6,mulcol[finger],3);
				}

				if (fingercount[H1] == 2 and finger == 1) {

					mouse(finger, init, H1, H2,screenx, screeny, features, dpy, event, out1, out2);
					init = false;

				} else if (fingercount[H1] == 1 and finger == 0) {

					mouse(finger, init, H1, H2, screenx, screeny, features, dpy, event, out1, out2);
					init = false;

				}
			}
		}

		if (fingercount[H1] < 1 or fingercount[H1] >2) {
			init = true;
		}
	}

	//printf("isleftright:\n");
	//printf("1: %d, 2: %d, 3: %d, 4: %d\n\n", isleftright[0],isleftright[1],isleftright[2],isleftright[3]);
}

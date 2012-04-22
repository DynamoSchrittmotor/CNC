/*
 * control.h
 *
 *  Created on: Aug 28, 2011
 *      Author: Alexander Peller
 *
 *      This library includes the interface for controlling the output
 *      while operating.
 *
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.h>

#ifndef CONTROL_H_
#define CONTROL_H_

void draw_backspace(int ox, int oy, Mat& out) {
	Scalar black = Scalar(0,0,0);

	line(out,Point(ox,oy),		Point(ox+80,oy),	black,2);
	line(out,Point(ox+80,oy),	Point(ox+80,oy+40),	black,2);
	line(out,Point(ox+80,oy+40),Point(ox,oy+40),	black,2);
	line(out,Point(ox,oy+40),	Point(ox,oy),		black,2);

	line(out,Point(ox+10,oy+20),Point(ox+20,oy+10), black,2);
	line(out,Point(ox+10,oy+20),Point(ox+70,oy+20), black,2);
	line(out,Point(ox+10,oy+20),Point(ox+20,oy+30), black,2);
}

void draw_return(int ox, int oy, Mat& out) {
	Scalar black = Scalar(0,0,0);

	line(out,Point(ox,oy),		Point(ox+80,oy),	black,2);
	line(out,Point(ox+80,oy),	Point(ox+80,oy+40),	black,2);
	line(out,Point(ox+80,oy+40),Point(ox,oy+40),	black,2);
	line(out,Point(ox,oy+40),	Point(ox,oy),		black,2);

	line(out,Point(ox+10,oy+20),Point(ox+20,oy+10), black,2);
	line(out,Point(ox+10,oy+20),Point(ox+70,oy+20), black,2);
	line(out,Point(ox+10,oy+20),Point(ox+20,oy+30), black,2);
	line(out,Point(ox+70,oy+20),Point(ox+70,oy+10), black,2);
}

void draw_arrow(int ox, int oy, int dir, Mat& out) {
	Scalar black = Scalar(0,0,0);

	line(out,Point(ox,oy),		Point(ox+40,oy),	black,2);
	line(out,Point(ox+40,oy),	Point(ox+40,oy+40),	black,2);
	line(out,Point(ox+40,oy+40),Point(ox,oy+40),	black,2);
	line(out,Point(ox,oy+40),	Point(ox,oy),		black,2);

	switch(dir){
	case 0:
		line(out,Point(ox+20,oy+10),Point(ox+30,oy+20), black,2);
		line(out,Point(ox+20,oy+10),Point(ox+10,oy+20), black,2);
		line(out,Point(ox+20,oy+10),Point(ox+20,oy+30), black,2);
		break;
	case 1:
		line(out,Point(ox+20,oy+30),Point(ox+30,oy+20), black,2);
		line(out,Point(ox+20,oy+30),Point(ox+10,oy+20), black,2);
		line(out,Point(ox+20,oy+30),Point(ox+20,oy+10), black,2);
		break;
	case 2:
		line(out,Point(ox+10,oy+20),Point(ox+20,oy+10), black,2);
		line(out,Point(ox+10,oy+20),Point(ox+30,oy+20), black,2);
		line(out,Point(ox+10,oy+20),Point(ox+20,oy+30), black,2);
		break;
	case 3:
		line(out,Point(ox+30,oy+20),Point(ox+20,oy+10), black,2);
		line(out,Point(ox+30,oy+20),Point(ox+10,oy+20), black,2);
		line(out,Point(ox+30,oy+20),Point(ox+20,oy+30), black,2);
		break;
	}
}

void draw_control(Mat& out) {

	Scalar black = Scalar(0,0,0);
	int ox = 400;
	int oy = 20;

	line(out,Point(380,20),	Point(380,20+440),		black,2);
	ox += 0;

	putText(out, "Usage", Point(ox+80,oy+20), FONT_HERSHEY_SIMPLEX, 0.7, black,2);

	oy += 30;
	draw_return(ox, oy, out);
	putText(out, "Return", Point(ox+90,oy+27), FONT_HERSHEY_SIMPLEX, 0.7, black);

	oy += 50;
	draw_arrow(ox, oy, 0, out);
	putText(out, "Up", Point(ox+50,oy+27), FONT_HERSHEY_SIMPLEX, 0.7, black);

	oy += 50;
	draw_arrow(ox, oy, 1, out);
	putText(out, "Down", Point(ox+50,oy+27), FONT_HERSHEY_SIMPLEX, 0.7, black);

	ox += 120;
	oy -= 50;
	draw_arrow(ox, oy, 2, out);
	putText(out, "Left", Point(ox+50,oy+27), FONT_HERSHEY_SIMPLEX, 0.7, black);

	oy += 50;
	draw_arrow(ox, oy, 3, out);
	putText(out, "Right", Point(ox+50,oy+27), FONT_HERSHEY_SIMPLEX, 0.7, black);

	ox -= 120;
	oy += 70;
	putText(out, "I) Remove all objects.", Point(ox,oy), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
	oy += 30;
	putText(out, "II) Press 'b' to stop/start", Point(ox,oy), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
	oy += 20;
	putText(out, "   background updating.", Point(ox,oy), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
	oy += 30;
	putText(out, "III) Show fingers to adjust.", Point(ox,oy), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
	oy += 30;
	putText(out, "IV) Use the shown keys to", Point(ox,oy), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
	oy += 20;
	putText(out, "   control the menue.", Point(ox,oy), FONT_HERSHEY_SIMPLEX, 0.5, black,1);

	oy += 30;
	putText(out, "Press 'r' to adjust fingers.", Point(ox,oy), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
	oy += 20;
	putText(out, "Press 'p' to pause/resume.", Point(ox,oy), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
	oy += 20;
	putText(out, "Press 's' for snapshot.", Point(ox,oy), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
	oy += 20;
	putText(out, "Press 'esc' for exit.", Point(ox,oy), FONT_HERSHEY_SIMPLEX, 0.5, black,1);

}

void draw_menu(int m_t, int m, int item, int press, int& show_out, bool& print_contour, bool& print_initk, bool& print_crop, bool& print_tang, bool& print_quad, bool& print_update, double t_readin, double t_filter, double t_bs, double t_cd, double t_fd, Mat& out) {

	Scalar black = Scalar(0,0,0);
	int ox = 20;
	int oy = 20;

	char		g_text[] 				= "Long long long long long long long long long long string";

	int ox2 = 40;
	int oy2 = 90;

	putText(out, "Info", Point(ox+20,oy+20), FONT_HERSHEY_SIMPLEX, 0.7, black,2);
	putText(out, "Output", Point(ox+90,oy+20), FONT_HERSHEY_SIMPLEX, 0.7, black,2);
	putText(out, "Print", Point(ox+190,oy+20), FONT_HERSHEY_SIMPLEX, 0.7, black,2);
	putText(out, "Time", Point(ox+270,oy+20), FONT_HERSHEY_SIMPLEX, 0.7, black,2);

	switch(m) {
	case 0:
		putText(out, "                 TUM", Point(ox2,oy2), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
		oy2 += 30;
		putText(out, "  Distributed Multimodal Information", Point(ox2,oy2), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
		oy2 += 20;
		putText(out, "          Processing Group", Point(ox2,oy2), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
		oy2 += 40;
		putText(out, "      Diploma Thesis", Point(ox2,oy2), FONT_HERSHEY_SIMPLEX, 0.7, black,2);
		oy2 += 20;
		putText(out, "        Hand Recognition and", Point(ox2,oy2), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
		oy2 += 20;
		putText(out, "        Tracking in 3D Space", Point(ox2,oy2), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
		oy2 += 30;
		putText(out, "           Alexander Peller", Point(ox2,oy2), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
		oy2 += 40;
		putText(out, "Advisors: Anup Chathoth, Chao Zhang", Point(ox2,oy2), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
		oy2 += 20;
		putText(out, "         Prof. Dr. Matthias Kranz", Point(ox2,oy2), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
		oy2 += 20;
		putText(out, "End of Development: 11/10/2011", Point(ox2,oy2), FONT_HERSHEY_SIMPLEX, 0.5, black,1);
		if (m_t == 0)
			rectangle(out,Point(ox+10,oy-5),Point(ox+70,oy+30),black,2);
		break;
	case 1:
		putText(out, "Input", 			Point(ox2,oy2), FONT_HERSHEY_SIMPLEX, 0.7, black, show_out==0 ? 2 : 1);
		putText(out, "Filtered Input", 	Point(ox2,oy2+40), FONT_HERSHEY_SIMPLEX, 0.7, black, show_out==1 ? 2 : 1);
		putText(out, "Segmented Input", Point(ox2,oy2+80), FONT_HERSHEY_SIMPLEX, 0.7, black, show_out==2 ? 2 : 1);
		putText(out, "Background", 		Point(ox2,oy2+120), FONT_HERSHEY_SIMPLEX, 0.7, black, show_out==3 ? 2 : 1);
		if (m_t == 0)
			rectangle(out,Point(ox+80,oy-5),Point(ox+170,oy+30),black,2);
		else {
			switch (item) {
			case 0:
				rectangle(out,Point(ox2-5,oy2-25),Point(ox2+310,oy2+10),black,2);
				break;
			case 1:
				rectangle(out,Point(ox2-5,oy2-25+40),Point(ox2+310,oy2+10+40),black,2);
				break;
			case 2:
				rectangle(out,Point(ox2-5,oy2-25+80),Point(ox2+310,oy2+10+80),black,2);
				break;
			case 3:
				rectangle(out,Point(ox2-5,oy2-25+120),Point(ox2+310,oy2+10+120),black,2);
				break;
			}
		}
		if (press == 1)
			show_out = item;

		break;
	case 2:
		putText(out, "Contour", 		Point(ox2,oy2), 	FONT_HERSHEY_SIMPLEX, 0.7, black, print_contour==true ? 2 : 1);
		putText(out, "Init Positions", 	Point(ox2,oy2+40), 	FONT_HERSHEY_SIMPLEX, 0.7, black, print_initk==true ? 2 : 1);
		putText(out, "Cropping", 		Point(ox2,oy2+80), 	FONT_HERSHEY_SIMPLEX, 0.7, black, print_crop==true ? 2 : 1);
		putText(out, "Tangentials", 	Point(ox2,oy2+120), FONT_HERSHEY_SIMPLEX, 0.7, black, print_tang==true ? 2 : 1);
		putText(out, "Quad-Tree", 		Point(ox2,oy2+160), FONT_HERSHEY_SIMPLEX, 0.7, black, print_quad==true ? 2 : 1);
		putText(out, "Running Update", 	Point(ox2,oy2+200), FONT_HERSHEY_SIMPLEX, 0.7, black, print_update==true ? 2 : 1);
		if (m_t == 0)
			rectangle(out,Point(ox+180,oy-5),Point(ox+250,oy+30),black,2);
		else {
			switch (item) {
			case 0:
				if (press == 1) {
					print_contour==true ? print_contour=false : print_contour=true;
				}
				rectangle(out,Point(ox2-5,oy2-25),Point(ox2+310,oy2+10),black, 2);
				break;
			case 1:
				if (press == 1) {
					print_initk==true ? print_initk=false : print_initk=true;
				}
				rectangle(out,Point(ox2-5,oy2-25+40),Point(ox2+310,oy2+10+40),black, 2);
				break;
			case 2:
				if (press == 1) {
					print_crop==true ? print_crop=false : print_crop=true;
				}
				rectangle(out,Point(ox2-5,oy2-25+80),Point(ox2+310,oy2+10+80),black,2);
				break;
			case 3:
				if (press == 1) {
					print_tang==true ? print_tang=false : print_tang=true;
				}
				rectangle(out,Point(ox2-5,oy2-25+120),Point(ox2+310,oy2+10+120),black,2);
				break;
			case 4:
				if (press == 1) {
					print_quad==true ? print_quad=false : print_quad=true;
				}
				rectangle(out,Point(ox2-5,oy2-25+160),Point(ox2+310,oy2+10+160),black,2);
				break;
			case 5:
				if (press == 1) {
					print_update==true ? print_update=false : print_update=true;
				}
				rectangle(out,Point(ox2-5,oy2-25+200),Point(ox2+310,oy2+10+200),black,2);
				break;
			}
		}
		break;
	case 3:
		rectangle(out,Point(ox+260,oy-5),Point(ox+330,oy+30),black,2);
		sprintf(g_text, "Capture = %2.1f ms", t_readin/((double)cvGetTickFrequency()*1000) );
		putText(out, g_text, 	Point(ox2,oy2), 	FONT_HERSHEY_SIMPLEX, 0.5, black, 1);
		oy2 += 20;
		sprintf(g_text, "Filter = %2.1f ms", t_filter/((double)cvGetTickFrequency()*1000) );
		putText(out, g_text, 	Point(ox2,oy2), 	FONT_HERSHEY_SIMPLEX, 0.5, black, 1);
		oy2 += 20;
		sprintf(g_text, "Background Sububtraction = %2.1f ms", t_bs/((double)cvGetTickFrequency()*1000) );
		putText(out, g_text, 	Point(ox2,oy2), 	FONT_HERSHEY_SIMPLEX, 0.5, black, 1);
		oy2 += 20;
		sprintf(g_text, "Contour Extraction = %2.1f ms", t_cd/((double)cvGetTickFrequency()*1000) );
		putText(out, g_text, 	Point(ox2,oy2), 	FONT_HERSHEY_SIMPLEX, 0.5, black, 1);
		oy2 += 20;
		sprintf(g_text, "Finger Extraction = %2.1f ms", t_fd/((double)cvGetTickFrequency()*1000) );
		putText(out, g_text, 	Point(ox2,oy2), 	FONT_HERSHEY_SIMPLEX, 0.5, black, 1);
		oy2 += 20;
	}

}

void key_control(char key, int& show_out, bool& print_contour, bool& print_initk, bool& print_crop, bool& print_tang, bool& print_quad, bool& print_update, double t_readin, double t_filter, double t_bs, double t_cd, double t_fd, Mat& out) {

	static int m_t = 0;
	static int m = 0;
	static int item = 0;
	int press = 0;

	static int menu[4];

	//info
	menu[0] = 0;
	//output
	menu[1] = 3;
	//print
	menu[2] = 5;
	//keys
	menu[3] = 0;

	char up = 82;
	char down = 84;
	char left = 81;
	char right = 83;
	char ret = 10;
	char up2 = 0;
	char down2 = 1;
	char left2 = 2;
	char right2 = 3;	
	char ret2 = 13;

	if (m_t == 0) {
		if (m > 0 and (key == left or key == left2)) {
			m--;
		} else if (m < 3 and (key == right or key == right2)) {
			m++;
		}
		if (key == down or key == down2) {
			m_t = 1;
			item = 0;
		}
	} else {
		if (item > 0 and (key == up or key == up2)) {
			item--;
		} else if (item < menu[m] and (key == down or key == down2)) {
			item++;
		} else if (key == up or key == up2) {
			m_t = 0;
		}
	}

	if ((key == ret or key == ret2) and m_t == 1) {
		press = 1;
	}

	draw_menu(	m_t,
				m,
				item,
				press,
				show_out,
				print_contour,
				print_initk,
				print_crop,
				print_tang,
				print_quad,
				print_update,
				t_readin,
				t_filter,
				t_bs,
				t_cd,
				t_fd,
				out);

}
#endif /* CONTROL_H_ */

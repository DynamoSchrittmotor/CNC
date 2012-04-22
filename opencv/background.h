/*
 * background.cpp
 *
 *  Created on: Aug 28, 2011
 *      Author: Alexander Peller
 *
 *      The background subtraction is performed here. It includes the algorithm
 *      adaptive mixed region background subtraction presented in the thesis. It
 *      concludes background subtraction on the one hand and edge detection on
 *      the other hand. See the thesis for details.
 *
 */


class BackgroundSubtractorA
{
public:
    //! the virtual destructor
    virtual ~BackgroundSubtractorA();
    //! the update operator that takes the next video frame and returns the current foreground mask as 8-bit binary image.
    CV_WRAP_AS(apply) virtual void operator()(const Mat& image, CV_OUT Mat& fgmask, CV_OUT Mat& out, bool print_quad = false, bool print_update = false,double learningRate=0, int num=0);
};


class BackgroundSubtractorMOGA : public BackgroundSubtractorA
{
public:
    //! the default constructor
    BackgroundSubtractorMOGA();
    //! the full constructor that takes the length of the history, the number of gaussian mixtures, the background ratio parameter and the noise strength
    BackgroundSubtractorMOGA(Size _framesize);
    //! the destructor
    virtual ~BackgroundSubtractorMOGA();
    //! the update operator
    virtual void operator()(const Mat& image, Mat& fgmask, Mat& out, bool print_quad = false, bool print_update = false, double learningRate=0, int num=0);

    Size frameSize;
    int frameType;
    Mat sigmas;
    Mat means;
    double noiseSigma;

    int upx;
    int upy;
};

BackgroundSubtractorA::~BackgroundSubtractorA() {}
void BackgroundSubtractorA::operator()(const Mat&, Mat&, Mat&, bool, bool, double, int)
{
}

BackgroundSubtractorMOGA::BackgroundSubtractorMOGA(Size _frameSize)
{
    frameSize = _frameSize;
    frameType = 0;

    noiseSigma = CV_BGFG_MOG_SIGMA_INIT*0.5;
    upx = 0;
    upy = 0;

    means.create(_frameSize,CV_32F);
    sigmas.create(_frameSize,CV_32F);
}

BackgroundSubtractorMOGA::~BackgroundSubtractorMOGA()
{
}

void draw_rect(Node* node, int thickness, Mat& out) {

	line(out,node->start,Point(node->start.x,node->end.y),CV_RGB( 255, 255, 255 ), thickness);
	line(out,Point(node->start.x,node->end.y),node->end,CV_RGB( 255, 255, 255 ), thickness);
	line(out,node->end,Point(node->end.x,node->start.y),CV_RGB( 255, 255, 255 ), thickness);
	line(out,Point(node->end.x,node->start.y),node->start,CV_RGB( 255, 255, 255 ), thickness);

}

void draw_rect(int x, int y, int thickness, int xjump, int yjump, Mat& out) {

	line(out,Point(x,y),Point(x,y+yjump),CV_RGB( 255, 255, 255 ), thickness);
	line(out,Point(x,y+yjump),Point(x+xjump,y+yjump),CV_RGB( 255, 255, 255 ), thickness);
	line(out,Point(x+xjump,y+yjump),Point(x+xjump,y),CV_RGB( 255, 255, 255 ), thickness);
	line(out,Point(x+xjump,y),Point(x,y),CV_RGB( 255, 255, 255 ), thickness);

}

void calc_morph_rect(bool again, Node* node, Mat image, Mat first, Mat& image_tree) {

	int an 			= 1;
	int iteration 	= 1;
	int thres 		= 10;

	Mat element = getStructuringElement(MORPH_RECT, Size(an*2+1, an*2+1), Point(an, an) );

	static Mat temp(Size(20+2,15+2),1);

	int x_start = node->start.x;
	int y_start = node->start.y;
	int x_end	= node->end.x;
	int y_end	= node->end.y;

	Mat roi_first = first(Rect(x_start,y_start, x_end - x_start, y_end - y_start));

	if (x_start > 2)
		x_start -= 2;
	if (y_start > 2)
		y_start -= 2;

	if (x_end < 640 - 1)
		x_end += 2;
	if (y_end < 480 - 1)
		y_end += 2;

	Mat roi_image = image(Rect(x_start,y_start, x_end - x_start, y_end - y_start));
	morphologyEx(roi_image, temp, CV_MOP_GRADIENT, element, Point(-1,-1), iteration);

	Mat roi_temp = temp(Rect(1,1,20,15));
	threshold(roi_temp, roi_first,thres,256,CV_THRESH_BINARY);

}

bool general_condition(int src, float mean, float sigma) {

	if (src <= (int) (mean - 4*sigma)) {
		return true;
	} else {
		return false;
	}

}

bool foreground_condition(int src, float mean, float sigma) {

	if ( src <= (int) (mean - 5*sigma) ) {
		return true;
	} else {
		return false;
	}

}

bool mixed_condition(int src, float mean, float sigma) {

	if ( src >  (int) (mean - 5*sigma) and src < (int) (mean - 3*sigma) ) {
		return true;
	} else {
		return false;
	}

}

bool mixed_condition2(int src, float mean, float sigma) {

	if ( src < (int) (mean - 3*sigma) ) {
		return true;
	} else {
		return false;
	}

}

void calc_sub_nodes(Mat image, Mat means, Mat sigmas, Node* root, Mat& out, int iterate, int x_init, int y_init, int rows, int cols, bool print_quad) {

	int x;
	int y;

	int divide;
	int xjump;
	int yjump;
	int i = 0;

	int min_level=4;

	divide = 2;
	yjump = rows/2;
	xjump = cols/2;

	iterate ++;

	if(iterate <= max_level) {

		for( y = y_init; y < rows + y_init; y += yjump )
		{
			const uchar* src = image.ptr<uchar>(y+yjump/2);
			uchar* mean = means.ptr<uchar>(y+yjump/2);
			uchar* sigma = sigmas.ptr<uchar>(y+yjump/2);

			for( x = x_init; x < cols + x_init; x += xjump )
			{
				if (iterate >= min_level) {
					if (iterate == max_level) {
						if (foreground_condition(src[x+xjump/2],mean[x+xjump/2],sigma[x+xjump/2] )) {

							root->child[i]->scan_me = true;
							if(print_quad)
								draw_rect(root->child[i], 2, out);
						} else {
							root->child[i]->scan_me = false;
						}

						if (mixed_condition(src[x+xjump/2],mean[x+xjump/2],sigma[x+xjump/2] )) {

							root->child[i]->morph_me = true;
							if(print_quad)
								draw_rect(root->child[i], 2, out);

						} else {
							root->child[i]->morph_me = false;
						}

						root->child[i]->scanned = false;
						root->child[i]->morphed = false;
					} else {
						calc_sub_nodes(image, means, sigmas, root->child[i], out, iterate, x, y, yjump, xjump, print_quad);
					}
				} else {
					calc_sub_nodes(image, means, sigmas, root->child[i], out, iterate, x, y, yjump, xjump, print_quad);
				}

				i++;
			}
		}
	}



}

void search_more_foreground(Node* node, Mat image, Mat first, Mat show, Mat means, Mat sigmas, Mat& image_foreground, Mat& image_tree, bool print_quad) {

	bool scanning	= false;
	bool morphing 	= false;

	Node* next;

	for(int round = 0; round < 4; round++){

		switch(round) {
		case 0:
			next = node->top;
			break;
		case 1:
			next = node->right;
			break;
		case 2:
			next = node->left;
			break;
		case 3:
			next = node->bottom;
			break;
		}

		if (next != NULL) {
			if (next->scan_me == false and next->scanned == false and next->morph_me == false and next->morphed == false) {
				if(print_quad)
					draw_rect(next, 2, image_tree);

				scanning = false;
				morphing = false;

				for(int y = next->start.y; y < next->end.y; y++) {

					const uchar* src 	= image.ptr<uchar>(y);
					uchar* mean 		= means.ptr<uchar>(y);
					uchar* sigma 		= sigmas.ptr<uchar>(y);
					uchar* foreground 	= image_foreground.ptr<uchar>(y);

					for(int x = next->start.x; x < next->end.x; x++) {

						if (foreground_condition(src[x], mean[x], sigma[x])) {
							foreground[x] = 255;
							scanning = true;
						}

						if (mixed_condition(src[x], mean[x], sigma[x])) {
							morphing = true;
						}

					}
				}


				if(morphing) {
					next->morph_me = true;
				}

				next->scanned = true;
				if(scanning) {

					search_more_foreground(next, image, first, show, means, sigmas, image_foreground, image_tree, print_quad);
				}

			}
		}
	}


}

void search_more_morphground(Node* node, Mat image, Mat first, Mat show, Mat means, Mat sigmas, Mat& image_foreground, Mat& image_tree, bool print_quad) {

	bool morphing 	= false;
	bool scanning 	= false;

	Node* next;

	for(int round = 0; round < 4; round++){

		switch(round) {
		case 0:
			next = node->top;
			break;
		case 1:
			next = node->right;
			break;
		case 2:
			next = node->left;
			break;
		case 3:
			next = node->bottom;
			break;
		}

		if (next != NULL) {
			if (next->morph_me == false and next->morphed == false) {

				if(print_quad)
					draw_rect(next, 2, image_tree);

				scanning = false;
				morphing = false;

				for(int y = next->start.y; y < next->end.y; y++) {

					const uchar* src 	= image.ptr<uchar>(y);
					uchar* mean 		= means.ptr<uchar>(y);
					uchar* sigma 		= sigmas.ptr<uchar>(y);
					uchar* foreground 	= image_foreground.ptr<uchar>(y);

					for(int x = next->start.x; x < next->end.x; x++) {

						if (foreground_condition(src[x], mean[x], sigma[x])) {
							scanning = true;
							foreground[x] = 255;
						}

						if (mixed_condition(src[x], mean[x], sigma[x])){
							morphing = true;
						}

					}
				}


				if(morphing) {

					next->morphed = true;
					calc_morph_rect(true, next, image, first, image_tree);
					search_more_morphground(next, image, first, show, means, sigmas, image_foreground, image_tree, print_quad);

				} else {

					next->morphed = true;
					calc_morph_rect(true, next, image, first, image_tree);

				}

			}
		}
	}


}

void calc_update(Mat image, Mat& means, Mat& sigmas, float alpha, Node* location[32][32], bool print_update, Mat& out, BackgroundSubtractorMOGA& obj) {

	int upx = obj.upx;
	int upy = obj.upy;

	alpha = 1;

	Node* node = location[upx][upy];

	int startx = node->start.x;
	int starty = node->start.y;
	int endx = node->end.x;
	int endy = node->end.y;

	const float minVar = (float)(obj.noiseSigma*obj.noiseSigma);

	if (node->morphed == false and node->scanned ==false) {

		if(print_update)
			draw_rect(node, 3, out);

		for(int y = starty; y < endy; y++) {

			const uchar* src 	= image.ptr<uchar>(y);
			uchar* mean 		= means.ptr<uchar>(y);
			uchar* sigma 		= sigmas.ptr<uchar>(y);

			for(int x = startx; x < endx; x++) {

				float pix = src[x];
				float mu = mean[x];
				float var = sigma[x]*sigma[x];
				float diff = pix - mu;
				float d2 = diff*diff;

				mean[x] = mu + alpha*diff;
				var = max(var + alpha*(d2 - var), minVar);
				//var = var + alpha*(d2 - var);
				sigma[x] = sqrt(var);
			}
		}
	}

	if (upx < 31)
		upx += 1;
	else {
		upx = 0;
		if(upy < 31)
			upy += 1;
		else
			upy = 0;
	}

	obj.upx = upx;
	obj.upy = upy;

}

void calc_scan_mes(Mat image,Mat first, Mat show, Mat means, Mat sigmas, Mat& image_foreground, Mat image_tree, Node* location[32][32], bool print_quad){

	bool morphing;
	bool scanning;

	for(int y_in = 0; y_in < 32; y_in++) {
		for(int x_in = 0; x_in < 32; x_in++) {

			morphing = false;
			scanning = false;

			if (location[x_in][y_in]->scan_me == true) {

				for(int y = y_in*15; y < (y_in+1)*15; y++) {

					const uchar* src 	= image.ptr<uchar>(y);
					uchar* mean 		= means.ptr<uchar>(y);
					uchar* sigma 		= sigmas.ptr<uchar>(y);
					uchar* foreground 	= image_foreground.ptr<uchar>(y);

					for(int x = x_in*20; x < (x_in+1)*20; x++) {

						if (foreground_condition(src[x], mean[x], sigma[x])) {
							scanning = true;
							foreground[x] = 255;
						} else {
							morphing = true;
						}
					}
				}

				location[x_in][y_in]->scanned = true;

				if (scanning) {
					search_more_foreground(location[x_in][y_in], image, first, show, means, sigmas, image_foreground, image_tree, print_quad);

				}

				if (morphing) {
					location[x_in][y_in]->morph_me = true;
				}


			}

		}
	}


}


void calc_morph_mes(Mat image,Mat first, Mat show, Mat means, Mat sigmas, Mat& image_foreground, Mat image_tree, Node* location[32][32], bool print_quad){

	bool morphing;
	bool scanning;

	for(int y_in = 0; y_in < 32; y_in++) {
		for(int x_in = 0; x_in < 32; x_in++) {

			Node* node = location[x_in][y_in];

			morphing = false;
			scanning = false;

			if (node->morph_me == true) {
				for(int y = node->start.y; y < node->end.y; y++) {

					const uchar* src 	= image.ptr<uchar>(y);
					uchar* mean 		= means.ptr<uchar>(y);
					uchar* sigma 		= sigmas.ptr<uchar>(y);
					uchar* foreground 	= image_foreground.ptr<uchar>(y);

					for(int x = node->start.x; x < node->end.x; x++) {

						if (foreground_condition(src[x], mean[x], sigma[x])) {
							scanning = true;
							foreground[x] = 255;
						}

						if (mixed_condition(src[x], mean[x], sigma[x])){
							morphing = true;
						}

					}
				}

				if (morphing) {

					calc_morph_rect(false, node, image, first, image_tree);
					node->morphed = true;

					search_more_morphground(node, image, first, show, means, sigmas, image_foreground, image_tree, print_quad);

				}
			}

		}
	}


}

//This is the main function processing the input to the binary representation.
static void process8uC1( BackgroundSubtractorMOGA& obj, const Mat& image, Mat& fgmask, Mat& out, bool print_quad, bool print_update, double learningRate, int num )
{

    int x, y, rows = image.rows, cols = image.cols;
    float alpha = (float)learningRate;

    fgmask.create( image.size(), CV_8U );
    Mat means = obj.means;
    Mat sigmas = obj.sigmas;

    float minVar = (float)(obj.noiseSigma*obj.noiseSigma);

    static Node* location[32][32];
    static Node* root = make_tree(location);

	static Mat image_tree( image.size(), CV_8U );
	static Mat show( image.size(), CV_8U );
	static Mat first( image.size(), CV_8U );

	// A background model is calculated here. It contains the standard deviation ('sigmas')
	// and mean intensity value ('means') for each pixel. 'fgmask' represents the mean
	// intensity values while updating the background.
  	if( alpha > 0 )
	{
		for( y = 0; y < rows; y++ )
		{
			const uchar* src = image.ptr<uchar>(y);
			uchar* dst = fgmask.ptr<uchar>(y);
			uchar* mean = means.ptr<uchar>(y);
			uchar* sigma = sigmas.ptr<uchar>(y);

			for( x = 0; x < cols; x++)
			{
				float pix = src[x];

				float mu = mean[x];
				float var = sigma[x]*sigma[x];
				float diff = pix - mu;
				float d2 = diff*diff;

				mean[x] = mu + alpha*diff;
				var = max(var + alpha*(d2 - var), minVar);
				sigma[x] = sqrt(var);

				if(  (int)pix < (int) (mean[x] - 3*sigma[x]))
				{
					dst[x] = 255;
				} else
					dst[x] = 0;

			}
		}
	}

	image_tree = out;

	if(num == 2 or num == 3)
		image_tree = Scalar(0,0,0);

	// If the learning rate is set to zero, the actual segmentation is performed.
	// The quad-tree is used to detect the regions of interest to perform segmentation.
	if (alpha == 0) {

	    first = Scalar(0,0,0);

		fgmask.setTo(Scalar(0,0,0));

		int iterate = 0;

		// In a first step, a rough scan is performed to detect the regions of interest:
		calc_sub_nodes(image, means, sigmas, root, image_tree, iterate, 0, 0, rows, cols, print_quad);

		// In a second step, two neighbour scans are performed to find missing parts of the regions of interest:
		calc_scan_mes(image, first, show, means, sigmas, fgmask, image_tree, location, print_quad);
		calc_morph_mes(image, first, show, means, sigmas, fgmask, image_tree, location, print_quad);

		for (int i = 0; i < 32; i++) {
			calc_update(image, means, sigmas, alpha, location, print_update, out, obj);
		}

		add(fgmask, first, fgmask);

	}

	if (num == 2 or num == 3) {

			Mat tmp_out(image.size(), CV_8U);
			Mat tmp_tree(image.size(), CV_8U);

			cvtColor(image_tree, tmp_tree, CV_RGB2GRAY );

			Mat input;

			if (num == 2)
				input = fgmask;
			else
				input = means;

			for(int yt = 0; yt < 480; yt++ )
			{
				uchar* fg 		= input.ptr<uchar>(yt);
				uchar* tree 	= tmp_tree.ptr<uchar>(yt);
				uchar* fin 		= tmp_out.ptr<uchar>(yt);

				for(int xt = 0; xt < 640; xt++)
				{
					if(tree[xt] == 255)
						fin[xt] = 255;
					else
						fin[xt] = fg[xt];
				}
			}

			cvtColor(tmp_out, out, CV_GRAY2RGB );
		}

}

void BackgroundSubtractorMOGA::operator()(const Mat& image, Mat& fgmask, Mat& out, bool print_quad, bool print_update, double learningRate, int num)
{

    process8uC1( *this, image, fgmask, out, print_quad, print_update, learningRate, num );

}


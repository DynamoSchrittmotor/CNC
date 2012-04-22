/*
 * quadtree.h
 *
 *  Created on: Aug 28, 2011
 *      Author: Alexander Peller
 *
 *      The quad-tree implementation allows a faster processing of the background
 *      subtraction method. The quad-tree is built in advance to save processing
 *      power while operating. Each node of the quad-tree has boolean variables
 *      giving information about the status of a template of certain size of the
 *      image. They store which template has to be segmented by the background sub-
 *      traction (scan_me/scanned) and which template are taken for edge detection
 *      (morph_me/morphed). The performing of a neighbour scan requires that every
 *      node has to know its top, down, left and right neighbour at maximum level
 *      (the finest level). The nodes are defined by a struct construct. 'calc_nodes'
 *      builds the quad-tree and 'find_neighbours' links the neighbours at maximum
 *      level.
 *
 */

#define max_level 5

struct Node
{
	Node();
	Node(Point _start, Point _end);
	Node(Point _start, Point _end, int yes, Mat& out);

    bool scan_me;
    bool morph_me;
    bool scanned;
    bool morphed;

    Node* child[4];

	Point start;
	Point end;

	Node* left;
	Node* right;
	Node* top;
	Node* bottom;

};

Node::Node(){

	scan_me = false;
	morph_me = false;
	scanned = false;
	morphed = false;

	for (int i = 0; i < 4; i++) {
		child[i] = NULL;
	}
}

Node::Node(Point _start, Point _end){

	scan_me = false;
	morph_me = false;
	scanned = false;
	morphed = false;

	for (int i = 0; i < 4; i++) {
		child[i] = NULL;
	}

	start = _start;
	end = _end;

}

Node::Node(Point _start, Point _end, int yes, Mat& out){

	scan_me = false;
	morph_me = false;
	scanned = false;
	morphed = false;

	for (int i = 0; i < 4; i++) {
		child[i] = NULL;
	}

	line(out,	_start,					Point(_start.x,_end.y),	CV_RGB( 255, 255, 255 ));
	line(out,	Point(_start.x,_end.y),	_end,					CV_RGB( 255, 255, 255 ));
	line(out,	_end,					Point(_end.x,_start.y),	CV_RGB( 255, 255, 255 ));
	line(out,	Point(_end.x,_start.y),	_start,					CV_RGB( 255, 255, 255 ));

	if (yes == 1)
		line(out,Point(_end.x,_start.y),Point(_start.x,_end.y),CV_RGB( 255, 255, 255 ));

}


void calc_nodes(Node* root, Node* location[32][32], int iterate, int x_init, int y_init, int rows, int cols) {

	int x;
	int y;

	int i = 0;

	int xjump;
	int yjump;

	yjump = rows/2;
	xjump = cols/2;

	iterate++;

	if(iterate <= max_level) {
		for( y = y_init; y < rows + y_init; y += yjump )
		{
			for( x = x_init; x < cols + x_init; x += xjump )
			{
				root->child[i] = new Node(Point(x,y),Point(x+xjump,y+yjump));
				calc_nodes(root->child[i], location, iterate, x, y, yjump, xjump);

				if (iterate == max_level) {
					//printf("x = %d, y = %d \n", x/xjump, y/yjump);
					int xtemp = x/xjump;
					int ytemp = y/yjump;
					location[xtemp][ytemp] = root->child[i];
				}

				//printf("iterate = %d, i = %d \n", iterate, i);

				i++;
			}
		}

	}

	//printf("\n");

}

void find_neighbours(Node* location[32][32]) {

	for(int x = 0; x < 32; x++) {
		for(int y = 0; y < 32; y++) {
			if (x == 0 and y == 0) {
				location[x][y]->top 	= NULL;
				location[x][y]->right 	= location[x+1][y];
				location[x][y]->bottom 	= location[x][y+1];
				location[x][y]->left 	= NULL;
			} else if (y == 0 and x > 0 and x < 31) {
				location[x][y]->top 	= NULL;
				location[x][y]->right 	= location[x+1][y];
				location[x][y]->bottom 	= location[x][y+1];
				location[x][y]->left 	= location[x-1][y];
			} else if (x == 31 and y == 0) {
				location[x][y]->top 	= NULL;
				location[x][y]->right 	= NULL;
				location[x][y]->bottom 	= location[x][y+1];
				location[x][y]->left 	= location[x-1][y];
			} else if (x == 31 and y > 0 and y < 31) {
				location[x][y]->top 	= location[x][y-1];
				location[x][y]->right 	= NULL;
				location[x][y]->bottom 	= location[x][y+1];
				location[x][y]->left 	= location[x-1][y];
			} else if (x == 31 and y == 31) {
				location[x][y]->top 	= location[x][y-1];
				location[x][y]->right 	= NULL;
				location[x][y]->bottom 	= NULL;
				location[x][y]->left 	= location[x-1][y];
			} else if (x > 0 and x < 31 and y == 31) {
				location[x][y]->top 	= location[x][y-1];
				location[x][y]->right 	= location[x+1][y];
				location[x][y]->bottom 	= NULL;
				location[x][y]->left 	= location[x-1][y];
			} else if (x == 0 and y == 31) {
				location[x][y]->top 	= location[x][y-1];
				location[x][y]->right 	= location[x+1][y];
				location[x][y]->bottom 	= NULL;
				location[x][y]->left 	= NULL;
			} else if (x == 0 and y > 0 and y < 31) {
				location[x][y]->top 	= location[x][y-1];
				location[x][y]->right 	= location[x+1][y];
				location[x][y]->bottom 	= location[x][y+1];
				location[x][y]->left 	= NULL;
			} else {
				location[x][y]->top 	= location[x][y-1];
				location[x][y]->right 	= location[x+1][y];
				location[x][y]->bottom 	= location[x][y+1];
				location[x][y]->left 	= location[x-1][y];
			}
		}
	}

}


Node* make_tree(Node* location[32][32]){

	int iterate = 0;

	Node* root;
	root = new Node;

	calc_nodes(root, location, iterate, 0, 0, 480, 640);
	find_neighbours(location);

	return root;

}

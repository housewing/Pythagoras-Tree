#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

Mat img = Mat(500,600,CV_8UC3);
int maxlevel =0;
int w = img.cols/2;
int h = img.rows-10;

const int angle_slider_max=90;
int angle_sider = 30;
const char* trackbar_value = "Angle";
void on_trackbar( int, void* );

void root(int x, int y, float len, float angle ,float width) 
{
	int x2,y2; // branch color and endpoint..
	Scalar c;

	x2=x+len*cos(angle); 
	y2=y-len*sin(angle);
	line(img,Point(x,y),Point(x2,y2),Scalar(50,100,150),width,CV_AA,0);
}
void Branch(int x, int y, float len, float angle ,float width) 
{
	int x2,y2; // branch color and endpoint..
	float childlength; // the length of any children off this branch..
	float childangle; // the offshoot angles of any childrenqq
	float childwidth;
	float templen = 100;
	Scalar c;

	if(len>25) // if its a long branch, make it brown. 
		c=Scalar(50,100,150);
	else
		c=Scalar(0,255,0); 

	x2=x+len*cos(angle); 
	y2=y-len*sin(angle);
	line(img,Point(x,y),Point(x2,y2),c,width,CV_AA,0);
	//cout << x2-x << " " << y2-y << endl;

	if(maxlevel>15)
		maxlevel = 15;
	if(maxlevel<0)
		maxlevel = 0;

	for(int l=0;l<maxlevel;l++){
		templen = templen*0.8f;
	}
	//cout << "temp " << templen << endl;

	if(len > templen){
		childlength=len*0.8f;
		childangle = CV_PI/180*90*(float)angle_sider/100.0;
		childwidth = width*0.5f;

		Branch(x2,y2,childlength,angle-childangle,childwidth); 
		Branch(x2,y2,childlength,angle+childangle,childwidth);
	}
}
// Callback function for mouse events
static void mouse_callback(int event, int x, int y, int, void *)
{
	// When the left mouse button is pressed, record its position and save it in corner1
	if(event == EVENT_LBUTTONDOWN)
	{
		maxlevel++;
		cout << maxlevel << endl;
		Branch(w,h,100,CV_PI/180*90,20);
		imshow("FractalTree",img);
	}
	else if(event == EVENT_RBUTTONDOWN)
	{
		maxlevel--;
		cout << maxlevel << endl;
		img.setTo(255);
		Branch(w,h,100,CV_PI/180*90,20);
		imshow("FractalTree",img);
	}
}
int main()
{
	cout << "-----Left click add branch" << endl;
	cout << "-----Right click decrease branch" << endl;
	img.setTo(255);
	root(w,h,100,CV_PI/180*90,20);
	imshow("FractalTree", img);
	// Set the mouse event callback function
	setMouseCallback("FractalTree", mouse_callback);

	createTrackbar( trackbar_value, "FractalTree", &angle_sider, angle_slider_max, on_trackbar );
	// Exit by pressing 'q'
	waitKey();
	return 0;
}

void on_trackbar( int, void* )
{
	img.setTo(255);
	Branch(w,h,100,3.14159/2.0,20);
 
	imshow( "FractalTree", img );
}
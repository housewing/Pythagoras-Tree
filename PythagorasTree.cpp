#include <opencv2\opencv.hpp>
#include <iostream>
#include <cmath>

using namespace cv;

Mat canvas = Mat::zeros(480, 640, CV_8UC3);
int size = 40;

std::vector<cv::Point> applyTransformation(std::vector<cv::Point> _points, cv::Mat transformation, cv::Point2f reference, bool _params)
{
	int p1 = 0, p2 = 0;
	if (_params)
	{
		p1 = 1; p2 = 2;
	}
	else
	{
		p1 = 0; p2 = 3;
	}

	Point2f target = _points[p1];

	for (unsigned int i = 0; i < _points.size(); ++i)
	{
		cv::Point2f tmp = _points[i];
		tmp.x = tmp.x - reference.x;
		tmp.y = tmp.y - reference.y;
		_points[i].x = tmp.x * transformation.at<float>(0, 0) + tmp.y * transformation.at<float>(0, 1) + reference.x;
		_points[i].y = tmp.x * transformation.at<float>(1, 0) + tmp.y * transformation.at<float>(1, 1) + reference.y;
	}

	target.x -= _points[p2].x;
	target.y -= _points[p2].y;
	for (auto& p : _points)
	{
		p.x += target.x;
		p.y += target.y;
	}

	return _points;
}

void draw(std::vector<Point>& _points, int _level)
{
	float angle = 30;
	float rotation = CV_PI / 180.0f * angle;
	//std::cout << "cos " << cos(rotation) << " sin " << sin(rotation) << " rotation " << rotation << std::endl;

	cv::Mat transformationRight = cv::Mat::zeros(2, 2, CV_32FC1);
	transformationRight.at<float>(0, 0) = pow(cos(rotation), 2);
	transformationRight.at<float>(0, 1) = -cos(rotation) * sin(rotation);
	transformationRight.at<float>(1, 0) = cos(rotation) * sin(rotation);
	transformationRight.at<float>(1, 1) = pow(cos(rotation), 2);

	cv::Mat transformationLeft = cv::Mat::zeros(2, 2, CV_32FC1);
	transformationLeft.at<float>(0, 0) = sin(rotation) * sin(rotation);
	transformationLeft.at<float>(0, 1) = cos(rotation) * sin(rotation);
	transformationLeft.at<float>(1, 0) = -cos(rotation) * sin(rotation);
	transformationLeft.at<float>(1, 1) = sin(rotation) * sin(rotation);

	std::vector<Point> pointRight = applyTransformation(_points, transformationRight, _points[3], true);
	std::vector<Point> pointLeft = applyTransformation(_points, transformationLeft, _points[3], false);

	//for (unsigned int i = 0; i < _points.size(); i++)
	//{
	//	circle(canvas, _points[i], 1, Scalar(0, 255, 0), 1, 8, 0);
	//	circle(canvas, pointLeft[i], 1, Scalar(0, 255, 255), 1, 8, 0);
	//	circle(canvas, pointRight[i], 1, Scalar(0, 0, 255), 1, 8, 0);
	//}

	const Point* root[1] = { &_points[0] };
	const Point* left[1] = { &pointLeft[0] };
	const Point* right[1] = { &pointRight[0] };
	int npt[] = { 4 };

	//int color = 255 * (_level / 10.0);
	//int colorR = 255 * ((_level + 1) / 10.0);
	int color = 255 * (1.0 - _level / 10.0);
	int colorG = 255 * (1.0 - (_level + 1) / 10.0);

	fillPoly(canvas, root, npt, 1, Scalar(0, color, 255 - color), 8);
	fillPoly(canvas, left, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	fillPoly(canvas, right, npt, 1, Scalar(0, colorG, 255 - colorG), 8);

	if (_level < 5)
	{
		draw(pointLeft, _level + 1);
		draw(pointRight, _level + 1);
	}
}

int main(int argc, char** argv)
{
	int height = canvas.rows;
	int weight = canvas.cols / 2;
	
	std::vector<Point> points;
	points.push_back(Point(weight - size, height - 2 * size));
	points.push_back(Point(weight + size, height - 2 * size));
	points.push_back(Point(weight + size, height));
	points.push_back(Point(weight - size, height));

	draw(points, 0);

	imshow("canvas", canvas);

	waitKey();
	return 0;
}
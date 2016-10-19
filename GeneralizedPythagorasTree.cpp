#include <opencv2\opencv.hpp>
#include <iostream>
#include <cmath>

using namespace cv;

Mat canvas = Mat::zeros(480, 640, CV_8UC3);
std::vector<Point> points;
int size = 60;

Point applyTransformation(Point _points, cv::Mat transformation, cv::Point reference)
{
	cv::Point tmp = _points;
	tmp.x = tmp.x - reference.x;
	tmp.y = tmp.y - reference.y;
	_points.x = (tmp.x * transformation.at<float>(0, 0) + tmp.y * transformation.at<float>(0, 1)) + reference.x;
	_points.y = (tmp.x * transformation.at<float>(1, 0) + tmp.y * transformation.at<float>(1, 1)) + reference.y;

	return _points;
}

Mat getTransformation(float _rotation)
{
	cv::Mat transformation = cv::Mat::zeros(2, 2, CV_32FC1);
	transformation.at<float>(0, 0) = cos(_rotation);
	transformation.at<float>(0, 1) = -sin(_rotation);
	transformation.at<float>(1, 0) = sin(_rotation);
	transformation.at<float>(1, 1) = cos(_rotation);

	return transformation;
}

std::vector<Point> transformationPoint(Point _points, Point _center, float _angle)
{
	float rotation = CV_PI / 180.0f * _angle;
	Mat transformation = getTransformation(rotation);

	std::vector<Point> pointSet;
	pointSet.push_back(_points);
	Point first = applyTransformation(pointSet[0], transformation, _center);
	pointSet.push_back(first);
	//circle(canvas, first, 2, Scalar(0, 255, 255), 2, 8, 0);

	float rotationNinty = CV_PI / 180.0f * 90.0;
	transformation = getTransformation(rotationNinty);

	Point second = applyTransformation(pointSet[0], transformation, pointSet[1]);
	pointSet.push_back(second);
	//circle(canvas, second, 2, Scalar(0, 255, 255), 2, 8, 0);

	Point third = applyTransformation(pointSet[1], transformation, pointSet[2]);
	pointSet.push_back(third);
	//circle(canvas, third, 2, Scalar(0, 255, 255), 2, 8, 0);

	return pointSet;
}

void tree(std::vector<Point> _points, int _level)
{
	Point2f center;
	center.x = (_points[2].x + _points[3].x) / 2.0;
	center.y = (_points[2].y + _points[3].y) / 2.0;
	//circle(canvas, center, 2, Scalar(0, 255, 255), 2, 8, 0);

	//std::vector<float> angle(3, 60);
	std::vector<float> angle;
	angle.push_back(30.0);
	angle.push_back(70.0);
	angle.push_back(80.0);
	float rotation = CV_PI / 180.0f * angle[0];
	Mat transformation = getTransformation(rotation);

	std::vector<std::vector<Point>> pointSet(angle.size());
	pointSet[0] = transformationPoint(_points[3], center, angle[0]);
	pointSet[1] = transformationPoint(pointSet[0][1], center, angle[1]);
	pointSet[2] = transformationPoint(pointSet[1][1], center, angle[2]);

	//std::vector<Point> pointFirst = transformationPoint(_points[3], center, angle[0]);
	//std::vector<Point> pointSecond = transformationPoint(pointFirst[1], center, angle[1]);
	//std::vector<Point> pointThird = transformationPoint(pointSecond[1], center, angle[2]);

	//for (unsigned int i = 0; i < _points.size(); i++)
	//{
	//	circle(canvas, _points[i], 2, Scalar(0, 255, 0), 2, 8, 0);
	//	circle(canvas, pointFirst[i], 2, Scalar(0, 255, 255), 2, 8, 0);
	//	circle(canvas, pointSecond[i], 2, Scalar(255, 0, 255), 2, 8, 0);
	//	circle(canvas, pointThird[i], 2, Scalar(0, 0, 255), 2, 8, 0);
	//}

	const Point* root[1] = { &_points[0] };
	const Point* left[1] = { &pointSet[0][0] };
	const Point* mid[1] = { &pointSet[1][0] };
	const Point* right[1] = { &pointSet[2][0] };
	int npt[] = { 4 };

	int color = 255 * (1.0 - _level / 10.0);
	int colorG = 255 * (1.0 - (_level + 1) / 10.0);

	fillPoly(canvas, root, npt, 1, Scalar(0, color, 255 - color), 8);
	fillPoly(canvas, left, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	fillPoly(canvas, mid, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	fillPoly(canvas, right, npt, 1, Scalar(0, colorG, 255 - colorG), 8);

	if (_level < 7)
	{
		tree(pointSet[0], _level + 1);
		tree(pointSet[1], _level + 1);
		tree(pointSet[2], _level + 1);
	}
}

int main(int argc, char **argv)
{
	int height = canvas.rows;
	int weight = canvas.cols / 2;
	
	points.push_back(Point(weight - size, height));
	points.push_back(Point(weight + size, height));
	points.push_back(Point(weight + size, height - 2 * size));
	points.push_back(Point(weight - size, height - 2 * size));

	tree(points, 0);
	imshow("canvas", canvas);

	waitKey();
	return 0;
}
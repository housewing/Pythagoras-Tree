#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;

Mat canvas;

Mat getTransform(float _scale, float _rotation)
{
	cv::Mat matrix = cv::Mat::zeros(2, 2, CV_32FC1);
	matrix.at<float>(0, 0) = _scale * cos(_rotation);
	matrix.at<float>(0, 1) = _scale * -sin(_rotation);
	matrix.at<float>(1, 0) = _scale * sin(_rotation);
	matrix.at<float>(1, 1) = _scale * cos(_rotation);

	return matrix;
}

std::vector<Point> applyTransform(std::vector<Point> _points, cv::Mat _matrix, cv::Point _reference)
{
	for (auto& p : _points)
	{
		Point tmp = p;
		tmp.x -= _reference.x;
		tmp.y -= _reference.y;
		p.x = tmp.x * _matrix.at<float>(0, 0) + tmp.y * _matrix.at<float>(0, 1) + _reference.x;
		p.y = tmp.x * _matrix.at<float>(1, 0) + tmp.y * _matrix.at<float>(1, 1) + _reference.y;
	}

	return _points;
}

void draw(std::vector<Point> _points, int _level)
{
	std::vector<std::vector<Point>> points(4);

	//first
	float rotation = CV_PI / 180.0f * 0.0;
	float scale = 1.0 / 3.0;
	Mat matrix = getTransform(scale, rotation);
	points[0] = applyTransform(_points, matrix, _points[0]);

	//second
	rotation = CV_PI / 180.0f * -60.0;
	matrix = getTransform(scale, rotation);
	points[1] = applyTransform(_points, matrix, _points[0]);

	Point2f tmpPt = points[0][1];
	tmpPt.x -= points[1][0].x;
	tmpPt.y -= points[1][0].y;
	for (auto& p : points[1])
	{
		p.x += tmpPt.x;
		p.y += tmpPt.y;
	}

	//third
	rotation = CV_PI / 180.0f * 60.0;
	matrix = getTransform(scale, rotation);
	points[2] = applyTransform(_points, matrix, _points[0]);

	tmpPt = points[1][1];
	tmpPt.x -= points[2][0].x;
	tmpPt.y -= points[2][0].y;
	for (auto& p : points[2])
	{
		p.x += tmpPt.x;
		p.y += tmpPt.y;
	}

	//fourth
	rotation = CV_PI / 180.0f * 0.0;
	matrix = getTransform(scale, rotation);
	points[3] = applyTransform(_points, matrix, _points[0]);

	tmpPt = points[2][1];
	tmpPt.x -= points[3][0].x;
	tmpPt.y -= points[3][0].y;
	for (auto& p : points[3])
	{
		p.x += tmpPt.x;
		p.y += tmpPt.y;
	}

	std::vector<Point> black;
	black.push_back(points[0][1]);
	black.push_back(points[3][0]);

	for (size_t i = 0; i < 1; i++)
	{
		line(canvas, black[i], black[i + 1], Scalar(0, 0, 0), 2, 8);
		line(canvas, points[0][i], points[0][i + 1], Scalar(0, 255, 255), 1, 8);
		line(canvas, points[1][i], points[1][i + 1], Scalar(0, 255, 255), 1, 8);
		line(canvas, points[2][i], points[2][i + 1], Scalar(0, 255, 255), 1, 8);
		line(canvas, points[3][i], points[3][i + 1], Scalar(0, 255, 255), 1, 8);
	}
	

	if (_level < 3)
	{
		draw(points[0], _level + 1);
		draw(points[1], _level + 1);
		draw(points[2], _level + 1);
		draw(points[3], _level + 1);
	}
}

int main(int argc, char** argv)
{
	canvas = Mat::zeros(600, 640, CV_8UC3);

	std::vector<std::vector<Point>> pts(3);
	pts[0].push_back(Point(120, 440));
	pts[0].push_back(Point(320, 40));
	pts[1].push_back(Point(320, 40));
	pts[1].push_back(Point(520, 440));
	pts[2].push_back(Point(520, 440));
	pts[2].push_back(Point(120, 440));

	//for (auto& p : pts[0])
	//{
	//	circle(canvas, p, 1, Scalar(0, 255, 0), 2, 8, 0);
	//}

	draw(pts[0], 0);
	draw(pts[1], 0);
	draw(pts[2], 0);

	imshow("canvsa", canvas);

	waitKey();
	return 0;
}
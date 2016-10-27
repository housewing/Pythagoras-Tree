#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;

Mat canvas;

std::vector<Point> applyTransform(std::vector<Point> _points, cv::Mat _scale, cv::Point _reference, cv::Point2f _transPt, float _len)
{
	for (auto& p : _points)
	{
		Point tmp = p;
		tmp.x -= _reference.x;
		tmp.y -= _reference.y;
		p.x = tmp.x * _scale.at<float>(0, 0) + tmp.y * _scale.at<float>(0, 1) + _reference.x + _transPt.x * _len;
		p.y = tmp.x * _scale.at<float>(1, 0) + tmp.y * _scale.at<float>(1, 1) + _reference.y - _transPt.y * _len;
	}

	return _points;
}

Point applyRotation(Point _points, cv::Mat _rotationMatrix, cv::Point _reference)
{
	cv::Point tmp = _points;
	tmp.x = tmp.x - _reference.x;
	tmp.y = tmp.y - _reference.y;
	_points.x = (tmp.x * _rotationMatrix.at<float>(0, 0) + tmp.y * _rotationMatrix.at<float>(0, 1)) + _reference.x;
	_points.y = (tmp.x * _rotationMatrix.at<float>(1, 0) + tmp.y * _rotationMatrix.at<float>(1, 1)) + _reference.y;

	return _points;
}

Mat getRotation(float _rotation)
{
	cv::Mat rotationMatrix = cv::Mat::zeros(2, 2, CV_32FC1);
	rotationMatrix.at<float>(0, 0) = cos(_rotation);
	rotationMatrix.at<float>(0, 1) = -sin(_rotation);
	rotationMatrix.at<float>(1, 0) = sin(_rotation);
	rotationMatrix.at<float>(1, 1) = cos(_rotation);

	return rotationMatrix;
}

Mat getScale(float _scale)
{
	cv::Mat transformation = cv::Mat::zeros(2, 2, CV_32FC1);
	transformation.at<float>(0, 0) = _scale;
	transformation.at<float>(0, 1) = 0.0;
	transformation.at<float>(1, 0) = 0.0;
	transformation.at<float>(1, 1) = _scale;

	return transformation;
}

void draw(std::vector<Point> _points, int _level)
{
	Mat scaleMatrix = getScale(0.381966);
	Point2f lenPt;
	lenPt.x = _points[0].x - _points[1].x;
	lenPt.y = _points[0].y - _points[1].y;
	float length = std::sqrt(std::pow(lenPt.x, 2) + std::pow(lenPt.y, 2));

	std::vector<std::vector<Point>> ptSet(5);
	Point2f transPt = Point2f(0.0, 0.0);
	ptSet[0] = applyTransform(_points, scaleMatrix, _points[0], transPt, length);

	transPt = Point2f(0.618, 0.0);
	ptSet[1] = applyTransform(_points, scaleMatrix, _points[0], transPt, length);

	transPt = Point2f(0.809, 0.588);
	ptSet[2] = applyTransform(_points, scaleMatrix, _points[0], transPt, length);

	transPt = Point2f(0.309, 0.951);
	ptSet[3] = applyTransform(_points, scaleMatrix, _points[0], transPt, length);

	transPt = Point2f(-1 * 0.191, 0.588);
	ptSet[4] = applyTransform(_points, scaleMatrix, _points[0], transPt, length);

	const Point* a[1] = { &ptSet[0][0] };
	const Point* b[1] = { &ptSet[1][0] };
	const Point* c[1] = { &ptSet[2][0] };
	const Point* d[1] = { &ptSet[3][0] };
	const Point* e[1] = { &ptSet[4][0] };
	const Point* f[1] = { &_points[0] };
	int npt[] = { 5 };

	int colorG = 255 * (1.0 - (_level + 1) / 10.0);
	fillPoly(canvas, f, npt, 1, Scalar(0, 0, 0), 8);
	fillPoly(canvas, a, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	fillPoly(canvas, b, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	fillPoly(canvas, c, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	fillPoly(canvas, d, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	fillPoly(canvas, e, npt, 1, Scalar(0, colorG, 255 - colorG), 8);

	if (_level < 3)
	{
		draw(ptSet[0], _level + 1);
		draw(ptSet[1], _level + 1);
		draw(ptSet[2], _level + 1);
		draw(ptSet[3], _level + 1);
		draw(ptSet[4], _level + 1);
	}
}

int main(int arc, char** argv)
{
	canvas = Mat::zeros(480, 640, CV_8UC3);
	std::vector<Point> pts;
	pts.push_back(Point(220, 380));
	pts.push_back(Point(420, 380));

	float rotation = CV_PI / 180.0f * 108;
	Mat rotationMatrix = getRotation(rotation);
	Point tmpPt = applyRotation(pts[0], rotationMatrix, pts[1]);
	pts.push_back(tmpPt);

	tmpPt = applyRotation(pts[1], rotationMatrix, pts[2]);
	pts.push_back(tmpPt);

	tmpPt = applyRotation(pts[2], rotationMatrix, pts[3]);
	pts.push_back(tmpPt);

	draw(pts, 0);

	imshow("canvas", canvas);

	waitKey();
	return 0;
}
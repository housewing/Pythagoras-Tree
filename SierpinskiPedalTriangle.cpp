#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;

Mat canvas;

float getUintVec(Point2f vec)
{
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

float getScalar(Point2f vec1, Point2f vec2)
{
	return (vec1.x * vec2.x + vec1.y * vec2.y) / std::sqrt(vec1.x * vec1.x + vec1.y * vec1.y);
}

void draw(std::vector<Point> _points, int _level)
{
	Point2f vecA1, vecA2, vecB1, vecB2, vecC1, vecC2;
	vecA1.x = _points[2].x - _points[0].x;
	vecA1.y = _points[2].y - _points[0].y;

	vecA2.x = _points[2].x - _points[1].x;
	vecA2.y = _points[2].y - _points[1].y;

	vecB1.x = _points[0].x - _points[1].x;
	vecB1.y = _points[0].y - _points[1].y;

	vecB2.x = _points[0].x - _points[2].x;
	vecB2.y = _points[0].y - _points[2].y;

	vecC1.x = _points[1].x - _points[2].x;
	vecC1.y = _points[1].y - _points[2].y;

	vecC2.x = _points[1].x - _points[0].x;
	vecC2.y = _points[1].y - _points[0].y;

	std::vector<std::vector<Point>> points(3);
	
	float scalarA = getScalar(vecA1, vecA2);
	float scalarB = getScalar(vecB1, vecB2);
	float scalarC = getScalar(vecC1, vecC2);
	//std::cout << "scalarA " << scalarA << " scalarB " << scalarB << " scalarC " << scalarC << std::endl;

	Point2f uintA, uintB, uintC;
	uintA.x = vecA1.x / getUintVec(vecA1);
	uintA.y = vecA1.y / getUintVec(vecA1);

	uintB.x = vecB1.x / getUintVec(vecB1);
	uintB.y = vecB1.y / getUintVec(vecB1);

	uintC.x = vecC1.x / getUintVec(vecC1);
	uintC.y = vecC1.y / getUintVec(vecC1);
	//std::cout << "uintA " << uintA << " uintB " << uintB << " uintC " << uintC << std::endl;

	Point2f p1, p2, p3;
	p1.x = _points[2].x + uintA.x * scalarA * -1;
	p1.y = _points[2].y + uintA.y * scalarA * -1;

	p2.x = _points[0].x + uintB.x * scalarB * -1;
	p2.y = _points[0].y + uintB.y * scalarB * -1;

	p3.x = _points[1].x + uintC.x * scalarC * -1;
	p3.y = _points[1].y + uintC.y * scalarC * -1;
	//std::cout << "p1 " << p1 << " p2 " << p2 << " p3 " << p3 << std::endl;

	points[0].push_back(_points[0]);
	points[0].push_back(p2);
	points[0].push_back(p1);

	points[1].push_back(p2);
	points[1].push_back(_points[1]);
	points[1].push_back(p3);

	points[2].push_back(p1);
	points[2].push_back(p3);
	points[2].push_back(_points[2]);

	const Point* d[1] = { &_points[0] };
	const Point* a[1] = { &points[0][0] };
	const Point* b[1] = { &points[1][0] };
	const Point* c[1] = { &points[2][0] };
	int npt[] = { 3 };

	fillPoly(canvas, d, npt, 1, Scalar(0, 0, 0), 8);
	fillPoly(canvas, a, npt, 1, Scalar(0, 255, 0), 8);
	fillPoly(canvas, b, npt, 1, Scalar(0, 255, 0), 8);
	fillPoly(canvas, c, npt, 1, Scalar(0, 255, 0), 8);

	if (_level < 3)
	{
		draw(points[0], _level + 1);
		draw(points[1], _level + 1);
		draw(points[2], _level + 1);
	}
}

int main(int argc, char** argv)
{
	canvas = Mat::zeros(480, 640, CV_8UC3);

	std::vector<Point> pts;
	pts.push_back(Point(120, 440));
	pts.push_back(Point(520, 440));
	pts.push_back(Point(450, 40));

	//for (auto& p : pts)
	//{
	//	circle(canvas, p, 1, Scalar(0, 255, 0), 2, 8, 0);
	//}

	draw(pts, 0);
	
	imshow("canvas", canvas);

	waitKey();
	return 0;
}
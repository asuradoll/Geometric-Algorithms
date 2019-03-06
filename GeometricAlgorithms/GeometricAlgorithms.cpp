// GeometricAlgorithms.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include "ConvexHull.h"
#include "Polygon.h"

/*

bool isLinesIntersecting(cv::Point a, cv::Point b, cv::Point c, cv::Point d)
{
	//we check the condition for non-intersection
	bool firstCombo = ((computeSignOfOrientationTest(a, b, c) == computeSignOfOrientationTest(a, b, d)) && (computeSignOfOrientationTest(c, d, a) != computeSignOfOrientationTest(c, d, b)));
	bool secondCombo = ((computeSignOfOrientationTest(a, b, c) != computeSignOfOrientationTest(a, b, d)) && (computeSignOfOrientationTest(c, d, a) == computeSignOfOrientationTest(c, d, b)));
	// we return the not of the answe because we look for intersection
	return !(firstCombo || secondCombo);
}*/


int main()
{
	
	/*std::vector<cv::Point> test({ cv::Point(30, 20), cv::Point(0, 10), cv::Point(5, 0),  cv::Point(10, 0),  cv::Point(15, 5), cv::Point(15, 20), cv::Point(20, 20) });
	Polygon P(test);
	std::vector<cv::Point> poly = P.ConnectCCW(test);*/

	Polygon P;
	std::vector<cv::Point> poly = P.GenerateRandom(10, 50, 50);
	cv::Mat img = P.DrawPolygon(poly);

	ConvexHull ch;
	/*std::vector<cv::Point> CH = ch.GrahamsScan(test);
	cv::Mat img = ch.DrawConvex(CH);*/
	//cv::Mat img = ch.DrawConvex(poly);

	cv::imshow("img", img);
	cv::waitKey();

	return 0;
}


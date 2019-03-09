// GeometricAlgorithms.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include "ConvexHull.h"
#include "Polygon.h"
#include "OrientationTest.h"



int main()
{
	Polygon P;
	std::vector<cv::Point> poly = P.GenerateRandom(36, 500, 500);
	cv::Mat imgPolygon = P.DrawPolygon(poly);

	
	ConvexHull ch;
	std::vector<cv::Point> CH = ch.GrahamsScan(poly, imgPolygon);
	cv::Mat imgCH = ch.DrawConvex(CH);
	cv::Mat res;
	cv::hconcat(imgPolygon, imgCH, res);

	/*std::vector<cv::Point> CH2 = ch.JarvisMarch(poly, imgPolygon);
	cv::Mat imgCH2 = ch.DrawConvex(CH2);
	cv::Mat res2;
	cv::hconcat(imgPolygon, imgCH2, res2);*/

	std::vector<cv::Point> CH2 = ch.ChansAlgorithm(poly, imgPolygon);
	cv::Mat imgCH2 = ch.DrawConvex(CH2);
	cv::Mat res2;
	cv::hconcat(imgPolygon, imgCH2, res2);


	cv::Point q(0, 1);
	//int m = ch.FindRightTangent(CH, q);
	int m = ch.FindMaximalDotProduct(CH, q, imgCH);
	cv::Mat imgTangent = ch.DrawConvexAndQueryPoint(CH, q, m);

	//cv::imshow("polygon vs. convex hull", res);
	cv::imshow("Right tangent", imgTangent);
	cv::waitKey();

	return 0;
}


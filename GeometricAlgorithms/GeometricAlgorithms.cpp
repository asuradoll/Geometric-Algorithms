// GeometricAlgorithms.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <time.h> 
#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include "ConvexHull.h"
#include "Polygon.h"
#include "OrientationTest.h"
#include <fstream>
#include <chrono>


std::vector<cv::Point2f> GenerateRandonPoint2fs(int n, int xRange, int yRange);
cv::Mat DrawRandom(std::vector<cv::Point2f> Point2fs);
std::vector<cv::Point2f> ParseKoljasDoc();

void Stuff()
{
	Polygon P;
	std::vector<cv::Point2f> poly = P.GenerateRandom(36, 500, 500);
	cv::Mat imgPolygon = P.DrawPolygon(poly);


	ConvexHull ch;
	std::vector<cv::Point2f> CH = ch.GrahamsScan(poly, imgPolygon);
	cv::Mat imgCH = ch.DrawConvex(CH);
	cv::Mat res;
	cv::hconcat(imgPolygon, imgCH, res);

	/*std::vector<cv::Point2f> CH2 = ch.JarvisMarch(poly, imgPolygon);
	cv::Mat imgCH2 = ch.DrawConvex(CH2);
	cv::Mat res2;
	cv::hconcat(imgPolygon, imgCH2, res2);*/

	std::vector<cv::Point2f> CH2 = ch.ChansAlgorithm(poly, imgPolygon);
	cv::Mat imgCH2 = ch.DrawConvex(CH2);
	cv::Mat res2;
	cv::hconcat(imgPolygon, imgCH2, res2);
	cv::imshow("Random Polygon -> Chans Convex Hull", res2);

	cv::Point2f q(0, 1);
	//int m = ch.FindRightTangent(CH, q);
	int m = ch.FindMaximalDotProduct(CH, q, imgCH);
	cv::Mat imgTangent = ch.DrawConvexAndQueryPoint2f(CH, q, m);

	//cv::imshow("polygon vs. convex hull", res);
	cv::imshow("Right tangent", imgTangent);
	cv::waitKey();
}

void TestPolygonConstructionSpeed()
{
	int size = 200000;
	std::cout << "generating random Point2fs..." << std::endl;
	auto start1 = std::chrono::steady_clock::now();

	std::vector<cv::Point2f> randomPoint2fs = GenerateRandonPoint2fs(size, 1000000, 1000000);
	auto end1 = std::chrono::steady_clock::now();
	auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
	std::cout << duration1.count() << std::endl;

	std::cout << "connecting random Point2fs..." << std::endl;
	Polygon p;
	auto start = std::chrono::steady_clock::now();
	std::vector<cv::Point2f> CCWPoint2fs = p.ConnectCCW(randomPoint2fs);
	auto end = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << duration.count() << std::endl;
}

void TestRightTangentSpeed()
{
	std::cout << "reading Point2fs from file..." << std::endl;
	std::vector<cv::Point2f> convexPoint2fs = ParseKoljasDoc();
	ConvexHull ch;
	cv::Point2f q(0, 0);

	std::cout << "computing right tangent..." << std::endl;

	auto start = std::chrono::steady_clock::now();
	int tanInd = ch.FindRightTangent(convexPoint2fs, q);
	auto end = std::chrono::steady_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << duration.count() << std::endl;
}

void TestRightTangentCorrectness()
{
	std::vector<cv::Point2f> randomPoint2fs = GenerateRandonPoint2fs(100, 1000, 1000);
	Polygon p;
	std::vector<cv::Point2f> CCWPoint2fs = p.ConnectCCW(randomPoint2fs);
	cv::Mat polyImg = p.DrawPolygon(CCWPoint2fs);

	ConvexHull ch;
	std::vector<cv::Point2f> convexPoint2fs = ch.GrahamsScan(CCWPoint2fs);
	cv::Mat convImg = ch.DrawConvex(convexPoint2fs);

	cv::Point2f q(0, 0);
	int ind = ch.FindRightTangent(convexPoint2fs, q);
	cv::Mat tanImg = ch.DrawConvexAndQueryPoint2f(convexPoint2fs, q, ind);

	std::cout << "done." << std::endl;
}

void TestPolygonConstructionCorrectness()
{
	std::vector<cv::Point2f> randomPoint2fs = GenerateRandonPoint2fs(100, 1000, 1000);
	cv::Mat randomImg = DrawRandom(randomPoint2fs);

	Polygon p;
	std::vector<cv::Point2f> CCWPoint2fs = p.ConnectCCW(randomPoint2fs);
	cv::Mat polyImg = p.DrawPolygon(CCWPoint2fs);

	std::cout << "done." << std::endl;
}

void TestChan()
{
	std::vector<cv::Point2f> randomPoint2fs = GenerateRandonPoint2fs(100, 1000, 1000);
	Polygon p;
	std::vector<cv::Point2f> CCWPoint2fs = p.ConnectCCW(randomPoint2fs);
	cv::Mat polyImg = p.DrawPolygon(CCWPoint2fs);

	ConvexHull ch;
	std::vector<cv::Point2f> convexPoint2fs = ch.ChansAlgorithm(CCWPoint2fs, polyImg);
	cv::Mat convImg = ch.DrawConvex(convexPoint2fs);

	std::cout << "done." << std::endl;
}

int main()
{
	//TestPolygonConstructionSpeed();
	//TestRightTangentSpeed();
	//TestRightTangentCorrectness();
	// TestPolygonConstructionCorrectness();
	TestChan();

	return 0;
}



std::vector<cv::Point2f> ParseKoljasDoc()
{
	std::ifstream inFile;
	inFile.open("convex_1000.txt", std::ios::in);
	std::vector<cv::Point2f> convexPoint2fs;

	std::string::size_type sz;
	if (inFile.is_open()) 
	{
		std::string line;
		while (getline(inFile, line)) 
		{
			//printf("%s", line.c_str());
			std::size_t pos = line.find(' ');
			std::string yStr = line.substr(pos + 1);
			std::string xStr = line.substr(0, pos);
			//std::cout << "x: " << xStr << " y: " << yStr << std::endl;
			float x = std::stof(xStr, &sz) * 100000.0f;
			float y = std::stof(yStr, &sz) * 100000.0f;
			convexPoint2fs.push_back(cv::Point2f(x, y));
		}
		inFile.close();
	}

	return convexPoint2fs;
}


cv::Mat DrawRandom(std::vector<cv::Point2f> Point2fs)
{
	cv::Rect rect = cv::boundingRect(Point2fs);
	cv::Mat img = cv::Mat::zeros(cv::Size(rect.x + rect.width + 10, rect.y + rect.height + 10), CV_8UC3);
	for (int i = 0; i < Point2fs.size(); ++i)
	{
		cv::Point2f p1 = Point2fs[i] + cv::Point2f(5, 5);
		cv::Point2f p2 = Point2fs[(i + 1) % Point2fs.size()] + cv::Point2f(5, 5);
		cv::circle(img, p1, 3, cv::Scalar(0, 0, 255));
		//cv::line(img, p1, p2, cv::Scalar(0, 255, 0));
	}

	cv::flip(img, img, 0);

	return img;
}


std::vector<cv::Point2f> GenerateRandonPoint2fs(int n, int xRange, int yRange)
{
	std::vector<cv::Point2f> P;
	srand(time(NULL));

	for (int i = 0; i < n; ++i)
	{
		int x = rand() % xRange;
		int y = rand() % yRange;
		cv::Point2f p(x, y);
		if (P.end() == std::find(P.begin(), P.end(), p))
		{
			P.push_back(p);
		}
	}

	return P;
}


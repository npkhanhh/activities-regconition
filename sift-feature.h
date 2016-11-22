#pragma once
#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
#include <nonfree/features2d.hpp>
#include <cv.h>
#include <opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
using namespace std;
using namespace cv;

#define width 320
#define height 240
struct TWindow2D
{
	double l, r, b, t;
};

struct TViewport2D
{
	int l, r, b, t;
};

int sift_feature();

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <random>
#include <iostream>
#include <fstream>
#include <math.h>


const int dim = 64000; //number of dimension

const int PTS = 3965; //number of point
const int K = 11;     //number of group
using namespace std;
typedef struct { int idx; int group; } point_t, *point;
typedef struct { float v[dim]; int group; } point_tc, *point_center;

int kmean();

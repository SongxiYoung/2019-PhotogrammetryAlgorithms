#ifndef MATCHING_H
#define MATCHING_H
#include<vector>
#include<math.h>
#include<stdio.h>
#include<Windows.h>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;

struct MATCHEDPOINT
{
    double coefficient;
    int x1;
    int y1;
    int x2;
    int y2;
};

class Matching
{
public:
    Matching();
    void coeffi(vector<POINT>* lPoint, vector<POINT>* rPoint, MatrixXi& lsrc, MatrixXi& rsrc, int m_window, double ReCoThresh);

    vector<MATCHEDPOINT>  Points_info;
};

#endif // MATCHING_H

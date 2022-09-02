#ifndef OPERATOR_H
#define OPERATOR_H
#include<vector>
#include<math.h>
#include<stdio.h>
#include<Windows.h>
#include <Eigen/Dense>
using namespace Eigen;

using namespace std;

typedef unsigned char uchar;

class EXoperator
{
public:
    EXoperator();
    void Moravec(MatrixXi& src, int kernal, float threshold, vector<POINT>* corPoint);
    void Harris(MatrixXd& src, int kernal, float threshold, vector<POINT>* corPoint);
//    MatrixXd conv(MatrixXd& src, MatrixXd& ker, int size);


    MatrixXd dst;
    vector<POINT>cornerPoint;
    vector<POINT>::iterator itr;


};

#endif // OPERATOR_H

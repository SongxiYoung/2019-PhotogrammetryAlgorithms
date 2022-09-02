#ifndef RESECTION_H
#define RESECTION_H

#include <Eigen/Dense>
using namespace Eigen;

class Resection      //空间后方交会，求外方位元素
{
public:
    Resection();
    MatrixXf WrapMat();
    void GetR(MatrixXf exterior);    //计算旋转矩阵
    void GetA();    //计算A矩阵
    void GetL();    //计算L矩阵
    void GetDelta(MatrixXf A, MatrixXf L);    //计算改正数

    MatrixXf wrap = MatrixXf::Zero(2,3);    //仿射变换矩阵
    MatrixXf five_coordinate = MatrixXf::Zero(4,5);       //xyXYZ
    MatrixXf interior = MatrixXf::Zero(1,3);          //内方位元素
    MatrixXf exterior = MatrixXf::Zero(1,6);          //外方位元素
    int row;

    MatrixXf Amatrix;           //A矩阵
    MatrixXf Lmatrix;           //L矩阵
    MatrixXf Delta = MatrixXf::Zero(6,1);                                 //改正数
    MatrixXf rotate = MatrixXf::Zero(3,3);            //旋转矩阵

};

#endif // RESECTION_H

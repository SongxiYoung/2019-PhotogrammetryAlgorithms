#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <Eigen/Dense>
using namespace Eigen;

class Intersection        //空间前方交会，求地面摄影坐标
{
public:
    Intersection();
    void GetR();                     //计算旋转矩阵
    void GetAssistCoor();            //计算摄影测量坐标

    MatrixXf coordinate = MatrixXf::Zero(9,4);        //左片xy+右片xy
    MatrixXf Final_coordinate = MatrixXf::Zero(9,3);        //XYZ

    MatrixXf L_wrap = MatrixXf::Zero(2,3);    //仿射变换矩阵
    MatrixXf R_wrap = MatrixXf::Zero(2,3);    //仿射变换矩阵

    MatrixXf L_exterior = MatrixXf::Zero(1,6);          //左片外方位元素
    MatrixXf R_exterior = MatrixXf::Zero(1,6);          //右片外方位元素

    MatrixXf interior = MatrixXf::Zero(1,3);          //内方位元素

    MatrixXf L_rotate = MatrixXf::Zero(3,3);            //左片旋转矩阵
    MatrixXf R_rotate = MatrixXf::Zero(3,3);            //右片旋转矩阵
};

#endif // INTERSECTION_H

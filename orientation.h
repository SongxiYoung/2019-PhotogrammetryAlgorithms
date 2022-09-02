#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <Eigen/Dense>
using namespace Eigen;

class orientation
{
public:
    orientation();
    void RelaOrientation(double fo); //相对定向
    void SinRelaOri(double fo);
    MatrixXf GetR(float f, float w, float k);  //计算旋转矩阵
    MatrixXf GetData();


    MatrixXf L_coor = MatrixXf::Zero(20,2);//左片数据
    MatrixXf R_coor = MatrixXf::Zero(20,2);//右片数据

private:
    //相对定向五参数
    MatrixXf ori_data = MatrixXf::Zero(1,5);  //f,w,k,u,v or f1,f2,w2,k1,k2
    int countx = 0; //相对定向循环次数

    float df; //焦距
    float bx;
    float by;
    float bz;

    float N1;
    float N2;

    double u1, v1, w1, u2, v2, w2;//单独法

};

#endif // ORIENTATION_H

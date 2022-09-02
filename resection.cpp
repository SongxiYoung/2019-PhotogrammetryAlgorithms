#include "resection.h"
#include <Eigen/Dense>
using namespace Eigen;

Resection::Resection()
{

}

MatrixXf Resection::WrapMat()
{
    wrap(0,0) = -114.4029;
    wrap(0,1) = 0.100029;
    wrap(0,2) = -0.001103;
    wrap(1,0) = 116.5334;
    wrap(1,1) = -0.001104;
    wrap(1,2) = -0.100014;
    for (int i=0; i<4; i++)
    {
        float tempx = wrap(0,1)*five_coordinate(i, 0) + wrap(0,2)*five_coordinate(i, 1) + wrap(0,0);
        float tempy = wrap(1,1)*five_coordinate(i, 0) + wrap(1,2)*five_coordinate(i, 1) + wrap(1,0);
        five_coordinate(i, 0) = tempx;
        five_coordinate(i, 1) = tempy;
    }
    return five_coordinate;
}

void Resection::GetR(MatrixXf exterior)   //计算旋转矩阵
{
//  float XS = exterior(0, 0);
//  float YS = exterior(0, 1);
//  float ZS = exterior(0, 2);
    float w = exterior(0, 4);
    float k = exterior(0, 5);
    float q = exterior(0, 3);

    rotate(0,0) = cos(q)*cos(k) - sin(q)*sin(w)*sin(k);     //a1
    rotate(0,1) = -cos(q)*sin(k) - sin(q)*sin(w)*cos(k);    //a2
    rotate(0,2) = -sin(q)*cos(w);                           //a3
    rotate(1,0) = cos(w)*sin(k);                            //b1
    rotate(1,1) = cos(w)*cos(k);                            //b2
    rotate(1,2) = -sin(w);                                  //b3
    rotate(2,0) = sin(q)*cos(k) + cos(q)*sin(w)*sin(k);     //c1
    rotate(2,1) = -sin(q)*sin(k) + cos(q)*sin(w)*cos(k);    //c2
    rotate(2,2) = cos(q)*cos(w);                            //c3
}

void Resection::GetA()    //计算A矩阵
{
    float XS = exterior(0, 0);
    float YS = exterior(0, 1);
    float ZS = exterior(0, 2);
    float w = exterior(0, 4);
    float k = exterior(0, 5);
//  float q = exterior(0, 3);

    GetR(exterior);

  //  interior(0,2) = interior(0,2) / 1000;

    for (int i = 0; i < 4; i++)
        {
            float ZA = five_coordinate(i, 4);
            float XA = five_coordinate(i, 2);
            float YA = five_coordinate(i, 3);
            float x = (five_coordinate(i, 0)-interior(0,0))/1000;
            float y = (five_coordinate(i, 1)-interior(0,1))/1000;
            float _Z = rotate(0,2)*(XA - XS) + rotate(1,2)*(YA - YS) + rotate(2,2)*(ZA - ZS);

            Amatrix(2*i, 0) = (rotate(0,0)*interior(0,2) + rotate(0,2)*x) / _Z;
            Amatrix(2*i, 1) = (rotate(1,0)*interior(0,2) + rotate(1,2)*x) / _Z;
            Amatrix(2*i, 2) = (rotate(2,0)*interior(0,2) + rotate(2,2)*x) / _Z;
            Amatrix(2*i, 3) = y*sin(w) - (x*(x*cos(k) - y*sin(k)) / interior(0,2) + interior(0,2)*cos(k))*cos(w);
            Amatrix(2*i, 4) = -interior(0,2)*sin(k) - x*(x*sin(k) + y*cos(k)) / interior(0,2);
            Amatrix(2*i, 5) = y;
            Amatrix(2*i+1, 0) = (rotate(0,1)*interior(0,2) + rotate(0,2)*y) / _Z;
            Amatrix(2*i+1, 1) = (rotate(1,1)*interior(0,2) + rotate(1,2)*y) / _Z;
            Amatrix(2*i+1, 2) = (rotate(2,1)*interior(0,2) + rotate(2,2)*y) / _Z;
            Amatrix(2*i+1, 3) = -x*sin(w) - (y*(x*cos(k) - y*sin(k)) / interior(0,2) - interior(0,2)*sin(k))*cos(w);
            Amatrix(2*i+1, 4) = -interior(0,2)*cos(k) - y/ interior(0,2)*(x*sin(k) + y*cos(k));
            Amatrix(2*i+1, 5) = -x;
        }
}

void Resection::GetL()    //计算L矩阵
{
    float XS = exterior(0, 0);
    float YS = exterior(0, 1);
    float ZS = exterior(0, 2);
//  float w = exterior(0, 4);
//  float k = exterior(0, 5);
//  float q = exterior(0, 3);

    GetR(exterior);

 //   interior(0,2) = interior(0,2) / 1000;

    for (int i = 0; i < row; i++)
        {
            float ZA = five_coordinate(i, 4);
            float XA = five_coordinate(i, 2);
            float YA = five_coordinate(i, 3);
            float x = (five_coordinate(i, 0) - interior(0,0)) / 1000;
            float y = (five_coordinate(i, 1) - interior(0,1)) / 1000;
            interior(0,0) = -interior(0,2)*(rotate(0,0)*(XA - XS) + rotate(1,0)*(YA - YS) + rotate(2,0)*(ZA - ZS)) / (rotate(0,2)*(XA - XS) + rotate(1,2)*(YA - YS) + rotate(2,2)*(ZA - ZS));
            interior(0,1) = -interior(0,2)*(rotate(0,1)*(XA - XS) + rotate(1,1)*(YA - YS) + rotate(2,1)*(ZA - ZS)) / (rotate(0,2)*(XA - XS) + rotate(1,2)*(YA - YS) + rotate(2,2)*(ZA - ZS));

            Lmatrix(2*i, 0) =x - interior(0,0);
            Lmatrix(2*i+1, 0) =y - interior(0,1);
        }
}

void Resection::GetDelta(MatrixXf A, MatrixXf L)      //计算改正数
{
    MatrixXf A_T,A_;
    A_T = A.transpose();
    A_ = (A_T*A).inverse();
    Delta = A_*A_T*L;
}

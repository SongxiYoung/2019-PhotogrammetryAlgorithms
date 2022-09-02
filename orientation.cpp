#include "orientation.h"
#include <iostream>
using namespace std;
#include <Eigen/Dense>
using namespace Eigen;


orientation::orientation()
{

}

MatrixXf orientation::GetR(float f, float w, float k)
{
    MatrixXf rotate = MatrixXf::Zero(3,3);//旋转矩阵
    rotate(0,0) = cos(f)*cos(k) - sin(f)*sin(w)*sin(k);     //a1
    rotate(0,1) = -cos(f)*sin(k) - sin(f)*sin(w)*cos(k);    //a2
    rotate(0,2) = -sin(f)*cos(w);                           //a3
    rotate(1,0) = cos(w)*sin(k);                            //b1
    rotate(1,1) = cos(w)*cos(k);                            //b2
    rotate(1,2) = -sin(w);                                  //b3
    rotate(2,0) = sin(f)*cos(k) + cos(f)*sin(w)*sin(k);     //c1
    rotate(2,1) = -sin(f)*sin(k) + cos(f)*sin(w)*cos(k);    //c2
    rotate(2,2) = cos(f)*cos(w);                            //c3
    return rotate;

}

void orientation::RelaOrientation(double fo)
{
    //误差方程参数
        MatrixXf A(20, 5), L(20, 1), X(5, 1), V(20, 1);

        //右片相对相空间坐标，相对摄影测量坐标
        MatrixXf mr(3, 1), mrimg(3, 1);

        bx = L_coor(0,0) - R_coor(0,0); //bx初始值
        df = fo;
        MatrixXf rotate = MatrixXf::Zero(3,3);//旋转矩阵

        //迭代运算
        while (1)
        {
            //计算旋转矩阵
            rotate = GetR(ori_data(0,0), ori_data(0,1), ori_data(0,2));

            //by,bz计算
            by = bx*ori_data(0,3);
            bz = bx*ori_data(0,4);

            //计算每个点参数，组成法方程矩阵
            for (int i = 0; i < 20; i++)
            {
                //左片相对摄影测量坐标
                float x1 = L_coor(i,0);
                float y1 = L_coor(i,1);
                float z1 = -df;

                //右片相对摄影测量坐标
                mr(0, 0) = R_coor(i,0);
                mr(1, 0) = R_coor(i,1);
                mr(2, 0) = -df;
                //计算相对摄影测量坐标
                mrimg = rotate*mr;

                N1 = (bx*mrimg(2, 0)- bz*mrimg(0, 0)) / (x1*mrimg(2, 0) - mrimg(0, 0)*z1);
                N2 = (bx*z1 - bz*x1) / (x1*mrimg(2, 0) - mrimg(0, 0)*z1);
                //计算每个点的Q值
                float Q = N1*y1 - N2*mrimg(1, 0) - by;
                //计算每个点误差系数
                MatrixXf v = MatrixXf::Zero(5,1);
                v(0,0) = -mrimg(0, 0)*mrimg(1, 0)*N2 / mrimg(2, 0);
                v(1,0) = -(mrimg(2, 0) + mrimg(1, 0)*mrimg(1, 0) / mrimg(2, 0))*N2;
                v(2,0) = mrimg(0, 0)*N2;
                v(3,0) = bx;
                v(4,0) = -mrimg(1, 0)*bx / mrimg(2, 0);

                //加入总误差系数阵
                for (int j = 0; j < 5; j++)
                    A(i, j) = v(j,0);
                L(i, 0) = Q;
            }
            //求解X
            MatrixXf A_T,A_;
            A_T = A.transpose();
            A_ = (A_T*A).inverse();
            X = A_*A_T*L;

            //累加五参数
            ori_data(0,0) += X(0, 0);
            ori_data(0,1) += X(1, 0);
            ori_data(0,2) += X(2, 0);
            ori_data(0,3) += X(3, 0);
            ori_data(0,4) += X(4, 0);

            //循环次数＋
            countx++;
//            cout << countx << endl;
//            cout << ori_data(0,0) << " " << ori_data(0,1) << " " << ori_data(0,2) << " " << ori_data(0,3) << "  " << ori_data(0,4)<< endl;

            //判断是否收敛
            if (abs(X(0, 0)) < 0.00003 && abs(X(1, 0)) < 0.00003 && abs(X(2, 0)) < 0.00003 && abs(X(3, 0)) < 0.00003 && abs(X(4, 0)) < 0.00003  || countx >=20)
            {
                //输出五参数
                cout << "f w k u v:" << endl;
                cout << ori_data(0,0) << " " << ori_data(0,1) << " " << ori_data(0,2) << " " << ori_data(0,3) << "  " << ori_data(0,4)<< endl;
                //评定精度
                V = A*X - L;
                float c1 = sqrt((V.transpose()*V)(0, 0) / 6);
                cout << "相对定向精度:" << c1 << endl;
                cout << "相对定向迭代次数:" << countx << endl;
                break;
            }
        }

}

MatrixXf orientation::GetData()
{
    return ori_data;
}

void orientation::SinRelaOri(double fo)
{
    //误差方程参数
    MatrixXf A(20, 5), L(20, 1), X(5, 1), V(20, 1);

    //左右片相对摄影测量坐标
    MatrixXf ml(3, 1), mr(3, 1);

    df = fo;
    MatrixXf lrotate = MatrixXf::Zero(3,3);//旋转矩阵
    MatrixXf rrotate = MatrixXf::Zero(3,3);

    //迭代运算
    while (1)
    {
        //计算旋转矩阵
        lrotate = GetR(ori_data(0,0), 0, ori_data(0,1));
        rrotate = GetR(ori_data(0,2), ori_data(0,3), ori_data(0,4));

        //计算每个点参数，组成法方程矩阵
        for (int i = 0; i < 20; i++)
        {
            MatrixXf tml(3, 1), tmr(3, 1);
            //左片
            tml(0,0) = L_coor(i,0);
            tml(1,0) = L_coor(i,1);
            tml(2,0) = -df;
            //右片
            tmr(0,0) = R_coor(i,0);
            tmr(1,0) = R_coor(i,1);
            tmr(2,0) = -df;
            //计算相对摄影测量坐标
            ml = lrotate*tml;
            mr = rrotate*tmr;

            A(i,0) = ml(0,0)*mr(1,0) / mr(2,0);
            A(i,1) = -mr(0,0)*ml(1,0) / ml(2,0);
            A(i,2) = df*(1 + ml(1,0)*mr(1,0) / (ml(2,0)*mr(2,0)));
            A(i,3) = -ml(0,0);
            A(i,4) = mr(0,0);
            L(i,0) = -df*ml(1,0) / ml(2,0) + df*mr(1,0) / mr(2,0);

        }
        //求解X
        MatrixXf A_T,A_;
        A_T = A.transpose();
        A_ = (A_T*A).inverse();
        X = A_*A_T*L;

        //累加五参数
        ori_data(0,0) += X(0, 0);
        ori_data(0,2) += X(1, 0);
        ori_data(0,3) += X(2, 0);
        ori_data(0,1) += X(3, 0);
        ori_data(0,4) += X(4, 0);

        //循环次数＋
        countx++;
        cout << countx << endl;
        cout << ori_data(0,0) << " " << ori_data(0,1) << " " << ori_data(0,2) << " " << ori_data(0,3) << "  " << ori_data(0,4)<< endl;

        //判断是否收敛
        if (abs(X(0, 0)) < 0.00003 && abs(X(1, 0)) < 0.00003 && abs(X(2, 0)) < 0.00003 && abs(X(3, 0)) < 0.00003 && abs(X(4, 0)) < 0.00003  || countx >=20)
        {
            //输出五参数
            cout << "f1,f2,w2,k1,k2:" << endl;
            cout << ori_data(0,0) << " " << ori_data(0,1) << " " << ori_data(0,2) << " " << ori_data(0,3) << "  " << ori_data(0,4)<< endl;
            cout << "相对定向迭代次数:" << countx << endl;
            break;
        }
    }


}

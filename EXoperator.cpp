#include "EXoperator.h"
#include<vector>
#include<math.h>
#include<stdio.h>
#include <Eigen/Dense>
#include <math.h>
#include <iostream>
using namespace std;
using namespace Eigen;

EXoperator::EXoperator()
{

}

void EXoperator::Moravec(MatrixXi& src, int kernal, float threshold, vector<POINT>* corPoint)
{
        int halfKernal = kernal / 2;
        float minValue;
        dst = MatrixXd(src.rows(),src.cols());
        //遍历图像
        for (int y = halfKernal; y < src.rows() - halfKernal; y++)
        {

            for (int x = halfKernal; x < src.cols() - halfKernal; x++)
            {
                MatrixXf moveValue = MatrixXf::Zero(1,4);
                //对图像的每一个点在 0°、45°、90°135°的变化量
                for (int win = -halfKernal; win < halfKernal; win++)
                {
                    moveValue(0,0) += pow(src(x + win, y) - src(x + win + 1, y), 2);//0°方向变化量
                    moveValue(0,1) += pow(src(x + win, y + win) - src(x + win + 1, y + win + 1), 2);//45°方向变化量
                    moveValue(0,2) += pow(src(x, y + win) - src(x, y + win + 1), 2);//90°方向变化量
                    moveValue(0,3) += pow(src(x - win, y + win) - src(x - win - 1, y + win + 1), 2);//135°方向变化量

                }
                //计算四个方向移动的最小值
                minValue = moveValue(0,0);
                minValue = minValue > moveValue(0,1) ? moveValue(0,1) : minValue;
                minValue = minValue > moveValue(0,2) ? moveValue(0,2) : minValue;
                minValue = minValue > moveValue(0,3) ? moveValue(0,3) : minValue;

                dst(x, y) = minValue;
          //      cout << x << " " << y << endl;

            }
        }
        //获取角点坐标
        float maxValue;
        int flag = 0;
        POINT maxLoc;
        float value;

        for (int y = halfKernal; y < src.rows() - halfKernal;)
        {
            for (int x = halfKernal; x < src.cols() - halfKernal;)
            {
                maxValue = 0;
                value = 0;
                flag = 0;
                maxLoc.x = -1;
                maxLoc.y = -1;
                //计算点（x,y）位中心的kernal的局部最大值。
                for (int winy = -halfKernal; winy <= halfKernal; winy++)
                {

                    for (int winx = -halfKernal; winx <= halfKernal; winx++)
                    {
                        value = dst(x + winx, y + winy);
                        if (value > maxValue)
                        {
                            maxValue = value;
                            maxLoc.x = x + winx;
                            maxLoc.y = y + winy;
                            flag = 1;
                        }

                    }

                }

                //阈值比较
                if (flag == 1 && (maxValue > threshold))
                {
                    //cout << maxLoc << endl;

                    corPoint->push_back(maxLoc);

                }
                x = x + kernal;
            }
            y = y + kernal;
        }

}

void EXoperator::Harris(MatrixXd &src, int kernal, float threshold, vector<POINT> *corPoint)
{
    //1.使用差分算子计算
    MatrixXd x_k = MatrixXd::Zero(3,3);    //水平、竖直差分算子
    x_k(0,0) = -1; x_k(0,1) = 0; x_k(0,2) = 1;
    x_k(1,0) = -1; x_k(1,1) = 0; x_k(1,2) = 1;
    x_k(2,0) = -1; x_k(2,1) = 0; x_k(2,2) = 1;
    MatrixXd y_k = MatrixXd::Zero(3,3);
    y_k(0,0) = -1; y_k(0,1) = -1; y_k(0,2) = -1;
    y_k(1,0) = 0; y_k(1,1) = 0; y_k(1,2) = 0;
    y_k(2,0) = 1; y_k(2,1) = 1; y_k(2,2) = 1;

    MatrixXd I_x = MatrixXd::Zero(src.rows(),src.cols());
    MatrixXd I_y = MatrixXd::Zero(src.rows(),src.cols());

  //  I_x = conv(src, x_k, 3);
    for (int y = 3/2; y < src.rows() - 3/2; y++)  //conv
    {
        for (int x = 3/2; x < src.cols() - 3/2; x++)
        {
            double sum1 = 0;
            double sum2 = 0;
            for(int i=0;i < 3; i++)
                for(int j=0; j < 3; j++)
                {
                    sum1 += src(y-3/2+i, x-3/2+j) * x_k(i, j);
                    sum2 += src(y-3/2+i, x-3/2+j) * y_k(i, j);
                }
            I_x(y, x) = sum1;
            I_y(y, x) = sum2;
        }
    }
  //  I_y = conv(src, y_k, 3);
    MatrixXd Ix2 = MatrixXd::Zero(src.rows(),src.cols()),
            Iy2 = MatrixXd::Zero(src.rows(),src.cols()),
            Ixy = MatrixXd::Zero(src.rows(),src.cols());
 //   Ix2 = I_x*I_x;
 //   Iy2 = I_y*I_y;
  //  Ixy = I_x*I_y;
   for (int y = 0; y < src.rows(); y++)  //conv
    {
        for (int x = 0; x < src.cols(); x++)
        {
            Ix2(y, x) = I_x(y, x)*I_x(y, x);
            Iy2(y, x) = I_y(y, x)*I_y(y, x);
            Ixy(y, x) = I_x(y, x)*I_y(y, x);
        }
    }
    cout << 1 << ".OK" << endl;

    //2.高斯平滑去噪
    MatrixXd gauss = MatrixXd::Zero(5,5);
    for(int i=0; i<5; i++)
        for(int j=0; j<5; j++)
            gauss(i, j)=exp(-((i-int(5/2))*(i-int(5/2))+(j-int(5/2))*(j-int(5/2)))/(2*0.8));
    MatrixXd gau_I_x = MatrixXd::Zero(src.rows(),src.cols());
    MatrixXd gau_I_y = MatrixXd::Zero(src.rows(),src.cols());
    MatrixXd gau_I_xy = MatrixXd::Zero(src.rows(),src.cols());
 //   gau_I_x = conv(Ix2, gauss,5);
  //  gau_I_y = conv(Iy2, gauss,5);
  //  gau_I_xy = conv(Ixy, gauss,5);
    for (int y = 5/2; y < src.rows() - 5/2; y++)        //gaussian
    {
        for (int x = 5/2; x < src.cols() - 5/2; x++)
        {
            double sum1 = 0;
            double sum2 = 0;
            double sum3 = 0;
            for(int i=0;i < 5; i++)
                for(int j=0; j < 5; j++)
                {
                    sum1 += Ix2(y-5/2+i, x-5/2+j) * gauss(i, j);
                    sum2 += Iy2(y-5/2+i, x-5/2+j) * gauss(i, j);
                    sum3 += Ixy(y-5/2+i, x-5/2+j) * gauss(i, j);
                }
            gau_I_x(y, x) = sum1;
            gau_I_y(y, x) = sum2;
            gau_I_xy(y, x) = sum3;
        }
    }

    MatrixXd I_x2 = MatrixXd::Zero(src.rows(),src.cols());
    MatrixXd I_y2 = MatrixXd::Zero(src.rows(),src.cols());
    MatrixXd I_xy = MatrixXd::Zero(src.rows(),src.cols());
    for (int y = 5/2; y < src.rows() - 5/2; y++)         //累加
    {
        for (int x = 5/2; x < src.cols() - 5/2; x++)
        {
            double sum1 = 0;
            double sum2 = 0;
            double sum3 = 0;
            for(int i=0;i < 5; i++)
                for(int j=0; j < 5; j++)
                {
                    sum1 += gau_I_x(y-5/2+i, x-5/2+j);
                    sum2 += gau_I_y(y-5/2+i, x-5/2+j);
                    sum3 += gau_I_xy(y-5/2+i, x-5/2+j);
                }
            I_x2(y, x) = sum1;
            I_y2(y, x) = sum2;
            I_xy(y, x) = sum3;
     //       cout << gau_I_x(y, x) << " ";
        }
 //       cout << endl;
    }
    cout << 2 << ".OK" << endl;

    //3.计算
    double max_res = 0;
    dst = MatrixXd::Zero(src.rows(),src.cols());
    for (int y = 3/2; y < src.rows()-3/2; y++)
    {
        for (int x = 3/2; x < src.cols()-3/2; x++)
        {
            double det = 0;
            double f_x = 0;
            double f_y = 0;
            double f_xy = 0;
            for(int i=0;i < 3; i++)
                for(int j=0; j < 3; j++)
                {
                    f_x += I_x2(y-3/2+i, x-3/2+j);
                    f_y += I_y2(y-3/2+i, x-3/2+j);
                    f_xy += I_xy(y-3/2+i, x-3/2+j);
                }
            det = f_x*f_y - f_xy*f_xy;
            dst(y, x) = det-0.04*(f_x+f_y)*(f_x+f_y);
            if(dst(y, x) > max_res)
                max_res = dst(y, x);
            cout << dst(y, x) << " ";
        }
        cout << endl;
    }
    cout << 3 << ".OK" << endl;


    //4.局部非极大值抑制
    MatrixXd tmp = MatrixXd::Zero(src.rows(),src.cols());
    for(int y = kernal/2; y < src.rows()-kernal/2; y++)
       for(int x = kernal/2; x < src.cols()-kernal/2; x++)
        {
           double max=2.22507e-308;
           for(int i=0;i < kernal; i+=kernal)
           {
               for(int j=0; j < kernal; j+=kernal)
                {
                    if(dst(y, x) > max)
                        max = dst(y, x);
                }
           }
           if(max>0)
                if(dst(y, x) > threshold*100000000000000 && dst(y, x) > 0.01*max_res)//然后大于这个阈值
                {
                    POINT maxLoc;
                    maxLoc.x = y;
                    maxLoc.y = x;//满足上两个条件，才是角点！
                    corPoint->push_back(maxLoc);
                }
            }
    cout << 4 << ".OK" << endl;

}
/*
MatrixXd EXoperator::conv(MatrixXd& src, MatrixXd& ker, int size)
{
    MatrixXd dst = MatrixXd(src.rows(),src.cols());
    for (int y = size/2; y < src.rows() - size/2; y++)
    {
        for (int x = size/2; x < src.cols() - size/2; x++)
        {
            double sum = 0;
            for(int i=0;i < size; i++)
                for(int j=0; j < size; j++)
                {
                    sum += src(y-size/2+i, x-size/2+j) * ker(i, j);
                }
            dst(x, y) = sum;
        }
    }
    return dst;
}
*/

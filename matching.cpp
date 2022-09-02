#include "matching.h"
#include <iostream>
using namespace std;

Matching::Matching()
{

}



void Matching::coeffi(vector<POINT>* lPoint, vector<POINT>* rPoint, MatrixXi& lsrc, MatrixXi& rsrc, int m_window, double ReCoThresh)
{
    cout << "the size of lpoint:" << lPoint->size() << endl;
    cout << "the size of rpoint:" << rPoint->size() << endl;
    int lMaxRow = 0;
    int lMaxCol = 0;

    for (vector<POINT>::iterator litr = lPoint->begin(); litr < lPoint->end(); litr++)
    {
        //计算左片该点对应的特征点
        double lpix_sum = 0.0;
        double lpix_sum_squ = 0.0;
        double R = 0.0;
        double MaxR = 0.0;

        if (litr->x < int(m_window/2) || litr->y < int(m_window/2) || litr->x > lsrc.rows() - int(m_window/2) || litr->y > lsrc.cols() - int(m_window/2))
        {
            continue;
        }

        //逐个搜索
        //1.将左片小窗口内的数据拷贝//可先求出灰度值和,平方和
        for (int i = -int(m_window/2); i < int(m_window/2); ++i)
        {
            for(int j = -int(m_window/2); j < int(m_window/2) ; ++j)
            {
            //lpix_sum = 小窗口所有像素灰度值求和
            //lpix_sum_squ = ....................的平方再求和
               lpix_sum += lsrc(j+litr->x, (i+litr->y));
               lpix_sum_squ += lsrc(j+litr->x, (i+litr->y)) * lsrc(j+litr->x, (i+litr->y));
            }
        }

        //2.逐个小窗口计算R
        for (vector<POINT>::iterator ritr = rPoint->begin(); ritr != rPoint->end(); ++ritr)
        {
            if (ritr->x < int(m_window/2) || ritr->y < int(m_window/2) || ritr->x > rsrc.rows() - int(m_window/2) || ritr->y > rsrc.cols() - int(m_window/2))
            {
                continue;
            }


            double pro_sum = 0.0;
            double rpix_sum = 0.0;
            double rpix_sum_squ = 0.0;
            int j = ritr->x;	//i,j为中心点
            int i = ritr->y;
            j -= int(m_window/2);			//i,j为顶点
            i -= int(m_window/2);
            int centerX = j + int(m_window/2);
            int centerY = i + int(m_window/2);
            //这里的centerX，centerY就是右片某个小窗口的中心
            //求参数
            for (int m = -int(m_window/2); m < int(m_window/2); ++m)
            {
                    for(int n = -int(m_window/2); n < int(m_window/2) ; ++n)
                    {
                        //rpix_sum = 小窗口所有像素灰度值求和
                        //rpix_sum_squ = ........................平方的求和
                        //pro_sum = 对应位置像素灰度直乘积之和

                        rpix_sum += rsrc(j+n, i+m);
                        rpix_sum_squ += rsrc(j+n, i+m) * rsrc(j+n, i+m);
                        pro_sum += rsrc(j+n, i+m) * lsrc(n+litr->x, m+litr->y);
                    }
            }

            double temp1 = lpix_sum_squ - lpix_sum*lpix_sum/m_window/m_window;
            double temp2 = rpix_sum_squ - rpix_sum*rpix_sum/m_window/m_window;
            if (temp1 != 0.0 && temp2 != 0.0)
            {
                R = (pro_sum - lpix_sum*rpix_sum/m_window/m_window) / sqrt(temp1*temp2);
            }
            else
                R = 0;
            if (R > MaxR)
            {
                MaxR = R;
                lMaxRow = centerX;
                lMaxCol = centerY;
        //        cout << R << endl;
            }
        }
 //1       cout << lMaxRow << " " << lMaxCol << " " << endl;
        //3.是否超过阈值
        if (MaxR >= ReCoThresh)
        {
            MATCHEDPOINT mp;
            mp.coefficient = MaxR;
            mp.x1 = litr->x;
            mp.y1 = litr->y;
            mp.x2 = lMaxRow;
            mp.y2 = lMaxCol;
            Points_info.push_back(mp);
        }

    }

}

#include "intersection.h"
#include <iostream>
using namespace std;

Intersection::Intersection()
{

}

void Intersection::GetR()
{
    float w = L_exterior(0, 4);
    float k = L_exterior(0, 5);
    float q = L_exterior(0, 3);

    L_rotate(0,0) = cos(q)*cos(k) - sin(q)*sin(w)*sin(k);     //a1
    L_rotate(0,1) = -cos(q)*sin(k) - sin(q)*sin(w)*cos(k);    //a2
    L_rotate(0,2) = -sin(q)*cos(w);                           //a3
    L_rotate(1,0) = cos(w)*sin(k);                            //b1
    L_rotate(1,1) = cos(w)*cos(k);                            //b2
    L_rotate(1,2) = -sin(w);                                  //b3
    L_rotate(2,0) = sin(q)*cos(k) + cos(q)*sin(w)*sin(k);     //c1
    L_rotate(2,1) = -sin(q)*sin(k) + cos(q)*sin(w)*cos(k);    //c2
    L_rotate(2,2) = cos(q)*cos(w);                            //c3

    w = R_exterior(0, 4);
    k = R_exterior(0, 5);
    q = R_exterior(0, 3);

    R_rotate(0,0) = cos(q)*cos(k) - sin(q)*sin(w)*sin(k);     //a1
    R_rotate(0,1) = -cos(q)*sin(k) - sin(q)*sin(w)*cos(k);    //a2
    R_rotate(0,2) = -sin(q)*cos(w);                           //a3
    R_rotate(1,0) = cos(w)*sin(k);                            //b1
    R_rotate(1,1) = cos(w)*cos(k);                            //b2
    R_rotate(1,2) = -sin(w);                                  //b3
    R_rotate(2,0) = sin(q)*cos(k) + cos(q)*sin(w)*sin(k);     //c1
    R_rotate(2,1) = -sin(q)*sin(k) + cos(q)*sin(w)*cos(k);    //c2
    R_rotate(2,2) = cos(q)*cos(w);                            //c3
}


void Intersection::GetAssistCoor()
{
    float Xs1 = L_exterior(0,0),Ys1 = L_exterior(0,1),Zs1 = L_exterior(0,2);
    float Xs2 = R_exterior(0,0),Ys2 = R_exterior(0,1),Zs2 = R_exterior(0,2);
    float X1,Y1,Z1,X2,Y2,Z2;
    float Bx = Xs2 - Xs1,By = Ys2 - Ys1,Bz = Zs2 - Zs1;
    float N1,N2;
    float Xa,Ya,Za;

 /*   L_wrap(0,0) = -114.4029;
    L_wrap(0,1) = 0.100029;
    L_wrap(0,2) = -0.001103;
    L_wrap(1,0) = 116.5334;
    L_wrap(1,1) = -0.001104;
    L_wrap(1,2) = -0.100014;
    R_wrap(0,0) = -114.1741;
    R_wrap(0,1) = 0.100046;
    R_wrap(0,2) = -0.001032;
    R_wrap(1,0) = 116.2799;
    R_wrap(1,1) = -0.001007;
    R_wrap(1,2) = -0.100023;*/
    for (int i=0; i<4; i++)
    {
        float tempx = L_wrap(0,1)*coordinate(i, 0) + L_wrap(0,2)*coordinate(i, 1) + L_wrap(0,0);
        float tempy = L_wrap(1,1)*coordinate(i, 0) + L_wrap(1,2)*coordinate(i, 1) + L_wrap(1,0);
        coordinate(i, 0) = tempx;
        coordinate(i, 1) = tempy;
        tempx = R_wrap(0,1)*coordinate(i, 2) + R_wrap(0,2)*coordinate(i, 3) + R_wrap(0,0);
        tempy = R_wrap(1,1)*coordinate(i, 2) + R_wrap(1,2)*coordinate(i, 3) + R_wrap(1,0);
        coordinate(i, 2) = tempx;
        coordinate(i, 3) = tempy;
    }


    for(int i=0;i<9;i++){
        X1 = L_rotate(0,0)*coordinate(i,0)+L_rotate(0,1)*coordinate(i,1)+L_rotate(0,2)*(-interior(0,2));
        Y1 = L_rotate(1,0)*coordinate(i,0)+L_rotate(1,1)*coordinate(i,1)+L_rotate(1,2)*(-interior(0,2));
        Z1 = L_rotate(2,0)*coordinate(i,0)+L_rotate(2,1)*coordinate(i,1)+L_rotate(2,2)*(-interior(0,2));

        X2 = R_rotate(0,0)*coordinate(i,2)+R_rotate(0,1)*coordinate(i,3)+R_rotate(0,2)*(-interior(0,2));
        Y2 = R_rotate(1,0)*coordinate(i,2)+R_rotate(1,1)*coordinate(i,3)+R_rotate(1,2)*(-interior(0,2));
        Z2 = R_rotate(2,0)*coordinate(i,2)+R_rotate(2,1)*coordinate(i,3)+R_rotate(2,2)*(-interior(0,2));

        N1 = (Bx*Z2 - Bz*X2)/(X1*Z2 - X2*Z1);
        N2 = (Bx*Z1 - Bz*X1)/(X1*Z2 - X2*Z1);
        cout << Bx << " " << Bz << endl;
        Xa = Xs1 + N1*X1;
        Ya = ((Ys1+N1*Y1)+(Ys2+N2*Y2))/2;
        Za = Zs1 + N1*Z1;

        Final_coordinate(i,0) = Xa;
        Final_coordinate(i,1) = Ya;
        Final_coordinate(i,2) = Za;

        cout << i << endl;
        cout << "X1" << X1 << endl;
        cout << "Y1" << Y1 << endl;
        cout << "Z1" << Z1 << endl;
        cout << "X2" << X2 << endl;
        cout << "Y2" << Y2 << endl;
        cout << "Z2" << Z2 << endl;
        cout << "N1" << N1 << endl;
        cout << "N2" << N2 << endl;
        cout << "Xa" << Xa << endl;
        cout << "Ya" << Ya << endl;
        cout << "Za" << Za << endl;
    }
}



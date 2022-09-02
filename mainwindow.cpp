#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QInputDialog>
#include <QLabel>
#include "resection.h"
#include <QString>
#include <iostream>
#include <sstream>
#include <QMessageBox>
#include "EXoperator.h"
#include "orientation.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Resection_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,     //打开文件资源管理器
            tr("Open File"),"","");
    if (!file_name.isNull())
        {
            //fileName是文件名
            QFile file;
            file.setFileName(file_name);
            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                int j=0;
                while(!file.atEnd())
                {
                    t = file.readLine();
                    QString str(t);
                    QStringList list = str.split(",");            //文件输入矩阵保存
                    for(int i=0; i < 5; i++)
                    {
                        QString tmp = list.at(i);
                        resec.five_coordinate(j, i) = tmp.toFloat();
                    }
                    j += 1;
                }
             //   ui->text_r->setText(QString(t));

                file.close();
            }
        }
    else{
            //点的是取消
        }

    resec.row = 4;
    resec.Amatrix = MatrixXf::Zero(resec.row*2,6);
    resec.Lmatrix = MatrixXf::Zero(resec.row*2,1);
    float m = 40000;
    float f = 153.24;
    //resec.interior(0, 0) = 0.01;
    //resec.interior(0, 1) = -0.02;
    resec.interior(0, 2) = f;

    float sumX = 0;
    float sumY = 0;
    float sumZ = 0;
    for (int i = 0; i < resec.row; i++)
    {
        sumX += resec.five_coordinate(i, 2);
        sumY += resec.five_coordinate(i, 3);
        sumZ += resec.five_coordinate(i, 4);
    }
    float XS = sumX / resec.row;
    float YS = sumY / resec.row;
    float ZS = sumZ / resec.row + f*m/1000;
    resec.exterior(0, 0) = XS;
    resec.exterior(0, 1) = YS;
    resec.exterior(0, 2) = ZS;
    resec.interior(0,2) = resec.interior(0,2) / 1000;

//    resec.five_coordinate = resec.WrapMat();

    //迭代计算：
    do {
        resec.GetA();
        resec.GetL();
        resec.GetDelta(resec.Amatrix, resec.Lmatrix);
        for (int n = 0; n < 6; n++)
            resec.exterior(0, n) = resec.exterior(0, n) + resec.Delta(n, 0);
    } while (resec.Delta(3, 0) >= 2.908882087e-5 || resec.Delta(4, 0) >= 2.908882087e-5 || resec.Delta(5, 0) >= 2.908882087e-5);//各角元素迭代计算至其改正值小于6秒,6s=2.908882087e-5 弧度。
    //输出结果
    for(int i=0; i<3; i++)
    {
        for (int j =0; j<3; j++)
            cout << resec.rotate(i, j);
        cout << endl;
    }
    for (int i=0; i<6; i++)
    {
        cout << resec.exterior(0,i) << " " << endl;
    }
    //写文件并保存
    QFile savefile;
    savefile.setFileName(QFileDialog::getSaveFileName(this, QString("保存路径"), QString("/"),QString("TEXT(*.txt)")));
    savefile.open(QIODevice::WriteOnly);
    savefile.write("旋转矩阵：\\n");
    for(int i=0; i<3; i++)
    {
        for (int j =0; j<3; j++)
        {
            QString data = QString("%1").arg(resec.rotate(i, j));
            QByteArray  strBytes=data.toUtf8();
            savefile.write(strBytes,strBytes.length());
            savefile.write(",");
        }
        savefile.write("\n");
    }
    savefile.write("外方位元素：\n");
    for (int i=0; i<6; i++)
    {
        QString data = QString("%1").arg(resec.exterior(0,i));
        QByteArray  strBytes=data.toUtf8();
        savefile.write(strBytes,strBytes.length());
        savefile.write(",");
    }
    savefile.close();
}

void MainWindow::on_Intersection_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,     //打开文件资源管理器
            tr("Open File"),"","");
    if (!file_name.isNull())
        {
            //fileName是文件名
            QFile file;
            file.setFileName(file_name);
            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                int j=0;
                while(!file.atEnd())
                {
                    t = file.readLine();
                    QString str(t);
                    QStringList list = str.split(",");            //文件输入矩阵保存
                    for(int i=0; i < 4; i++)
                    {
                        QString tmp = list.at(i);
                        insec.coordinate(j, i) = tmp.toFloat();
                    }
                    j += 1;
                }
                file.close();
            }
        }
    else{
            //点的是取消
        }

    insec.interior(0,0) = -0.002,insec.interior(0,1) = -0.002,insec.interior(0,2) = 153.124;
    MatrixXf tmpM = MatrixXf::Zero(4,6);
    file_name = QFileDialog::getOpenFileName(this,     //打开文件资源管理器
                tr("Open File"),"","");
        if (!file_name.isNull())
            {
                //fileName是文件名
                QFile file;
                file.setFileName(file_name);
                if(file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    int j=0;
                    while(!file.atEnd())
                    {
                        t = file.readLine();
                        QString str(t);
                        QStringList list = str.split(",");            //文件输入矩阵保存
                        for(int i=0; i < 6; i++)
                        {
                            QString tmp = list.at(i);
                            tmpM(j, i) = tmp.toFloat();
                        }
                        j += 1;
                    }
                    file.close();
                }
            }
        else{
                //点的是取消
            }
        for(int i=0; i < 6; i++)
        {
            insec.L_exterior(0,i) = tmpM(0, i);
            insec.R_exterior(0,i) = tmpM(1, i);
            insec.L_wrap(int(i/3), i>=3?i-3:i ) = tmpM(2, i);
            insec.R_wrap(int(i/3), i>=3?i-3:i ) = tmpM(3, i);
        }
 /*   insec.L_exterior(0,0) = 666590.8609;
    insec.L_exterior(0,1) = 116526.5794;
    insec.L_exterior(0,2) = 8855.3746;
    insec.L_exterior(0,4) = -4.6140;
    insec.L_exterior(0,3) = -1.0464;
    insec.L_exterior(0,5) = 91.00;

    insec.R_exterior(0,0) = 666386.4477;
    insec.R_exterior(0,1) = 119776.3750;
    insec.R_exterior(0,2) = 8575.1355;
    insec.R_exterior(0,4) = -3.4451;
    insec.R_exterior(0,3) = -2.0964;
    insec.R_exterior(0,5) = 90.05;*/
    insec.GetR();
    insec.GetAssistCoor();

    for(int i=0; i<9; i++)
    {
        for (int j =0; j<3; j++)
            cout << insec.Final_coordinate(i, j) << " , ";
        cout << endl;
    }
}

void MainWindow::on_open_image_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),".",tr("Image File(*.png *.jpg *.jpeg *.bmp *.tif)"));
        if (fileName.isEmpty())
        {
            return;
        }

    FilePath = fileName;
    QImage* img=new QImage;
    if(! ( img->load(fileName) ) ) //加载图像
    {
        QMessageBox::information(this,
                                 tr("打开图像失败"),
                                 tr("打开图像失败!"));
        delete img;
        return;
    }
    selfimg = img;
    QGraphicsScene *scene = new QGraphicsScene;//图像显示
    scene->addPixmap(QPixmap::fromImage(*img));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();

}

void MainWindow::on_M_clicked()
{
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("User input:"));
    // Input Kernal
    QString value1 = QString("Please Input Kernal: ");
    QLineEdit *kernal_edit = new QLineEdit(&dialog);
    kernal_edit->setEchoMode(QLineEdit::Normal);//输入方式
    form.addRow(value1, kernal_edit);
    // Input threshold
    QString value2 = QString("Please Input threshold: ");
    QLineEdit *threshold_edit = new QLineEdit(&dialog);
    threshold_edit->setEchoMode(QLineEdit::Normal);//输入方式
    form.addRow(value2, threshold_edit);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        // Do something here
        QString kernal_str = kernal_edit->text();
        int k_val = kernal_str.toInt();
        QString threshold_str = threshold_edit->text();
        double t_val = threshold_str.toDouble();

        MatrixXi src = MatrixXi::Zero(selfimg->width(),selfimg->height());//图像读入到src矩阵
            for(int i=0;i<selfimg->width();i++)
                { for(int j=0;j<selfimg->height();j++)
                    {
                      src(i,j) = qGray(selfimg->pixel(i,j));
                    }
                }

            EXoperator ope;
            ope.Moravec(src, k_val, t_val, &ope.cornerPoint);  //进行算子运算5,20000

            QPixmap pix;     //设置画笔在图像上绘制特征点
            pix.load(FilePath);
            QPainter painter(&pix);

            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(241,50,50));

            for (ope.itr = ope.cornerPoint.begin(); ope.itr < ope.cornerPoint.end(); ope.itr++)
                {
                    painter.drawEllipse(ope.itr->x,ope.itr->y,4,4);
                }
            painter.end();

            QGraphicsScene *scene = new QGraphicsScene;//图像显示
            scene->addPixmap(pix);
            ui->graphicsView->setScene(scene);
            ui->graphicsView->show();

            QMessageBox::information(this,tr("结果"),tr("运算完成!"));
    }
    else
    {
        QMessageBox::information(this,tr("结果"),tr("操作中止!"));
    }

}

void MainWindow::on_orientation_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,     //打开文件资源管理器
            tr("Open File"),"","");
    if (!file_name.isNull())
        {
            //fileName是文件名
            QFile file;
            file.setFileName(file_name);
            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                int j=0;
                while(!file.atEnd())
                {
                    t = file.readLine();
                    QString str(t);
                    QStringList list = str.split(",");            //文件输入矩阵保存
                    for(int i=0; i < 2; i++)
                    {
                        QString tmp = list.at(i+1);
//                        QString tmp = list.at(i);
                        reori.L_coor(j, i) = tmp.toFloat();
                    }
                    j += 1;
                }
            }
            QMessageBox::information(this,tr("左片数据读取成功"),tr("左片数据读取成功!"));
    }

    file_name = QFileDialog::getOpenFileName(this,     //打开文件资源管理器
            tr("Open File"),"","");
    if (!file_name.isNull())
        {
            //fileName是文件名
            QFile file;
            file.setFileName(file_name);
            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                int j=0;
                while(!file.atEnd())
                {
                    t = file.readLine();
                    QString str(t);
                    QStringList list = str.split(",");            //文件输入矩阵保存
                    for(int i=0; i < 2; i++)
                    {
                        QString tmp = list.at(i+1);
                     //   QString tmp = list.at(i);
                        reori.R_coor(j, i) = tmp.toFloat();
                    }
                    j += 1;
                }
            }
            QMessageBox::information(this,tr("右片数据读取成功"),tr("右片数据读取成功!"));
    }


    bool ok;
    double i = QInputDialog::getDouble(this, tr("QInputDialog::getDouble()"),
                                       tr("Please Input Focal Length:"), 0, 0, 1000, 4, &ok);
    if (ok)
    {
        QDialog dialog(this);
        QFormLayout form(&dialog);

        form.addRow(new QLabel("User select method:"));
        QRadioButton*  radio1 = new QRadioButton("Continuous Method", &dialog);
        QRadioButton*  radio2 = new QRadioButton("Single Method", &dialog);
        form.addRow(radio1);
        form.addRow(radio2);

        // Add Cancel and OK button
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
            Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);
        QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

        // Process when OK button is clicked
        if (dialog.exec() == QDialog::Accepted)
        {
            if(radio1->isChecked())
                reori.RelaOrientation(i);
            else if(radio2->isChecked())
                reori.SinRelaOri(i);
            else {}
        MatrixXf tmp = reori.GetData();
        //写文件并保存
        QFile savefile;
        savefile.setFileName(QFileDialog::getSaveFileName(this, QString("保存路径"), QString("/"),QString("TEXT(*.txt)")));
        savefile.open(QIODevice::WriteOnly);
        savefile.write("相对定向参数：\\n");
        for (int j = 0; j < tmp.rows(); j++)
        {
            for (int k=0; k< tmp.cols(); k++)
            {
                QString data = QString::number(tmp(j, k));
                QByteArray  strBytes=data.toUtf8();
                savefile.write(strBytes,strBytes.length());
                savefile.write(",");
                savefile.write("\n");
            }
        }
        savefile.close();
        }
    QMessageBox::information(this,tr("结果"),tr("相对定向成功!"));
    }
}

void MainWindow::on_coeffi_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),".",tr("Image File(*.png *.jpg *.jpeg *.bmp *.tif)"));
        if (fileName.isEmpty())
        {
            return;
        }
    FilePath = fileName;
    QImage* Limg=new QImage;
    if(! ( Limg->load(fileName) ) ) //加载左片图像
    {
        QMessageBox::information(this,  tr("打开图像失败"),  tr("打开图像失败!"));
        delete Limg;
        return;
    }
    fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),".",tr("Image File(*.png *.jpg *.jpeg *.bmp *.tif)"));
        if (fileName.isEmpty())
        {
            return;
        }
    QImage* Rimg=new QImage;
    if(! ( Rimg->load(fileName) ) ) //加载右片图像
    {
        QMessageBox::information(this, tr("打开图像失败"), tr("打开图像失败!"));
        delete Rimg;
        return;
    }

    MatrixXi lsrc = MatrixXi::Zero(Limg->width(),Limg->height()); //加载像对到矩阵中
    for(int i=0;i<Limg->width();i++)
        { for(int j=0;j<Limg->height();j++)
            {
              lsrc(i,j) = qGray(Limg->pixel(i,j));
            }
        }
    MatrixXi rsrc = MatrixXi::Zero(Rimg->width(),Rimg->height());
    for(int i=0;i<Rimg->width();i++)
        { for(int j=0;j<Rimg->height();j++)
            {
              rsrc(i,j) = qGray(Rimg->pixel(i,j));
            }
        }

    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("User input:"));
    // Input Kernal
    QString value1 = QString("Please Input Left Kernal: ");
    QLineEdit *lkernal_edit = new QLineEdit(&dialog);
    lkernal_edit->setEchoMode(QLineEdit::Normal);//输入方式
    form.addRow(value1, lkernal_edit);
    // Input threshold
    QString value2 = QString("Please Input Left threshold: ");
    QLineEdit *lthreshold_edit = new QLineEdit(&dialog);
    lthreshold_edit->setEchoMode(QLineEdit::Normal);//输入方式
    form.addRow(value2, lthreshold_edit);

    // Input Kernal
    QString value3 = QString("Please Input Right Kernal: ");
    QLineEdit *rkernal_edit = new QLineEdit(&dialog);
    rkernal_edit->setEchoMode(QLineEdit::Normal);//输入方式
    form.addRow(value3, rkernal_edit);
    // Input threshold
    QString value4 = QString("Please Input Right threshold: ");
    QLineEdit *rthreshold_edit = new QLineEdit(&dialog);
    rthreshold_edit->setEchoMode(QLineEdit::Normal);//输入方式
    form.addRow(value4, rthreshold_edit);

    // Input Kernal
    QString value5 = QString("Please Input Matching Kernal: ");
    QLineEdit *mkernal_edit = new QLineEdit(&dialog);
    mkernal_edit->setEchoMode(QLineEdit::Normal);//输入方式
    form.addRow(value5, mkernal_edit);
    // Input threshold
    QString value6 = QString("Please Input Matching threshold: ");
    QLineEdit *mthreshold_edit = new QLineEdit(&dialog);
    mthreshold_edit->setEchoMode(QLineEdit::Normal);//输入方式
    form.addRow(value6, mthreshold_edit);

    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted)
        {
        QString lkernal_str = lkernal_edit->text();           //5，28000，5，28000，11，0.7
        int lk = lkernal_str.toInt();
      //  cout << value1.toStdString() << lk << endl;
        QString lth_str = lthreshold_edit->text();
        float lt = lth_str.toFloat();
     //   cout << value2.toStdString() << lt << endl;
        QString rkernal_str = rkernal_edit->text();
        int rk = rkernal_str.toInt();
     //   cout << value3.toStdString() << rk << endl;
        QString rth_str = rthreshold_edit->text();
        float rt = rth_str.toFloat();
//        cout << value4.toStdString() << rt << endl;
        QString mk_str = mkernal_edit->text();
        int mk = mk_str.toInt();
 //       cout << value5.toStdString() << mk << endl;
        QString mt_str = mthreshold_edit->text();
        double mt = mt_str.toDouble();
  //      cout << value6.toStdString() << mt << endl;

        EXoperator lope;                                            //对左右影像特征值进行计算
        lope.Moravec(lsrc, lk, lt, &lope.cornerPoint);
        QMessageBox::information(this,tr("结果"),tr("左片运算完成!"));
        EXoperator rope;
        rope.Moravec(rsrc, rk, rt, &rope.cornerPoint);
        QMessageBox::information(this,tr("结果"),tr("右片运算完成!"));

        mat.coeffi(&lope.cornerPoint, &rope.cornerPoint, lsrc, rsrc, mk, mt);  //相关系数匹配

        vector<MATCHEDPOINT>::iterator iter;
        QPixmap lpix, rpix;

        lpix.load(FilePath);            //初始化画笔绘制匹配点
        rpix.load(fileName);
        QPainter painter(&lpix), painter2(&rpix);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(241,50,50));
        painter2.setRenderHint(QPainter::Antialiasing, true);
        painter2.setPen(Qt::NoPen);
        painter2.setBrush(QColor(241,50,50));
        int i = 1;

        //写文件并保存
        QFile savefile;
        savefile.setFileName(QFileDialog::getSaveFileName(this, QString("保存路径"), QString("/"),QString("TEXT(*.txt)")));
        savefile.open(QIODevice::WriteOnly);
        savefile.write("同名点：\n");

        for (iter = mat.Points_info.begin(); iter < mat.Points_info.end(); iter++)
        {
            stringstream ss;
            ss<<i;
            string s1 = ss.str();
            painter.drawEllipse(iter->x1, iter->y1,30,30);
            painter2.drawEllipse(iter->x2, iter->y2,30,30);

            QString data = QString::number(iter->x1);
            QByteArray  strBytes=data.toUtf8();
            savefile.write(strBytes,strBytes.length());
            savefile.write(",");
            data = QString::number(iter->y1);
            strBytes=data.toUtf8();
            savefile.write(strBytes,strBytes.length());
            savefile.write(",");
            data = QString::number(iter->x2);
            strBytes=data.toUtf8();
            savefile.write(strBytes,strBytes.length());
            savefile.write(",");
            data = QString::number(iter->y2);
            strBytes=data.toUtf8();
            savefile.write(strBytes,strBytes.length());
            savefile.write("\n");
        //    painter.drawText(iter->x1, iter->y1, QString::fromStdString(s1));
        //    painter2.drawText(iter->x1, iter->y1, QString::fromStdString(s1));
            i += 1;
        }
        painter.end();
        painter2.end();
        savefile.close();

/*        QGraphicsScene *scene = new QGraphicsScene;//图像显示
        scene->addPixmap(lpix);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->show();
        QMessageBox::information(this,tr("结果"),tr("完成!"));
        scene->addPixmap(rpix);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->show();*/

        lpix.save("lpix.png");
        rpix.save("rpix.png");
        QMessageBox::information(this,tr("结果"),tr("文件保存完成!"));
    }
    else
        {
        QMessageBox::information(this,tr("结果"),tr("操作中止!"));
    }


}

void MainWindow::on_H_clicked()
{
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("User input:"));
    // Input Kernal
    QString value1 = QString("Please Input Kernal: ");
    QLineEdit *kernal_edit = new QLineEdit(&dialog);
    kernal_edit->setEchoMode(QLineEdit::Normal);//输入方式
    form.addRow(value1, kernal_edit);
    // Input threshold
    QString value2 = QString("Please Input threshold: ");
    QLineEdit *threshold_edit = new QLineEdit(&dialog);
    threshold_edit->setEchoMode(QLineEdit::Normal);//输入方式
    form.addRow(value2, threshold_edit);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        // Do something here
        QString kernal_str = kernal_edit->text();
        int k_val = kernal_str.toInt();
        QString threshold_str = threshold_edit->text();
        double t_val = threshold_str.toDouble();

        MatrixXd src = MatrixXd::Zero(selfimg->width(),selfimg->height());//图像读入到src矩阵
            for(int i=0;i<selfimg->width();i++)
                { for(int j=0;j<selfimg->height();j++)
                    {
                      src(i,j) = qGray(selfimg->pixel(i,j));
                    }
                }

            EXoperator oper;
            oper.Harris(src, k_val, t_val, &oper.cornerPoint);  //进行算子运算5,20;0.2

            QPixmap pix;     //设置画笔在图像上绘制特征点
            pix.load(FilePath);
            QPainter painter(&pix);

            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(241,50,50));

            for (oper.itr = oper.cornerPoint.begin(); oper.itr < oper.cornerPoint.end(); oper.itr++)
                {
                    painter.drawEllipse(oper.itr->x,oper.itr->y,4,4);
                }
            painter.end();

            QGraphicsScene *scene = new QGraphicsScene;//图像显示
            scene->addPixmap(pix);
            ui->graphicsView->setScene(scene);
            ui->graphicsView->show();

            QMessageBox::information(this,tr("结果"),tr("运算完成!"));
    }
    else
    {
        QMessageBox::information(this,tr("结果"),tr("操作中止!"));
    }
}

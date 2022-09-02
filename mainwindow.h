#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>
#include "resection.h"
#include "intersection.h"
#include "orientation.h"
#include "matching.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Resection resec;//后方交会对象
    Intersection insec;//前方交会对象
    orientation reori;//相对定向对象
    Matching  mat;//匹配对象

private slots:
    void on_Resection_clicked();

    void on_Intersection_clicked();

    void on_open_image_clicked();

    void on_M_clicked();

    void on_orientation_clicked();

    void on_coeffi_clicked();

    void on_H_clicked();

private:
    Ui::MainWindow *ui;
    QByteArray t ;
    QString strk;
    QString strt;
    QImage* selfimg;
    QString FilePath;

};

#endif // MAINWINDOW_H

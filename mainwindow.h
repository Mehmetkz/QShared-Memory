#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedMemory>
#include <QFileDialog>
#include <QBuffer>
#include <iostream>
#include <QDebug>
//OpenCV Library
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
  void loadFromFile();
  void loadFromMemory();

private slots:
  void on_file_clicked();

  void on_memory_clicked();

private:
    void detach();

private:
    Ui::MainWindow *ui;
    QSharedMemory smemory;
};
#endif // MAINWINDOW_H

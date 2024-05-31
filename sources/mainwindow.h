#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTimer>
#include <QGridLayout>
#include "openglplot.h"
#include <vector>
#include <math.h>
#include <iostream>


class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  OpenGLPlot *glplot0;
  OpenGLPlot *glplot1;

  std::vector<float> x1;
  std::vector<float> y1;

  std::vector<float> x2;
  std::vector<float> y2;

public slots:
  void replot();
};
#endif // MAINWINDOW_H

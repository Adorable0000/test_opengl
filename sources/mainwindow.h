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
//  QCustomPlot *graph1;
  OpenGLPlot *glplot0;
  OpenGLPlot *glplot1;

  std::vector<double> x;
  std::vector<double> y;

public slots:
  void replot();
};
#endif // MAINWINDOW_H

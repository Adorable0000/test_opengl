#include "mainwindow.h"
#include <stdio.h>
#include <stdlib.h>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{

  QWidget *wgt = new QWidget();
  this->setCentralWidget(wgt);
  QGridLayout *grid = new QGridLayout();
  wgt->setLayout(grid);

  glplot0 = new OpenGLPlot();
//  glplot1 = new OpenGLPlot();
  grid->addWidget(glplot0,0,0,1,1);
//  grid->addWidget(glplot1,1,0,1,1);

  x1.resize(20000);
  y1.resize(20000);
  x2.resize(20000);
  y2.resize(20000);
  for(int i = 0; i < 20000; i++)
  {
    x1[i] = i;
    y1[i] = (sin(2 * 3.14 * i) * (1 << 11)) + (1 << 11);
    x2[i] = i;
    y2[i] = (cos(2 * 3.14 * i) * (1 << 11)) + (1 << 11);
  }

  QTimer *timer = new QTimer(this);
  timer->connect(timer, &QTimer::timeout, this, &MainWindow::replot);
  timer->start(10);
}

MainWindow::~MainWindow()
{
//  graph1 = nullptr;
  glplot0 = nullptr;
  glplot1 = nullptr;
}


void MainWindow::replot()
{
  glplot0->addData(x1, y1);
//  glplot1->addData(x2, y2);
  glplot0->update();
//  glplot1->update();
//  printf("Width %d ", glplot0->width());
//  printf("Height %d \n", glplot0->height());

//  QVector<double> x1(20000);
//  QVector<double> y1(20000);
//  for(int i = 0; i < 20000; i++)
//  {
//    y1[i] = (double)(sin(2 * 3.14 * i) * (1 << 11)) + (1 << 11);
//    x1[i] = (double)i;
//  }
}

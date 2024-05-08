#include "mainwindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <QSurfaceFormat>


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

  //int size = 20000;
  int size = 100000;
  x1.resize(size);
  y1.resize(size);
  x2.resize(size);
  y2.resize(size);
  for(int i = 0; i < size; i++)
  {
    x1[i] = i;
    y1[i] = (sin(2 * 3.14 * i)/* * (1 << 11)) + (1 << 11*/);
    x2[i] = i;
    y2[i] = (cos(2 * 3.14 * i) * (1 << 11)) + (1 << 11);
  }

  QTimer *timer = new QTimer(this);
  timer->connect(timer, &QTimer::timeout, this, &MainWindow::replot);
  timer->start(100);
//  timer->singleShot(15,this, &MainWindow::replot);
  glplot0->setColor(OGL::blue);
  glplot0->setYRange(*std::min_element(y1.begin(), y1.end()), *std::max_element(y1.begin(), y1.end()));
  glplot0->setXRange(0, y1.size());
}

MainWindow::~MainWindow()
{
//  graph1 = nullptr;
  glplot0 = nullptr;
  glplot1 = nullptr;
}


void MainWindow::replot()
{
  double time1 = clock() / (double)CLOCKS_PER_SEC;

  glplot0->addData(x1, y1);
//  glplot0->setColor(Qt::blue);

//  glplot1->addData(x2, y2);
  glplot0->update();
//  glplot1->update();

  double time2 = clock() / (double)CLOCKS_PER_SEC;
  double cpu_time = time2 - time1;

  printf("GPU TIME: %.6f sec\n", cpu_time);
}

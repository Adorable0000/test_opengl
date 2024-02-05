#include "mainwindow.h"
#include <stdio.h>
#include <stdlib.h>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{

  QWidget *wgt = new QWidget();
  this->setCentralWidget(wgt);
  QGridLayout *grid = new QGridLayout();
  wgt->setLayout(grid);

//  graph0 = new QCustomPlot();
//  graph0->setOpenGl(true);
//  graph1 = new QCustomPlot();
//  graph1->setOpenGl(true);
//  graph0->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::MinimumExpanding);
//  graph1->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::MinimumExpanding);

//  grid->addWidget(graph0, 0, 0, 1, 1);
//  grid->addWidget(graph1, 1, 0, 1, 1);
  glplot0 = new OpenGLPlot();
//  glplot1 = new OpenGLPlot();
  grid->addWidget(glplot0,0,0,1,1);
//  grid->addWidget(glplot1,1,0,1,1);

  QTimer *timer = new QTimer(this);
  timer->connect(timer, &QTimer::timeout, this, &MainWindow::replot);
  timer->start(10);
}

MainWindow::~MainWindow()
{
//  graph1 = nullptr;
  glplot0 = nullptr;
}


void MainWindow::replot()
{
  glplot0->update();

//  printf("Width %d ", glplot0->width());
//  printf("Height %d \n", glplot0->height());

//  QVector<double> x1(20000);
//  QVector<double> y1(20000);
//  for(int i = 0; i < 20000; i++)
//  {
//    y1[i] = (double)(sin(2 * 3.14 * i) * (1 << 11)) + (1 << 11);
//    x1[i] = (double)i;
//  }

//  graph0->addGraph();
//  graph0->graph(0)->setData(x1, y1);
//  graph0->xAxis->setRange(0,20000);
//  graph0->yAxis->setRange(0,4100);
//  graph1->addGraph();
//  graph1->addGraph();
//  graph1->graph(1)->setData(x1,y1);
//  graph1->xAxis->setRange(0,100000);
//  graph1->yAxis->setRange(0,100000);
//  graph0->replot();
//  graph1->replot();
}

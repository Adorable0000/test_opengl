#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGridLayout>
#include "openglplot.h"


class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
//  QCustomPlot *graph1;
  OpenGLPlot *glplot0;
  OpenGLPlot *glplot1;
public slots:
  void replot();
};
#endif // MAINWINDOW_H

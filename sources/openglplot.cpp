#include "openglplot.h"
#include <math.h>
#include <iostream>

OpenGLPlot::OpenGLPlot(QWidget *parent): QOpenGLWidget(parent)
{
  makeCurrent();
//  for(int i = 0; i < 100000; i++)
//    {
//      LiveVertex[i][0] = i;
//      LiveVertex[i+100000][0] = 100000 + i;
//      LiveVertex[i][1] = i;
//      LiveVertex[i+100000][1] = 100000 - i;
//    }
//  for(int i = 60000; i < 80000; i++)
//    {
//      LiveVertex[i][1] = 0;
//    }
//  LiveVertex[10000][1] = (LiveVertex[100000-1][1] + LiveVertex[100000+1][1])/2;
  for(int i = 0; i < 200000; i++)
    {
      LiveVertex[i][0] = i;
      LiveVertex[i][1] = (sin(2 * 3.14 * i) * (1 << 11)) + (1 << 11);
    }
  dataChanged = false;
  drawAxis.xRange.lower = 0;
  drawAxis.xRange.upper = 5;
  drawAxis.yRange.lower = 0;
  drawAxis.yRange.upper = 5;
}


OpenGLPlot::~OpenGLPlot()
{
  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_BLEND);
  makeCurrent();
}


void OpenGLPlot::initializeGL()
{
  glClearColor(255,255,255,255);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
}


void OpenGLPlot::resizeGL(int width, int height)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, (GLint)width, (GLint)height);
  glOrtho(0, 20000, -1, 4100, -1, 1);
}


void OpenGLPlot::paintGL()
{
  makeCurrent();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnableClientState(GL_VERTEX_ARRAY);
  glColor3f(0, 0, 255);
  glVertexPointer(2, GL_DOUBLE, 0, &LiveVertex);
  glDrawArrays(GL_LINE_STRIP_ADJACENCY_EXT, 0, 20000);
  glDisableClientState(GL_VERTEX_ARRAY);
}


void OpenGLPlot::addData(std::vector<double> keys, std::vector<double> values)
{
  if((keys.size() == 0) || (values.size() == 0) || (keys.size() != values.size()))
    {
      return;
    }
  paintData.xData.clear();
  paintData.yData.clear();
  if(paintData.xData.size() != keys.size())
    {
      paintData.xData.resize(keys.size());
      paintData.yData.resize(values.size());
//      xMax = *std::max_element(paintData.xData.begin(), paintData.xData.end());
//      yMax = *std::max_element(paintData.yData.begin(), paintData.yData.end());
      drawAxis.xRange.lower = *std::min_element(keys.begin(), keys.end());
      drawAxis.xRange.upper = *std::max_element(keys.begin(), keys.end());
      drawAxis.yRange.lower = *std::min_element(values.begin(), values.end());
      drawAxis.yRange.upper = *std::max_element(values.begin(), values.end());
    }
  memmove(paintData.xData.data(), keys.data(), paintData.xData.size());
  memmove(paintData.yData.data(), values.data(), paintData.yData.size());
  dataChanged = true;
}

#include "openglplot.h"
#include <math.h>


OpenGLPlot::OpenGLPlot(QWidget *parent): QOpenGLWidget(parent)
{
  makeCurrent();
//  for(int i = 0; i < 200000; i++)
//    {
//      LiveVertex[i][0] = i;
//      LiveVertex[i][1] = (sin(2 * 3.14 * i) * (1 << 11)) + (1 << 11);
//    }
  dataChanged = true;
//  sizeAxis.xRange.lower = 0;
//  sizeAxis.xRange.upper = 5;
//  sizeAxis.yRange.lower = 0;
//  sizeAxis.yRange.upper = 5;
//  paintData.xData.resize(5);
//  paintData.yData.resize(5);
//  sizeAxis.xRange.lower = 0;
//  sizeAxis.xRange.upper = 5;
//  sizeAxis.yRange.lower = 0;
//  sizeAxis.yRange.upper = 5;
  sizeAxis.xRange.lower = 0;
  sizeAxis.xRange.upper = 20000;
  sizeAxis.yRange.lower = 0;
  sizeAxis.yRange.upper = 20000;
  paintData.xData.resize(20000);
  paintData.yData.resize(20000);
  sizeAxis.xRange.lower = 0;
  sizeAxis.xRange.upper = 20000;
  sizeAxis.yRange.lower = -1;
  sizeAxis.yRange.upper = 4100;
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
  glOrtho(sizeAxis.xRange.lower, sizeAxis.xRange.upper, sizeAxis.yRange.lower, sizeAxis.yRange.upper, -1, 1);
//  glOrtho(0, 20000, -1, 4100, -1, 1);
}


void OpenGLPlot::paintGL()
{
  GLdouble Vertex[paintData.xData.size()][2];
  if(dataChanged)
    {
      for(int i = (int)sizeAxis.xRange.lower; i < (int)sizeAxis.xRange.upper; i++)
        {
          Vertex[i][0] = /*paintData.xData[i]*/ i;
          Vertex[i][1] = paintData.yData[i];
        }
      dataChanged = false;
    }
//  GLdouble Vertex[20000][2];
//  if(dataChanged)
//    {
//      for(int i = 0; i < 20000; i++)
//        {
//          Vertex[i][0] = i;
//          Vertex[i][1] = (sin(2 * 3.14 * i) * (1 << 11)) + (1 << 11);
//        }
//      dataChanged = false;
//    }
  makeCurrent();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(sizeAxis.xRange.lower, sizeAxis.xRange.upper, sizeAxis.yRange.lower, sizeAxis.yRange.upper, -1, 1);
//  glOrtho(0, 20000, -1, 4100, -1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnableClientState(GL_VERTEX_ARRAY);
  glColor3f(0, 0, 255);
  glVertexPointer(2, GL_DOUBLE, 0, &Vertex);
//  glDrawArrays(GL_LINE_STRIP_ADJACENCY_EXT, sizeAxis.xRange.lower, sizeAxis.xRange.upper);
  glDrawArrays(GL_LINE_STRIP_ADJACENCY_EXT, 0, 20000);
  glDisableClientState(GL_VERTEX_ARRAY);
}


void OpenGLPlot::addData(std::vector<double> &keys, std::vector<double> &values)
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
      sizeAxis.xRange.lower = keys[0];
      sizeAxis.xRange.upper = keys.size();
      sizeAxis.yRange.lower = std::floor(*std::min_element(values.begin(), values.end())) - 1;
      sizeAxis.yRange.upper = std::ceil(*std::max_element(values.begin(), values.end())) + 1;
    }
  memmove(paintData.xData.data(), keys.data(), keys.size());
  memmove(paintData.yData.data(), values.data(), values.size());
  dataChanged = true;
}

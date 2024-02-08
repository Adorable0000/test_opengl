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
  drawAxis.xRange.lower = 0;
  drawAxis.xRange.upper = 5;
  drawAxis.yRange.lower = 0;
  drawAxis.yRange.upper = 5;
  paintData.xData.resize(5);
  paintData.yData.resize(5);
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
  glOrtho(drawAxis.xRange.lower, drawAxis.xRange.upper, drawAxis.yRange.lower-1, drawAxis.yRange.upper, -1, 1);
//  glOrtho(0, 20000, -1, 4100, -1, 1);
}


void OpenGLPlot::paintGL()
{
  makeCurrent();
  GLdouble Vertex[paintData.xData.size()][2];
  if(dataChanged)
    {
      for(int i = drawAxis.xRange.lower; i < drawAxis.xRange.upper; i++)
        {
          Vertex[i][0] = paintData.xData[i];
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
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(drawAxis.xRange.lower, drawAxis.xRange.upper, drawAxis.yRange.lower, drawAxis.yRange.upper, -1, 1);
//  glOrtho(0, 20000, -1, 4100, -1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnableClientState(GL_VERTEX_ARRAY);
  glColor3f(0, 0, 255);
  glVertexPointer(2, GL_DOUBLE, 0, &Vertex);
  glDrawArrays(GL_LINE_STRIP_ADJACENCY_EXT, drawAxis.xRange.lower, drawAxis.xRange.upper);
//  glDrawArrays(GL_LINE_STRIP_ADJACENCY_EXT, 0, 20000);
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
      drawAxis.xRange.lower = keys[0];
      drawAxis.xRange.upper = keys.size();
      drawAxis.yRange.lower = std::floor(*std::min_element(values.begin(), values.end())) - 1;
      drawAxis.yRange.upper = std::ceil(*std::max_element(values.begin(), values.end())) + 1;
    }
  memmove(paintData.xData.data(), keys.data(), keys.size());
  memmove(paintData.yData.data(), values.data(), values.size());
  dataChanged = true;
}

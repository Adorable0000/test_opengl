#include "openglplot.h"
#include <math.h>


OpenGLPlot::OpenGLPlot(QWidget *parent): QOpenGLWidget(parent)
{
  makeCurrent();
  dataChanged = false;
  showGrid = false;
  showAxis = false;
  sizeAxis.xRange.lower = 0;
  sizeAxis.xRange.upper = 5;
  sizeAxis.yRange.lower = 0;
  sizeAxis.yRange.upper = 5;
  paintData.xData.resize(5);
  paintData.yData.resize(5);
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
}


void OpenGLPlot::paintGL()
{
  GLdouble Vertex[paintData.xData.size()][2];
  if(dataChanged)
    {
      for(int i = (int)sizeAxis.xRange.lower; i < (int)sizeAxis.xRange.upper; i++)
        {
          Vertex[i][0] = paintData.xData[i];
          Vertex[i][1] = paintData.yData[i];
        }
      dataChanged = false;
    }
  makeCurrent();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(sizeAxis.xRange.lower, sizeAxis.xRange.upper, sizeAxis.yRange.lower, sizeAxis.yRange.upper, -1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnableClientState(GL_VERTEX_ARRAY);
  glColor3f(0, 0, 255);
  glVertexPointer(2, GL_DOUBLE, 0, &Vertex);
  glDrawArrays(GL_LINE_STRIP_ADJACENCY_EXT, sizeAxis.xRange.lower, sizeAxis.xRange.upper);
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
    }
  memmove(paintData.xData.data(), keys.data(), keys.size() * sizeof (double));
  memmove(paintData.yData.data(), values.data(), values.size() * sizeof (double));
  dataChanged = true;
}


void OpenGLPlot::setRange(double xmin, double xmax, double ymin, double ymax)
{
  sizeAxis.xRange.lower = xmin;
  sizeAxis.xRange.upper = xmax;
  sizeAxis.yRange.lower = ymin - 1;
  sizeAxis.yRange.upper = ymax + 1;
}


void OpenGLPlot::gridVisible(bool state)
{
  showGrid = state;
}


void OpenGLPlot::axisVisible(bool state)
{
  showAxis = state;
}


bool OpenGLPlot::eventFilter(QObject *obj, QEvent *event)
{

}


void OpenGLPlot::setColor()
{

}




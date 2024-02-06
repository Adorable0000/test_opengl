#include "openglplot.h"
#include <math.h>

OpenGLPlot::OpenGLPlot(QWidget *parent): QOpenGLWidget(parent)
{
//  makeCurrent();
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
//  LiveVertex[100000][1] = (LiveVertex[100000-1][1] + LiveVertex[100000+1][1])/2;
  for(int i = 0; i < 200000; i++)
    {
      LiveVertex[i][0] = i;
      LiveVertex[i][1] = (sin(2 * 3.14 * i) * (1 << 11)) + (1 << 11);
    }
  dataChanged = true;
  xMax = 5;
  yMax = 5;
//  LiveVertex[100000][1] = 100000;
//  LiveVertex[99999][1] = 99999;
//  LiveVertex[10000][1] = 10000;
}


OpenGLPlot::~OpenGLPlot()
{
//  glDisable(GL_LINE_STIPPLE);
  makeCurrent();
}


void OpenGLPlot::initializeGL()
{
  glClearColor(255,255,255,255);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}


void OpenGLPlot::resizeGL(int width, int height)
{
//  if(dataChanged)
//    {
//      xMax = *std::max_element(std::begin(paintData.at(0).xData), std::end(paintData.at(0).xData));
//      yMax = *std::max_element(std::begin(paintData.at(0).yData), std::end(paintData.at(0).yData));
//    }
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


void OpenGLPlot::addData(std::vector<double> keys, std::vector<double> values, int graph)
{
  if((keys.size() == 0) || (values.size() == 0) || (keys.size() != values.size()) || (graph < 0))
    {
      return;
    }
  paintData.at(graph).xData.clear();
  paintData.at(graph).yData.clear();
  paintData.at(graph).xData.resize(keys.size());
  paintData.at(graph).yData.resize(values.size());
  memmove(paintData.at(graph).xData.data(), keys.data(), keys.size());
  memmove(paintData.at(graph).yData.data(), values.data(), values.size());
  dataChanged = true;
}


void OpenGLPlot::addGraph()
{
  drawData draw;
  paintData.push_back(draw);
}



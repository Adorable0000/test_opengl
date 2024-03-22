#include "openglplot.h"
#include <iostream>
#include <vector>



OpenGLPlot::OpenGLPlot(QWidget *parent): QOpenGLWidget(parent)
{
  dataChanged = false;
  showGrid = false;
  showAxis = false;
  sizeAxis.xRange.lower = 0;
  sizeAxis.xRange.upper = 5;
  sizeAxis.yRange.lower = 0;
  sizeAxis.yRange.upper = 5;
  paintData.xData.resize(5);
  paintData.yData.resize(5);
  mouseMove = 0;
  mousePressPos = 0;
}


OpenGLPlot::~OpenGLPlot()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_ALPHA_TEST);
  glLoadIdentity();
}


void OpenGLPlot::initializeGL()
{
  glClearColor(255,255,255,255);                      // Set white background color
  glEnable(GL_DEPTH_TEST);                            // Enable depth test to exclude some odd artifacts
  glDepthFunc(GL_ALWAYS);                             // Element always pass depth test
  glEnable(GL_BLEND);                                 // Enable color mix
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Mix colors using scale func for input and output color to smooth lines
  glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);            // Set fastest line smoothing
  glEnable(GL_LINE_SMOOTH);                           // Enable line smoothing
  glEnable(GL_ALPHA_TEST);                            // Enable alpha test to use transparency for smoothing
}


void OpenGLPlot::resizeGL(int width, int height)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                                 // Clear render matrix
  glViewport(0, 0, (GLint)width, (GLint)height);    // Change size of render window
  win_width = width;
  win_height = height;

  int mar_h = win_height % 4;
  if(mar_h > 0)
    {
      win_height -= mar_h;
    }
//  TESTING--------------------
  int mar_w = win_width % 4;
  if(mar_w > 0)
    {
      win_width -= mar_w;
    }
//-----------------------------
  dataChanged = true;
}


void OpenGLPlot::paintGL()
{
  int xlow = sizeAxis.xRange.lower;
  int xup = sizeAxis.xRange.upper;
  int xbounds = xup - xlow;
  double ylow = sizeAxis.yRange.lower;
  double yup = sizeAxis.yRange.upper;
  double ybounds = yup - ylow;
  int vsize = win_height;
  int hsize = win_width;

  if(xbounds < 15)
    {
      return;
    }

  std::vector<GLdouble> Vertex;
  Vertex.resize(xbounds*2);

  std::vector<GLdouble> hLine1;
//  hLine1.resize(xbounds*2);

//  TESTING --------------
  hLine1.resize(hsize);
//------------------------

  std::vector<GLdouble> hLine2;
  hLine2.resize(xbounds*2);

  std::vector<GLdouble> hLine3;
  hLine3.resize(xbounds*2);

  std::vector<GLdouble> hLine4;
  hLine4.resize(2*2);

  std::vector<GLdouble> hLine5;
  hLine5.resize(xbounds*2);

  if(dataChanged)
    {
      hLine4[0] = xlow;
      hLine4[1] = ylow;
      hLine4[2] = xup;
      hLine4[3] = ylow;

//  TESTING ------------------------------------------
      for(int i = 0; i < hsize; i+=2)
        {
          hLine1[i] = xlow + i * ((xup - xlow)/hsize);
          hLine1[i+1] = (yup + ylow)/2;
        }
//----------------------------------------------------

      for (int i = 0; i < xbounds*2; i+=2)
        {
          Vertex[i] = paintData.xData[i/2 + xlow];
          Vertex[i+1] = paintData.yData[i/2 + xlow];
//          hLine1[i] = paintData.xData[i/2 + xlow];
//          hLine1[i+1] = (yup + ylow)/2;
          hLine2[i] = paintData.xData[i/2 + xlow];
          hLine2[i+1] = (((yup + ylow)/2) + yup)/2;
          hLine3[i] = paintData.xData[i/2 + xlow];
          hLine3[i+1] = (((yup + ylow)/2) + ylow)/2;
          hLine5[i] = paintData.xData[i/2 + xlow];
          hLine5[i+1] = yup;
        }
      dataChanged = false;
    }

  std::vector<GLdouble> vLine1;
  vLine1.resize(vsize);

  for(int i = 0; i < vsize; i+=2)
    {
      vLine1[i] = (xup + xlow)/2;
      vLine1[i+1] = ylow + i * ((yup - ylow)/vsize);
    }

  makeCurrent();                                                  // Change render context
  glMatrixMode(GL_PROJECTION);                                    // Change to projection mode to enable multiplication between current and perspective matrix
  glLoadIdentity();                                               // Clear current render matrix
  glOrtho(xlow, xup, ylow-(ybounds/100), yup+(ybounds/100), -1, 1);   // Create perspective matrix with pixel based coordinates
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);             // Clear current color buffer
  glMatrixMode(GL_MODELVIEW);                                     // Change to object-view matrix
  glLoadIdentity();                                               // Clear current render matrix
  glEnableClientState(GL_VERTEX_ARRAY);                           // Enable vertex matrix

  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_ALPHA_TEST);

  glColor4f(0,0,0,0.5);

  glVertexPointer(2, GL_DOUBLE, 0, vLine1.data());
  glDrawArrays(GL_LINES, 0, vLine1.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, hLine1.data());
  glDrawArrays(GL_LINES, 0, hLine1.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, hLine2.data());
  glDrawArrays(GL_LINES, 0, hLine2.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, hLine3.data());
  glDrawArrays(GL_LINES, 0, hLine3.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, hLine5.data());
  glDrawArrays(GL_LINES, 0, hLine5.size()/2);

  glColor4f(0,0,0,1);
  glVertexPointer(2, GL_DOUBLE, 0, hLine4.data());
  glDrawArrays(GL_LINE_STRIP, 0, hLine4.size()/2);

  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_ALPHA_TEST);

  glColor4f(penColor.red(), penColor.green(), penColor.blue(),penColor.alpha());   // Set texture color
  glVertexPointer(2, GL_DOUBLE, 0, Vertex.data());
  glDrawArrays(GL_LINE_STRIP, 0, Vertex.size()/2);
  glDisableClientState(GL_VERTEX_ARRAY);                          // Disable vertex matrix

//  GLint a;
//  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &a);
//  printf("%d\n", a);
}


void OpenGLPlot::addData(std::vector<double> &keys, std::vector<double> &values)
{
  if((keys.size() == 0) || (values.size() == 0) || (keys.size() != values.size()))
    {
      return;
    }

  sizeAxis.xRange.lower = 0;
  sizeAxis.xRange.upper = keys.size();

  paintData.xData.clear();
  paintData.yData.clear();
  if((paintData.xData.size() != keys.size()) || (paintData.yData.size() != values.size()))
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
  // Unused for now //
  printAxisRange.xRange.lower = xmin;
  printAxisRange.xRange.upper = xmax;
  printAxisRange.yRange.lower = ymin;
  printAxisRange.yRange.upper = ymax;
  //
  sizeAxis.yRange.lower = ymin;
  sizeAxis.yRange.upper = ymax;
}


void OpenGLPlot::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons() & Qt::LeftButton)
    {
      mouseMove = (mousePressPos - event->pos().x()) * 4;
      if((sizeAxis.xRange.lower + mouseMove > 0) &&
         (sizeAxis.xRange.lower + mouseMove < sizeAxis.xRange.upper - 15))
        {
          sizeAxis.xRange.lower += mouseMove;
          dataChanged = true;
          this->update();
        }
      if((sizeAxis.xRange.upper + mouseMove < paintData.xData.size()) &&
         (sizeAxis.xRange.upper + mouseMove > sizeAxis.xRange.lower + 15))
        {
          sizeAxis.xRange.upper += mouseMove;
          dataChanged = true;
          this->update();
        }
      mousePressPos = event->pos().x();
    }
  event->accept();
}


void OpenGLPlot::mousePressEvent(QMouseEvent *event)
{
  if(event->button() == Qt::LeftButton)
    {
      mousePressPos = event->pos().x();
    }
}


void OpenGLPlot::wheelEvent(QWheelEvent *event)
{
  if(sizeAxis.xRange.lower + event->angleDelta().y() >= sizeAxis.xRange.upper - 15 - event->angleDelta().y())
    {
      return;
    }
  if(sizeAxis.xRange.upper - event->angleDelta().y() <= paintData.xData.size())
    {
      sizeAxis.xRange.upper -= event->angleDelta().y();
    }
  if(sizeAxis.xRange.lower + event->angleDelta().y() > 0)
    {
      sizeAxis.xRange.lower += event->angleDelta().y();
    }
  dataChanged = true;
  this->update();
  event->accept();
}


void OpenGLPlot::setColor(QColor col)
{
  penColor = col;
}


// Unused for now //
void OpenGLPlot::gridVisible(bool state)
{
  showGrid = state;
}
//


// Unused for now //
void OpenGLPlot::axisVisible(bool state)
{
  showAxis = state;
}
//

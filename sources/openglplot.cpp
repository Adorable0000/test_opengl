#include "openglplot.h"


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
  mouseMove = 0;
  mousePressPos = 0;
}


OpenGLPlot::~OpenGLPlot()
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_ALPHA_TEST);
  makeCurrent();
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
  dataChanged = true;
}


void OpenGLPlot::paintGL()
{
  int bounds = sizeAxis.xRange.upper - sizeAxis.xRange.lower;
  int low = sizeAxis.xRange.lower;
  if(bounds <= 0)
    {
      return;
    }
  GLdouble Vertex[bounds][2];                                     // Creating vertex matrix
  GLdouble hLine[bounds][2];
  if(dataChanged)
    {
      for(int i = 0; i < bounds; i++)
        {
          Vertex[i][0] = paintData.xData[i + low];
          Vertex[i][1] = paintData.yData[i + low];
          hLine[i][0] = paintData.xData[i + low];
          hLine[i][1] = (sizeAxis.yRange.upper + sizeAxis.yRange.lower)/2;
        }
      dataChanged = false;
    }

  GLdouble vLine[(int)sizeAxis.yRange.upper][2];
  for(int i = 0; i < (int)sizeAxis.yRange.upper; i++)
    {
      vLine[i][0] = (sizeAxis.xRange.upper + sizeAxis.xRange.lower)/2;
      vLine[i][1] = i;
    }

  makeCurrent();                                                  // Change render context
  glMatrixMode(GL_PROJECTION);                                    // Change to projection mode to enable multiplication between current and perspective matrix
  glLoadIdentity();                                               // Clear current render matrix
  glOrtho(sizeAxis.xRange.lower, sizeAxis.xRange.upper, sizeAxis.yRange.lower, sizeAxis.yRange.upper, -1, 1);   // Create perspective matrix with pixel based coordinates
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);             // Clear current color buffer
  glMatrixMode(GL_MODELVIEW);                                     // Change to object-view matrix
  glLoadIdentity();                                               // Clear current render matrix
  glEnableClientState(GL_VERTEX_ARRAY);                           // Enable vertex matrix

  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_ALPHA_TEST);
  glColor4f(0,0,0,0.3);
  glVertexPointer(2, GL_DOUBLE, 0, &vLine);
  glDrawArrays(GL_LINE_STRIP_ADJACENCY_EXT, 0, (int)sizeAxis.yRange.upper);
  glVertexPointer(2, GL_DOUBLE, 0, &hLine);
  glDrawArrays(GL_LINE_STRIP_ADJACENCY_EXT, 0, bounds);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_ALPHA_TEST);

  glColor4f(penColor.red(), penColor.green(), penColor.blue(),penColor.alpha());   // Set texture color
  glVertexPointer(2, GL_DOUBLE, 0, &Vertex);                      // Set vertex matrix
  glDrawArrays(GL_LINE_STRIP_ADJACENCY_EXT, 0, bounds);           // Render vertex matrix
  glDisableClientState(GL_VERTEX_ARRAY);                          // Disable vertex matrix
}


void OpenGLPlot::addData(std::vector<double> &keys, std::vector<double> &values)
{
  if(values.size() > GL_MAX_TEXTURE_SIZE*GL_MAX_TEXTURE_SIZE/(8*4))
    {
      return;
    }
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
      if(sizeAxis.xRange.lower + mouseMove > 0)
        {
          sizeAxis.xRange.lower += mouseMove;
        }
      if(sizeAxis.xRange.upper + mouseMove < paintData.xData.size())
        {
          sizeAxis.xRange.upper += mouseMove;
        }
      dataChanged = true;
      this->update();
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
  if(sizeAxis.xRange.lower + event->angleDelta().y() >= sizeAxis.xRange.upper)
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

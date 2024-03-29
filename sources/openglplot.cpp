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
  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_BLEND);
  makeCurrent();
}


void OpenGLPlot::initializeGL()
{
  glClearColor(255,255,255,255);                      // Set background color
  glEnable(GL_LINE_SMOOTH);                           // Enable line smoothing
  glEnable(GL_BLEND);                                 // Enable color mix
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Mix colors using scale func for input and output color to smooth lines
  glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);            // Set fastest line smoothing
}


void OpenGLPlot::resizeGL(int width, int height)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                                 // Clear render matrix
  glViewport(0, 0, (GLint)width, (GLint)height);    // Change size of render window
  dataChanged = true;
  this->update();
}


void OpenGLPlot::paintGL()
{
  if(sizeAxis.xRange.upper - sizeAxis.xRange.lower <= 0)
    {
      return;
    }
  GLdouble Vertex[(int)(sizeAxis.xRange.upper - sizeAxis.xRange.lower)][2];   // Creating vertex matrix with pixel based coord.
  if(dataChanged)
    {
      for(int i = 0; i < (int)(sizeAxis.xRange.upper - sizeAxis.xRange.lower); i++)
        {
          Vertex[i][0] = paintData.xData[i + (int)sizeAxis.xRange.lower];
          Vertex[i][1] = paintData.yData[i + (int)sizeAxis.xRange.lower];
        }
      dataChanged = false;
    }
  makeCurrent();                                                  // Change render context
  glMatrixMode(GL_PROJECTION);                                    // Change to projection mode to enable multiplication between current and perspective matrix
  glLoadIdentity();                                               // Clear current render matrix
  glOrtho(sizeAxis.xRange.lower, sizeAxis.xRange.upper, sizeAxis.yRange.lower, sizeAxis.yRange.upper, -1, 1);   // Create perspective matrix with pixel based coord.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);             // Clear current color buffer
  glMatrixMode(GL_MODELVIEW);                                     // Change to object-view matrix
  glLoadIdentity();                                               // Clear current render matrix
  glEnableClientState(GL_VERTEX_ARRAY);                           // Enable vertex matrix
  glColor3f(penColor.red(), penColor.green(), penColor.blue());   // Set texture color
  glVertexPointer(2, GL_DOUBLE, 0, &Vertex);                      // Set vertex matrix
  glDrawArrays(GL_LINE_STRIP_ADJACENCY_EXT, 0, (sizeAxis.xRange.upper - sizeAxis.xRange.lower));    // Render vertex matrix
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
  sizeAxis.yRange.lower = ymin;
  sizeAxis.yRange.upper = ymax;
  if(ymax > 1)
    {
      sizeAxis.yRange.lower--;
      sizeAxis.yRange.upper++;
    }
}


void OpenGLPlot::gridVisible(bool state)
{
  showGrid = state;
}


void OpenGLPlot::axisVisible(bool state)
{
  showAxis = state;
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




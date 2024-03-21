#ifndef OPENGLPLOT
#define OPENGLPLOT

#include <QOpenGLFunctions_3_1>
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QColor>

#include <GL/gl.h>
#include <GL/glu.h>

//class paintBuf
//{
//public:
//  explicit paintBuf(int size);
//  ~paintBuf();

//  GLdouble *buf_ptr[2];
//private:
//  int buf_size;
//};

class OpenGLPlot : public QOpenGLWidget
{
  Q_OBJECT

public:
  explicit OpenGLPlot(QWidget *parent = nullptr);
  ~OpenGLPlot();
  void addData(std::vector<double> &keys, std::vector<double> &values);
  void setColor(QColor col);
  void setRange(double xmin, double xmax, double ymin, double ymax);
  void gridVisible(bool state);
  void axisVisible(bool state);
//  void addGraph();

protected:
  void initializeGL() override;
  void resizeGL(int width, int height) override;
  void paintGL() override;

  void wheelEvent(QWheelEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

private:
  void insertColomn(int pos, double min, double max);
  void insertRow();

  struct drawData{std::vector<double> xData; std::vector<double> yData;};
  drawData paintData;

  struct Range{double lower; double upper;};
  struct Axis{Range xRange; Range yRange;};
  Axis sizeAxis;
  // Unused for now //
  Axis printAxisRange;
  //

  bool dataChanged;

  // Unused for now //
  bool showGrid;
  bool showAxis;
  //

  int mouseMove;
  int mousePressPos;

  int wheelMove;

  QColor penColor;
  int win_width;
  int win_height;
};

#endif // OPENGLPLOT

#ifndef OPENGLPLOT
#define OPENGLPLOT

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QColor>


class OpenGLPlot : public QOpenGLWidget
{
  Q_OBJECT

public:
  OpenGLPlot(QWidget *parent = nullptr);
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
  struct drawData{std::vector<double> xData; std::vector<double> yData;};
  drawData paintData;

  struct Range{double lower; double upper;};
  struct Axis{Range xRange; Range yRange;};
  Axis sizeAxis;

  bool dataChanged;

  bool showGrid;
  bool showAxis;

  int mouseMove;
  int mousePressPos;

  int wheelMove;

  QColor penColor;
};

#endif // OPENGLPLOT

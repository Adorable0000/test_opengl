#ifndef OPENGLPLOT
#define OPENGLPLOT

#include <QOpenGLWidget>
#include <QOpenGLFunctions>


class OpenGLPlot : public QOpenGLWidget
{
  Q_OBJECT

public:
  OpenGLPlot(QWidget *parent = nullptr);
  ~OpenGLPlot();
  void addData(std::vector<double> &keys, std::vector<double> &values);
  void setColor();
//  void addGraph();

protected:
  void initializeGL() override;
  void resizeGL(int width, int height) override;
  void paintGL() override;

private:
  struct drawData{std::vector<double> xData; std::vector<double> yData;};
  drawData paintData;
//  std::vector<drawData> paintBuf;
  struct Range{double lower; double upper;};
  struct Axis{Range xRange; Range yRange;};
  Axis sizeAxis;
  bool dataChanged;
//  double xMax;
//  double yMax;

};

#endif // OPENGLPLOT

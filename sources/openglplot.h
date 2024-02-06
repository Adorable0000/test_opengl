#ifndef OPENGLPLOT
#define OPENGLPLOT

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>

class OpenGLPlot : public QOpenGLWidget
{
  Q_OBJECT

public:
  OpenGLPlot(QWidget *parent = nullptr);
  ~OpenGLPlot();
  void addData(std::vector<double> keys, std::vector<double> values, int graph);
  void addGraph();

protected:
  void initializeGL() override;
  void resizeGL(int width, int height) override;
  void paintGL() override;

private:
  GLdouble LiveVertex[200000][2];
  struct drawData{std::vector<double> xData; std::vector<double> yData;};
  std::vector<drawData> paintData;
  bool dataChanged;
  double xMax;
  double yMax;

};

#endif // OPENGLPLOT

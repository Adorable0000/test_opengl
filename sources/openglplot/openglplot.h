#ifndef OPENGLPLOT
#define OPENGLPLOT

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QColor>

#include <GL/gl.h>
#include <GL/glu.h>


class OpenGLPlot : public QOpenGLWidget
{
  Q_OBJECT

public:
  explicit OpenGLPlot(QWidget *parent = nullptr);
  ~OpenGLPlot();
  void addData(std::vector<double> &keys, std::vector<double> &values);
  void setColor(QColor col);
  void setYRange(double min, double max);
  void setXRange(double min, double max);
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

  bool dataChanged;

  // Unused for now //
  bool showGrid;
  bool showAxis;
  //

  int mouseMove;
  int mousePressPos;

  int wheelMove;

  QColor penColor;

  int lines_width;
  int lines_height;

  std::vector<GLdouble> Vertex;
};


//----------------------------------------------
//  TESTING TEXT RENDER USING BITMAP
//
class BitmapFont
{
public:
  BitmapFont();
 ~BitmapFont();
  bool Load(char *fname);
  void Print(char *Text, int x, int y);
  void Select();
  void Bind();
  void SetBlend();
  GLuint TexID;

private:
  int CellX,CellY,YOffset,RowPitch;
  float RowFactor,ColFactor;
  char Base;
  char Width[256];
  int RenderStyle;
  int CurX,CurY;
};
//
//----------------------------------------------

#endif // OPENGLPLOT

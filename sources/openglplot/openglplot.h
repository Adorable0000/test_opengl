#ifndef OPENGLPLOT_H
#define OPENGLPLOT_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QColor>

#include <GL/gl.h>
#include <GL/glu.h>


//----------------------------------------------
// OpenGL colors
// float type nums
namespace OGL {
  enum Colors
  {
    black,
    white,
    darkGray,
    gray,
    lightGray,
    red,
    green,
    blue,
    cyan,
    magenta,
    yellow,
    darkRed,
    darkGreen,
    darkBlue,
    darkCyan,
    darkMagenta,
    darkYellow,
    purple,
    brown
  };
}


class OGLColor
{
public:
  OGLColor();
  OGLColor(OGL::Colors col);
  OGLColor(const OGLColor &col);

  OGLColor &operator=(const OGLColor &col);
  OGLColor &operator=(OGL::Colors col);
  bool operator==(const OGLColor &col) const;
  bool operator!=(const OGLColor &col) const;

  ~OGLColor();

  int red();
  int green();
  int blue();
  int alpha();

  float redF();
  float greenF();
  float blueF();
  float alphaF();

  void setRgbF(float redF, float greenF, float blueF, float alphaF);
  void setRgb(int red, int green, int blue, float alpha);

  struct {float redF; float greenF; float blueF; float alphaF = 1;}rgba;
};
//
//----------------------------------------------


class Grid
{
public:
  Grid();
  ~Grid();

  void addHorizontalLine(double x, double y);
  void addWerticalLine(double x, double y);
  std::vector<std::vector<GLdouble>> Elements;
};


class TicksHandler
{
public:
  TicksHandler();
  ~TicksHandler();
  void getTicks();
  void getTickStep();
  void getSubTickStep();
};


class Axis
{
public:
  Axis();
  ~Axis();
};


class OpenGLPlot : public QOpenGLWidget
{
  Q_OBJECT

public:
  explicit OpenGLPlot(QWidget *parent = nullptr);
  ~OpenGLPlot();
  void addData(std::vector<double> &keys, std::vector<double> &values);

  void setColorf(float red, float green, float blue);
  void setColor(int red, int green, int blue);
  void setQColor(QColor col);
  void setColor(OGLColor col);

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
//  void insertColomn(int pos, double min, double max);
//  void insertRow();

  struct drawData{std::vector<double> xData; std::vector<double> yData;};
  drawData paintData;

  struct Range{double lower; double upper;};
  struct {Range xRange; Range yRange;}sizeRange;

  int plotWidth;
  int plotHeight;

  bool dataChanged;

  // Unused for now //
  bool showGrid;
  bool showAxis;
  //----------------//

  int mouseMove;
  int mousePressPos;

  int wheelMove;

  QColor penQColor;
  OGLColor penColor;

  int gridLinesWidth;
  int gridLinesHeight;

  std::vector<GLdouble> Vertex;
};


//----------------------------------------------
//  TESTING TEXT RENDER USING FREETYPE 2
//
#include "ft2build.h"
#include FT_FREETYPE_H

class FreeTypeFont
{
public:
  FreeTypeFont();
  ~FreeTypeFont();

  void ftInit();
};
//
//----------------------------------------------


//----------------------------------------------
//  TESTING TEXT RENDER USING BITMAP
//
class BitmapFont
{
public:
  BitmapFont();
 ~BitmapFont();
  bool Load(char *fname);
  void Print(char *Text, double x, double y);
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
  double CurX,CurY;
};
//
//----------------------------------------------


std::string get_working_path();
#endif // OPENGLPLOT_H

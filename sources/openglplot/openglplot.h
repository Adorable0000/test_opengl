#ifndef OPENGLPLOT_H
#define OPENGLPLOT_H


#if defined(QT_CORE_LIB)
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <QMouseEvent>
#include <QWheelEvent>
#endif

#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>


/*!
 * Defines preimplemeted colors for painting
 *
 * \see OGLColor(OGL::Colors col)
 */
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
  OGLColor(float r, float g, float b, float a = 1);
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

private:
  std::vector<std::vector<GLdouble>> Elements;
};


class TicksHandler
{
public:
  TicksHandler();
  ~TicksHandler();

  void getTickCount();
  void getTickStep();

  void getSubTickCount();
  void getSubTickStep();
};


class Axis
{
public:
  Axis();
  ~Axis();
};


#if defined(QT_CORE_LIB)
class OpenGLPlot : public QOpenGLWidget, protected QOpenGLFunctions
#else
class OpenGLPlot
#endif
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

#if defined(QT_CORE_LIB)
  void wheelEvent(QWheelEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
#endif

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

  OGLColor penColor;

  int gridLinesWidth;   /// optimal size of the horizontal dotted lines
  int gridLinesHeight;  /// optimal size of the vertical dotted lines

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
};
//
//----------------------------------------------


std::string get_working_path();
#endif // OPENGLPLOT_H

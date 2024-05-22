#ifndef OPENGLPLOT_H
#define OPENGLPLOT_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_1>

#include <QMouseEvent>
#include <QWheelEvent>


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


class Grid
{
public:
  Grid();
  ~Grid();

  void drawGrid();
  void addHorizontalLine(double xmin, double xmax, double y);
  void addWerticalLine(double x, double ymin, double ymax);

  void setMaxXSize(double width);
  void setMaxYSize(double height);

private:
  std::vector<std::vector<GLdouble>> Elements; 
  int maxXSize;   /// optimal size of the horizontal dotted lines
  int maxYSize;  /// optimal size of the vertical dotted lines
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

  void drawAxis();

  void addLine(double xmin, double xmax, double ymin, double ymax);
  void addTick(double xmin, double xmax, double ymin, double ymax);
};


class OpenGLPlot : public QOpenGLWidget, protected QOpenGLFunctions_3_1

{
  Q_OBJECT

public:
  explicit OpenGLPlot(QWidget *parent = nullptr);
  ~OpenGLPlot();
  float getGLversion();
  int getGLSLversion();
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

  void vertexChanged(double size, double shift);

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

  // Unused for now //
  bool showGrid;
  bool showAxis;
  //----------------//

  int mouseMove;
  int mousePressPos;

  int wheelMove;

  OGLColor penColor;
  QColor penQColor;

  int gridLinesWidth;   /// optimal size of the horizontal dotted lines
  int gridLinesHeight;  /// optimal size of the vertical dotted lines

  std::vector<GLdouble> Vertex;
};


//----------------------------------------------
//  TESTING TEXT RENDER USING FREETYPE 2
//

class FreeTypeFont
{
public:
  FreeTypeFont();
  ~FreeTypeFont();

  void ftInit(const char *font_name);
};
//
//----------------------------------------------

#endif // OPENGLPLOT_H

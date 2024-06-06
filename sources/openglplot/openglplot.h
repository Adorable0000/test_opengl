#ifndef OPENGLPLOT_H
#define OPENGLPLOT_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

#include <QMouseEvent>
#include <QWheelEvent>

#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H


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

  void addHorizontalLine(double y);
  void addWerticalLine(double x);

  void setPixelSize(double height, double width);

  void setGridSize(double height, double width);

  void setTickCount(int count);
  void setTickStep(double step);

private:
  std::vector<std::vector<GLdouble>> Elements; 
  int maxLineSizeX;   /// optimal size of the horizontal dotted lines
  int maxLineSizeY;   /// optimal size of the vertical dotted lines

  double dotWidth;
  double dotHeight;

  int tickCount;
  double tickStep;

  GLuint VBO;
  GLuint VAO;
};


double getMantissa(double input);
double getMagnitude(double input);
double pickClosest(double target, std::vector<double> &candidates);
double cleanMantissa(double input);
int getFirstStep(double min, double tickStep);
int getLastStep(double max, double tickStep);
int getTickCount(double firstStep, double maxNum, double tickStep);
double getTickStep(double min, double max, int count);
int getSubTickCount(double tickStep);
double getSubTickStep(double tickStep, int subTickCount);
std::vector<double> createTickPosVector(double tickStep, double minMun, double maxNum);
std::vector<double> createSubTickPosVector(double tickStep, std::vector<double> &ticks, double minNum, double maxNum);



class Axis
{
public:
  Axis();
  ~Axis();

  void addLines(double xmin, double xmax, double ymin, double ymax);
  void addTick(double xmin, double xmax, double ymin, double ymax);

  void setPixelSize(double height, double width);
  void setAxisSize(double height, double width);

  void setTickCount(int count);
  void setTickStep(double step);
  void setSubTickCount(int count);
  void setSubTickStep(double step);

private:
  double pixelWidth;
  double pixelHeight;

  GLuint VBO;
  GLuint VAO;
};


//----------------------------------------------
//  TESTING TEXT RENDER USING FREETYPE 2
//
struct TextCharacter
{
  GLchar character;
  GLuint TextureID;
  GLfloat xTexCoord;
  GLfloat yTexCoord;
  GLfloat size;
};


class FreeTypeFont
{
public:
  FreeTypeFont();
  ~FreeTypeFont();

  void ftInit(const char *font_name);
  void RenderText(std::string &text, GLfloat x, GLfloat y);

private:

  FT_Face face;       // test
  FT_Library ft;      // test
};
//
//----------------------------------------------


class OpenGLPlot : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

public:
  explicit OpenGLPlot(QWidget *parent = nullptr);
  ~OpenGLPlot();
  float getGLversion();
  void setData(std::vector<float> &keys, std::vector<float> &values);

  void setColorf(float red, float green, float blue);
  void setColor(int red, int green, int blue);

  void setQColor(QColor col);
  void setColor(OGLColor col);

  void setYRange(float min, float max);
  void setXRange(float min, float max);

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

  void shaderInit();
  void vertexChanged(float size, float shift);
  const char* getShader(int type, float version);
  void drawAxis();
  void drawGrid();

  GLuint VBO;
  GLuint VAO;
  GLuint FBO;
  GLuint RBO;

  GLuint vertexSmoothShader;
  GLuint fragmentSmoothShader;
  GLuint shaderProgram;

  struct drawData{std::vector<float> xData; std::vector<float> yData;};
  drawData paintData;

  struct Range{float lower; float upper;};
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

  float pixelWidth;
  float pixelHeight;

  std::vector<GLfloat> Vertex;

  FreeTypeFont fr;    // test
};

#endif // OPENGLPLOT_H

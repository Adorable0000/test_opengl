#include "openglplot.h"
#include <iostream>

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H


FreeTypeFont fr;    // test
FT_Face face;       // test
FT_Library ft;      // test


/*!
 * \brief vertexShaderSmooth shader source code.
 *
 * Calculating the distance from a fragment
 * to the line using the interpolated line
 * center point attribute.
 * Vertex transforming the normalized projected
 * vertex position to the viewport space
 */
static const char *vertexShaderSmooth_120 =
    "#version 120\n"
    "uniform vec2 uViewPort;\n"
    "varying vec2 vLineCenter;\n"
    "void main()\n"
    "{\n"
    " vec4 pp = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    " gl_Position = pp;\n"
    " vec2 vp = uViewPort;\n"
    " vLineCenter = 0.5*(pp.xy + vec2(1, 1))*vp;\n"
    "}\n";


/*!
 * \brief fragmentShaderSmooth shader source code
 *
 * Filtering line by adjusting the fragment's alpha
 * value based on the distance to the line.
 * To calculate the fragment's alpha value, the
 * fragment's distance to the line is compared
 * with the current line width and apply the power
 * function to the normalized difference between
 * those to achieve desired blurriness of the line
 */
static const char *fragmentShaderSmooth_120 =
    "#version 120\n"
    "uniform float uLineWidth;\n"
    "uniform vec4 uColor;\n"
    "uniform float uBlendFactor;\n"
    "varying vec2 vLineCenter;\n"
    "void main(void)\n"
    "{\n"
    " vec4 col = uColor;\n"
    " double d = length(vLineCenter-gl_FragCoord.xy);\n"
    " double w = uLineWidth;\n"
    " if (d>w){\n"
    "   col.w = 0;\n"
    " }\n"
    " else{\n"
    "   col.w *= pow(float((w-d)/w), uBlendFactor);\n"
    " }\n"
    " gl_FragColor = col;"
    "}\n";


#if defined (QT_CORE_LIB)
OpenGLPlot::OpenGLPlot(QWidget *parent): QOpenGLWidget(parent)
#else
OpenGLPlot::OpenGLPlot()
#endif
{
  //dataChanged = false;
  showGrid = false;
  showAxis = false;
  sizeRange.xRange.lower = 0;
  sizeRange.xRange.upper = 15;
  sizeRange.yRange.lower = 0;
  sizeRange.yRange.upper = 15;
  paintData.xData.resize(15);
  paintData.yData.resize(15);
  mouseMove = 0;
  mousePressPos = 0;
}


OpenGLPlot::~OpenGLPlot()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_ALPHA_TEST);
  glLoadIdentity();
}


GLuint VBO;
/*!
 * \brief OpenGLPlot::initializeGL Initializing OpenGL functions.
 *
 * Gets called once before the first time
 * resizeGL() or paintGL() is called
 * When using Qt, it is virtual function which is
 * called during creating widget
 * All used vertex and array (if supported) buffer
 * objects should be created and binded there, also
 * sets up the OpenGL resources and state
 */
void OpenGLPlot::initializeGL()
{
#if defined (QT_CORE_LIB)
  initializeOpenGLFunctions();
#endif
  float version = getGLversion();;
  printf("%f\n", version);

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);               // Set background color
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
//  glEnable(GL_DEPTH_TEST);                            // Enable depth test to exclude some odd artifacts
//  glDepthFunc(GL_ALWAYS);                             // Element always pass depth test
  glEnable(GL_BLEND);                                 // Enable color mix
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Mix colors using scale func for input and output color to smooth lines
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);             // Set fastest line smoothing
  glEnable(GL_LINE_SMOOTH);                           // Enable line smoothing
  glEnable(GL_ALPHA_TEST);                            // Enable alpha test to use transparency for smoothing

//----------------------------------------------
//  TESTING TEXT RENDER USING FREETYPE 2
  fr.ftInit("DejaVuMathTeXGyre.ttf");
//
//----------------------------------------------
}


/*!
 * \brief OpenGLPlot::resizeGL Main resize event.
 * \param width
 * \param height
 *
 * Gets called whenever the widget has been resized
 * and also when it is shown for the first time
 * because all newly created widgets should be
 * resized. When using Qt, widget get a resize event
 * automatically.
 * Sets up the OpenGL viewport and projection,
 * clear current matrix when called
 */
void OpenGLPlot::resizeGL(int width, int height)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                                 // Clear render matrix
  glViewport(0, 0, reinterpret_cast<GLint>(width), reinterpret_cast<GLint>(height));    // Change size of render window
  plotWidth = width;
  plotHeight = height;
  gridLinesHeight = height;
  gridLinesWidth = width;
/*
  Keep grid dotted lines sizes divisible by 4 so that
  the last dot of line will always touch the higher line
  Remember that 1 line need 4 values for each dot
*/
  int mar_h = gridLinesHeight % 4;
  if(mar_h > 0)
    {
      gridLinesHeight += (4-mar_h);
    }

  int mar_w = gridLinesWidth % 4;
  if(mar_w > 0)
    {
      gridLinesWidth += (4-mar_w);
    }

  //dataChanged = true;
}

double wpix;        // test value
double hpix;        // test value
/*!
 * \brief OpenGLPlot::paintGL Main paint event
 *
 * Renders the OpenGL scene. Gets called
 * whenever the widget needs to be updated.
 * When using Qt, this is a virtual function
 * that gets called everytime you call
 * (widget name).update()
 */
void OpenGLPlot::paintGL()
{
  double time1 = clock() / static_cast<double>(CLOCKS_PER_SEC);

  double xlow = sizeRange.xRange.lower;
  double xup = sizeRange.xRange.upper;
  double xbounds = xup - xlow;
  double ylow = sizeRange.yRange.lower;
  double yup = sizeRange.yRange.upper;
  double ybounds = yup - ylow;
  int hlinesize = gridLinesHeight;
  int wlinesize = gridLinesWidth;
  double wpixel = xbounds/plotWidth;    // width of pixel relative to graph
  double hpixel = ybounds/plotHeight;    // height of pixel relative to graph
  wpix = xbounds/plotWidth;  // test value
  hpix = ybounds/plotHeight;  // test value

/*
  if(xbounds < 10)
    {
      return;
    }

  std::vector<GLdouble> hLine1;
  hLine1.resize(wlinesize);     // Horizontal dotted line

  std::vector<GLdouble> hLine2;
  hLine2.resize(wlinesize);

  std::vector<GLdouble> hLine3;
  hLine3.resize(wlinesize);

  std::vector<GLdouble> hLine4;
  hLine4.resize(2*2);

  std::vector<GLdouble> vLine1;
  vLine1.resize(hlinesize);

  std::vector<GLdouble> vLine2;
  vLine2.resize(2*2);

  std::vector<GLdouble> vLine3;
  vLine3.resize(hlinesize);

  std::vector<GLdouble> hLine5;
  hLine5.resize(wlinesize);

  hLine4[0] = xlow;
  hLine4[1] = ylow;
  hLine4[2] = xup;
  hLine4[3] = ylow;

  for(int i = 0; i < wlinesize; i+=2)
    {
      hLine1[i] = xlow + i * wpixel;  // to keep the minium number of X values for line
      hLine1[i+1] = (yup + ylow)/2;   // Y position of the line

      hLine2[i] = xlow + i * wpixel;
      hLine2[i+1] = (((yup + ylow)/2) + yup)/2;

      hLine3[i] = xlow + i * wpixel;
      hLine3[i+1] = (((yup + ylow)/2) + ylow)/2;

      hLine5[i] = xlow + i * wpixel;
      hLine5[i+1] = yup;
    }

  for(int i = 0; i < hlinesize; i+=2)
    {
      vLine1[i] = (xup + xlow)/2;
      vLine1[i+1] = ylow + i * hpixel;

      vLine3[i] = xup;
      vLine3[i+1] = ylow + i * hpixel;
    }

  vLine2[0] = xlow;
  vLine2[1] = ylow;
  vLine2[2] = xlow;
  vLine2[3] = yup;
*/

//    if(dataChanged)
//    {
//      vertexChanged(xbounds, xlow);
//      dataChanged = false;
//    }

  makeCurrent();                                                  // Change render context
  glMatrixMode(GL_PROJECTION);                                    // Change to projection mode to enable multiplication between current and perspective matrix
  glLoadIdentity();                                               // Clear current render matrix
//  glOrtho(xlow-(wpixel*10), xup+(wpixel*10), ylow-(hpixel*18), yup+(hpixel*10), -1, 1);   // Create perspective matrix with pixel based coordinates

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);             // Clear current color buffer
  glMatrixMode(GL_MODELVIEW);                                     // Change to object-view matrix
  glLoadIdentity();                                               // Clear current render matrix

  glBufferData(GL_ARRAY_BUFFER, Vertex.size()*sizeof(double), Vertex.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  //-----------------------------------------------------------------
  //  TESTING LINE SMOOTHING WITH SHADER
  //
//  GLuint vertexSmoothShader;
//  vertexSmoothShader = glCreateShader(GL_VERTEX_SHADER);
//  glShaderSource(vertexSmoothShader, 1, &vertexShaderSmooth_120, NULL);
//  glCompileShader(vertexSmoothShader);
//  GLint succ;
//  GLchar infolog[512];
//  glGetShaderiv(vertexSmoothShader, GL_COMPILE_STATUS, &succ);
//  if(!succ)
//    {
//      glGetShaderInfoLog(vertexSmoothShader, 512, NULL, infolog);
//      printf("vertex shader, %s\n", infolog);
//    }

//  GLuint fragmentSmoothShader;
//  fragmentSmoothShader = glCreateShader(GL_FRAGMENT_SHADER);
//  glShaderSource(fragmentSmoothShader, 1, &fragmentShaderSmooth_120, NULL);
//  glCompileShader(fragmentSmoothShader);
//  glGetShaderiv(fragmentSmoothShader, GL_COMPILE_STATUS, &succ);
//  if(!succ)
//    {
//      glGetShaderInfoLog(fragmentSmoothShader, 512, NULL, infolog);
//      printf("fragment shader, %s\n", infolog);
//    }

//  GLuint shaderProgram;
//  shaderProgram = glCreateProgram();
//  glAttachShader(shaderProgram, vertexSmoothShader);
//  glAttachShader(shaderProgram, fragmentSmoothShader);
//  glLinkProgram(shaderProgram);
//  glUseProgram(shaderProgram);
//  printf("create shader program, %d\n", glGetError());

//  glDeleteShader(vertexSmoothShader);
//  glDeleteShader(fragmentSmoothShader);
//  glVertexAttribPointer(0, Vertex.size()/2, GL_FLOAT, GL_FALSE, Vertex.size()/2*sizeof(GL_FLOAT), (GLvoid*)0);
//  glEnableVertexAttribArray(0);
//  printf("vertex attrib, %d\n", glGetError());
  //
  //-----------------------------------------------------------------

  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_ALPHA_TEST);
  glEnableClientState(GL_VERTEX_ARRAY);

/*
  glColor4f(0,0,0,0.5);

  glVertexPointer(2, GL_DOUBLE, 0, hLine1.data());
  glDrawArrays(GL_LINES, 0, hLine1.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, vLine1.data());
  glDrawArrays(GL_LINES, 0, vLine1.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, hLine2.data());
  glDrawArrays(GL_LINES, 0, hLine2.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, hLine3.data());
  glDrawArrays(GL_LINES, 0, hLine3.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, hLine4.data());
  glDrawArrays(GL_LINE_STRIP, 0, hLine4.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, vLine2.data());
  glDrawArrays(GL_LINE_STRIP, 0, vLine2.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, vLine3.data());
  glDrawArrays(GL_LINES, 0, vLine3.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, hLine5.data());
  glDrawArrays(GL_LINES, 0, hLine5.size()/2);
*/
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_ALPHA_TEST);

  glColor4f(penColor.redF(), penColor.greenF(), penColor.blueF(), penColor.alphaF());   // Set texture color
//  glVertexPointer(2, GL_DOUBLE, 0, Vertex.data());
  glVertexPointer(2, GL_DOUBLE, 0, NULL);                          // Enable vertex matrix
  glDrawArrays(GL_LINE_STRIP, 0, Vertex.size()/2);
  glDisableClientState(GL_VERTEX_ARRAY);                          // Disable vertex matrix
//--------------------------------------------------------------------------------------------
//  TESTING TEXT RENDER USING BITMAP
//
//  glEnable(GL_TEXTURE_2D);
//  Font.Select();
//  Font.Print("1, 2, 3, 4, 5, 6, 7, 8, 9, 0", 0, 0);
//  Font.Print("1, 2, 3, 4, 5, 6, 7, 8, 9, 0", xlow, ylow-(hpixel*18));     // size of textures is 16 pixels, so we should add 2 pixels
//  glDisable(GL_TEXTURE_2D);
//  glFlush();
//
//--------------------------------------------------------------------------------------------
  double time2 = clock() / static_cast<double>(CLOCKS_PER_SEC);
  double gpu_time = time2 - time1;

  printf("GPU TIME: %.6f sec\n", gpu_time);
  printf("\n");
}


void OpenGLPlot::vertexChanged(double size, double shift)
{
  if(Vertex.size() != size*2)  {Vertex.resize(size*2);}
  if(paintData.xData.size() < Vertex.size()/2) {return;}
  float strideY = (sizeRange.yRange.upper + sizeRange.yRange.lower) * 0.5;
  float diffY = (sizeRange.yRange.upper - sizeRange.yRange.lower) * 0.5;
  float strideX = (sizeRange.xRange.upper + sizeRange.xRange.lower) * 0.5;
  float diffX = (sizeRange.xRange.upper - sizeRange.xRange.lower) * 0.5;
  float downScale = strideX/diffX;
  for (int i = 0; i < size*2; i+=2)
    {
//      Vertex[i] = paintData.xData[i/2 + shift];
//      Vertex[i+1] = paintData.yData[i/2 + shift];
      Vertex[i] = ((paintData.xData[i/2 + shift]/diffX) - downScale) * 0.96;
      Vertex[i+1] = ((paintData.yData[i/2 + shift] - strideY)/diffY) * 0.96;
    }
}


float OpenGLPlot::getGLversion()
{
  GLfloat majver = 0;
  GLfloat minver = 0;
  glGetFloatv(GL_MAJOR_VERSION, &majver);
  glGetFloatv(GL_MINOR_VERSION, &minver);
  return majver + (minver/10);
}


int OpenGLPlot::getGLSLversion()
{

}


/*!
 * \brief OpenGLPlot::addData
 * \param keys
 * \param values
 *
 *
 */
void OpenGLPlot::addData(std::vector<double> &keys, std::vector<double> &values)
{
  if((keys.size() == 0) || (values.size() == 0) || (keys.size() != values.size()))
    {
      return;
    }
  if((paintData.xData.size() != keys.size()) || (paintData.yData.size() != values.size()))
    {
      paintData.xData.resize(keys.size());
      paintData.yData.resize(values.size());
    }

  memmove(paintData.xData.data(), keys.data(), keys.size() * sizeof (double));
  memmove(paintData.yData.data(), values.data(), values.size() * sizeof (double));
  vertexChanged(paintData.xData.size(), 0);
}


void OpenGLPlot::setYRange(double min, double max)
{
  sizeRange.yRange.lower = min;
  sizeRange.yRange.upper = max;
}


void OpenGLPlot::setXRange(double min, double max)
{
  sizeRange.xRange.lower = min;
  sizeRange.xRange.upper = max;
}


/*!
 * \fn OpenGLPlot::setColor
 * \param col can be OGLColor class or preimplemented
 * color from OGL namespace
 *
 * Set color of the graph. Passed parameter can be both
 * OGLColor class or reimplemented color from
 * OGL namespace. If using OGLColor class, you need to
 * call the class constructor OGLColor::OGLColor(), then
 * use OGLColor::setRgbF() and call this function.
 * Also, you can use this function with OGL namespace
 * color like OGL::(color name). Color name can be:
 * black
 * white
 * darkGray
 * gray
 * lightGray
 * red
 * green
 * blue
 * cyan
 * magenta
 * yellow
 * darkRed
 * darkGreen
 * darkBlue
 * darkCyan
 * darkMagenta
 * darkYellow
 * purple
 * brown.
 */
void OpenGLPlot::setColor(OGLColor col)
{
  if(penColor != col)
    {
      penColor = col;
    }
}


void OpenGLPlot::setQColor(QColor col)
{
  if(penQColor != col)
    {
      penQColor = col;
    }
}


//---------------------------------------
// unused for now
void OpenGLPlot::gridVisible(bool state)
{
  showGrid = state;
}


void OpenGLPlot::axisVisible(bool state)
{
  showAxis = state;
}
//
//--------------------------------------


#if defined(QT_CORE_LIB)
void OpenGLPlot::mousePressEvent(QMouseEvent *event)
{
  if(event->button() == Qt::LeftButton)
    {
      mousePressPos = event->pos().x();
    }
}


void OpenGLPlot::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons() & Qt::LeftButton)
    {
      mouseMove = (mousePressPos - event->pos().x()) * ((sizeRange.xRange.upper - sizeRange.xRange.lower)/5000);
      if((sizeRange.xRange.lower + mouseMove > 0) &&
         (sizeRange.xRange.lower + mouseMove < sizeRange.xRange.upper - 15))
        {
          sizeRange.xRange.lower += mouseMove;
          vertexChanged(sizeRange.xRange.upper - sizeRange.xRange.lower, sizeRange.xRange.lower);
          this->update();
        }
      if((sizeRange.xRange.upper + mouseMove < paintData.xData.size()) &&
         (sizeRange.xRange.upper + mouseMove > sizeRange.xRange.lower + 15))
        {
          sizeRange.xRange.upper += mouseMove;
          vertexChanged(sizeRange.xRange.upper - sizeRange.xRange.lower, sizeRange.xRange.lower);
          this->update();
        }
      mousePressPos = event->pos().x();
    }
  event->accept();
}


void OpenGLPlot::wheelEvent(QWheelEvent *event)
{
  int move =  event->angleDelta().y() * ((sizeRange.xRange.upper - sizeRange.xRange.lower)/10000);
  if(sizeRange.xRange.lower + move >= sizeRange.xRange.upper - 15 - move)
    {
      return;
    }
  if(sizeRange.xRange.upper - move <= paintData.xData.size())
    {
      sizeRange.xRange.upper -= move;
    }
  if(sizeRange.xRange.lower + move > 0)
    {
      sizeRange.xRange.lower += move;
    }
  vertexChanged(sizeRange.xRange.upper - sizeRange.xRange.lower, sizeRange.xRange.lower);
  this->update();
  event->accept();
}
#endif


/*!
 * \class OGLColor
 * \brief OGLColor class provides colors based on RGB float values
 *
 * Color is specified in terms of RGB (red, green, blue).
 *
 * OGLColor constructor creates the color based on RGB values.
 * Passed values should be float type because of OpenGL
 * specs. Alternatively, constructor can be called with
 * preimplemented color from OGL namespace.
*/

/*!
 * \brief OGLColor::OGLColor
 *
 * Construct OGLColor class with all components set to zero
 * Red = 0, Green = 0, Blue = 0, Alpha = 0
 */
OGLColor::OGLColor()
{
  rgba.redF = 0;
  rgba.greenF = 0;
  rgba.blueF = 0;
  rgba.alphaF = 0;
}


/*!
 * \brief OGLColor::OGLColor
 * \param col premplemented color from OGL namespace
 *
 * Construct OGLColor class using prepremplemented
 * color from OGL namespace. Color is set using
 * OGL::(color name).
 * Color name can be:
 * black
 * white
 * darkGray
 * gray
 * lightGray
 * red
 * green
 * blue
 * cyan
 * magenta
 * yellow
 * darkRed
 * darkGreen
 * darkBlue
 * darkCyan
 * darkMagenta
 * darkYellow
 * purple
 * brown.
 */
OGLColor::OGLColor(OGL::Colors col)
{
  static const float colors[][4] = {
    {0.0f, 0.0f, 0.0f, 1.0f},     // black
    {1.0f, 1.0f, 1.0f, 1.0f},     // white
    {0.1f, 0.1f, 0.1f, 1.0f},     // darkGray
    {0.5f, 0.5f, 0.1f, 1.0f},     // gray
    {0.7f, 0.7f, 0.7f, 1.0f},     // lightGray
    {1.0f, 0.0f, 0.0f, 1.0f},     // red
    {0.0f, 1.0f, 0.0f, 1.0f},     // green
    {0.0f, 0.0f, 1.0f, 1.0f},     // blue
    {0.5f, 1.0f, 1.0f, 1.0f},     // cyan
    {1.0f, 0.0f, 1.0f, 1.0f},     // magenta
    {1.0f, 1.0f, 0.0f, 1.0f},     // yellow
    {0.5f, 0.0f, 0.0f, 1.0f},     // darkRed
    {0.0f, 0.5f, 0.0f, 1.0f},     // darkGreen
    {0.0f, 0.0f, 0.5f, 1.0f},     // darkBlue
    {0.0f, 0.5f, 0.5f, 1.0f},     // darkCyan
    {0.5f, 0.0f, 0.5f, 1.0f},     // darkMagenta
    {0.5f, 0.5f, 0.0f, 1.0f},     // darkYellow
    {1.0f, 0.0f, 1.0f, 1.0f},     // purple
    {0.5f, 0.25f, 0.0f, 1.0f},    // brown
  };

  setRgbF(
      colors[col][0],
      colors[col][1],
      colors[col][2],
      colors[col][3]);
}


/*!
 * \brief OGLColor::OGLColor
 * \param col OGLColor class
 *
 *
 */
OGLColor::OGLColor(const OGLColor &col)
{
  rgba = col.rgba;
}


/*!
 * \brief OGLColor::~OGLColor
 *
 *
 */
OGLColor::~OGLColor()
{

}


/*!
 * \brief OGLColor::setRgbF Set color in float
 * \param redF
 * \param greenF
 * \param blueF
 * \param alphaF
 *
 *
 */
void OGLColor::setRgbF(float redF, float greenF, float blueF, float alphaF)
{
  if((redF > 1) || (greenF > 1)|| (blueF > 1) || (alphaF > 1))
    {
      return;
    }
  rgba.redF = redF;
  rgba.greenF = greenF;
  rgba.blueF = blueF;
  rgba.alphaF = alphaF;
}


/*!
 * \brief OGLColor::setRgb
 * \param red
 * \param green
 * \param blue
 * \param alpha
 *
 *
 */
void OGLColor::setRgb(int red, int green, int blue, float alpha)
{
  if((red > 255) || (green > 255)|| (blue > 255) || (alpha > 255))
    {
      return;
    }
  rgba.redF = red / 255;
  rgba.greenF = green / 255;
  rgba.blueF = blue / 255;
  rgba.alphaF = alpha / 255;
}


/*!
 * \brief OGLColor::redF
 * \return red component of color. float type
 *
 *
 */
float OGLColor::redF()
{
  return rgba.redF;
}


/*!
 * \brief OGLColor::greenF
 * \return green component of color. float type
 *
 *
 */
float OGLColor::greenF()
{
  return rgba.greenF;
}


/*!
 * \brief OGLColor::blueF
 * \return blue component of color. float type
 *
 *
 */
float OGLColor::blueF()
{
  return rgba.blueF;
}


/*!
 * \brief OGLColor::alphaF
 * \return alpha component of color, or color intensity. float type
 *
 *
 */
float OGLColor::alphaF()
{
  return rgba.alphaF;
}


/*!
 * \brief OGLColor::operator !=
 * \param col
 * \return bool type enum
 *
 *
 */
bool OGLColor::operator!=(const OGLColor &col) const
{
  return !operator==(col);
}


/*!
 * \brief OGLColor::operator ==
 * \param col
 * \return
 *
 *
 */
bool OGLColor::operator==(const OGLColor &col) const
{
return((rgba.redF == col.rgba.redF) &&
       (rgba.greenF == col.rgba.greenF) &&
       (rgba.blueF == col.rgba.blueF) &&
       (rgba.alphaF == col.rgba.alphaF));
}


/*!
 * \brief OGLColor::operator =
 * \param col
 * \return
 *
 *
 */
OGLColor &OGLColor::operator=(const OGLColor &col)
{
  rgba = col.rgba;
  return *this;
}


/*!
 * \brief OGLColor::operator =
 * \param col
 * \return
 *
 *
 */
OGLColor &OGLColor::operator=(OGL::Colors col)
{
  return operator=(OGLColor(col));
}



//----------------------------------------------
//  TESTING TEXT RENDER USING FREETYPE 2
//
FreeTypeFont::FreeTypeFont()
{

}


FreeTypeFont::~FreeTypeFont()
{
  FT_Done_FreeType(ft);
}


void FreeTypeFont::ftInit(const char *font_name)
{
  if(FT_Init_FreeType(&ft))
    {
      printf("Error, can't load freetype\n");
      return;
    }
  if(FT_New_Face(ft, /*"DejaVuMathTeXGyre.ttf"*/ font_name, 0, &face))
    {
      printf("Error, can't load font\n");
      return;
    }
  if(FT_Set_Pixel_Sizes(face, 0, 48))
    {
      printf("Error, can't set size");
      return;
    }
  if(FT_Load_Char(face, '0', FT_LOAD_RENDER))
    {
      printf("Error, can't load glyph\n");
      return;
    }
}

//
//----------------------------------------------

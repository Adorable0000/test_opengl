#include "openglplot.h"
#include <iostream>


BitmapFont Font;    // test
FreeTypeFont fr;    // test
FT_Face face;       // test
FT_Library ft;      // test


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
static const char *fragmentShaderSmooth =
    "#version 200\n"
    "uniform float uLineWidth;\n"
    "uniform vec4 uColor;\n"
    "uniform float uBlendFactor;\n"
    "varying vec2 vLineCenter;\n"
    "void main(void)\n"
    "{\n"
    " vec4 col = uColor;\n"
    " double d = length(vLineCenter-gl_FragCoord.xy);\n"
    " double w = uLineWidth;\n"
    " if (d>w)\n"
    "   col.w = 0;\n"
    " else\n"
    "   col.w *= pow(float((w-d)/w), uBlendFactor);\n"
    " gl_FragColor = col;"
    "}\n";


/*!
 * \brief vertexShaderSmooth shader source code.
 *
 * Calculating the distance from a fragment
 * to the line using the interpolated line
 * center point attribute.
 * Vertex transforming the normalized projected
 * vertex position to the viewport space
 */
static const char *vertexShaderSmooth =
    "#version 200\n"
    "uniform vec2 uViewPort;\n"
    "varying vec2 vLineCenter;\n"
    "void main()\n"
    "{\n"
    " vec4 pp = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    " gl_Position = pp;\n"
    " vec2 vp = uViewPort;\n"
    " vLineCenter = 0.5*(pp.xy + vec2(1, 1))*vp;\n"
    "}\n";


#if defined (QT_CORE_LIB)
OpenGLPlot::OpenGLPlot(QWidget *parent): QOpenGLWidget(parent)
#else
OpenGLPlot::OpenGLPlot()
#endif
{
  dataChanged = false;
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

//----------------------------------------------
//  TESTING TEXT RENDER USING FREETYPE 2
  fr.ftInit();
//
//----------------------------------------------
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


/*!
 * \brief OpenGLPlot::initializeGL OpenGL functions initialization
 *
 *
 */
void OpenGLPlot::initializeGL()
{
#if defined (QT_CORE_LIB)
  initializeOpenGLFunctions();
#endif

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);               // Set background color
  glEnable(GL_DEPTH_TEST);                            // Enable depth test to exclude some odd artifacts
  glDepthFunc(GL_ALWAYS);                             // Element always pass depth test
  glEnable(GL_BLEND);                                 // Enable color mix
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Mix colors using scale func for input and output color to smooth lines
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);             // Set fastest line smoothing
  glEnable(GL_LINE_SMOOTH);                           // Enable line smoothing
  glEnable(GL_ALPHA_TEST);                            // Enable alpha test to use transparency for smoothing

//----------------------------------------------
//  TESTING TEXT RENDER USING BITMAP
//
  if(!Font.Load("/home/smely/test_opengl/sources/openglplot/Microsoft_JhengHei_UI_high_res.bff"))
    {
      printf("Can't load\n");
      return;
    }
//
//----------------------------------------------
}


/*!
 * \brief OpenGLPlot::resizeGL window resize event
 * \param width
 * \param height
 *
 *
 */
void OpenGLPlot::resizeGL(int width, int height)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                                 // Clear render matrix
  glViewport(0, 0, (GLint)width, (GLint)height);    // Change size of render window
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

  dataChanged = true;
}

double wpix;        // test value
double hpix;        // test value
/*!
 * \brief OpenGLPlot::paintGL main paint event
 *
 *
 */
void OpenGLPlot::paintGL()
{
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

//  std::vector<std::vector<GLdouble>> h1;
//  h1.resize(2);
//  h1.at(1).resize(2);
//  h1.at(1).at(1) = 1;
//  printf("%f\n", h1.at(1).at(1));
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

    if(dataChanged)
    {
      if(Vertex.size() != xbounds*2)  {Vertex.resize(xbounds*2);}
      for (int i = 0; i < xbounds*2; i+=2)
        {
          Vertex[i] = paintData.xData[i/2 + xlow];
          Vertex[i+1] = paintData.yData[i/2 + xlow];
        }
      dataChanged = false;
    }

  makeCurrent();                                                  // Change render context
  glMatrixMode(GL_PROJECTION);                                    // Change to projection mode to enable multiplication between current and perspective matrix
  glLoadIdentity();                                               // Clear current render matrix

  glOrtho(xlow-(wpixel*10), xup+(wpixel*10), ylow-(hpixel*18), yup+(hpixel*10), -1, 1);   // Create perspective matrix with pixel based coordinates
//  glOrtho(0,(w),0,(h),-1,1);
  printf("Width %d\n", plotWidth);
  printf("Height %d\n", plotHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);             // Clear current color buffer
  glMatrixMode(GL_MODELVIEW);                                     // Change to object-view matrix
  glLoadIdentity();                                               // Clear current render matrix

  glEnableClientState(GL_VERTEX_ARRAY);                           // Enable vertex matrix

  //-----------------------------------------------------------------
  //  TESTING LINE SMOOTHING WITH SHADER
  //
  GLuint vertexSmoothShader;
  vertexSmoothShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexSmoothShader, 1, &vertexShaderSmooth, NULL);
  glCompileShader(vertexSmoothShader);

  GLuint fragmentSmoothShader;
  fragmentSmoothShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentSmoothShader, 1, &fragmentShaderSmooth, NULL);
  glCompileShader(fragmentSmoothShader);

  GLuint shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexSmoothShader);
  glAttachShader(shaderProgram, fragmentSmoothShader);
  glLinkProgram(shaderProgram);
  //
  //-----------------------------------------------------------------

  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_ALPHA_TEST);
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
  glVertexPointer(2, GL_DOUBLE, 0, Vertex.data());
  glDrawArrays(GL_LINE_STRIP, 0, Vertex.size()/2);
  glDisableClientState(GL_VERTEX_ARRAY);                          // Disable vertex matrix

//--------------------------------------------------------------------------------------------
//  TESTING TEXT RENDER USING BITMAP
//
  glEnable(GL_TEXTURE_2D);
  Font.Select();
//  Font.Print("1, 2, 3, 4, 5, 6, 7, 8, 9, 0", 0, 0);
  Font.Print("1, 2, 3, 4, 5, 6, 7, 8, 9, 0", xlow, ylow-(hpixel*18));     // size of textures is 16 pixels, so we should add 2 pixels
  glDisable(GL_TEXTURE_2D);
//
//--------------------------------------------------------------------------------------------

//  GLint a;
//  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &a);
//  printf("%d\n", a);

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

  paintData.xData.clear();
  paintData.yData.clear();
  if((paintData.xData.size() != keys.size()) || (paintData.yData.size() != values.size()))
    {
      paintData.xData.resize(keys.size());
      paintData.yData.resize(values.size());
    }

  memmove(paintData.xData.data(), keys.data(), keys.size() * sizeof (double));
  memmove(paintData.yData.data(), values.data(), values.size() * sizeof (double));
  dataChanged = true;
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
      mouseMove = (mousePressPos - event->pos().x()) * 4;
      if((sizeRange.xRange.lower + mouseMove > 0) &&
         (sizeRange.xRange.lower + mouseMove < sizeRange.xRange.upper - 15))
        {
          sizeRange.xRange.lower += mouseMove;
          dataChanged = true;
          this->update();
        }
      if((sizeRange.xRange.upper + mouseMove < paintData.xData.size()) &&
         (sizeRange.xRange.upper + mouseMove > sizeRange.xRange.lower + 15))
        {
          sizeRange.xRange.upper += mouseMove;
          dataChanged = true;
          this->update();
        }
      mousePressPos = event->pos().x();
    }
  event->accept();
}


void OpenGLPlot::wheelEvent(QWheelEvent *event)
{
  if(sizeRange.xRange.lower + event->angleDelta().y() >= sizeRange.xRange.upper - 15 - event->angleDelta().y())
    {
      return;
    }
  if(sizeRange.xRange.upper - event->angleDelta().y() <= paintData.xData.size())
    {
      sizeRange.xRange.upper -= event->angleDelta().y();
    }
  if(sizeRange.xRange.lower + event->angleDelta().y() > 0)
    {
      sizeRange.xRange.lower += event->angleDelta().y();
    }
  dataChanged = true;
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
//
//---------------------------------------


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


void FreeTypeFont::ftInit()
{
  if(FT_Init_FreeType(&ft))
    {
      printf("Error, can't load freetype\n");
      return;
    }
  if(FT_New_Face(ft, "/usr/share/fonts/truetype/dejavu/DejaVuMathTeXGyre.ttf", 0, &face))
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


//----------------------------------------------
//  TESTING TEXT RENDER USING BITMAP
//
BitmapFont::BitmapFont()
{
  CellX = 0; CellY = 0; YOffset = 0; RowPitch = 0;
  RowFactor = 0; ColFactor = 0;
  Base = 0;
  RenderStyle = 0;
  TexID = 0;
}

BitmapFont::~BitmapFont()
{

}


#include <fstream>
#define WIDTH_DATA_OFFSET  20 // Offset to width data with BFF file
#define MAP_DATA_OFFSET   276 // Offset to texture image data with BFF file
#define BFG_MAXSTRING 255     // Maximum string length
#define BFG_RS_NONE  0x0      // Blend flags
#define BFG_RS_ALPHA 0x1
#define BFG_RS_RGB   0x2
#define BFG_RS_RGBA  0x4
bool BitmapFont::Load(char *fname)
{
  char *dat, *img;
  std::fstream in;
  unsigned long fileSize;
  char bpp;
  int ImgX, ImgY;

  in.open(fname, std::ios_base::binary | std::ios_base::in);

  if(in.fail())
    {
      return false;
    }

  in.seekg(0, std::ios_base::end);
  fileSize = in.tellg();
  in.seekg(0, std::ios_base::beg);

  dat = new char[fileSize];
  if(!dat)
    {
      return false;
    }

  in.read(dat,fileSize);
  if(in.fail())
    {
      delete [] dat;
      in.close();
      return false;
    }
  in.close();

  if((unsigned char)dat[0] != 0xBF || (unsigned char)dat[1] != 0xF2)
    {
      delete [] dat;
      return false;
    }
  memcpy(&ImgX,&dat[2],sizeof(int));
  memcpy(&ImgY,&dat[6],sizeof(int));
  memcpy(&CellX,&dat[10],sizeof(int));
  memcpy(&CellY,&dat[14],sizeof(int));
  bpp=dat[18];
  Base=dat[19];

  if(fileSize != ((MAP_DATA_OFFSET)+((ImgX*ImgY)*(bpp/8))))
          return false;

  RowPitch=ImgX/CellX;
  ColFactor=(float)CellX/(float)ImgX;
  RowFactor=(float)CellY/(float)ImgY;
  YOffset=CellY;

  // Determine blending options based on BPP
   switch(bpp)
    {
     case 8: // Greyscale
      RenderStyle=BFG_RS_ALPHA;
      break;

     case 24: // RGB
      RenderStyle=BFG_RS_RGB;
      break;

     case 32: // RGBA
      RenderStyle=BFG_RS_RGBA;
      break;

     default: // Unsupported BPP
      delete [] dat;
      return false;
      break;
    }

  img=new char[(ImgX*ImgY)*(bpp/8)];
  if(!img)
   {
    delete [] dat;
    return false;
   }

  // Grab char widths
  memcpy(Width,&dat[WIDTH_DATA_OFFSET],256);
  // Grab image data
  memcpy(img,&dat[MAP_DATA_OFFSET],(ImgX*ImgY)*(bpp/8));
  // Create Texture
  glGenTextures(1,&TexID);
  glBindTexture(GL_TEXTURE_2D,TexID);
  // Fonts should be rendered at native resolution so no need for texture filtering
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  // Stop chararcters from bleeding over edges
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_LINEAR_MIPMAP_LINEAR);

  switch(RenderStyle)
   {
    case BFG_RS_ALPHA:
     glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,ImgX,ImgY,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,img);
     break;

    case BFG_RS_RGB:
     glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,ImgX,ImgY,0,GL_RGB,GL_UNSIGNED_BYTE,img);
     break;

    case BFG_RS_RGBA:
     glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,ImgX,ImgY,0,GL_RGBA,GL_UNSIGNED_BYTE,img);
     break;
   }

  // Clean up
  delete [] img;
  delete [] dat;

  return true;
}


// Prints text at a specifed position, again cursor is updated
void BitmapFont::Print(char* Text, double x, double y)
 {
  int sLen,Loop;
  int Row,Col;
  float U,V,U1,V1;

  double CurX=x;
  double CurY=y;

  sLen=(int)strnlen(Text,BFG_MAXSTRING);

  glBegin(GL_QUADS);
  int divider = 16;   // to reduce size of texture to 16 pixels, current texture size is 256
  for(Loop=0;Loop!=sLen;++Loop)
   {
    Row=(Text[Loop]-Base)/RowPitch;
    Col=(Text[Loop]-Base)-Row*RowPitch;

    U=Col*ColFactor;
    V=Row*RowFactor;
    U1=U+ColFactor;
    V1=V+RowFactor;

//     glTexCoord2f(U, V1); glVertex2i(CurX,      CurY);
//     glTexCoord2f(U1,V1); glVertex2i((CurX+CellX), CurY);
//     glTexCoord2f(U1,V ); glVertex2i((CurX+CellX), (CurY+CellY));
//     glTexCoord2f(U, V ); glVertex2i(CurX,      (CurY+CellY));

    glTexCoord2f(U, V1); glVertex2f( (CurX/divider * wpix) , ((CurY)/divider * hpix)+CurY);
    glTexCoord2f(U1,V1); glVertex2f( ((CurX + CellX)/divider * wpix), (CurY/divider * hpix)+CurY);
    glTexCoord2f(U1,V ); glVertex2f( ((CurX + CellX)/divider * wpix), ((CurY + CellY)/divider * hpix)+CurY );
    glTexCoord2f(U, V ); glVertex2f( (CurX/divider * wpix), ((CurY + CellY)/divider * hpix)+CurY );
    CurX+=(Width[Text[Loop]]);
   }
  glEnd();
 }


void BitmapFont::Select()
 {
  Bind();
  glColor3f(0,0,0);
  SetBlend();
 }

void BitmapFont::Bind()
 {
  glBindTexture(GL_TEXTURE_2D,TexID);
 }

void BitmapFont::SetBlend()
 {
  switch(RenderStyle)
   {
    case BFG_RS_ALPHA: // 8Bit
     glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
     glEnable(GL_BLEND);
     break;

    case BFG_RS_RGB:   // 24Bit
     glDisable(GL_BLEND);
     break;

    case BFG_RS_RGBA:  // 32Bit
     glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
     glEnable(GL_BLEND);
     break;
   }
 }
//
//----------------------------------------------

#if defined (__linux__)
#include <unistd.h>
#elif (WIN32)
#include <direct.h>
#endif
std::string get_working_path()
{
  char dir[256];
  return(getcwd(dir, 256) ? std::string(dir) : std::string(""));
}

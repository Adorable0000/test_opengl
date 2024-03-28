#include "openglplot.h"
#include <iostream>
#include <vector>


OpenGLPlot::OpenGLPlot(QWidget *parent): QOpenGLWidget(parent)
{
  dataChanged = false;
  showGrid = false;
  showAxis = false;
  sizeAxis.xRange.lower = 0;
  sizeAxis.xRange.upper = 15;
  sizeAxis.yRange.lower = 0;
  sizeAxis.yRange.upper = 15;
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


void OpenGLPlot::initializeGL()
{
  glClearColor(1,1,1,1);                      // Set white background color
  glEnable(GL_DEPTH_TEST);                            // Enable depth test to exclude some odd artifacts
  glDepthFunc(GL_ALWAYS);                             // Element always pass depth test
  glEnable(GL_BLEND);                                 // Enable color mix
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Mix colors using scale func for input and output color to smooth lines
  glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);            // Set fastest line smoothing
  glEnable(GL_LINE_SMOOTH);                           // Enable line smoothing
  glEnable(GL_ALPHA_TEST);                            // Enable alpha test to use transparency for smoothing
}

int w, h;
void OpenGLPlot::resizeGL(int width, int height)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                                 // Clear render matrix
  glViewport(0, 0, (GLint)width, (GLint)height);    // Change size of render window
  w = width; h = height;
  lines_width = width;
  lines_height = height;

//  Keep grid lines sizes divisible by 4 so that
//  last line will always touch the higher line
//  because 1 line need 4 numbers
  int mar_h = lines_height % 4;
  if(mar_h > 0)
    {
      lines_height -= mar_h;
    }

  int mar_w = lines_width % 4;
  if(mar_w > 0)
    {
      lines_width -= mar_w;
    }

  dataChanged = true;
}


void OpenGLPlot::paintGL()
{
  double xlow = sizeAxis.xRange.lower;
  double xup = sizeAxis.xRange.upper;
  double xbounds = xup - xlow;
  double ylow = sizeAxis.yRange.lower;
  double yup = sizeAxis.yRange.upper;
  double ybounds = yup - ylow;
  double hsize = lines_height;
  double wsize = lines_width;
  double wpixel = xbounds/wsize;
  double hpixel = ybounds/hsize;

/*  if(xbounds < 10)
    {
      return;
    }

  std::vector<GLdouble> hLine1;
  hLine1.resize(wsize);

  std::vector<GLdouble> hLine2;
  hLine2.resize(wsize);

  std::vector<GLdouble> hLine3;
  hLine3.resize(wsize);

  std::vector<GLdouble> hLine4;
  hLine4.resize(2*2);

  std::vector<GLdouble> hLine5;
  hLine5.resize(wsize);

  std::vector<GLdouble> vLine1;
  vLine1.resize(hsize);

  std::vector<GLdouble> vLine2;
  vLine2.resize(2*2);

  std::vector<GLdouble> vLine3;
  vLine3.resize(hsize);

//  std::vector<std::vector<GLdouble>> h1;
//  h1.resize(2);
//  h1.at(1).resize(2);
//  h1.at(1).at(1) = 1;
//  printf("%f\n", h1.at(1).at(1));

  hLine4[0] = xlow;
  hLine4[1] = ylow;
  hLine4[2] = xup;
  hLine4[3] = ylow;

  for(int i = 0; i < wsize; i+=2)
    {
      hLine1[i] = xlow + i * wpixel;
      hLine1[i+1] = (yup + ylow)/2;

      hLine2[i] = xlow + i * wpixel;
      hLine2[i+1] = (((yup + ylow)/2) + yup)/2;

      hLine3[i] = xlow + i * wpixel;
      hLine3[i+1] = (((yup + ylow)/2) + ylow)/2;

      hLine5[i] = xlow + i * wpixel;
      hLine5[i+1] = yup;
    }

  for(int i = 0; i < hsize; i+=2)
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
*/
  makeCurrent();                                                  // Change render context
  glMatrixMode(GL_PROJECTION);                                    // Change to projection mode to enable multiplication between current and perspective matrix
  glLoadIdentity();                                               // Clear current render matrix

//  glOrtho(xlow-(wpixel*10), xup+(wpixel*10), ylow-(hpixel*10), yup+(hpixel*10), -1, 1);   // Create perspective matrix with pixel based coordinates
  glOrtho(0,(w),0,(h),-1,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);             // Clear current color buffer
  glMatrixMode(GL_MODELVIEW);                                     // Change to object-view matrix
  glLoadIdentity();                                               // Clear current render matrix

/*  glEnableClientState(GL_VERTEX_ARRAY);                           // Enable vertex matrix

  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_ALPHA_TEST);

  glColor4f(0,0,0,0.5);

  glVertexPointer(2, GL_DOUBLE, 0, vLine1.data());
  glDrawArrays(GL_LINES, 0, vLine1.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, vLine3.data());
  glDrawArrays(GL_LINES, 0, vLine3.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, hLine1.data());
  glDrawArrays(GL_LINES, 0, hLine1.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, hLine2.data());
  glDrawArrays(GL_LINES, 0, hLine2.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, hLine3.data());
  glDrawArrays(GL_LINES, 0, hLine3.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, hLine5.data());
  glDrawArrays(GL_LINES, 0, hLine5.size()/2);

  glColor4f(0,0,0,0.6);
  glVertexPointer(2, GL_DOUBLE, 0, hLine4.data());
  glDrawArrays(GL_LINE_STRIP, 0, hLine4.size()/2);

  glVertexPointer(2, GL_DOUBLE, 0, vLine2.data());
  glDrawArrays(GL_LINE_STRIP, 0, vLine2.size()/2);

  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_ALPHA_TEST);

  glColor4f(penColor.red(), penColor.green(), penColor.blue(),penColor.alpha());   // Set texture color
  glVertexPointer(2, GL_DOUBLE, 0, Vertex.data());
  glDrawArrays(GL_LINE_STRIP, 0, Vertex.size()/2);
  glDisableClientState(GL_VERTEX_ARRAY);                          // Disable vertex matrix
*/

  //--------------------------------------------------------------------------------------------
  //  TESTING TEXT RENDER USING BITMAP
  //
  BitmapFont Font;
  if(!Font.Load("/home/smely/test_opengl/sources/openglplot/Microsoft_JhengHei_UI_256_128.bff"))
    {
      printf("Can't load\n");
      return;
    }
  glColor4f(0,0,0,0);
  Font.Select();
  Font.Print("1, 2, 3, 4, 5, 6, 7, 8, 9, 0", 0, 0);
  //
  //--------------------------------------------------------------------------------------------


//  GLint a;
//  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &a);
//  printf("%d\n", a);
}


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
  sizeAxis.yRange.lower = min;
  sizeAxis.yRange.upper = max;
}


void OpenGLPlot::setXRange(double min, double max)
{
  sizeAxis.xRange.lower = min;
  sizeAxis.xRange.upper = max;
}


void OpenGLPlot::setColor(QColor col)
{
  if(penColor != col)
    {
      penColor = col;
    }
}


//  Unused for now  --------------------
void OpenGLPlot::gridVisible(bool state)
{
  showGrid = state;
}


void OpenGLPlot::axisVisible(bool state)
{
  showAxis = state;
}
//  -----------------------------------


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
      if((sizeAxis.xRange.lower + mouseMove > 0) &&
         (sizeAxis.xRange.lower + mouseMove < sizeAxis.xRange.upper - 15))
        {
          sizeAxis.xRange.lower += mouseMove;
          dataChanged = true;
          this->update();
        }
      if((sizeAxis.xRange.upper + mouseMove < paintData.xData.size()) &&
         (sizeAxis.xRange.upper + mouseMove > sizeAxis.xRange.lower + 15))
        {
          sizeAxis.xRange.upper += mouseMove;
          dataChanged = true;
          this->update();
        }
      mousePressPos = event->pos().x();
    }
  event->accept();
}


void OpenGLPlot::wheelEvent(QWheelEvent *event)
{
  if(sizeAxis.xRange.lower + event->angleDelta().y() >= sizeAxis.xRange.upper - 15 - event->angleDelta().y())
    {
      return;
    }
  if(sizeAxis.xRange.upper - event->angleDelta().y() <= paintData.xData.size())
    {
      sizeAxis.xRange.upper -= event->angleDelta().y();
    }
  if(sizeAxis.xRange.lower + event->angleDelta().y() > 0)
    {
      sizeAxis.xRange.lower += event->angleDelta().y();
    }
  dataChanged = true;
  this->update();
  event->accept();
}


//----------------------------------------------
//  TESTING TEXT RENDER USING FREETYPE 2
//
FreeTypeFont::FreeTypeFont()
{
  if(FT_Init_FreeType(&ft))
    {
      printf("Error, can't load freetype\n");
    }
}


FreeTypeFont::~FreeTypeFont()
{
  FT_Done_FreeType(ft);
}

void FreeTypeFont::ftInit()
{
  if(FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
    {
      printf("Error, can't load font\n");
    }
  FT_Set_Pixel_Sizes(face, 0, 48);
  if(FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
      printf("Error, can't load glyph\n");
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
  CurX=CurY=0;
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
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  // Stop chararcters from bleeding over edges
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);

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
void BitmapFont::Print(char* Text, int x, int y)
 {
  int sLen,Loop;
  int Row,Col;
  float U,V,U1,V1;

  CurX=x;
  CurY=y;

  sLen=(int)strnlen(Text,BFG_MAXSTRING);

  glBegin(GL_QUADS);

   for(Loop=0;Loop!=sLen;++Loop)
    {
     Row=(Text[Loop]-Base)/RowPitch;
     Col=(Text[Loop]-Base)-Row*RowPitch;

     U=Col*ColFactor;
     V=Row*RowFactor;
     U1=U+ColFactor;
     V1=V+RowFactor;

     glTexCoord2f(U, V1);  glVertex2i(CurX,      CurY);
     glTexCoord2f(U1,V1);  glVertex2i(CurX+CellX,CurY);
     glTexCoord2f(U1,V ); glVertex2i(CurX+CellX,CurY+CellY);
     glTexCoord2f(U, V ); glVertex2i(CurX,      CurY+CellY);

     CurX+=Width[Text[Loop]];
    }
   glEnd();
 }


void BitmapFont::Select()
 {
  glEnable(GL_TEXTURE_2D);
  Bind();
  SetBlend();
 }

void BitmapFont::Bind()
 {
  glBindTexture(GL_TEXTURE_2D,TexID);
 }

void BitmapFont::SetBlend()
 {
  glColor3f(0,0,0);

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

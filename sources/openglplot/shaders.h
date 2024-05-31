#ifndef SHADERS_H
#define SHADERS_H


enum ShaderType {
  vertex,
  fragment,
  geometry
};

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


static const char *vertexShaderSmooth_330 =
    "#version 330\n"
    "uniform vec4 uViewPort;\n"
    "uniform mat4 ModelViewProjectionMatrix;\n"
    "layout(location = 0) in vec2 Vertex;\n"
    "out vec2 vLineCenter;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(Vertex, 0.0f, 1.0f);\n"
    " vLineCenter = 0.5f*(gl_Position.xy + vec2(1, 1)) * uViewPort.xy;\n"
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
    "uniform double uLineWidth;\n"
    "uniform vec4 uColor;\n"
    "uniform double uBlendFactor;\n"
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
    "   col.w *= vec4 pow(double((w-d)/w), uBlendFactor);\n"
    " }\n"
    " gl_FragColor = col;"
    "}\n";


static const char *fragmentShaderSmooth_330 =
    "#version 330\n"
    "uniform float uBlendFactor;\n"
    "uniform vec4 uColor;\n"
    "uniform float uLineWidth;\n"
    "in vec2 vLineCenter;\n"
    "out vec4 color;\n"
    "void main(void)\n"
    "{\n"
    " color = uColor;\n"
    " float d = float(length(vLineCenter-gl_FragCoord.xy));\n"
    " if (d>uLineWidth){\n"
    "   color.w = 0;\n"
    " }\n"
    " else{\n"
    "   color.w *= pow(float((uLineWidth-d)/uLineWidth), 2);\n"
    " }\n"
    "}\n";


struct Shaders {
  int type;
  float gl_min_ver;
  const char* shader;
};


struct Shaders Shaders[] = {
  {vertex,   2.1, vertexShaderSmooth_120},
  {fragment, 2.1, fragmentShaderSmooth_120},
  {vertex,   3.3, vertexShaderSmooth_330},
  {fragment, 3.3, fragmentShaderSmooth_330},
};


#endif // SHADERS_H

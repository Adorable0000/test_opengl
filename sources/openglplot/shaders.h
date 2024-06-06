#ifndef SHADERS_H
#define SHADERS_H


static const char *vertexShader_330 =
    "#version 330\n"
    "uniform mat4 ModelViewProjectionMatrix;\n"
    "layout(location = 0) in vec4 Vertex;\n"
    "layout(location = 1) in vec4 Color;\n"
    "out vec4 mColor;\n"
    "void main(void)\n"
    "{\n"
    "   mColor = Color;\n"
    "   gl_Position = ModelViewProjectionMatrix * Vertex;\n"
    "}\n";


static const char *geometryShader_330 =
    "version 330\n"
    "";


static const char *fragmentShader_330 =
    "#version 330\n"
    "in vec4 mColor;\n"
    "void main(void)\n"
    "{\n"
    "   gl_FragColor = VertexIn.mColor;\n"
    "}\n";


#endif // SHADERS_H

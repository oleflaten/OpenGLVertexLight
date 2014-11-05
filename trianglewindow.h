#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include "openglwindow.h"
#include <QtGui/QOpenGLShaderProgram>

class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();
    ~TriangleWindow();

    void initialize();
    void render();

private:
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_normalAttr;
    GLuint m_mvMatrixUniform;
    GLuint m_pMatrixUniform;
    GLuint m_nMatrixUniform;
    GLuint m_lightPosUniform;
    GLuint m_ambientColorUniform;
    GLuint m_diffuseColorUniform;
    GLuint m_specularColorUniform;

    QVector3D lightPos = QVector3D(10.0f, 0.0f, 30.0f);
    //QVector3D ambientColor= QVector3D(0.3f, 1.0f, 1.0f);
    //QVector3D ambientColor= QVector3D(0.9f, 0.9f, 1.0f);
    //QVector3D ambientColor= QVector3D(1.0f, 1.0f, 1.0f);
    QVector3D ambientColor= QVector3D(0.1f, 0.1f, 0.1f);
    QVector3D diffuseColor = QVector3D(0.7f, 0.7f, 0.7f);
    QVector3D specularColor = QVector3D(1.0f, 1.0f, 1.0f);

    QOpenGLShaderProgram *m_program;

    int m_frame;

    float lousyTimer = 0.0; //Cheap timer to get some variants in rendering

    //VBOs:
    GLuint vboIds[2];

    //Geometry:
    GLfloat vertices[27] =
    {
         0.0f, 0.707f, 0.0f,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   //vertex 1
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   //vertex 2
         0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f    //vertex 3
    };

    GLushort indices[3] =
    {
        0, 1, 2
    };

//    GLfloat light[] =
//    {


//    }

};

#endif // TRIANGLEWINDOW_H

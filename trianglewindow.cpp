#include "trianglewindow.h"

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QMatrix4x4>
#include <QtGui/QScreen>
#include <QtCore/qmath.h>
//#include <QTime>

#include <QDebug>

TriangleWindow::TriangleWindow()
    : m_program(0)
    , m_frame(0)
{
}

TriangleWindow::~TriangleWindow()
{
    //Don't know if this is absolutely necessary
    glDeleteBuffers(2, vboIds);
}

void TriangleWindow::initialize()
{
    //qDebug("init");

    //Read and compile shaders:
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment.glsl");
    m_program->link();

    //
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_normalAttr = m_program->attributeLocation("normalAttr");
    m_mvMatrixUniform = m_program->uniformLocation("mvMatrix");
    m_pMatrixUniform = m_program->uniformLocation("pMatrix");
    m_nMatrixUniform = m_program->uniformLocation("nMatrix");

    m_lightPosUniform = m_program->uniformLocation("lightPosition");
    m_ambientColorUniform = m_program->uniformLocation("ambientColor");
    m_diffuseColorUniform = m_program->uniformLocation("diffuseColor");
    m_specularColorUniform = m_program->uniformLocation("specularColor");

    //Generate buffers
    glGenBuffers(2, vboIds);

    // Transfer vertex data to VBO 0
    //GL_ARRAY_BUFFER for vertices
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    //size = size in bytes of all the vertices.
    //1 vertex = 3 floats + 3 color floats + 3 normal floats
    //GL_STATIC_DRAW - not a dynamic mesh
    glBufferData(GL_ARRAY_BUFFER, 3 * 9 * sizeof(GL_FLOAT), vertices, GL_STATIC_DRAW);
    //qDebug() << "Size of Float: " << sizeof(GL_FLOAT);
    //qDebug() << "vboIds[0] " << vboIds[0];

    // Transfer index data to VBO 1
    //GL_ELEMENT_ARRAY_BUFFER for indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLushort), indices, GL_STATIC_DRAW);
    //qDebug() << "Size of GLushort: " << sizeof(GLushort);
}

void TriangleWindow::render()
{
    // Tell OpenGL which VBOs to use
    // Not necessary just now, because they are bound already
    //glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);

    const qreal retinaScale = devicePixelRatio();

    //Do this each rendered frame to support resizing
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    //Clear screen and buffers
    glClearColor(0.1, 0.1, 0.1, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Binds the shader
    m_program->bind();

    //making projection Matrix:
    QMatrix4x4 pMatrix;
    //                  angle, aspect,    near, far
    pMatrix.perspective(40.0f, 4.0f/3.0f, 0.1f, 100.0f);
    m_program->setUniformValue(m_pMatrixUniform, pMatrix);

    //making model view Matrix
    QMatrix4x4 mvMatrix;

    mvMatrix.translate(0, 0, -3);
    mvMatrix.rotate(50.0f * m_frame / screen()->refreshRate(), 0, 1, 0);
    //sending matrix to shader
    m_program->setUniformValue(m_mvMatrixUniform, mvMatrix);

    //matrix for normals - inverted mv-matrix
    //bool *test;
    QMatrix3x3 nMatrix = mvMatrix.normalMatrix();
    //hooking matrix to shader
    m_program->setUniformValue(m_nMatrixUniform, nMatrix);

    //set up light
    //uses lousy timer to vary ambient light
    lousyTimer+= 0.02;
    QVector3D tempAmbient = ambientColor;
    tempAmbient.operator *=(qFabs(qSin(lousyTimer)));

    m_program->setUniformValue(m_lightPosUniform, lightPos);
    //m_program->setUniformValue(m_ambientColorUniform, tempAmbient);
    m_program->setUniformValue(m_ambientColorUniform, ambientColor);
    m_program->setUniformValue(m_diffuseColorUniform, diffuseColor);
    m_program->setUniformValue(m_specularColorUniform, specularColor);

    //set up vertex info
    //I forgot to enable normals, and spent 2 weeks looking through
    //my shaders and the math. And this was the error....
    m_program->enableAttributeArray(m_posAttr);
    m_program->enableAttributeArray(m_colAttr);
    m_program->enableAttributeArray(m_normalAttr);

    quintptr offset = 0;
    //vertices:
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), (const void *)offset);

    //colors:
    offset += 3*sizeof(GLfloat);
    //qDebug() << "offset" << offset;
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), (const void *)offset);

    //normals:
    offset += 3*sizeof(GLfloat);
    //qDebug() << "offset" << offset;
    glVertexAttribPointer(m_normalAttr, 3, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), (const void *)offset);

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    //Just 3 vertices:
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

    m_program->release();

    ++m_frame;
}

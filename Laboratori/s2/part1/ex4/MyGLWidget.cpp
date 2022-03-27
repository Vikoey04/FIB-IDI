// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[])
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502:
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::paintGL() {
  projectTransform();
  viewTransform();
  modelTransform();

  glClear (GL_COLOR_BUFFER_BIT);
  glBindVertexArray(VAO_HomerProves);
  glDrawArrays(GL_TRIANGLES, 0, HomerProves.faces().size() * 3);
  glBindVertexArray(0);

  //BL2GLWidget::paintGL();
}

void MyGLWidget::carregaShaders() {
  BL2GLWidget::carregaShaders();
  projLoc = glGetUniformLocation (program -> programId(), "proj");
  viewLoc = glGetUniformLocation (program -> programId(), "view");
}

void MyGLWidget::projectTransform() {
  // glm::perspective (FOV en rad, RA window, znear, zfar)
  glm::mat4 Proj = glm::perspective (float(M_PI)/2.0f, 1.0f, 0.4f, 3.0f);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform () {
  // glm::lookAt (OBS, VRP, UP)
  glm::mat4 View = glm::lookAt (glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

// Afegim funcio createBuffers
void MyGLWidget::createBuffers() {
  // HomerProves
  HomerProves.load("/home2/users/alumnes/1264129/dades/IDI/models/HomerProves.obj");

  // Creació del Vertex Array Object per pintar (VAO)
  glGenVertexArrays(1, &VAO_HomerProves);
  glBindVertexArray(VAO_HomerProves);

  GLuint VBO_HomerProvesPos;
  glGenBuffers(1, &VBO_HomerProvesPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_HomerProvesPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * HomerProves.faces().size() * 3 * 3,
      HomerProves.VBO_vertices(), GL_STATIC_DRAW); //posició

  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_HomerProvesPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * HomerProves.faces().size() * 3 * 3,
      HomerProves.VBO_matdiff(), GL_STATIC_DRAW); //color

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
}

void MyGLWidget::initializeGL() {
  HomerProves.load("/home2/users/alumnes/1264129/dades/IDI/models/HomerProves.obj");
  BL2GLWidget::initializeGL();
}

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

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_F:
      if (focusDeCamera) { // passem a Focus d'Escena
        focusDeCamera = false;
        posF = glm::vec3(1.0, 1.0, 1.0);
      } else {             // passem a Focus de Camera
        focusDeCamera = true;
        posF = glm::vec3(0.0, 0.0, 0.0);
      }
      posicioFocus();
      break;
    
    case Qt::Key_K:
      if (posF.x > -3.0) {
        posF.x -= 0.1;
        posicioFocus();
      }
      break;
    
    case Qt::Key_L:
      if (posF.x < 3.0) {
        posF.x += 0.1;
        posicioFocus();
      }
      break;
  
    default: BL3GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::initializeGL() {
  BL3GLWidget::initializeGL();

  // Uniforms del focus
  posFocusLoc     = glGetUniformLocation (program->programId(), "posFocus");
  colFocusLoc     = glGetUniformLocation (program->programId(), "colFocus");
  llumAmbientLoc  = glGetUniformLocation (program->programId(), "llumAmbient");

  focusDeCamera = true;
  posF = glm::vec3(0.0, 0.0, 0.0);

  posicioFocus();
  colorFocus();
  llumAmbient();
}

void MyGLWidget::posicioFocus()
{
  glm::vec3 posFres;
  if (!focusDeCamera) { // focus d'Escena
    posFres = View * glm::vec4(posF, 1.0);
    posFres = glm::vec3(posFres.x, posFres.y, posFres.z);
  }
  else {                // focus de Camera
    posFres = posF;
  }

  glUniform3fv (posFocusLoc, 1, &posFres[0]);
}

void MyGLWidget::colorFocus()
{
  glm::vec3 colF = glm::vec3(0.8, 0.8, 0.8);
  glUniform3fv (colFocusLoc, 1, &colF[0]);
}

void MyGLWidget::llumAmbient()
{
  glm::vec3 llum = glm::vec3(0.2, 0.2, 0.2);
  glUniform3fv (llumAmbientLoc, 1, &llum[0]);
}

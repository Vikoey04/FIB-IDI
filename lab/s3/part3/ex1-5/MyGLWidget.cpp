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
    case Qt::Key_F: {
      if (focusDeCamera) { // passem a Focus d'Escena
        focusDeCamera = false;
        posF = glm::vec3(1.0, 1.0, 1.0);
      } else {             // passem a Focus de Camera
        focusDeCamera = true;
        posF = glm::vec3(0.0, 0.0, 0.0);
      }
      posicioFocus();
      break;
    }
    case Qt::Key_K: {
      if (posF.x > -3.0) {
        posF.x -= 0.1;
        posicioFocus();
      }
      break;
    }
    case Qt::Key_L: {
      if (posF.x < 3.0) {
        posF.x += 0.1;
        posicioFocus();
      }
      break;
    }
    case Qt::Key_1: {
      if (F1_ON) { F1_ON = false; }
      else { F1_ON = true; }
      interruptorF1();
      break;
    }
    case Qt::Key_2: {
      if (F2_ON) { F2_ON = false; }
      else { F2_ON = true; }
      interruptorF2();
      break;
    }
    case Qt::Key_Left: {
      movPatX -= 0.1f;
      posF2[0] -= 0.1f;
      posicioFocus2();
      break;
    }
    case Qt::Key_Right: {
      movPatX += 0.1f;
      posF2[0] += 0.1f;
      posicioFocus2();
      break;
    }
    case Qt::Key_Up: {
      movPatY += 0.1f;
      posF2[1] += 0.1f;
      posicioFocus2();
      break;
    }
    case Qt::Key_Down: {
      movPatY -= 0.1f;
      posF2[1] -= 0.1f;
      posicioFocus2();
      break;
    }
    case Qt::Key_G: { // Exercici 4 per canviar a groc
      if (focEscGroc) {
        focEscGroc = false;
        colorFocus2();
      }
      else {
        focEscGroc = true;
        colorFocus2();
      }
      break;
    }
    case Qt::Key_B: {
      if (bfculling) {
        bfculling = false;
        glDisable(GL_CULL_FACE);
      }
      else {
        bfculling = true;
        glEnable(GL_CULL_FACE);
      }
      break;
    }
    default: BL3GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::modelTransformPatricio ()
{
  TG = glm::mat4(1.f); 
  TG = glm::translate(TG, glm::vec3(movPatX, movPatY, 0.0f));
  TG = glm::scale(TG, glm::vec3(escala*0.3/2, escala*0.3/2, escala*0.3/2));
  TG = glm::translate(TG, -centrePatr);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::initializeGL() {
  BL3GLWidget::initializeGL();

  // Uniforms del focus
  posFocusLoc     = glGetUniformLocation (program->programId(), "posFocus");
  colFocusLoc     = glGetUniformLocation (program->programId(), "colFocus");
  llumAmbientLoc  = glGetUniformLocation (program->programId(), "llumAmbient");

  posFocusLoc2     = glGetUniformLocation (program->programId(), "posFocus2");
  colFocusLoc2     = glGetUniformLocation (program->programId(), "colFocus2");

  focusDeCamera = true;

  F1_ON = true;
  F2_ON = true;
  focEscGroc = false;
  bfculling = false;

  movPatX = 0;
  movPatY = 0;

  posF = glm::vec3(0.0, 0.0, 0.0);
  posF2 = glm::vec3(1, 0.8, 1);

  posicioFocus();
  colorFocus();
  llumAmbient();

  posicioFocus2();
  colorFocus2();
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

void MyGLWidget::posicioFocus2()
{
  glm::vec3 posFres;
  posFres = View * glm::vec4(posF2, 1.0);
  posFres = glm::vec3(posFres.x, posFres.y, posFres.z);

  glUniform3fv (posFocusLoc2, 1, &posFres[0]);
}

void MyGLWidget::colorFocus2()
{
  glm::vec3 colF;
  if (focEscGroc) {
    colF = glm::vec3(0.8, 0.8, 0);
  } 
  else {
    colF = glm::vec3(0.8, 0.8, 0.8);
  }
  glUniform3fv (colFocusLoc2, 1, &colF[0]);
}

void MyGLWidget::interruptorF1() {
  if (F1_ON) {
    colorFocus();
  }
  else {
    glm::vec3 colF = glm::vec3(0,0,0);
    glUniform3fv (colFocusLoc, 1, &colF[0]);
  }
}

void MyGLWidget::interruptorF2() {
  if (F2_ON) {
    colorFocus2();
  }
  else {
    glm::vec3 colF = glm::vec3(0,0,0);
    glUniform3fv (colFocusLoc2, 1, &colF[0]);
  }
}
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
      // ...
    break;
	}
  case Qt::Key_Right: {
    rotacio -= 30;
    modelTransformPatricio();
    break;
	}
  case Qt::Key_Left: {
    rotacio += 30;
    modelTransformPatricio();
    break;
	}
  case Qt::Key_C: {
    interruptorC();
    break;
  }
  case Qt::Key_E: {
    interruptorE();
    break;
  }
  case Qt::Key_P: {
    interruptorP();
    break;
  }
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::initializeGL() {
  LL4GLWidget::initializeGL();

  posFocLoc1 = glGetUniformLocation (program->programId(), "posFocus1");
  colFocLoc1 = glGetUniformLocation (program->programId(), "colFocus1");
  posFocLoc2 = glGetUniformLocation (program->programId(), "posFocus2");
  colFocLoc2 = glGetUniformLocation (program->programId(), "colFocus2");
  posFocLoc3 = glGetUniformLocation (program->programId(), "posFocus3");
  colFocLoc3 = glGetUniformLocation (program->programId(), "colFocus3");

  rotacio = 0.0;

  C_ON = false;
  E_ON = false;
  P_ON = false;

  veczero = glm::vec3(0.0, 0.0, 0.0);
  col1 = glm::vec3(0.9, 0.0, 0.9);
  col2 = glm::vec3(0.9, 0.9, 0.9);
  col3 = glm::vec3(0.9, 0.9, 0.2);

  glm::vec3 posF;
  posF = glm::vec3(0.0, 0.0, 0.0);
  glUniform3fv(posFocLoc1, 1, &posF[0]); // Focus de càmera
  posF = glm::vec3(10.0, 3.0, 5.0);
  glUniform3fv(posFocLoc2, 1, &posF[0]); // Focus d'escena

  interruptorC();
  interruptorE();
  interruptorP();
}

// També fem la transformació del focusP (el del cap del Patricio)
void MyGLWidget::modelTransformPatricio()
{
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,0,2));

  TG = glm::translate(TG, glm::vec3(0,0,3));
  TG = glm::rotate(TG, float(rotacio*M_PI/180.f), glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(0,0,-3));

  //  Aprofitem la TG amb algunes transformacions per a calcular el
  //  punt on ha d'anar el focus quan rotem el Patricio
  glm::vec4 aux1 = TG * glm::vec4(glm::vec3(0, 3, 0), 1);
  glm::vec3 aux2 = glm::vec3(aux1.x, aux1.y, aux1.z);     // Nova posició del focusP
  glUniform3fv (posFocLoc3, 1, &aux2[0]);                 // Passem el valor al uniform
  //---------------------------------------------------------------

  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  TG = glm::translate(TG, -centreBasePatr);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::interruptorC() {
  if (C_ON) {
    glUniform3fv (colFocLoc1, 1, &veczero[0]);
    C_ON = false;
  }
  else {
    glUniform3fv (colFocLoc1, 1, &col1[0]);
    C_ON = true;
  }
}

void MyGLWidget::interruptorE() {
  if (E_ON) {
    glUniform3fv (colFocLoc2, 1, &veczero[0]);
    E_ON = false;
  }
  else {
    glUniform3fv (colFocLoc2, 1, &col2[0]);
    E_ON = true;
  }
}

void MyGLWidget::interruptorP() {
  if (P_ON) {
    glUniform3fv (colFocLoc3, 1, &veczero[0]);
    P_ON = false;
  }
  else {
    glUniform3fv (colFocLoc3, 1, &col3[0]);
    P_ON = true;
  }
}
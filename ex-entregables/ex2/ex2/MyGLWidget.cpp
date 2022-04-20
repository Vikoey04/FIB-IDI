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

MyGLWidget::~MyGLWidget()
{

}

// Reescrbim paintGL de LL2
void MyGLWidget::paintGL ()
{
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Patricio
  glBindVertexArray (VAO_Patr);
  patrTransform();
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);

  // Pilota
  glBindVertexArray (VAO_Pil);
  pilTransform();
  glDrawArrays(GL_TRIANGLES, 0, pil.faces().size()*3);

  // Cub
  glBindVertexArray (VAO_Cub);
  paret1Transform();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  paret2Transform();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  paret3Transform();
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Terra
  glBindVertexArray (VAO_Terra);
  identTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

// Reescribim patrTransform de LL2
void MyGLWidget::patrTransform() {
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, posPorter);
  TG = glm::rotate(TG, -(float)M_PI/2, glm::vec3(0.0, 1.0, 0.0));
  TG = glm::scale(TG, glm::vec3(escalaPatr/0.25, escalaPatr/0.25, escalaPatr/0.25));
  TG = glm::translate(TG, -centreBasePatr);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::paret1Transform() {
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, glm::vec3(0, 0, -6.9));
    TG = glm::scale(TG, glm::vec3(20, 2, 0.2));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::paret2Transform() {
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, glm::vec3(0, 0, 6.9));
    TG = glm::scale(TG, glm::vec3(20, 2, 0.2));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::paret3Transform() {
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, glm::vec3(-9.9, 0, 0));
    TG = glm::scale(TG, glm::vec3(0.2, 2, 14));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform() {
    glm::mat4 VM(1.0f);
    VM = glm::translate(VM, glm::vec3(0, 0, -26.67));
    VM = glm::rotate(VM, (float)M_PI/4, glm::vec3(1, 0, 0));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &VM[0][0]);
}

// Reescribim keyPressEvent de LL2
void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { // moviment de la pilota
      if (posPilota[0] == 7.0)
        mourePilota ();
      break;
    }
    case Qt::Key_I: { // reinicia posició pilota
      iniciPilota ();
      break;
    }

    case Qt::Key_Left: {
        if (posPorter[2] != 5) {
            posPorter[2] += 0.5;
            mouPorterLeft();
        }
        break;
    }
    case Qt::Key_Right: {
        if (posPorter[2] != -5) {
            posPorter[2] -= 0.5;
            mouPorterRight();
        }
        break;
    }

    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mouPorterLeft() {
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, glm::vec3(0, 0, 0.5));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::mouPorterRight() {
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, glm::vec3(0, 0, -0.5));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

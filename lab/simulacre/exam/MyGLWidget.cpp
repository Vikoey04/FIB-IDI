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

void MyGLWidget::mouseMoveEvent(QMouseEvent *event) {
  if (!camPlanta) ExamGLWidget::mouseMoveEvent(event);
}

void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
  //ExamGLWidget::paintGL();

  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  //  glViewport (0, 0, ample, alt);

    // Esborrem el frame-buffer i el depth-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Pintem el terra
    glBindVertexArray (VAO_Terra);
    modelTransformTerra ();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Pintem el Patricio
    if (!pintaCubs) { // Quan no pinta cubs, pinta el Patricio
      glBindVertexArray (VAO_Patr);
      modelTransformPatricio ();
      glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
    }

    // Pintem el cubs
    if (pintaCubs) {
      glBindVertexArray(VAO_Cub);
      if (posPat == 1) {
        modelTransformCub (4.0, 0.0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        modelTransformCub (5.0, float(2*M_PI/3));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        modelTransformCub (6.0, float(4*M_PI/3));
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
      else if (posPat == 2) {
        modelTransformCub (6.0, 0.0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        modelTransformCub (4.0, float(2*M_PI/3));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        modelTransformCub (5.0, float(4*M_PI/3));
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
      else if (posPat == 3) {
        modelTransformCub (5.0, 0.0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        modelTransformCub (6.0, float(2*M_PI/3));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        modelTransformCub (4.0, float(4*M_PI/3));
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
    }

    glBindVertexArray(0);
}

void MyGLWidget::iniEscena() {
  ExamGLWidget::iniEscena();

  pintaCubs = false;
  focusGroc = false;
}

void MyGLWidget::enviaColFocus() {
  if (focusGroc) {
    colFoc = glm::vec3(1, 1, 0);
  }
  else {
    colFoc = glm::vec3(1, 1, 1);
  }
  ExamGLWidget::enviaColFocus();
}

void MyGLWidget::modelTransformCub (float escala, float angle)
{
  //ExamGLWidget::modelTransformCub (1.0, 0.0);
  // En aquest mètode has de substituir aquest codi per construir la
  // transformació geomètrica (TG) del cub usant els paràmetres adientment
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG, angle, glm::vec3(0, 1, 0));
  TG = glm::translate(TG, glm::vec3(5, 0, 0));
  TG = glm::scale(TG, glm::vec3 (escala, escala, escala));

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  //ExamGLWidget::modelTransformPatricio ();
  TG = glm::mat4(1.f);
  if (posPat == 2) {
    TG = glm::rotate(TG, float(2*M_PI/3), glm::vec3(0, 1, 0));
  }
  else if (posPat == 3) {
    TG = glm::rotate(TG, float(4*M_PI/3), glm::vec3(0, 1, 0));
  }
  TG = glm::translate(TG, glm::vec3(5, 0, 1)); // per moure a la pos de Cub1
  TG = glm::rotate(TG, float(-M_PI/2), glm::vec3(1, 0, 0)); // per posarlo panxa amunt
  TG = glm::scale(TG, glm::vec3 (2*escala, 2*escala, 2*escala));
  TG = glm::translate(TG, -centreBasePat);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
    // Codi per a la viewMatrix de la Càmera-2
    // obs, vrp, up
    View = lookAt(glm::vec3(0, 5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
    // Codi per a la projectMatrix de la Càmera-2
    glm::mat4 Proj;
    // left, right, bottom, top, ZNear, ZFar
    if (ra > 1) {
      Proj = glm::ortho(-radiEsc*ra, radiEsc*ra, -radiEsc, radiEsc, 0.f, 2*radiEsc);
    }
    else {
      Proj = glm::ortho(-radiEsc, radiEsc, -radiEsc/ra, radiEsc/ra, 0.f, 2*radiEsc);
    }
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
    if (pintaCubs) {
      pintaCubs = false;
    }
    else {
      pintaCubs = true;
    }
    break;
	}
  case Qt::Key_1: {
    posPat = 1;
    break;
	}
  case Qt::Key_2: {
    posPat = 2;
    break;
	}
  case Qt::Key_3: {
    posPat = 3;
    break;
	}
  case Qt::Key_F: {
    if (focusGroc) {
      focusGroc = false;
    }
    else {
      focusGroc = true;
    }
    enviaColFocus();
    break;
	}
  case Qt::Key_C: {
    if (camPlanta) {
      camPlanta = false;
    }
    else {
      camPlanta = true;
    }
    viewTransform();
    projectTransform();
    break;
	}
  case Qt::Key_Right: {
    if (posPat < 3) ++posPat;
    else {
      posPat = 1;
    }
    break;
	}
  case Qt::Key_Left: {
    if (posPat > 1) --posPat;
    else {
      posPat = 3;
    }
    break;
	}
  case Qt::Key_R: {
      // ...
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}

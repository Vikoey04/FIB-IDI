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

// Reescribim
void MyGLWidget::iniEscena()
{
    pintaPilota = true;

    radiEscena = sqrt(153);
    centreEscena = glm::vec3(0,2,0);

    altPorter = 4;
    posPorter = glm::vec3(9.0, 0.0, 0.0);  // posició inicial del porter
    posPilota = glm::vec3(7.0, 0.0, 0.0);  // posició inicial de la pilota
    dirInicialPilota();    // direcció inicial de la pilota
}


// Reescribim
void MyGLWidget::iniCamera(){

  obs = glm::vec3(14, 4, 14);
  vrp = glm::vec3(0, 2, 0);
  up = glm::vec3(0, 1, 0);
  fov = 2*asin(1./2.);
  ra  = 1.0;
  znear =  radiEscena;
  zfar  = 3*radiEscena;

  factorAngleX = (float)M_PI/4;
  factorAngleY = 0;

  viewTransform();
  projectTransform();
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
  if (pintaPilota) {
    glBindVertexArray (VAO_Pil);
    pilTransform();
    glDrawArrays(GL_TRIANGLES, 0, pil.faces().size()*3);
  }

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

// Reescribim
void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  Proj = glm::perspective (fov, ra, znear, zfar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}


void MyGLWidget::viewTransform() {
    glm::mat4 VM(1.0f); //-26.67

    if (cameraPerspectiva) {
        VM = glm::translate(VM, glm::vec3(0, 0, -2*radiEscena));
        VM = glm::rotate(VM, factorAngleX, glm::vec3(1, 0, 0));
        VM = glm::rotate(VM, factorAngleY, glm::vec3(0, 1, 0));
        VM = glm::translate(VM, -vrp);
    }
    else {
        VM = glm::lookAt();
    }
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &VM[0][0]);
}

// Reescribim
void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
	makeCurrent();
	
	if(e->x() > xClick) factorAngleY -= 0.03;
	else if(e->x() < xClick) factorAngleY += 0.03;
	
	if(e->y() > yClick) factorAngleX -= 0.03;
	else if(e->y() < yClick) factorAngleX += 0.03;
	
	xClick = e->x();
	yClick = e->y();
	viewTransform();
	update();
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

    case Qt::Key_R: {
        iniCamera();
        iniEscena();
        break;
    }
    case Qt::Key_C: {

        break;
    }

    case Qt::Key_W: {
        
        break;
    }
    case Qt::Key_S: {
        
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

// Allarguem
void MyGLWidget::tractamentGol() {
    LL2GLWidget::tractamentGol();

    pintaPilota = false;
}

// Allarguem
void MyGLWidget::dirInicialPilota() {
    pintaPilota = true;
    LL2GLWidget::dirInicialPilota();
}
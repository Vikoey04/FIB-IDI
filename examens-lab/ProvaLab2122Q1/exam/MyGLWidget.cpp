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

void MyGLWidget::iniEscena ()
{
  creaBuffersPatricio();
  creaBuffersAvio();
  creaBuffersHangar();
  creaBuffersTerra();

  // Paràmetres de l'escena - arbitraris
  centreEsc = glm::vec3 (0, 0, 0);
  radiEsc = sqrt(20*20+10*10+2*2);
  
  //centreEsc = glm::vec3(15, 0, 12);
  //radiEsc = sqrt( 15*15+12*12 * 2.5*2.5 );

  focCamera = true;
  altitud = 0;
  rotAvio = 0;

/*
  TGAvio = glm::mat4(1.f);
  TGAvio = glm::translate(TGAvio, glm::vec3 (25, 0, 12));
  TGAvio = glm::scale(TGAvio, glm::vec3 (1.5*escalaAvio, 1.5*escalaAvio, 1.5*escalaAvio));
  TGAvio = glm::translate(TGAvio, glm::vec3(0, float(capsaAvioMax[1]), -float(capsaAvioMax[2]/2)));
  posFoc = glm::vec3(float(capsaAvioMax[0]-capsaAvioMin[0]/2.0),float(capsaAvioMax[1]/2.f),float(capsaAvioMin[2]));

  posFoc = glm::vec3(0, 0, 0);
  glm::vec4 aux = TGAvio * glm::vec4(posFoc, 1.0);
  posFoc = glm::vec3(aux[0], aux[1], aux[2]);
  */
}

void MyGLWidget::iniMaterialTerra()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.2,0.1,0.2);
  diff = glm::vec3(0.0,0.6,0.6);
  spec = glm::vec3(0.5,0.5,0.5);
  shin = 500; //500
}

void MyGLWidget::enviaPosFocus()
{
  glm::vec3 aux;
  if (focCamera) {
    aux = glm::vec3(0,0,0);
  }
  else {
    posFoc = glm::vec3(0,0,0);
    TGAvio = glm::mat4(1.f);

    TGAvio = glm::translate(TGAvio, glm::vec3 (15, altitud, 12));
    TGAvio = glm::rotate(TGAvio , -float(rotAvio*M_PI/180), glm::vec3(0, 1, 0));
    TGAvio = glm::translate(TGAvio, glm::vec3(10, float(capsaAvioMax[2]/2), -float((capsaAvioMax[2])/2)));

    glm::vec4 aux2 = View * TGAvio * glm::vec4(posFoc, 1.0);
    aux = glm::vec3(aux2[0], aux2[1], aux2[2]);
  }
  glUniform3fv (posfocusLoc, 1, &aux[0]);
}

void MyGLWidget::iniCamera ()
{
  angleX = -0.5;
  angleY = 0.5;
  camera2 = false;
  ra = float(width())/height();
  fov = float(M_PI/3.0);
  zn = 15;
  zf = 65;

  projectTransform ();
  viewTransform ();
}

void MyGLWidget::paintGL ()
{
  ExamGLWidget::paintGL();
}

void MyGLWidget::modelTransformPatricio ()
{
  glm::mat4 TG(1.f);
  TG = glm::translate(TG, glm::vec3(15, 0, 12));
  TG = glm::rotate(TG, float(-M_PI/2), glm::vec3(0, 1, 0));
  TG = glm::scale(TG, glm::vec3 (2*escalaPat, 2*escalaPat, 2*escalaPat));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformAvio ()
{
  TGAvio = glm::mat4(1.f);
  TGAvio = glm::translate(TGAvio, glm::vec3 (15, altitud, 12));
  TGAvio = glm::rotate(TGAvio , -float(rotAvio*M_PI/180), glm::vec3(0, 1, 0));
  TGAvio = glm::translate(TGAvio, glm::vec3 (10, 0, 0));
  TGAvio = glm::scale(TGAvio, glm::vec3 (1.5*escalaAvio, 1.5*escalaAvio, 1.5*escalaAvio));
  TGAvio = glm::translate(TGAvio, -centreBaseAvio);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TGAvio[0][0]);
}

void MyGLWidget::viewTransform ()
{
  if (!camera2) {
    //ExamGLWidget::viewTransform();
    
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, angleX, glm::vec3(-1, 0, 0));
    View = glm::rotate(View, angleY, glm::vec3(0, 1, 0));
    View = glm::translate(View, glm::vec3(-15, 0, -12));

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
  else
  {
    // Codi per a la viewMatrix de la Càmera-2
    //obs, vrp, up
    View = glm::lookAt(glm::vec3(15, 3, 12), glm::vec3(0, 3, 12), glm::vec3(0, 1, 0));
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camera2) {
    //ExamGLWidget::projectTransform();
    
    glm::mat4 Proj;  // Matriu de projecció
    Proj = glm::perspective(fov, ra, zn, zf);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
  else
  {
    // Codi per a la projectMatrix de la Càmera-2
    glm::mat4 Proj;
    Proj = glm::perspective(float(M_PI/2), ra, 0.01f, radiEsc);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_Up: {
    if (altitud < 5) {
      altitud++;
      modelTransformAvio();
    }
    break;
	}
  case Qt::Key_Down: {
    if (altitud > 0) {
      altitud--;
      modelTransformAvio();
    }
    break;
	}
  case Qt::Key_Right: {
    if (rotAvio < 348.75) {
      rotAvio += 11.25;
    }
    else {
      rotAvio = 0;
    }
    modelTransformAvio();
    break;
	}
  case Qt::Key_F: {
    if (focCamera) {
      focCamera = false;
    }
    else {
      focCamera = true;
    }
    break;
	}
  case Qt::Key_C: {
    if (camera2) {
      camera2 = false;
    }
    else {
      camera2 = true;
    }
    projectTransform();
    viewTransform();
    break;
	}
  case Qt::Key_R: {
    iniEscena();
    iniCamera();
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if ((DoingInteractive == ROTATE) && !camera2)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX -= (e->y() - yClick) * M_PI / alt;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::pujaAvio() {
  makeCurrent();
  if (altitud < 5) {
    altitud++;
    modelTransformAvio();
  }
  update();
}

void MyGLWidget::baixaAvio() {
  makeCurrent();
  if (altitud > 0) {
    altitud--;
    modelTransformAvio();
  }
  update();
}

void MyGLWidget::activaCamera1() {
  makeCurrent();

  camera2 = false;
  projectTransform();
  viewTransform();

  update();
}

void MyGLWidget::activaCamera2() {
  makeCurrent();

  camera2 = true;
  projectTransform();
  viewTransform();

  update();
}
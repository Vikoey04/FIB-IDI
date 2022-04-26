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
    modelTransform();
    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if (espatricio) {
      glBindVertexArray(VAO_Casa);
    }
    else {
      glBindVertexArray();
    }

    glDrawArrays(GL_TRIANGLES, 0, m.faces().size() * 3);
    glBindVertexArray (0);

    //BL2GLWidget::paintGL();
}

void MyGLWidget::carregaShaders() {
    BL2GLWidget::carregaShaders();
    projLoc = glGetUniformLocation(program->programId(), "proj");
    viewLoc = glGetUniformLocation(program->programId(), "view");
}

void MyGLWidget::projectTransform() {
    glm::mat4 Proj = glm::perspective (FOV, raw, Znear, Zfar);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() {
   // glm::mat4 View = glm::lookAt (OBS, VRP, UP);
    glm::mat4 VM = glm::mat4 (1.0f);
    VM = glm::translate(VM, glm::vec3(0, 0, -distance));
    VM = glm::rotate(VM, -angz, glm::vec3(0, 0, 1));
    VM = glm::rotate(VM, angx, glm::vec3(1, 0, 0));
    VM = glm::rotate(VM, -angy, glm::vec3(0, 1, 0));
    VM = glm::translate(VM, glm::vec3(-VRP));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &VM[0][0]);
}

void MyGLWidget::creaBuffers() {
    glGenVertexArrays(1, &VAO_Casa);
    glBindVertexArray(VAO_Casa);

    GLuint VBO_Casa[2];
    glGenBuffers(2, VBO_Casa);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Casa[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_vertices(), GL_STATIC_DRAW);

    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Casa[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);
}

void MyGLWidget::initializeGL() {
    m.load("./HomerProves.obj");
    x_ini= 0;
    y_ini = 0;
    BL2GLWidget::initializeGL();
    glEnable(GL_DEPTH_TEST);
    iniCamara();

    espatricio = true;
}

void MyGLWidget::calculaAtributs(glm::vec3 min, glm::vec3 max, glm::vec3& centre, float& radi) {
    centre.x = (max.x + min.x)/2.0;
    centre.y = (max.y + min.y)/2.0;
    centre.z = (max.z + min.z)/2.0;
    radi = glm::distance(min, max)/2.0;
}

void MyGLWidget::iniCamara () {
    calculaAtributs(glm::vec3(-4.0, -1.0, 4.0), glm::vec3(4.0, 1.0, -4.0), centre, radi);
    distance = 2.0 * radi;
    //FOV = float(M_PI)/6.0f;
    FOV = float (0.7/(M_PI));   //Ens hem patillat l'angle perque no sabem fer arcsin xd
    raw = 1.0f;
    Znear = distance - radi;
    Zfar = distance + radi;
    projectTransform();
    angx = 45*(M_PI)/180;
    angy = 45*(M_PI)/180;
    angz = 0*(M_PI)/180;




    OBS = glm::vec3(0,0, distance);
    VRP = glm::vec3(0,0,0);
    UP = glm::vec3(0,1,0);
    viewTransform();
}

/*
void MyGLWidget::mouseEvent(QMouseEvent* event)
{
  makeCurrent();
  if (event-> buttons() == Qt::LeftButton) {
      x_ini = event->x();
      y_ini = event->y();
      while (event-> buttons() == Qt::LeftButton) ();
      x_fi = event->x();
      y_fi = event->y();
      angy = (x_fi - x_ini)*(M_PI)/180;
      angx = (y_fi - y_ini)*(M_PI)/180;
      viewTransform();
  }
  update();
}
*/

void MyGLWidget::mousePressEvent(QMouseEvent *event) {
    makeCurrent();
    if (event-> buttons() == Qt::LeftButton) {
        y_ini = event->y();
        x_ini = event->x();
    }
}

void MyGLWidget::mouseMoveEvent(QMouseEvent * event) {
    makeCurrent();
    x_fi = event->x();
    y_fi = event->y();
    angy -= float (x_fi - x_ini)/width()*(M_PI);
    angx += float (y_fi - y_ini)/height()*(M_PI);
    y_ini = event->y();
    x_ini = event->x();
    viewTransform();
    update();
}


void MyGLWidget::tractacanvifov(int fov) {
  makeCurrent();
  FOV = (99-fov)*M_PI/180;
  projectTransform();
  update();
}

void MyGLWidget::canviamodel() {

}

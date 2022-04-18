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


// Reescribim initializeGL
void MyGLWidget::initializeGL() {
    initializeOpenGLFunctions();  
    glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)

    glEnable(GL_DEPTH_TEST); // ACTIVEM Z-BUFFER

    carregaShaders();
    creaBuffers();
    escala = 1.0f;

    ini_camera();
}

void MyGLWidget::ini_camera() {
    FOV = float(M_PI)/2.0;
    ra = 1.0;
    znear = 0.4;
    zfar = 3.0;
    projectTransform();

    OBS = glm::vec3(0, 0, 1);
    VRP = glm::vec3(0, 0, 0);
    UP = glm::vec3(0, 1, 0);
    viewTransform();

    modelRotate(); // Per inicialitzar la Mat de rotacio (amb el valor inicial que és rotacio de 0 radiants)
}

// Reescribim paintGL
void MyGLWidget::paintGL() {
    glViewport(0, 0, width(), height());

    // Esborrem el buffer de profunditats a la vegada que el frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Carreguem la tranformació del model
    modelTransform();

    // Activem el VAO per pintar el model
    glBindVertexArray(VAO_Homer);

    // Pintem
    glDrawArrays(GL_TRIANGLES, 0, homer.faces().size()*3);

    // Desactivem VAO
    glBindVertexArray(0);
}

void MyGLWidget::creaBuffers() {
    //homer.load("~/Desktop/IDI/agit-idi/lab/models/HomerProves.obj");
    // Creem VAO del model
    homer.load("HomerProves.obj");

    glGenVertexArrays(1, &VAO_Homer);
    glBindVertexArray(VAO_Homer);

    // VBO pos
    glGenBuffers(1, &VBO_HomerPos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_HomerPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*homer.faces().size()*3*3, homer.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    // VBO col
    glGenBuffers(1, &VBO_HomerCol);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_HomerCol);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*homer.faces().size()*3*3, homer.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);
}

// Allarguem carregaShaders de BL2 per darrere
void MyGLWidget::carregaShaders() {
    BL2GLWidget::carregaShaders();
    
    projLoc = glGetUniformLocation(program -> programId(), "proj");
    viewLoc = glGetUniformLocation(program -> programId(), "view");
    rotLoc = glGetUniformLocation(program -> programId(), "rot");
}

// Calcul de transformació de proj i enviar uniform a la mat cap al VS
void MyGLWidget::projectTransform() {
    // glm::perspective(FOV en rad, RA window, znear, zfar)

    glm::mat4 Proj = glm::perspective(FOV, ra, znear, zfar);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() {
    // glm::lookAt(OBS, VRP, up)

    glm::mat4 View = glm::lookAt(OBS, VRP, UP);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::modelRotate() {
    glm::mat4 rotateMat(1.0f);
    rotateMat = glm::rotate(rotateMat, rotacio, glm::vec3(0, 1, 0));
    glUniformMatrix4fv(rotLoc, 1, GL_FALSE, &rotateMat[0][0]);
}

// Reescribim keyPressEvent
void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
        escala += 0.05;
        break;
    }
    case Qt::Key_D: { // escalar a més petit
        escala -= 0.05;
        break;
    }
    case Qt::Key_R: {
        rotacio += M_PI/4;
        modelRotate();
        break;
    }
    default: event->ignore(); break;
  }
  update();
}







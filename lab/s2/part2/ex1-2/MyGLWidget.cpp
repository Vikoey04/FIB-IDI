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
    Pmin[0] = -2;
    Pmin[1] = -1;
    Pmin[2] = -2;

    Pmax[0] = 2;
    Pmax[1] = 1;
    Pmax[2] = 2;

    // VRP com a centre de l'escena
    VRP[0] = (Pmin[0]+Pmax[0])/2;
    VRP[1] = (Pmin[1]+Pmax[1])/2;
    VRP[2] = (Pmin[2]+Pmax[2])/2;

    r = sqrt( (Pmax[0]-VRP[0])*(Pmax[0]-VRP[0]) +
              (Pmax[1]-VRP[1])*(Pmax[1]-VRP[1]) +
              (Pmax[2]-VRP[2])*(Pmax[2]-VRP[2])  );

    d = 1.5*r;

    OBS = VRP + d*glm::vec3(0, 0, 1);

    // UP per defecte
    UP = glm::vec3(0, 1, 0);
    viewTransform();

    FOV = 2 * asin(r/d);
    ra = 1.0;
    znear = d - r;
    zfar = d + r;
    projectTransform();

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

    glBindVertexArray(VAO_Terra);
    glm::mat4 rotateMat(1.0f);
    glUniformMatrix4fv(rotLoc, 1, GL_FALSE, &rotateMat[0][0]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

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

    glm::vec3 TerraPos[4] = {
        glm::vec3(2, -1, 2),
        glm::vec3(2, -1, -2),
        glm::vec3(-2, -1, 2),
        glm::vec3(-2, -1, -2)
    };

    glm::vec3 TerraCol[4] = {
        glm::vec3(0.58, 0.56, 0.85),
        glm::vec3(0.58, 0.56, 0.85),
        glm::vec3(0.58, 0.56, 0.85),
        glm::vec3(0.58, 0.56, 0.85)
    };
    
    // VAO Terra
    glGenVertexArrays(1, &VAO_Terra);
    glBindVertexArray(VAO_Terra);

    // VBO pos
    glGenBuffers(1, &VBO_TerraPos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TerraPos), TerraPos, GL_STATIC_DRAW);
    
    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    // VBO col
    glGenBuffers(1, &VBO_TerraCol);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraCol);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TerraCol), TerraCol, GL_STATIC_DRAW);

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







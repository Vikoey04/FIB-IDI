
//#include <GL/glew.h>
#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();
}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  //modelTransform();

  // Activem array VAO1 i pintem
  glBindVertexArray(VAO1);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void MyGLWidget::creaBuffers ()
{
  glm::vec3 Vertices[3];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-1.0, -1.0, 0.0);    
  Vertices[1] = glm::vec3(1.0, -1.0, 0.0);    
  Vertices[2] = glm::vec3(0.0, 1.0, 0.0);     
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  //COLORS
  glm::vec3 Colors[3];  
  Colors[0] = glm::vec3(1.0, 0.0, 0.0);    
  Colors[1] = glm::vec3(0.0, 1.0, 0.0);    
  Colors[2] = glm::vec3(0.0, 0.0, 1.0);  

  GLuint VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);

  // Activem l'atribut que farem servir per color vertex
  glVertexAttribPointer(vertexCol, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexCol);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “color” del vertex shader
  vertexCol = glGetAttribLocation (program->programId(), "color");

  // Associem id al shader
  transLoc = glGetUniformLocation(program -> programId(), "TG");

  // Per a que a l'inici es pinti sabent quina es la TG inicial
  glm::mat4 TG(1.0);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransform() {
  glm::mat4 TG(1.0); // Matriu de transformació, inicialitzada a la identitat
  TG = glm::scale(TG, glm::vec3(scl_x, scl_y, 0.0));
  TG = glm::translate(TG, glm::vec3(tx, ty, 0));
  TG = glm::rotate(TG, rotacio, glm::vec3(0.0, 0.0, 1.0));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
  makeCurrent();
  float rx_new = e->x();
  float ry_new = e->y();
  
  if (rx_new > rx) scl_x += 0.01;
  else if (rx_new < rx) scl_x -= 0.01;
  if (ry_new > ry) scl_y += 0.01;
  else if (ry_new < ry) scl_y -= 0.01;
  modelTransform();
  
  update();
  rx = rx_new;
  ry = ry_new;
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
  makeCurrent(); // fa actiu el nostre context d'OpenGL
  switch( e->key() ) {
      case Qt::Key_Left:
        tx -= 0.1; rotacio += M_PI/4;
        modelTransform();
        break;
      
      case Qt::Key_Right:
        tx += 0.1; rotacio += M_PI/4;
        modelTransform();
        break;

      case Qt::Key_Up:
        ty += 0.1; rotacio += M_PI/4;
        modelTransform();
        break;

      case Qt::Key_Down:
        ty -= 0.1; rotacio += M_PI/4;
        modelTransform();
        break;

      default: e->ignore(); //propagar al pare
    }
  update(); // provoca que es torni a pintar l'escena
}

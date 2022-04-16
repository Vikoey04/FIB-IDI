#include "LL2GLWidget.h"

#include <iostream>

LL2GLWidget::LL2GLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  xClick = yClick = 0;
  connect(&timer, SIGNAL(timeout()), this, SLOT(mourePilota()));  // per al moviment de la pilota
  DoingInteractive = NONE;
}

LL2GLWidget::~LL2GLWidget ()
{
  if (program != NULL)
    delete program;
}

void LL2GLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  
  glEnable(GL_DEPTH_TEST);
  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffersPatr();
  creaBuffersPil();
  creaBuffersCub();
  creaBuffersTerra();
  iniEscena();
  iniCamera();
}

void LL2GLWidget::iniEscena()
{
  radiEscena = sqrt(80);
  centreEscena = glm::vec3(0,0,0);

  altPorter = 4;
  posPorter = glm::vec3(9.0, 0.0, 0.0);  // posició inicial del porter
  posPilota = glm::vec3(7.0, 0.0, 0.0);  // posició inicial de la pilota
  dirInicialPilota();    // direcció inicial de la pilota
}

void LL2GLWidget::iniCamera(){

  obs = glm::vec3(14, 4, 14);
  vrp = glm::vec3(0, 0, 0);
  up = glm::vec3(0, 1, 0);
  fov = float(M_PI)/4.0f;
  ra  = 1.0;
  znear =  10;
  zfar  = 30;

  viewTransform();
  projectTransform();
}

void LL2GLWidget::paintGL ()
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
  identTransform();
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Terra
  glBindVertexArray (VAO_Terra);
  identTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

void LL2GLWidget::patrTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, -centreBasePatr);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void LL2GLWidget::pilTransform ()
{
  glm::mat4 TG(1.f);
  TG = glm::translate(TG, posPilota);
  TG = glm::scale(TG, glm::vec3(escalaPil, escalaPil, escalaPil));
  TG = glm::translate(TG, -centreBasePil);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void LL2GLWidget::identTransform ()
{
  glm::mat4 TG(1.0f);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void LL2GLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  Proj = glm::perspective (fov, ra, znear, zfar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void LL2GLWidget::viewTransform ()
{
  // Matriu de posició i orientació de l'observador
  glm::mat4 View(1.0f);
  View = glm::lookAt (obs, vrp, up);
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}


void LL2GLWidget::resizeGL (int w, int h) 
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
  ra = float(ample)/float(alt);
  factorAngleY = M_PI / ample;
  factorAngleX = M_PI / alt;
  projectTransform();
}

void LL2GLWidget::iniciPilota()
{
  timer.stop();
  posPilota = glm::vec3(7.0, 0.0, 0.0);
  dirInicialPilota();    // direcció inicial de la pilota
}

void LL2GLWidget::rebotaParets() 
{
} 

void LL2GLWidget::tractamentGol()
{
  timer.stop();
} 

void LL2GLWidget::dirInicialPilota()
{
  int randNum = rand() % 90 + 1;
  randNum -= 45;
  float rad = randNum * M_PI / 180.0f;
  dirPilota[0] = -cos(rad);
  dirPilota[1] = 0;
  dirPilota[2] = sin(rad);
}

void LL2GLWidget::mourePilota()
{
  makeCurrent();
  timer.start(100);
  mouPilota(dirPilota);
  if (posPilota[0] >= 10.0)  // ens l'han colat
    tractamentGol();
}

void LL2GLWidget::mouPilota(glm::vec3 dir)
{
  posPilota += dir;

  rebotaParets();
  if (rebotaPorter())
    canviaDireccio();
  else  
    if (posPilota[0] <= -10.0)
    {
      posPilota[0] = -11.0;
      timer.stop();
    }
  update();
}

bool LL2GLWidget::rebotaPorter()
{
  return ((posPilota[0] > 7.5) && (posPilota[0] < 8.5) && 
          (posPilota[2] >= posPorter[2]-(altPorter/2.0)) && 
          (posPilota[2] <= posPorter[2]+(altPorter/2.0)));
}

void LL2GLWidget::canviaDireccio()
{
  // Inventem una nova direcció quan la pilota rebota amb el porter
  dirPilota = posPilota-posPorter;
} 

void LL2GLWidget::keyPressEvent(QKeyEvent* event) 
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
    default: event->ignore(); break;
  }
  update();
}

void LL2GLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void LL2GLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void LL2GLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();

  xClick = e->x();
  yClick = e->y();

  update ();
}

void LL2GLWidget::creaBuffersTerra ()
{
  // VBO amb la posició dels vèrtexs
  glm::vec3 posTerra[4] = {
        glm::vec3(-10.0, 0.0, -7.0),
        glm::vec3(-10.0, 0.0,  7.0),
        glm::vec3( 10.0, 0.0, -7.0),
        glm::vec3( 10.0, 0.0,  7.0)
  }; 

  glm::vec3 c(0.2, 0.6, 0.1);
  glm::vec3 colTerra[4] = { c, c, c, c };

  // VAO
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[2];
  glGenBuffers(2, VBO_Terra);

  // geometria
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posTerra), posTerra, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // color
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colTerra), colTerra, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glBindVertexArray (0);
}

void LL2GLWidget::creaBuffersCub ()
{
  // Vèrtexs del cub
  glm::vec3 vertexs[8] = {
       /* 0*/ glm::vec3( -0.5, 0.0, -0.5),  /* 1*/ glm::vec3( 0.5, 0.0, -0.5),
       /* 2*/ glm::vec3( -0.5, 1.0, -0.5),  /* 3*/ glm::vec3( 0.5, 1.0, -0.5),
       /* 4*/ glm::vec3( -0.5, 0.0, 0.5),  /* 5*/ glm::vec3( 0.5, 0.0, 0.5),
       /* 6*/ glm::vec3( -0.5, 1.0, 0.5),  /* 7*/ glm::vec3( 0.5, 1.0, 0.5)
  };

  // posició dels vèrtexs del cub per al VBO
  glm::vec3 poscub[36] = {  // 12 triangles
       vertexs[0], vertexs[2], vertexs[1],
       vertexs[1], vertexs[2], vertexs[3],
       vertexs[5], vertexs[1], vertexs[7],
       vertexs[1], vertexs[3], vertexs[7],
       vertexs[2], vertexs[6], vertexs[3],
       vertexs[3], vertexs[6], vertexs[7],
       vertexs[0], vertexs[4], vertexs[6],
       vertexs[0], vertexs[6], vertexs[2],
       vertexs[0], vertexs[1], vertexs[4],
       vertexs[1], vertexs[5], vertexs[4],
       vertexs[4], vertexs[5], vertexs[6],
       vertexs[5], vertexs[7], vertexs[6]
  };

  // Definim el color
  glm::vec3 c(0.2, 0.2, 0.8);
  glm::vec3 color[36] = {
	c, c, c, c, c, c,
	c, c, c, c, c, c,
	c, c, c, c, c, c,
	c, c, c, c, c, c,
	c, c, c, c, c, c,
	c, c, c, c, c, c
  };

  // VAO
  glGenVertexArrays(1, &VAO_Cub);
  glBindVertexArray(VAO_Cub);

  GLuint VBO_Cub[2];
  glGenBuffers(2, VBO_Cub);

  // geometria
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(poscub), poscub, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // color
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray(0);
}

void LL2GLWidget::calculaCapsaModel (Model &p, float &escala, glm::vec3 &centreBase)
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = p.vertices()[0];
  miny = maxy = p.vertices()[1];
  minz = maxz = p.vertices()[2];
  for (unsigned int i = 3; i < p.vertices().size(); i+=3)
  {
    if (p.vertices()[i+0] < minx)
      minx = p.vertices()[i+0];
    if (p.vertices()[i+0] > maxx)
      maxx = p.vertices()[i+0];
    if (p.vertices()[i+1] < miny)
      miny = p.vertices()[i+1];
    if (p.vertices()[i+1] > maxy)
      maxy = p.vertices()[i+1];
    if (p.vertices()[i+2] < minz)
      minz = p.vertices()[i+2];
    if (p.vertices()[i+2] > maxz)
      maxz = p.vertices()[i+2];
  }
  escala = 1.0/(maxy-miny);
  centreBase[0] = (minx+maxx)/2.0; centreBase[1] = miny; centreBase[2] = (minz+maxz)/2.0;
}

void LL2GLWidget::creaBuffersPatr ()
{
  // Càrrega del model
  patr.load("./models/Patricio.obj");

  // Calculem la capsa contenidora del model
  calculaCapsaModel (patr, escalaPatr, centreBasePatr);
  
  // Creació de VAOs i VBOs per pintar
  // Homer
  glGenVertexArrays(1, &VAO_Patr);
  glBindVertexArray(VAO_Patr);

  GLuint VBO_Patr[2];
  glGenBuffers(2, VBO_Patr);

  // geometria
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3,
           patr.VBO_vertices(), GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // color
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3,
           patr.VBO_matdiff(), GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glBindVertexArray (0);
}

void LL2GLWidget::creaBuffersPil ()
{
  // Càrrega del model
  pil.load("./models/Pilota.obj");

  // Calculem la capsa contenidora del model
  calculaCapsaModel (pil, escalaPil, centreBasePil);
  
  // Creació de VAOs i VBOs per pintar
  // Homer
  glGenVertexArrays(1, &VAO_Pil);
  glBindVertexArray(VAO_Pil);

  GLuint VBO_Pil[2];
  glGenBuffers(2, VBO_Pil);

  // geometria
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Pil[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*pil.faces().size()*3*3,
           pil.VBO_vertices(), GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // color
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Pil[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*pil.faces().size()*3*3,
           pil.VBO_matdiff(), GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glBindVertexArray (0);
}

void LL2GLWidget::carregaShaders()
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

  // Identificador per als  atributs
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");

  // Identificadors dels uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc  = glGetUniformLocation (program->programId(), "Proj");
  viewLoc  = glGetUniformLocation (program->programId(), "View");
}



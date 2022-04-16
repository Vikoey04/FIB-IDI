#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class LL2GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    LL2GLWidget (QWidget *parent=0);
    ~LL2GLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);
    // mouse{Press/Release/Move}Event - Són cridades quan es realitza l'event
    // corresponent de ratolí
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);

    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void creaBuffersPatr ();
    virtual void creaBuffersPil ();
    virtual void creaBuffersCub ();
    virtual void creaBuffersTerra ();
    virtual void carregaShaders ();
    virtual void viewTransform ();
    virtual void projectTransform ();
    virtual void patrTransform();
    virtual void pilTransform();
    virtual void identTransform();
    virtual void calculaCapsaModel (Model &p, float &escala, glm::vec3 &CentreBase);
    
    virtual void iniciPilota ();
    virtual void rebotaParets ();
    virtual void tractamentGol ();
    void dirInicialPilota ();
    void mouPilota (glm::vec3 dir);
    bool rebotaPorter ();
    void canviaDireccio ();

    // attribute locations
    GLuint vertexLoc, colorLoc;

    // uniform locations
    GLuint transLoc, viewLoc, projLoc;

    // VAO i VBO names
    GLuint VAO_Patr, VAO_Pil, VAO_Cub, VAO_Terra;

    // Program
    QOpenGLShaderProgram *program;

    // Viewport
    GLint ample, alt;

    // Mouse interaction
    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float factorAngleX, factorAngleY;

    // Internal vars
    float radiEscena;
    float fov, ra, znear, zfar;
    glm::vec3 centreEscena, obs, vrp, up;

    Model patr, pil;
    glm::vec3 centreBasePatr, centreBasePil;
    float escalaPatr, escalaPil;

    // per al moviment al joc
    QTimer timer;
    glm::vec3 dirPilota;
    glm::vec3 posPilota, posPorter;
    float altPorter;

  public slots:
    void mourePilota ();
};


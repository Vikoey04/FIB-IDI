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

class ExamGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    ExamGLWidget (QWidget *parent=0);
    ~ExamGLWidget ();

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
    
    virtual void modelTransformTerra ();
    virtual void modelTransformPatricio ();
    virtual void modelTransformAvio ();
    virtual void modelTransformHangar ();
    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void projectTransform ();
    virtual void viewTransform ();
    virtual void enviaPosFocus ();
    virtual void iniMaterialTerra ();

    void creaBuffersPatricio ();
    void creaBuffersAvio ();
    void creaBuffersHangar ();
    void creaBuffersTerra ();
    void carregaShaders ();
    void calculaCapsaModel (bool avio, Model &p, float &escala, glm::vec3 &centreBase);

    // VAO names
    GLuint VAO_Patr, VAO_Avio, VAO_Hang, VAO_Terra;
    // Program
    QOpenGLShaderProgram *program;
    // Viewport
    GLint ample, alt;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc, posfocusLoc;
    // attribute locations
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc;

    // Matriu de posició i orientació
    glm::mat4 View;
    // Matriu de transformació de l'avio
    glm::mat4 TGAvio;

    // models
    Model patr, avio, hang;
    // paràmetres calculats a partir de la capsa contenidora dels models
    glm::vec3 centreBasePat, centreBaseAvio, centreBaseHang;
    float escalaPat, escalaAvio, escalaHang;
    glm::vec3 capsaAvioMax, capsaAvioMin;

    glm::vec3 centreEsc;
    float radiEsc, ra, fov, zn, zf;

    // Definim els paràmetres del material del terra
    glm::vec3 amb, diff, spec;
    float shin;

    glm::vec3 posFocusAvio, posFoc;
    float angleX, angleY;
    bool camera2;

    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
};


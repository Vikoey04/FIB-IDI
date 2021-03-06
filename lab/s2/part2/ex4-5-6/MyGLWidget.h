// MyGLWidget.h
#include "BL2GLWidget.h"
#include "./Model/model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void initializeGL(); //De BL2, la reescribim

    virtual void paintGL(); //De BL2, l'allarguem per davant

    virtual void creaBuffers(); //De BL2, la reescribim

    virtual void carregaShaders(); //De BL2, l'allarguem per darrere

    virtual void keyPressEvent(QKeyEvent* event); // De BL2, la reescribim

    virtual void resizeGL(int w, int h); // De BL2, la reescribim

  private:
    int printOglError(const char file[], int line, const char func[]);

    void ini_camera();
    void projectTransform(); // Calcul de transformaciÃ³ de proj i enviar uniform a la mat cap al VS
    void viewTransform();
    
    void modelRotate();

    void calculaCapsaModel(Model &p, float &altura, glm::vec3 &centreEscena);

    void modelTransformPatricio1();

    // uniform locations
    GLuint projLoc, viewLoc, rotLoc;

    // VAO names
    GLuint VAO_Patricio;
    GLuint VAO_Terra;

    // VBO names
    GLuint VBO_PatricioPos, VBO_PatricioCol;
    GLuint VBO_TerraPos, VBO_TerraCol;

    // internal vars
    Model patricio;
    GLfloat rotacio = 0;


    // add vars
    GLfloat FOV, ra, znear, zfar;
    GLfloat FOVini;
    glm::vec3 OBS, VRP, UP;

    float alturaPat;
    glm::vec3 centrePatricio;
    
    bool cameraPerspectiva;

    GLfloat r, d;
    glm::vec3 Pmin, Pmax;

};

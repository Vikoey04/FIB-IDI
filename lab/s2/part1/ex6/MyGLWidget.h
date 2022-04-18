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

  private:
    int printOglError(const char file[], int line, const char func[]);

    void ini_camera();
    void projectTransform(); // Calcul de transformació de proj i enviar uniform a la mat cap al VS
    void viewTransform();
    
    void modelRotate();

    // uniform locations
    GLuint projLoc, viewLoc, rotLoc;

    // VAO names
    GLuint VAO_Homer;
    GLuint VAO_Terra;

    // VBO names
    GLuint VBO_HomerPos, VBO_HomerCol;
    GLuint VBO_TerraPos, VBO_TerraCol;

    // add vars
    GLfloat FOV, ra, znear, zfar;

    glm::vec3 OBS;
    glm::vec3 VRP;
    glm::vec3 UP;

    GLfloat rotacio = 0;

    Model homer;

};

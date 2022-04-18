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

  private:
    int printOglError(const char file[], int line, const char func[]);

    void ini_camera();
    void projectTransform(); // Calcul de transformació de proj i enviar uniform a la mat cap al VS
    void viewTransform();
    

    // uniform locations
    GLuint projLoc, viewLoc;

    // VAO names
    GLuint VAO_Homer;

    // VBO names
    GLuint VBO_HomerPos, VBO_HomerCol;

    // add vars
    GLfloat FOV, ra, znear, zfar;

    glm::vec3 OBS;
    glm::vec3 VRP;
    glm::vec3 UP;

    Model homer;
};

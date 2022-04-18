// MyGLWidget.h
#include "BL2GLWidget.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void carregaShaders(); //De BL2, l'allarguem per darrere

    virtual void paintGL(); //De BL2, l'allarguem per davant

  private:
    int printOglError(const char file[], int line, const char func[]);

    void ini_camera();
    void projectTransform(); // Calcul de transformació de proj i enviar uniform a la mat cap al VS
    void viewTransform();
    

    // atribute locations
    GLuint projLoc, viewLoc;

    // add vars
    GLfloat FOV, ra, znear, zfar;

    glm::vec3 OBS;
    glm::vec3 VRP;
    glm::vec3 UP;
};

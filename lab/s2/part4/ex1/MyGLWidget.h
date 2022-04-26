// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    void carregaShaders();
    void projectTransform();
    void paintGL();
    GLuint projLoc;
    GLuint viewLoc;
    void viewTransform();
    Model m;
    float radi;
    glm::vec3 centre;
    float distance, scale, rotate, FOV, raw, Znear, Zfar, angx, angy, angz;
    glm::vec3 OBS, VRP, UP;
    void creaBuffers();
    void initializeGL();
    int x_ini, x_fi, y_ini, y_fi;

    void calculaAtributs(glm::vec3 min, glm::vec3 max, glm::vec3& centre, float& radi);
    void iniCamara ();
    //void mouseEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

  private:
    int printOglError(const char file[], int line, const char func[]);

  public slots:

    void tractacanvifov(int fov);
};

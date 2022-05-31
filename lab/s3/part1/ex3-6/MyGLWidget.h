// MyGLWidget.h
#include "BL3GLWidget.h"

class MyGLWidget : public BL3GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL3GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);

    virtual void initializeGL();
    virtual void iniMaterialTerra();

  private:
    int printOglError(const char file[], int line, const char func[]);

    void passaPosFocus();
    void iniColorFocus();
    void iniLlumAmbient();

    GLuint posFocLoc, colFocLoc, llumAmbLoc;
    glm::vec3 posFoc;
};

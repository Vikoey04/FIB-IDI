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

    virtual void initializeGL(); //allarguem la del pare

  private:
    int printOglError(const char file[], int line, const char func[]);

    void posicioFocus();
    void colorFocus();
    void llumAmbient();

    GLuint posFocusLoc, colFocusLoc, llumAmbientLoc;
    glm::vec3 posF;
    bool focusDeCamera;
};

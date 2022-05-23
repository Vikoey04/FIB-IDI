// MyGLWidget.h
#include "LL4GLWidget.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);

    virtual void initializeGL(); // La reescribim de LL4
    virtual void modelTransformPatricio(); // La reescribim de LL4

  private:
    int printOglError(const char file[], int line, const char func[]);

    void interruptorC();
    void interruptorE();
    void interruptorP();

    GLuint posFocLoc1, colFocLoc1, posFocLoc2, colFocLoc2, posFocLoc3, colFocLoc3;
    float rotacio; // Per rotar el Patricio
    bool C_ON;
    bool E_ON;
    bool P_ON;

    glm::vec3 veczero;

    glm::vec3 col1;
    glm::vec3 col2;
    glm::vec3 col3;
    
};

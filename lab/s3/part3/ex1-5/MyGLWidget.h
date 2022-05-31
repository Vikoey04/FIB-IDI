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
    virtual void modelTransformPatricio();

  private:
    int printOglError(const char file[], int line, const char func[]);

    void posicioFocus();
    void colorFocus();
    void llumAmbient();

    void posicioFocus2();
    void colorFocus2();

    void interruptorF1();
    void interruptorF2();

    GLuint posFocusLoc, colFocusLoc, llumAmbientLoc;
    GLuint posFocusLoc2, colFocusLoc2;

    glm::vec3 posF, posF2;

    bool focusDeCamera;
    bool F1_ON;
    bool F2_ON;

    float movPatX, movPatY;

    bool focEscGroc;

    bool bfculling;
};

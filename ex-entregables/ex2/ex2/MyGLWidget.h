#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void paintGL(); // De LL2, la reescribim

    virtual void patrTransform(); //De LL2, la reescribim

    virtual void viewTransform(); // De LL2, la reescribim

    virtual void keyPressEvent(QKeyEvent* event); // De LL2, la reescribim

  private:
    int printOglError(const char file[], int line, const char func[]);

    void paret1Transform();
    void paret2Transform();
    void paret3Transform();

    void mouPorterLeft();
    void mouPorterRight();
};
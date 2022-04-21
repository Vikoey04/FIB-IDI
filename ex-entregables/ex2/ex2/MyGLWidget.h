#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void iniEscena(); // De LL2, la reescribim
    virtual void iniCamera(); // De LL2, la reescribim

    virtual void paintGL(); // De LL2, la reescribim

    virtual void patrTransform(); //De LL2, la reescribim

    virtual void projectTransform(); // De LL2, la reescribim

    virtual void viewTransform(); // De LL2, la reescribim

    virtual void mouseMoveEvent(QMouseEvent *e); // De LL2, la reescribim

    virtual void keyPressEvent(QKeyEvent* event); // De LL2, la reescribim

    virtual void tractamentGol(); // De LL2, l'allarguem per darrere

    virtual void dirInicialPilota(); // De LL2, l'allarguem per davant
    

  private:
    int printOglError(const char file[], int line, const char func[]);

    void paret1Transform();
    void paret2Transform();
    void paret3Transform();

    void mouPorterLeft();
    void mouPorterRight();

    bool pintaPilota;
};

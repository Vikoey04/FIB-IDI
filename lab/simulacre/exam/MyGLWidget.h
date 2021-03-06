#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio ();
    virtual void projectTransform ();
    virtual void viewTransform ();

    virtual void mouseMoveEvent(QMouseEvent *event);

    virtual void iniEscena();
    virtual void enviaColFocus();

  private:
    int printOglError(const char file[], int line, const char func[]);

    bool pintaCubs;
    bool focusGroc;
    int cubPat; // 1, 2 o 3 -> cub al que està associat el patricio
    int posCubs;

  public slots:
    void cameraPlanta();
    void cameraCustom();

    void patCub1();
    void patCub2();
    void patCub3();
};

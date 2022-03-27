// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    // initializeGL - Inicialitzacions del contexte grafic
    virtual void initializeGL();

    // paintGL - Cridat cada cop que cal refrescar la finestra
    // Tot el que es dibuixa es dibuixa aqui
    virtual void paintGL();

  private:
    int printOglError(const char file[], int line, const char func[]);

    void createBuffers();
    void carregaShaders();
    //void modelTransform();
    void projectTransform();
    void viewTransform ();

    // attribute locations


    // uniform locations
    GLuint projLoc, viewLoc;

    // VAO i VBO names
    GLuint VAO_HomerProves;
    //GLuint VBO_HomerProvesPos;

    // Program
    QOpenGLShaderProgram *program;

    // internal vars


    Model HomerProves;
};

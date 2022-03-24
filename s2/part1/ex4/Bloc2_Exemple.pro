TEMPLATE    = app
QT         += opengl

INCLUDEPATH +=  /usr/include/glm

FORMS += MyForm.ui

HEADERS += MyForm.h BL2GLWidget.h MyGLWidget.h

SOURCES += main.cpp MyForm.cpp \
        BL2GLWidget.cpp MyGLWidget.cpp

INCLUDEPATH += /home2/users/alumnes/1264129/dades/IDI/Model
SOURCES += /home2/users/alumnes/1264129/dades/IDI/Model/model.cpp

#include <QApplication>
#include "MyForm.h"

int main(int argc, char **argv){
    
    QApplication app(argc, argv);
    
    MyForm m;
    m.show();
    
    return app.exec();
}

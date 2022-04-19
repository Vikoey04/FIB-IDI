#include "MyLineEdit.h"

// constructor
MyLineEdit::MyLineEdit(QWidget *parent)
  : QLineEdit(parent) {
    connect(this, SIGNAL(returnPressed()), this, SLOT(tractaReturn()));

    // Inicialització d'atributs si cal
  }

// implementació slots
void MyLineEdit::tractaReturn() {
  // Implementació de tractaReturn

  emit returnPressed(text());
}

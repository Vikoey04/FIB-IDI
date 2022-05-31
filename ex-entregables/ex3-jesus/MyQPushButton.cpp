#include "MyQPushButton.h"
#include <iostream>

int MyQPushButton::col1 = -1;
int MyQPushButton::col2 = -1;
int MyQPushButton::col3 = -1;

MyQPushButton::MyQPushButton(QWidget *parent=0):QPushButton(parent)
{
    colr = -1;
}
//myWidget->setStyleSheet("background-color:black;");
//r v b g, 0 1 2 3
void MyQPushButton::assignarRand()
{
	QString numButt = text();//pilla el texto del button (numero)
    colr = rand()%4;
    if (!colr) setStyleSheet("background-color:red;");
    else if (colr == 1) setStyleSheet("background-color:green;");
    else if (colr == 2) setStyleSheet("background-color:blue;");
    else if (colr == 3) setStyleSheet("background-color:yellow;");

    if (numButt == QString::number(1)) col1 = colr;
    else if (numButt == QString::number(2)) col2 = colr;
    else if (numButt == QString::number(3)) col3 = colr;
    countTrios(); 
    //std::cout << col1 << ' ' << col2 << ' ' << col3 << std::endl;
}

void MyQPushButton::asignarStart()
{
    QString numButt = text();//pilla el texto del button (numero)
    colr = rand()%4;
    if (!colr) setStyleSheet("background-color:red;");
    else if (colr == 1) setStyleSheet("background-color:green;");
    else if (colr == 2) setStyleSheet("background-color:blue;");
    else if (colr == 3) setStyleSheet("background-color:yellow;");

    if (numButt == QString::number(1)) col1 = colr;
    else if (numButt == QString::number(2)) col2 = colr;
    else if (numButt == QString::number(3)) col3 = colr;

    emit asigStart();
}

void MyQPushButton::countTrios()
{
	if (col1 == col2 and col2 == col3) emit incrementarTrios();
}

void MyQPushButton::RESTART()
{
	setStyleSheet("background-color:rgb(124,124,124);");
	emit restartTOT();
}
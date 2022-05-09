#include "MyQPushButton.h"
#include <iostream>

int col1 = -1;
int col2 = -1;
int col3 = -1;
int trios = 0;

// Constructora
MyQPushButton::MyQPushButton(QWidget *parent=0):QPushButton(parent)
{
    colbut = -1;
}

void MyQPushButton::assignarRand()
{
    QString numBut = text();
    colbut = rand()%4;
    if (!colbut) setStyleSheet("background-color:red;");
    else if (colbut == 1) setStyleSheet("background-color:green;");
    else if (colbut == 2) setStyleSheet("background-color:blue;");
    else if (colbut == 3) setStyleSheet("background-color:yellow;");


    if (numBut == QString::number(1)) col1 = colbut;
    else if (numBut == QString::number(2)) col2 = colbut;
    else if (numBut == QString::number(3)) col3 = colbut;
    //countTrios();

}

void MyQPushButton::assignarStart()
{
    QString numBut = text();
    colbut = rand()%4;
    if (!colbut) setStyleSheet("background-color:red;");
    else if (colbut == 1) setStyleSheet("background-color:green;");
    else if (colbut == 2) setStyleSheet("background-color:blue;");
    else if (colbut == 3) setStyleSheet("background-color:yellow;");

    if (numBut == QString::number(1)) col1 = colbut;
    else if (numBut == QString::number(2)) col2 = colbut;
    else if (numBut == QString::number(3)) col3 = colbut;
}


/*
void MyQPushButton::countTrios()
{
    if (col1 == col2 and col1 == col3) {
        emit incrementarTrios();   
    }
}


void MyQPushButton::Restart()
{
    trios = 0;
    //emit returnTrios(trios);
}

void incrementarTrios() {
    trios++;
    //emit returnTrios(trios);
}
*/


//Signals
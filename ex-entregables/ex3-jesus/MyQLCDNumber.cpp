#include "MyQLCDNumber.h"
#include <iostream>

MyQLCDNumber::MyQLCDNumber(QWidget *parent=0):QLCDNumber(parent)
{
    ntrios = 0;
}

void MyQLCDNumber::incrementarTrios() 
{
    ++ntrios;
    display(ntrios);
    //std::cout << ntrios << std::endl;
}

void MyQLCDNumber::zero() 
{
    ntrios = 0;
    display(0);
}
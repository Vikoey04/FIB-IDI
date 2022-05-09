#include <QPushButton>
#include <QLCDNumber>


class MyQPushButton: public QPushButton
{
    Q_OBJECT

  public:
  
    int colbut;

    MyQPushButton (QWidget *parent);

  public slots:
    void assignarRand();
    void assignarStart();
    //void tractaReturn();
    /*
    void countTrios();
    void Restart();
    void incrementarTrios();
    */

  //signals:
    //void returnTrios (const int &);
    //void returnPressed (const QString &);
};
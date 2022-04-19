#include <QLineEdit>

class MyLineEdit: public QLineEdit
{

    Q_OBJECT

  public:
    MyLineEdit (QWidget *parent);

  public slots:
    void tractaReturn();

  signals:
    void returnPressed (const QString &);
}

#include <QLCDNumber>
class MyQLCDNumber:public QLCDNumber
{
	Q_OBJECT
	public:
		MyQLCDNumber(QWidget *parent);
		int ntrios;
	public slots:
		void incrementarTrios();
		void zero();

	signals:
		
}; 

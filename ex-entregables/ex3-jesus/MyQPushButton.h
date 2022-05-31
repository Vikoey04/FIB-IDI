#include <QPushButton>
class MyQPushButton:public QPushButton
{
	Q_OBJECT
	public:
		MyQPushButton(QWidget *parent);
		int colr, ntrios;
		static int col1, col2, col3;
	public slots:
		void assignarRand();
		void countTrios();
		void asignarStart();
		void RESTART();

	signals:
		void incrementarTrios();
		void asigStart();
		void restartTOT();
}; 

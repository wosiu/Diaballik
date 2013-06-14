#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gra.h"
#include "edytor.h"
#include "iplansza.h"
#include "nowagradialog.h"
#include <QPropertyAnimation>
#include <QLabel>
#include <QMessageBox>
#include <QtGui> //do resize eventu

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	void resizeEvent( QResizeEvent *event );
	IPlansza *scena;
	Tryb *tryb;

private:
	Ui::MainWindow *ui;
	qreal defaultWindowHeight;
	qreal defaultWindowWidth;
	QLabel *statusBarMonit;
	QMessageBox* boxMonit;
	const QString idGraczToKolor[3] = {"CZARNY", "BIAŁY", "EDYTOR"};

	void connector();

private slots:
	void setValidMoves(int);
	void showMonitOnStatusBar( QString monit );
	void showMonitInBox( QString monit );
	void showWinnerBox( int gracz );
	void aktualnyGracz( int graczId );
	void wykonaneRuchy( int przesuniec, int podan );

	void on_Zatwierdz_pushButton_clicked();
	void on_Cofnij_pushButton_clicked();
	void on_Powtorz_pushButton_clicked();
	void on_actionNowa_Gra_triggered();
	void on_actionEdytuj_plansze_triggered();

	void on_actionZapisz_gr_triggered();

public slots:
	void ustawNowyTryb( Tryb* nowyTryb );
};

#endif // MAINWINDOW_H

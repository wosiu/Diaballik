#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QLabel>
#include <QMessageBox>
#include <QtGui> //do resize eventu

#include "gra.h"
#include "edytor.h"
#include "iplansza.h"
#include "nowagradialog.h"
#include "ai.h"


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
	void off_AutoKomputer();
	bool kompAutoPlay;
	void connector();
	void disconnector();
	bool moveLock;
	AI* ai;

private slots:
	void setValidMoves(int);
	void showMonitOnStatusBar( QString monit );
	void showMonitInBox( QString monit );
	void showWinnerBox( int gracz );
	void nowaTura(int graczId);
	void aktualnyGracz( int graczId );
	void wykonaneRuchy( int przesuniec, int podan );
	void wzbudzKomputer();
	void setButtonsEnabled(bool v);

	void on_Zatwierdz_pushButton_clicked();
	void on_Cofnij_pushButton_clicked();
	void on_Powtorz_pushButton_clicked();
	void on_actionNowa_Gra_triggered();
	void on_actionEdytuj_plansze_triggered();
	void on_actionZapisz_gr_triggered();
	void on_actionZakoncz_triggered();
	void on_actionAutor_triggered();
	void on_actionPomoc_triggered();
	void on_AutoKomputer_clicked();
	void on_actionWzbudzKomputer_triggered();
	void on_actionDaj_podpowiedz_triggered();
	void on_Przerwij_AI_pushButton_clicked();

public slots:
	void ustawNowyTryb( Tryb* nowyTryb );

signals:
	void stopAI();
};

#endif // MAINWINDOW_H

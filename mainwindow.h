#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gra.h"
#include "iplansza.h"
#include <QPropertyAnimation>
#include <QLabel>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	IPlansza *scena;
	Tryb *tryb;

private:
	Ui::MainWindow *ui;
	QLabel *statusBarMonit;
	QMessageBox* boxMonit;

private slots:
	void setValidMoves(int);
	void showMonitOnStatusBar( QString monit );
	void showMonitInBox( QString monit );
	void aktualnyGracz(int graczId );

	void on_Zatwierdz_pushButton_clicked();
};

#endif // MAINWINDOW_H

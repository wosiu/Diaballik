#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gra.h"
#include "iplansza.h"
#include <QPropertyAnimation>

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

private slots:
	void askForAndSetValidMoves(int);

};

#endif // MAINWINDOW_H

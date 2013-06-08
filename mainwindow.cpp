#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	scena = new IPlansza(this);
	ui->graphicsView->setScene( scena );
	tryb = new Gra();

	//wykrywa klik na pionek, pyta gre o dostepne ruchy dla niego, nakazuje scenie dodac dostepne ruchy
	connect( scena, SIGNAL(clicked(int)), this, SLOT(askForAndSetValidMoves(int)) );
	///connect( scena, SIGNAL(chose(int,int)), scena, SLOT(move(int, int)));
	//wykrywa klik na pole dostepnego ruchu i przekazuje do gry
	connect( scena, SIGNAL(chose(int,int)), tryb, SLOT(move(int, int)));
	//gra informuje o ruchu logicznym = nakazanie wykonania ruchu na scenie
	connect( tryb, SIGNAL(moved(int,int)), scena, SLOT(move(int, int)));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::askForAndSetValidMoves(int pionekId)
{
	std::vector<int> pozycje = tryb->findValidMoves( pionekId );
	while( !pozycje.empty() )
	{
		scena->dodajDostepnePole( pozycje.back() );
		pozycje.pop_back();
	}
}

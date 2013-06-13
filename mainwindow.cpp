#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>


#include <ipilkarzyk.h>
#include <QSequentialAnimationGroup>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	scena = new IPlansza(this);
	ui->graphicsView->setScene( scena );

	statusBarMonit = new QLabel( this );
	ui->statusBar->addWidget( statusBarMonit );

	boxMonit = new QMessageBox(this);


	// TEST
/*
	QSequentialAnimationGroup *group = new QSequentialAnimationGroup;
	IPilkarzyk *a = new IPilkarzyk(scena, 3, 30);
	scena->addItem( a );
	a->setPos( 100,100 );

	QPropertyAnimation *animation = new QPropertyAnimation(a,"pos");
	animation->setDuration(1000);
	//animation->setStartValue( a->pos() );
	//animation->setEndValue( a->mapToParent( 0*IPole::rozmiar, 3*IPole::rozmiar ) );
	animation->setEndValue( a->mapToScene ( 0*IPole::rozmiar, 3*IPole::rozmiar ) );
	animation->setEasingCurve(QEasingCurve::InOutSine);
	animation->DeleteWhenStopped;
	//animation->start( QPropertyAnimation::DeleteWhenStopped );

	QPropertyAnimation *animation2 = new QPropertyAnimation(a,"pos");
	animation2->setDuration(1000);
	//animation2->setStartValue( a->pos() );
	//animation2->setEndValue( a->mapToParent( 0*IPole::rozmiar, -2*IPole::rozmiar ) );
	animation2->setEndValue( a->mapToScene ( 0*IPole::rozmiar, -2*IPole::rozmiar ) );
	animation2->setEasingCurve(QEasingCurve::InOutSine);
	animation2->DeleteWhenStopped;
	//animation2->start( QPropertyAnimation::DeleteWhenStopped );

	//group->start();
	group->addAnimation( animation );
	group->start();
	group->addPause(2000);
	group->addAnimation( animation2 );
	//group->start();
	*/
	// END

	//Komunikator komunikator = new Komunikator();
	tryb = new Edytor();

	//wykrywa klik na pionek, pyta gre o dostepne ruchy dla niego, nakazuje scenie dodac dostepne ruchy
	connect( scena, SIGNAL(clicked(int)), this, SLOT(setValidMoves(int)) );
	//wykrywa klik na pole dostepnego ruchu i przekazuje do gry
	connect( scena, SIGNAL(chose(int,int)), tryb, SLOT(move(int, int)));
	//gra informuje o ruchu logicznym = nakazanie wykonania ruchu na scenie
	connect( tryb, SIGNAL(moved(int,int)), scena, SLOT(move(int, int)));
	//laczymy komunikatory (uwagi, bledy, logi) tekstowe trybu z monitami okna:
	connect( tryb, SIGNAL(uwaga(QString)), this, SLOT( showMonitInBox(QString) ) );
	//laczymy informacje o obecnym graczu z monitem
	connect( tryb, SIGNAL(nowaTura(int)), this, SLOT(aktualnyGracz(int)) );
	//laczymy informacje o wygranej z oknem wygranej
	connect( tryb, SIGNAL(winDetector(int)), this, SLOT(showWinnerBox(int)) );
	//laczymy informacje o remisie
	//...polaczono showWinnderBox

	tryb->turaStart();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::setValidMoves( int pionekId )
{
	//qDebug()<<" mainwindow: setValidMoves(pionekId = " << pionekId << ")";
	std::vector<int> pozycje = tryb -> findValidMoves( pionekId );
	while( !pozycje.empty() )
	{
		scena->dodajDostepnePole( pozycje.back() );
		pozycje.pop_back();
	}
}


void MainWindow::showMonitInBox( QString monit )
{
	//box->setWindowTitle( QString("Komunikat sędziego") );
	boxMonit->setText( monit );
	boxMonit->setButtonText(1,"Ok");
	boxMonit->show();
}

void MainWindow::showWinnerBox( int gracz )
{
	//box->setWindowTitle( QString("Komunikat sędziego") );
	//TO DO: przyciski: zapisz historię gry lub kontynuuj (zeby mial mozliwosc cofniecia)
	//Nowa Gra
	QString monit;

	if ( gracz == 0 || gracz == 1)
		monit = "Wygrał gracz: " + QString::number(gracz) + "!";
	else
		monit = "Remis!";

	boxMonit->setText( monit );
	boxMonit->setButtonText(1,"Ok");
	boxMonit->show();
}

void MainWindow::showMonitOnStatusBar( QString monit )
{
	statusBarMonit->setText( monit );
	//ui->statusBar->showMessage( monit );
}

void MainWindow::on_Zatwierdz_pushButton_clicked()
{
	tryb->zatwierdz();
}

void MainWindow::aktualnyGracz( int graczId )
{
	scena->czyscDostepneRuchy();

	QString typ="";
	if( tryb->dajTypGracza( graczId ) == Tryb::KOMPUTER )
		typ = "Ruch komputera. ";

	showMonitOnStatusBar( typ + "Gracz: " + QString::number( graczId ) );
}

//TO DO: pressed()
void MainWindow::on_Cofnij_pushButton_clicked()
{
	if ( !scena->getLock() )
		tryb->undo();
}

void MainWindow::on_Powtorz_pushButton_clicked()
{
	if ( !scena->getLock() )
		tryb->redo();
}

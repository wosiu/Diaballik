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

	boxMonit = new QMessageBox( this );

	defaultWindowHeight = this->size().height();
	defaultWindowWidth = this->size().width();

	//gra czysta
	tryb = new Gra();
	connector();
	tryb->turaStart();
	scena->dodajPionki();
}

MainWindow::~MainWindow()
{
	delete tryb;
	delete ui;
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
	qreal dScale = 1;

	if( event->size().width() == defaultWindowWidth ) {}
	else if(event->oldSize().width() != -1)
		dScale = (qreal)qMin( event->size().width(), event->size().height() ) /
				 (qreal)qMin( defaultWindowWidth,  defaultWindowHeight );


	ui->graphicsView->resetTransform();
	ui->graphicsView->scale( dScale, dScale );
}

void MainWindow::connector()
{
	//wykrywa klik na pionek, pyta gre o dostepne ruchy dla niego, nakazuje scenie dodac dostepne ruchy
	connect( scena, SIGNAL(clicked(int)), this, SLOT(setValidMoves(int)) );
	//wykrywa klik na pole dostepnego ruchu i przekazuje do gry
	connect( scena, SIGNAL(chose(int,int)), tryb, SLOT(move(int, int)));
	//gra informuje o ruchu logicznym = nakazanie wykonania ruchu na scenie
	connect( tryb, SIGNAL(moved(int,int)), scena, SLOT(move(int, int)));
	//laczymy komunikatory (uwagi, bledy, logi) tekstowe trybu z monitami okna:
	connect( tryb, SIGNAL(uwaga(QString)), this, SLOT( showMonitInBox(QString) ) );
	//laczymy informacje o obecnym graczu z monitem
	connect( tryb, SIGNAL( nowaTura(int) ), this, SLOT( aktualnyGracz(int)) );
	//laczymy informacje o wygranej z oknem wygranej
	connect( tryb, SIGNAL(winDetector(int)), this, SLOT(showWinnerBox(int)) );
	//laczymy informacje o remisie
	//...polaczono showWinnerBox
	//laczymy informacje o ruchach
	connect( tryb, SIGNAL(wykonaneRuchy(int,int)), this, SLOT( wykonaneRuchy(int,int)) );
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
		monit = "Wygrał gracz: <b>" + idGraczToKolor[ gracz ] + "</b>!";
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
		typ = "Ruch komputera. \n";

	QString kolor="";

	ui->graczTablica->setText( typ + "Gracz: <b>" + idGraczToKolor[ graczId ] + "</b>" );
	//showMonitOnStatusBar( typ  );
}

void MainWindow::wykonaneRuchy(int przesuniec, int podan)
{
	QString info = "Przesunięć: " + QString::number( przesuniec ) + "\n";
	info += "Podań: " + QString::number( podan ) + "\n";

	ui->ruchyTablica->setText( info );
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

void MainWindow::on_actionNowa_Gra_triggered()
{
	delete tryb;
	tryb = new Gra();
	scena->ustawPionki();
	connector();
	tryb->turaStart();
}

void MainWindow::on_actionEdytuj_plansze_triggered()
{

	Tryb* nowyTryb = new Edytor( tryb->plansza );
	delete tryb;
	tryb = nowyTryb;

	scena->ustawPionki( tryb->plansza );
	connector();
	tryb->turaStart();
}

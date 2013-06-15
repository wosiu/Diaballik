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

	kompAutoPlay = ui->AutoKomputer->isChecked();
}

MainWindow::~MainWindow()
{
	delete tryb;
	delete scena;
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
	//laczymy ifnroamcje o nowej turze
	connect( tryb, SIGNAL( nowaTura(int) ), this, SLOT( nowaTura(int)) );
	//laczymy informacje o uaktualnieniu gracza z monitem
	connect( tryb, SIGNAL( graczUpdate(int) ), this, SLOT( aktualnyGracz(int)) );
	//laczymy informacje o wygranej z oknem wygranej
	connect( tryb, SIGNAL(winDetector(int)), this, SLOT(showWinnerBox(int)) );
	//laczymy informacje o remisie
	//...polaczono showWinnerBox
	//laczymy informacje o ruchach
	connect( tryb, SIGNAL(wykonaneRuchy(int,int)), this, SLOT( wykonaneRuchy(int,int)) );
	//laczymy ukrywanie przyciskow Cofnij / Powtorz
	//connect( tryb, SIGNAL(undoAble(bool)), ui->Cofnij_pushButton, SLOT(setEnabled(bool)) );
	//connect( tryb, SIGNAL(redoAble(bool)), ui->Powtorz_pushButton, SLOT(setEnabled(bool)) );
	//laczymy informacje o powrcoei z trybu edycji do gry
	connect( tryb, SIGNAL(zmianaTrybu(Tryb*)), this, SLOT(ustawNowyTryb(Tryb*)) );

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

void MainWindow::nowaTura( int graczId )
{
	aktualnyGracz( graczId );

	if ( kompAutoPlay )
		on_actionWzbudzKomputer_triggered();
}

void MainWindow::poprawDostepnoscPrzyciskow()
{
	int graczId = tryb->plansza.czyjRuch();

	//jesli ktorys z graczow to komputer
	if ( tryb->typGracza[0] == Tryb::KOMPUTER || tryb->typGracza[1] == Tryb::KOMPUTER )
	{
		ui->AutoKomputer->show();
	}
	else
	{
		ui->AutoKomputer->hide();
	}


	//blokowanie / odblokowywanie przyciskow UI:
	if ( tryb->typGracza[graczId] == Tryb::KOMPUTER )
	{
		//sterowanie rozgrywka i historia
		ui->Cofnij_pushButton->setEnabled( false );
		ui->Powtorz_pushButton->setEnabled( false );
		ui->Zatwierdz_pushButton->setEnabled( false );

		ui->actionDaj_podpowiedz->setEnabled( false );

		if ( kompAutoPlay ) ui->actionWzbudzKomputer->setEnabled( false );
		else				ui->actionWzbudzKomputer->setEnabled( true );
	}

	if ( tryb->typGracza[graczId] == Tryb::CZLOWIEK )
	{
		//sterowanie rozgrywka i historia

		qDebug() << "wszedl" <<(tryb->historyIterator > -1);

		ui->Cofnij_pushButton->setEnabled( false );
		ui->Cofnij_pushButton->setEnabled( (tryb->historyIterator > -1) );
		ui->Powtorz_pushButton->setEnabled( (tryb->historyIterator + 1 < (int)tryb->history.size()) );
		ui->Zatwierdz_pushButton->setEnabled( true );

		ui->actionDaj_podpowiedz->setEnabled( true );
		ui->actionWzbudzKomputer->setEnabled( false );
	}
}

void MainWindow::aktualnyGracz( int graczId )
{
	scena->czyscDostepneRuchy();

	//poprawDostepnoscPrzyciskow();

	//poprawienie monitu:
	QString typ="";
	if( tryb->dajTypGracza( graczId ) == Tryb::KOMPUTER )
		typ = "Ruch komputera. <br>";

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
	NowaGraDialog dialog;

	//jesli kliknieto OK
	if( dialog.exec() == QDialog::Accepted )
		ustawNowyTryb( dialog.ustawienia );
}

void MainWindow::on_actionEdytuj_plansze_triggered()
{
	Tryb* nowyTryb = new Edytor( tryb );
	ustawNowyTryb( nowyTryb );
}

void MainWindow::ustawNowyTryb( Tryb* nowyTryb )
{
	delete tryb;
	tryb = nowyTryb;

	scena->ustawPionki( tryb->plansza );
	connector();
	tryb->turaStart();
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

void MainWindow::on_actionZapisz_gr_triggered()
{
	if ( tryb->plansza.doubleWinCheck() )
	{
		showMonitInBox("Nie moża zapisać stanu planszy z niepoprawnym układem pionków (obaj gracze wygrywają). Popraw i ponów próbę zapisu.");
		return;
	}

	QString sciezka;

	sciezka = QFileDialog::getSaveFileName(
				this, tr("Zapis obecnego stanu gry"),
				QDesktopServices::storageLocation(QDesktopServices::HomeLocation),
				"Diabalik save (*.dblsave)" );

	if ( sciezka.lastIndexOf( ".dblsave" ) == -1 ||
		 sciezka.lastIndexOf( ".dblsave" ) + 9 != sciezka.size() )
			sciezka.append(".dblsave");

	QFile file( sciezka );
	file.open(QIODevice::WriteOnly);
	QDataStream out(&file);

	//zapisuje plansze poczatkową + kto zaczyna ruch:
	for( int i = 0; i < 17; i++ )
		out << tryb->dajPlanszePoczatkowa().dane[i];
	//zapisuje historie rozgrywki
	//for( int i = 0; i < tryb->history.size(); i++ )
	for( int i = 0; i < qMin( tryb->historyIterator + 1, (int)tryb->history.size() )
		 ; i++ )
	{
		out << tryb->history[i].pionekId << tryb->history[i].skad << tryb->history[i].dokad;
		//qDebug() << tryb->history[i].pionekId << tryb->history[i].skad << tryb->history[i].dokad;
	}

	file.close();
}

void MainWindow::on_actionZakoncz_triggered()
{
	qApp->quit();
}

void MainWindow::on_actionAutor_triggered()
{
	QMessageBox* box = new QMessageBox(this);
	box->setWindowTitle(QString("Autor"));
	box->setText(QString(
					 "Michał Woś\nmw336071@students.mimuw.edu.pl"));
	box->setButtonText(1,"Zamknij");
	box->show();
}

void MainWindow::on_actionPomoc_triggered()
{
	QMessageBox* box = new QMessageBox(this);
	box->setWindowTitle(QString("Pomoc"));
	QString tekst;

	tekst += "Gra Diabalik.<br>";
	tekst += "Zapis rozgrywki obejmuje historię do obecnie widocznego stanu planszy (w celu zapisu całej historii, przewiń w przód). Jeśli w obecnie aktywnej turze nie zostały wykonane żadne ruchy, tura ta zostanie porzucona w zapisie.<br>";
	tekst += "Gracz (człowiek) nie ma możliwości cofania ruchów komputera. W grze człowieka przeciwko komputerowi cofnięcie / powtórzenie ruchów komputera to cofnięcie wszystkich ruchów komputera w danej tuże wraz z jednym odpowiednim ruchem człowieka.<br>";

	box->setText( tekst );
	box->setButtonText(1,"Zamknij");
	box->show();
}

void MainWindow::on_AutoKomputer_clicked()
{
	kompAutoPlay = ui->AutoKomputer->isChecked();
	on_actionWzbudzKomputer_triggered();
}

void MainWindow::on_actionWzbudzKomputer_triggered()
{
	static int roundCounter = 0;
	qDebug() << "Gra::zatwierdz(): tura = " << roundCounter++;
	if( roundCounter > 50 )
	{
		qDebug() <<" STOOOOP!";
		return;
	}

	tryb->komputerGraj();
}


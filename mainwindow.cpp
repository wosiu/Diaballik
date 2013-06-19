#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "ipilkarzyk.h"
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
	moveLock = false;
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
	//laczymy informacje o ruchach
	connect( tryb, SIGNAL(wykonaneRuchy(int,int)), this, SLOT( wykonaneRuchy(int,int)) );
	//laczymy informacje o powrcoei z trybu edycji do gry
	connect( tryb, SIGNAL(zmianaTrybu(Tryb*)), this, SLOT(ustawNowyTryb(Tryb*)) );
	//laczymy informacje o stabilnosci planszy z wywolaniem komputera
	//(aby komputer nie wywolywal sie, gdy sa zakolejkowane fizyczne ruchy, poniewaz takowe mają
	//niski priorytet i wykonają sie po zakonczonych obliczeniach komputera
	connect( scena, SIGNAL(silent()), this, SLOT( wzbudzKomputer() ) );


}

void MainWindow::disconnector()
{
	disconnect( scena, SIGNAL(clicked(int)), this, SLOT(setValidMoves(int)) );
	disconnect( scena, SIGNAL(chose(int,int)), tryb, SLOT(move(int, int)));
	disconnect( tryb, SIGNAL(moved(int,int)), scena, SLOT(move(int, int)));
	disconnect( tryb, SIGNAL(uwaga(QString)), this, SLOT( showMonitInBox(QString) ) );
	disconnect( tryb, SIGNAL( nowaTura(int) ), this, SLOT( nowaTura(int)) );
	disconnect( tryb, SIGNAL( graczUpdate(int) ), this, SLOT( aktualnyGracz(int)) );
	disconnect( tryb, SIGNAL(winDetector(int)), this, SLOT(showWinnerBox(int)) );
	disconnect( tryb, SIGNAL(wykonaneRuchy(int,int)), this, SLOT( wykonaneRuchy(int,int)) );
	disconnect( tryb, SIGNAL(zmianaTrybu(Tryb*)), this, SLOT(ustawNowyTryb(Tryb*)) );
	disconnect( scena, SIGNAL(silent()), this, SLOT( wzbudzKomputer() ) );
}

void MainWindow::nowaTura( int graczId )
{
	aktualnyGracz( graczId );
	wzbudzKomputer();
}

void MainWindow::wzbudzKomputer()
{
	//qDebug() << "Wzbudzam kompa. Na planszy ruch: " << tryb->plansza.czyjRuch();
	moveLock = false;

	if ( kompAutoPlay )
		on_actionWzbudzKomputer_triggered();
}

void MainWindow::showMonitInBox( QString monit )
{
	boxMonit->setText( monit );
	boxMonit->setButtonText(1,"Ok");
	boxMonit->show();
}

void MainWindow::showWinnerBox( int gracz )
{
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
}

void MainWindow::on_Zatwierdz_pushButton_clicked()
{
	emit stopAI();
	tryb->zatwierdz();
}

void MainWindow::aktualnyGracz( int graczId )
{
	scena->czyscDostepneRuchy();

	//poprawienie monitu:
	QString typ="";
	if( tryb->dajTypGracza( graczId ) == Tryb::KOMPUTER )
		typ = "Ruch komputera. <br>";

	QString kolor="";

	ui->graczTablica->setText( typ + "Gracz: <b>" + idGraczToKolor[ graczId ] + "</b>" );
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
	if ( scena->getLock() ) return;

	emit stopAI();
	tryb->undo();
}

void MainWindow::on_Powtorz_pushButton_clicked()
{
	if ( scena->getLock() ) return;

	emit stopAI();
	tryb->redo();
}

void MainWindow::on_actionNowa_Gra_triggered()
{
	off_AutoKomputer();

	NowaGraDialog dialog;

	//jesli kliknieto OK
	if( dialog.exec() == QDialog::Accepted )
		ustawNowyTryb( dialog.ustawienia );
}

void MainWindow::on_actionEdytuj_plansze_triggered()
{
	off_AutoKomputer();

	if ( !tryb->history.empty() )
		showMonitInBox("Historia ruchów zostaje porzucona.");

	Tryb* nowyTryb = new Edytor( tryb );
	ustawNowyTryb( nowyTryb );
}

void MainWindow::ustawNowyTryb( Tryb* nowyTryb )
{
	off_AutoKomputer();

	disconnector();
	delete tryb;
	tryb = nowyTryb;
	scena->ustawPionki( tryb->plansza );
	connector();
	tryb->turaStart();
}

void MainWindow::setValidMoves( int pionekId )
{
	emit stopAI();

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
	off_AutoKomputer();

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
	off_AutoKomputer();

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

void MainWindow::off_AutoKomputer()
{
	ui->AutoKomputer->setChecked( false );
	on_AutoKomputer_clicked();
}

void MainWindow::on_AutoKomputer_clicked()
{
	kompAutoPlay = ui->AutoKomputer->isChecked();

	if( !kompAutoPlay )
		emit stopAI();
	else
		wzbudzKomputer();

}

void MainWindow::on_actionWzbudzKomputer_triggered()
{
	int gracz = tryb->plansza.czyjRuch();

	//sprawdzam czy tura nalezy do komputera
	if ( tryb->typGracza[ gracz ] == Tryb::KOMPUTER )
		on_actionDaj_podpowiedz_triggered();
}

//wykonuje podpowiedz na planszy
void MainWindow::on_actionDaj_podpowiedz_triggered()
{
	if ( moveLock ) return;
	moveLock = true;

	setButtonsEnabled( false );

	ai = new AI( &(tryb->plansza), tryb->przesuniecWTurze, tryb->podanWTurze );

	//laczymy z checkboxem "Auto Komputer" - odznaczenie checkboxa powoduje
	//przerwanie obliczen AI
	connect( this, SIGNAL( stopAI() ), ai, SLOT( stop() ) );

	//odpalamy watek liczacy
	ai->start();

	showMonitOnStatusBar("Obliczanie ruchu...");

	//w trakcie liczenia obslugujemy okno:
	while ( ai->isRunning() )
		QCoreApplication::processEvents();


	//po zakonczonych lub zastopowanych obliczeniach czyscimu komunikat,
	//zrywamy polaczenie, szarzymy przicisk AI, dalej usuwamy ai
	showMonitOnStatusBar("");
	disconnect( this, SIGNAL( stopAI() ), ai, SLOT( stop() ) );
	setButtonsEnabled( true );

	//jesli obliczylismy ruch i nie wykonano stopu, wykonujemy go
	if ( ai->done )
	{
		ruch r = ai->hint;
		delete ai;

		if ( r.czyRuch() )	tryb->move( r );
		else				tryb->zatwierdz();
	}
	else
	{
		delete ai;
		moveLock = false;
	}
}


void MainWindow::on_Przerwij_AI_pushButton_clicked()
{
	if( tryb->typGracza[ tryb->plansza.czyjRuch() ] == Tryb::KOMPUTER )
		off_AutoKomputer();
	else
		emit stopAI();
}

void MainWindow::setButtonsEnabled( bool v )
{
	ui->Cofnij_pushButton->setEnabled(v);
	ui->Powtorz_pushButton->setEnabled(v);
	ui->Zatwierdz_pushButton->setEnabled(v);
	ui->actionDaj_podpowiedz->setEnabled(v);
	ui->actionWzbudzKomputer->setEnabled(v);
}

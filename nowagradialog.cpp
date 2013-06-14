#include "nowagradialog.h"
#include "ui_nowagradialog.h"

NowaGraDialog::NowaGraDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NowaGraDialog)
{
	ui->setupUi(this);
	ui->lineEditAdres->setReadOnly(true);
}

NowaGraDialog::~NowaGraDialog()
{
	delete ui;
}

bool NowaGraDialog::validateAndSetSave()
{
	QFile save( saveSciezka );

	qDebug() << "validate start";

	//sprawdzamy czy wybrany plik dobrze sie otwiera
	if ( ! save.open( QIODevice::ReadOnly ) )
	{
		qDebug() << "blad otwiernia";
		save.close();
		return false;
	}
	//sprawdzam poprawnosc save'u (planszy poczatkowej i historii)
	else
	{
		QDataStream in( &save );

		//usuwam wszystkie pionki z planszy
		ustawienia->plansza.usunPionki();

		//ustawiam pionki z save
		for ( int i = 0; i < 16; i++ )
		{
			int8_t poz;

			//przedwczesny koniec pliku
			if ( in.atEnd() ) {qDebug("EOF1"); save.close(); return false;}

			in >> poz;

			// 2 pionki ustawione na tym samym polu lub niewlasciwa pozycja
			//lub ustawienie pilki na polu bez pionka
			if ( !ustawienia->plansza.czyDostepne( i,  poz )  )
				{qDebug()<<"2"<<poz ;save.close(); return false;}

			ustawienia->plansza.dane[i] = poz;
		}

		int8_t graczRozpoczynajacy;
		in >> graczRozpoczynajacy;

		//niewlasciwy gracz
		if ( graczRozpoczynajacy < 0 || graczRozpoczynajacy > 1 )
			{save.close(); qDebug("3"); return false;}

		ustawienia->plansza.dane[ 16 ] = graczRozpoczynajacy;

		//kopiuje plansze, na ktorej beda wykonywane ruchy do planszyPoczatkowej
		ustawienia->planszaPoczatkowa = ustawienia->plansza;

		//startujemy symulacyjna rozgrywke
		ustawienia->turaStart();

		//symuluje rozgrywke sprawdzajac poprawnosc danych i budujac historie w symulacji
		int8_t pionekId,skad,dokad;
		while( !in.atEnd() )
		{
			in >> pionekId >> skad >> dokad;

			//sprawdzam czy istnieje taki pionek
			if ( pionekId < 0 || pionekId > 15 ) {save.close(); qDebug("4"); return false;}

			//przechodzimy do nastepnej tury jesli obecny ruch w historii nalezy do kolejnego gracza
			if ( ustawienia->plansza.czyjRuch() != ustawienia->plansza.ktoryGracz( pionekId ) )
			{
				//sprawdzamy czy mozna przejsc do kolejnej tury
				if ( ustawienia->podanWTurze == 0 && ustawienia->przesuniecWTurze == 0 )
					{save.close(); qDebug("5 kolejna tura"); return false;}
				else
					ustawienia->zatwierdz();
			}

			//sprawdzam czy pionek stoi obecnie na miejscu zgodnym z historią
			if ( !ustawienia->plansza.dajPozycje( pionekId ) == skad  )
				{qDebug("6"); save.close(); return false;}

			//sprawdzam czy ruch z historii jest dostepny do wykonania
			if ( !ustawienia->isValidMove( pionekId, dokad ) )
				{qDebug("7"); save.close(); return false;}

			ustawienia->move( pionekId, dokad );
		}
	}

	return true;
}

void NowaGraDialog::accept()
{
	if ( ui->radioButtonPlanszaEdytor ->isChecked() )
		ustawienia = new Edytor();
	else //gra lub z pliku
		ustawienia = new Gra();

	if ( ui->radioButtonCZCzlowiek->isChecked() )
		ustawienia->typGracza[ 0 ] = Tryb::CZLOWIEK;
	else
		ustawienia->typGracza[ 0 ] = Tryb::KOMPUTER;

	if ( ui->radioButtonBCzlowiek->isChecked() )
		ustawienia->typGracza[ 1 ] = Tryb::CZLOWIEK;
	else
		ustawienia->typGracza[ 1 ] = Tryb::KOMPUTER;

	if ( ui->radioButtonPlanszaPlik ->isChecked() )
		if( !validateAndSetSave() )
		{
			QMessageBox msg;
			msg.setStandardButtons(QMessageBox::Ok);
			msg.setText("Niepoprawny plik save. Wybierz nowy.");
			msg.exec();

			delete ustawienia;
			return;
		}
	QDialog::accept();
}



void NowaGraDialog::on_radioButtonPlanszaPlik_clicked()
{
	ui->pushButtonWybierz->setEnabled(true);
	ui->lineEditAdres->setEnabled(true);
}

void NowaGraDialog::on_radioButtonPlanszaEdytor_clicked()
{
	ui->pushButtonWybierz->setEnabled(false);
	ui->lineEditAdres->setEnabled(false);
}

void NowaGraDialog::on_radioButtonPlanszaPusta_clicked()
{
	ui->pushButtonWybierz->setEnabled(false);
	ui->lineEditAdres->setEnabled(false);
}

void NowaGraDialog::on_pushButtonWybierz_clicked()
{
	saveSciezka = QFileDialog::getOpenFileName(
				this, tr("Wybierz save"),
				QDesktopServices::storageLocation(QDesktopServices::HomeLocation),
				"Diabalik save (*.dblsave)");

	if ( saveSciezka != "" )
		ui->lineEditAdres->setText( saveSciezka );
}

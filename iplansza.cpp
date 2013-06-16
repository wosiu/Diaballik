#include "iplansza.h"

/* INICJACJA PLANSZY */

IPlansza::IPlansza(QObject *parent) :
	QGraphicsScene(parent)
{
	rysujPodklad();
	stworzPionki();
	//dodajPionki();
	polaczPionki();
	locked = false;

	stillMoving = 0;
}

void IPlansza::rysujPodklad()
{
	this->setBackgroundBrush( QBrush( (QColor(205,133,63) ) ) );

	QBrush pola( QColor(210,180,140) );

	for (int i = 0; i < 7; i++ )
		for (int j = 0; j < 7; j++ )
		{
			this->addEllipse(
			   toPixels(i) + IPilkarzyk::przesuniecieWzgledne(),
			   toPixels(j) + IPilkarzyk::przesuniecieWzgledne(),
			   IPilkarzyk::rozmiar, IPilkarzyk::rozmiar,
			   QPen(), pola );


			//siatka:
			//IPole *pole = new IPole(this);pole->moveable(false);
			//this->addItem( pole );pole->setPos( i*IPole::rozmiar,j*IPole::rozmiar );
		}
}

void IPlansza::stworzPionki()
{
	for ( int i = 0; i < 7; i++ )
	{
		pionki[ i ] = new IPilkarzyk(this, 0, i );
		pionki[ i + 7 ] = new IPilkarzyk(this, 1, i+7 );
	}

	pionki[ 14 ] = new IPilka(this, 0, 14);
	pionki[ 15 ] = new IPilka(this, 1, 15);

	ustawPionki();
}

void IPlansza::dodajPionki()
{
	for ( int i = 0; i < 16; i++ )
		this->addItem( pionki[ i ] );
}

void IPlansza::dodajPionki(Plansza plansza)
{
	ustawPionki( plansza );
	for ( int i = 0; i < 16; i++ )
		this->addItem( pionki[ i ] );
}

void IPlansza::ustawPionki()
{
	for ( int i = 0; i < 7; i++ )
	{
		pionki[ i ]->setPos( toPixels(i), toPixels(0) );
		pionki[ i + 7 ]->setPos( toPixels(i), toPixels(6) );
	}

	pionki[ 14 ]->setPos( toPixels(3), toPixels(0) );
	pionki[ 15 ]->setPos( toPixels(3), toPixels(6) );
}

void IPlansza::ustawPionki( Plansza plansza )
{
	int x,y;
	for ( int i = 0; i < 16; i++ )
	{
		x = toPixels( plansza.dajPozycje( i ) % 7 );
		y = toPixels( plansza.dajPozycje( i ) / 7 );

		pionki[ i ]->setPos( x, y );
	}
}

void IPlansza::polaczPionki()
{
	for ( int i = 0; i < 16; i++ )
	{
		connect( pionki[i], SIGNAL(clicked(int)), this, SLOT(clickDetector(int)) );
		//lacze info o zakonczonych ruchach z sprawdzarka stanu stablinego
		//connect( pionki[i], SIGNAL(finished()), this, SLOT(decreaseStillMovin()) );
		//lub poiwyzsze laczyc bezposrednio z silent(), bo i tak grajKomputer w trybie nie pozwoli na wywolanie podczas wykonywania poprzedniego wywolania
		connect( pionki[i], SIGNAL( finished() ), this, SIGNAL(silent()) );

	}
		//gdybym nie chcial robic cos po drodze to:
		//connect( pionki[i], SIGNAL(clicked(int)), this, SIGNAL(clicked(int)) );
}

/* OBSŁUGA PLANSZY PODCZAS GRY */

void IPlansza::decreaseStillMovin()
{
	//movelock.tryLock(250);
	////stillMoving--;

	////qDebug() << "IPlansza:: still moving: "<< stillMoving;
	////Q_ASSERT( stillMoving >= 0 );

	////if ( stillMoving == 0 )
	////{
	////	emit silent();
	////	qDebug() << "Iplansza::decreaseSill..() finished";
	////}

	//movelock.unlock();
}

// (dx,dy) - wersor
void IPlansza::move(int pionekId, int dx, int dy)
{
	//qDebug() << "wszedl do move";
	//movelock.lock();
	//movelock.tryLock(250);
	//qDebug() << "wszedl za locka";
	////stillMoving++;
	//movelock.unlock();

	pionki[ pionekId ]->move(dx,dy);

	//przesunieto pionek wiec czyszcze dostepne ruchy z jego pierwotnej pozycji
	//zeby sie nie wyswietlaly po przejsciu do docelowej
	czyscDostepneRuchy();
	//i odklikuje, jesli byl klikniety
	clickedId = -1;

	//a moze unlockowoac sygnalem finished z pola?
	//movelock.unlock();
}

void IPlansza::move(int pionekId, int poz)
{
	qDebug() << "IPlansza::move( pionekId=" << pionekId <<", pozycja=" << poz <<")";
	int x = fromPixels ( pionki[ pionekId ]->pos().x() );
	int y = fromPixels ( pionki[ pionekId ]->pos().y() );

	int nx = poz % 7;
	int ny = poz / 7;

	move(pionekId, nx - x, ny - y);
}


//wykrywa klikniecie na pionek i informuje o tym MainWindow
void IPlansza::clickDetector( int poleId )
{
	//if( !(0 <= polId && poleId <= 15) ) return;
	if( locked ) return;
	//blokuje clickDetectora - klikanie na wszystkie pionki nieaktywne
	locked = true;
	czyscDostepneRuchy();

	//odklikuję = odznaczam dostepne obszary po ponownym klknieciu tego samego pionka
	//bo wykona sie tylko powyzsze czyscDostepneRuchy
	if ( clickedId == poleId )
	{
		clickedId = -1;
		locked = false;
		return;
	}

	clickedId = poleId;
	emit clicked( poleId );

	locked = false;
}

void IPlansza::ustawPole( IPole *pole, int pos )
{
	pole->setPos( toPixels(pos % 7), toPixels(pos / 7 ) );
}

//nastepnie MainWindow pyta Gre/Edytor o dostepne do ruchu pola i nakazuje plnaszy
//je wyrysowac:
void IPlansza::dodajDostepnePole( int pos )
{
	ICel *pole = new ICel( pos );
	ustawPole( pole, pos );
	connect( pole, SIGNAL(clicked(int)), this, SLOT(moveDetector(int)) );
	dostepneRuchy.push_back( pole );
	this->addItem( pole );
}

//wykryto klikniecie na ICel - pole dostepnego ruchu
void IPlansza::moveDetector( int pos )
{
	czyscDostepneRuchy();
	emit chose( clickedId, pos ); //z tym bedzie conneckniety gracz, ktory nakarze grze wykonac move na planszy, ta przekaze przez mainwidnow wykonanie ruchu na planszy
}

void IPlansza::czyscDostepneRuchy()
{
	while ( !dostepneRuchy.empty() )
	{
		disconnect( dostepneRuchy.back(), SIGNAL(clicked(int)), this, SLOT(moveDetector(int)) );
		this->removeItem( dostepneRuchy.back() );
		delete dostepneRuchy.back();
		dostepneRuchy.pop_back();
	}
}


bool IPlansza::getLock()
{
	for ( int i = 0; i < 16; i++ )
		if ( pionki[i]->getLock() ) return true;

	return false;
}

void IPlansza::clear()
{
	czyscDostepneRuchy();
	for( int i = 0; i < 16; i++ )
		this->removeItem( pionki[i] );
}

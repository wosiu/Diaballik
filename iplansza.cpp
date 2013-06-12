#include "iplansza.h"

/* INICJACJA PLANSZY */

IPlansza::IPlansza(QObject *parent) :
	QGraphicsScene(parent)
{
	rysujPodklad();
	dodajPionki();
	polaczPionki();
	locked = false;
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

void IPlansza::dodajPionki()
{
	for ( int i = 0; i < 7; i++ )
	{
		pionki[ i ] = new IPilkarzyk(this, 0, i );
		pionki[ i ]->setPos( toPixels(i), toPixels(0) );
		this->addItem( pionki[ i ] );
		pionki[ i + 7 ] = new IPilkarzyk(this, 1, i+7 );
		pionki[ i + 7 ]->setPos( toPixels(i), toPixels(6) );
		this->addItem( pionki[ i + 7 ] );
	}

	pionki[ 14 ] = new IPilka(this, 0, 14);
	pionki[ 14 ]->setPos( toPixels(3), toPixels(0) );
	this->addItem( pionki[ 14 ] );
	pionki[ 15 ] = new IPilka(this, 1, 15);
	pionki[ 15 ]->setPos( toPixels(3), toPixels(6) );
	this->addItem( pionki[ 15 ] );

}

void IPlansza::polaczPionki()
{
	for ( int i = 0; i < 16; i++ )
		connect( pionki[i], SIGNAL(clicked(int)), this, SLOT(clickDetector(int)) );
		//gdybym nie chcial robic cos po drodze to:
		//connect( pionki[i], SIGNAL(clicked(int)), this, SIGNAL(clicked(int)) );
}

/* OBSŁUGA PLANSZY PODCZAS GRY */

// (dx,dy) - wersor
void IPlansza::move(int pionekId, int dx, int dy)
{
	pionki[ pionekId ]->move(dx,dy);

	//przesunieto pionek wiec czyszcze dostepne ruchy z jego pierwotnej pozycji
	//zeby sie nie wyswietlaly po przejsciu do docelowej
	czyscDostepneRuchy();
	//i odklikuje, jesli byl klikniety
	clickedId = -1;
}

void IPlansza::move(int pionekId, int poz)
{
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

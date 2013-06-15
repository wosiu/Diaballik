#include "tryb.h"
#include <QDebug>

Tryb::Tryb()
{
	//TO DO w zaleznosci od konfigu gry:
	typGracza[1] = CZLOWIEK;
	typGracza[0] = CZLOWIEK;
	podanWTurze = przesuniecWTurze = 0;
	historyIterator = -1;
	isKomputerAutoPlay = false;
}

Tryb::~Tryb()
{
}

/*Tryb::TYPGRACZA Tryb::dajTypGracza( int graczId )
{
	return KOMPUTER;
}*/


void Tryb::physicalMove( int pionekId, int pozycja )
{
	qDebug() << "Tryb::physicalMove ( pionekId = " << pionekId <<", pozycja = "<< pozycja << ")";
	plansza.przesun( pionekId, pozycja );
	emit moved( pionekId, pozycja );
}

void Tryb::komputerAutoPlay( bool autoplay )
{
	isKomputerAutoPlay = autoplay;
}

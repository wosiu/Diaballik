#include "tryb.h"
#include <QDebug>

Tryb::Tryb()
{
	plansza = new Plansza();
}

Tryb::~Tryb()
{
	delete plansza;
}

/*Tryb::TYPGRACZA Tryb::dajTypGracza( int graczId )
{
	return KOMPUTER;
}*/


void Tryb::physicalMove( int pionekId, int pozycja )
{
	qDebug() << "Tryb::physicalMove ( pionekId = " << pionekId <<", pozycja = "<< pozycja << ")";
	plansza->przesun( pionekId, pozycja );
	emit moved( pionekId, pozycja );
}

#include "tryb.h"
#include <QDebug>

Tryb::Tryb()
{
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

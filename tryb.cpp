#include "tryb.h"
#include <QDebug>

const int Tryb::CZLOWIEK = 0;
const int Tryb::KOMPUTER = 1;
const int Tryb::EDYTOR = 2;

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
	//wykonac ruch na (kopi?) planszy

	emit moved( pionekId, pozycja );
}

bool Tryb::isValidMove( int pionekId, int pos )
{
	Q_ASSERT_X(false,"isValidMove","odwolanie do funkcji z nadklasy.");
	return true;
}


std::vector<int> Tryb::findValidMoves( int pionekId )
{
	Q_ASSERT_X(false,"findValidMoves()","odwolanie do funkcji z nadklasy.");
	std::vector<int>res;
	for(int i=0;i<49;i++)
		res.push_back(i);

	return res;
}


void Tryb::zatwierdz()
{
	Q_ASSERT_X(false,"zatwierdz()","odwolanie do slota z nadklasy.");
}


void Tryb::move( int pionekId, int pozycja )
{
	Q_ASSERT_X(false,"move()","odwolanie do slota z nadklasy.");
	physicalMove( pionekId, pozycja );
}

void Tryb::moveDetector( int pionekId, int pozycja )
{
	Q_ASSERT_X(false,"moveDetector()","odwolanie do slota z nadklasy.");
	move( pionekId, pozycja );
}

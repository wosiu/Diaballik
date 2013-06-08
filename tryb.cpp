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

bool Tryb::isValidMove( int pionekId, int pos )
{
	return true;
}

std::vector<int> Tryb::findValidMoves( int pionekId )
{
	std::vector<int>res;
	for(int i=0;i<49;i++)
		res.push_back(i);

	return res;
}

void Tryb::moveFromUIDetected( int pionekId, int pos )
{
	if( ! isValidMove( pionekId, pos  ) ) return;

	gracz[ plansza->ktoryGracz() ].move( pionekId, pos );
}

struct Tryb::ruch
{
	int8_t pionekId,skad,dokad;

	/*ruch(int pionekId, int skad, int dokad) :
		pionekId(pionekId),
		skad(skad),
		dokad(dokad);*/

	ruch(int pionekId, int skad, int dokad)
	{
		this->skad = skad;
		this->dokad = dokad;
		this->pionekId = pionekId;
	}
};

void Tryb::physiacalMove( pionekId, pozycja )
{
	Q_ASSERT( isValidMove( pionekId, pozycja  ) );
	//wykonac ruch na (kopi?) planszy

	emit moved( pionekId, pozycja );
}

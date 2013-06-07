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

void Tryb::userMoveDetected( int pionekId, int pos )
{
	if( ! isValidMove( pionekId, pos  ) ) return;
	emit moved( pionekId, pos );
}

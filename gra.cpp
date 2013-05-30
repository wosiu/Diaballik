#include "gra.h"

Gra::Gra()
{
}

bool Gra::isValidMove( int pionekId, int pos )
{
	return true;
}

std::vector<int> Gra::findValidMoves( int pionekId )
{
	std::vector<int>res;
	for(int i=13;i<16;i++)
		res.push_back(i);

	return res;
}

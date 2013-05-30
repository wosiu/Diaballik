#ifndef GRA_H
#define GRA_H

#include "tryb.h"

class Gra : public Tryb
{
public:
	Gra();
	bool isValidMove( int pionekId, int pos );
	std::vector<int> findValidMoves( int pionekId );
};

#endif // GRA_H

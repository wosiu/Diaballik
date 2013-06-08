#ifndef GRA_H
#define GRA_H

#include "tryb.h"
#include "komputer.h"
#include "czlowiek.h"
#include <algorithm>

class Gra : public Tryb
{

public:
	Gra();
	~Gra();
	bool isValidMove( int pionekId, int pos );
	std::vector<int> findValidMoves( int pionekId );

public slots:
	void zatwierdz();
	void move( int pionekId, int pozycja );
	//bool undo(); TODO
	//bool redo(); TODO

private:
	std::vector < ruch > ruchy;
	Gracz* gracze[2];

};

#endif // GRA_H

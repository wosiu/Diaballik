#ifndef GRA_H
#define GRA_H

#include "tryb.h"
#include "ai.h"
#include <algorithm>
#include <vector>

class Gra : public Tryb
{

public:
	Gra();
	~Gra();
	bool isValidMove( int pionekId, int pos );
	std::vector<int> findValidMoves( int pionekId );
	TYPGRACZA dajTypGracza( int graczId );
	void turaStart();

protected:
	TYPGRACZA typGracza[2]; //TO DO: sprawdzic czy readonly z zewnatrz

private:
	std::vector < ruch > ruchy; //w turze, jednego gracza
	AI *ai;
	std::vector < int > validateAllMoves( int pionekId );
	std::vector < Plansza > history;
	int historyIterator;

public slots:
	void zatwierdz();
	void move( int pionekId, int pozycja );
	void moveDetector( int pionekId, int pozycja );
	bool undo();
	bool redo();

public slots:
	void komputerGraj( int gracz );
	//void czlowiekGraj(); == moveDetector

};

#endif // GRA_H

#ifndef GRA_H
#define GRA_H

#include <algorithm>
#include <vector>

#include "tryb.h"

class Gra : public Tryb
{
	Q_OBJECT

public:
	Gra();
	Gra(Tryb* innyTryb);
	void inicjuj();
	bool isValidMove( int pionekId, int pos );
	std::vector<int> findValidMoves( int pionekId );
	TYPGRACZA dajTypGracza( int graczId );
	void turaStart();
	Plansza dajPlanszePoczatkowa();

private:
	std::vector < int > validateAllMoves( int pionekId );
	bool isEndGame();
	bool isMoveLocked;

	//obsluga historii:
	void zliczRuchyWTurze();
	void addToHistory( ruch r );
	void poprawGraczaWzgledemHistorii();

public slots:
	void zatwierdz();
	void move( int pionekId, int pozycja );
	void moveDetector( int pionekId, int pozycja );
	bool undo();
	bool redo();

};

#endif // GRA_H

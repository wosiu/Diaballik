#ifndef GRA_H
#define GRA_H

#include "tryb.h"
#include "ai.h"
#include <algorithm>
#include <vector>
#include "ai.h"

class Gra : public Tryb
{

public:
	Gra();
	Gra( Tryb* innyTryb );
	//Gra( const Gra innaGra );
	~Gra();
	void inicjuj();
	bool isValidMove( int pionekId, int pos );
	std::vector<int> findValidMoves( int pionekId );
	TYPGRACZA dajTypGracza( int graczId );
	void turaStart();
	Plansza dajPlanszePoczatkowa();

private:
	//std::vector < ruch > ruchy; //w turze, jednego gracza
	AI *ai;
	std::vector < int > validateAllMoves( int pionekId );
	bool isEndGame();
	bool isMoveLocked;

	//obsluga historii:
	void zliczRuchyWTurze();
	void addToHistory( ruch r );
	void poprawGraczaWzgledemHistorii();
	//Plansza planszaStartowa;

public slots:
	void zatwierdz();
	void move( int pionekId, int pozycja );
	void moveDetector( int pionekId, int pozycja );
	bool undo();
	bool redo();
	void komputerGraj();
	//void czlowiekGraj(); == moveDetector

};

#endif // GRA_H

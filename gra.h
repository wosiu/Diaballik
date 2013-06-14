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
	Gra( Tryb* innyTryb );
	//Gra( const Gra innaGra );
	~Gra();
	void inicjuj();
	bool isValidMove( int pionekId, int pos );
	std::vector<int> findValidMoves( int pionekId );
	TYPGRACZA dajTypGracza( int graczId );
	void turaStart();

private:
	//std::vector < ruch > ruchy; //w turze, jednego gracza
	AI *ai;
	std::vector < int > validateAllMoves( int pionekId );
	bool isEndGame();

	//obsluga historii:
	int historyIterator;
	std::vector < ruch > history;
	void zliczRuchyWTurze();
	void addToHistory( ruch r );
	void poprawGracza();
	//Plansza planszaStartowa;

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

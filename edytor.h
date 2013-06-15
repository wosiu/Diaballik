#ifndef EDYTOR_H
#define EDYTOR_H

#include "tryb.h"
#include "gra.h"

class Edytor : public Tryb
{
public:
	Edytor();
	Edytor( Tryb* innyTryb );
	void inicjuj();
	//Edytor( Plansza plansza );
	std::vector<int> findValidMoves( int pionekId ); //done
	TYPGRACZA dajTypGracza( int graczId ); //done
	virtual bool isValidMove( int pionekId, int pos );
	void turaStart(); //done
	Plansza dajPlanszePoczatkowa();

public slots:
	void zatwierdz(); //done
	void move( int pionekId, int pozycja ); //done
	void moveDetector( int pionekId, int pozycja ); //done
	bool undo();
	bool redo();
	void komputerGraj();
};

#endif // EDYTOR_H

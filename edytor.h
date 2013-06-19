#ifndef EDYTOR_H
#define EDYTOR_H

#include "tryb.h"
#include "gra.h"

class Edytor : public Tryb
{
	Q_OBJECT

public:
	Edytor();
	Edytor( Tryb* innyTryb );
	void inicjuj();
	std::vector<int> findValidMoves( int pionekId );
	TYPGRACZA dajTypGracza( int graczId );
	virtual bool isValidMove( int pionekId, int pos );
	void turaStart();
	Plansza dajPlanszePoczatkowa();

public slots:
	void zatwierdz();
	void move( int pionekId, int pozycja );
	void moveDetector( int pionekId, int pozycja );
	bool undo();
	bool redo();
	void komputerGraj();
};

#endif // EDYTOR_H

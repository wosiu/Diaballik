#ifndef EDYTOR_H
#define EDYTOR_H

#include "tryb.h"

class Edytor : public Tryb
{
public:
	Edytor();
	Edytor( Tryb* innyTryb );
	std::vector<int> findValidMoves( int pionekId ); //done
	TYPGRACZA dajTypGracza( int graczId ); //done
	virtual bool isValidMove( int pionekId, int pos );
	void turaStart(); //done

public slots:
	void zatwierdz(); //done
	void move( int pionekId, int pozycja ); //done
	void moveDetector( int pionekId, int pozycja ); //done
	bool undo();
	bool redo();
};

#endif // EDYTOR_H

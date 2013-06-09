#ifndef GRA_H
#define GRA_H

#include "tryb.h"
#include <algorithm>
#include <vector>

class Gra : public Tryb
{

public:
	Gra();
	~Gra();
	bool isValidMove( int pionekId, int pos );
	std::vector<int> findValidMoves( int pionekId );

private:
	std::vector < ruch > ruchy;
	int typGracza[2];

public slots:
	void zatwierdz();
	void move( int pionekId, int pozycja );
	void moveDetector( int pionekId, int pozycja );
	//bool undo(); TODO
	//bool redo(); TODO

public slots:
	void komputerGraj( int gracz );
	//void czlowiekGraj(); == moveDetector


signals:
	//to do:
	//void nowaTura( int gracz );

};

#endif // GRA_H

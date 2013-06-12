#ifndef EDYTOR_H
#define EDYTOR_H

#include "tryb.h"
#include "gra.h"

class Edytor //: public Tryb
{
public:
	Edytor();



public slots:
	void zatwierdz();
	void move( int pionekId, int pozycja );
	void moveDetector( int pionekId, int pozycja );





};

#endif // EDYTOR_H

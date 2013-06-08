#ifndef GRACZ_H
#define GRACZ_H

//#include "gra.h"
#include <vector>

class Gracz
{
public:
	Gracz();
	virtual void move( int pionekId, int pozycja );
	virtual std::vector<int> dajDostepneRuchy(int pionekId );
	// Gra *gra;

private:


};

#endif // GRACZ_H

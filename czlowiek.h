#ifndef CZLOWIEK_H
#define CZLOWIEK_H

#include "gracz.h"

class Czlowiek : public Gracz
{

public:
	Czlowiek(/* Gra *gra*/ );
	void move( int pionekId, int pozycja );
	std::vector<int> dajDostepneRuchy(int pionekId );

private:
	//gracz widzi gre, zeby widziec plansze, etc..
	Gra gra;

};

#endif // CZLOWIEK_H

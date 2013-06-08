#ifndef CZLOWIEK_H
#define CZLOWIEK_H

#include "gracz.h"
#include "gra.h"

class Czlowiek : public Gracz
{
	Q_OBJECT

public:
	Czlowiek(/* Gra *gra*/ );
	void move(int pionekId, int pozycja , Gra * gra);
	std::vector<int> dajDostepneRuchy(int pionekId );

private:
	//gracz widzi gre, zeby widziec plansze, etc..

};

#endif // CZLOWIEK_H

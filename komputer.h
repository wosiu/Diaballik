#ifndef KOMPUTER_H
#define KOMPUTER_H

#include "gracz.h"

class Komputer : public Gracz
{
public:
	Komputer();
	virtual std::vector<int> dajDostepneRuchy(int pionekId );

};

#endif // KOMPUTER_H

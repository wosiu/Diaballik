#ifndef AI_H
#define AI_H

#include "plansza.h"


struct hint {
	int pionekId, pozycja;
	hint( int pionekId, int pozycja ) {
		this->pionekId = pionekId; this->pozycja = pozycja;
	}
};


class AI
{

private:
	const int INF = 1073741824;
	int alfabeta(int alfa, int beta);

public:
	AI();

	hint dajRuch( Plansza* plansza, int gracz, int zuzytychPrzesuniec, int zuzytychPodan );

};

#endif // AI_H

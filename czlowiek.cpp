#include "czlowiek.h"

Czlowiek::Czlowiek()
{
	//this->gra = gra;
}

void Czlowiek::move(int pionekId, int pozycja , Gra* gra )
{
	//gra->makeMove( pionekId, pozycja );
}

std::vector<int> Czlowiek::dajDostepneRuchy(int pionekId )
{
	//return gra->plansza->dajRuchy();
}

/*
//gra sprawdza czy ruchy gracza są poprawne
Czlowiek::wykonajRuch()
{
	while( przechwyoneRuchy.empty() ) {}

	ruch = przechwyconeRuchy.poll();
	if( ruch == zatwierdzenie ) przechwyconeRuchy.clear();

	return ruch;

}
*/

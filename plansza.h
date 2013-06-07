#ifndef PLANSZA_H
#define PLANSZA_H

#include <cstdint>
#include <vector>
#include <algorithm>
#include <QtGlobal>

class Plansza
{
public:
	Plansza();
	int8_t* zapisz();
	int dajPozycje( int pionekId );
	std::vector<int> dajWszystkiePuste();
	bool czyPuste( int x, int y );
	std::vector<int> dajSasiedniePuste(int pilkarzId );
	std::vector<int> dajPodania( int pionekId );

private:
//[0-6] - nr pól pionków gracza 0; [7-13] - gracza 1, [14]/[15] - pilki gracz 0/1,
//[16] - 0/1 - ktory gracz zaczyna
	int8_t dane[17];

};

#endif // PLANSZA_H

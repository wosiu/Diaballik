#ifndef PLANSZA_H
#define PLANSZA_H

#include <cstdint>
#include <vector>
#include <algorithm>
#include <QtGlobal>
#include <QDebug>

class Plansza
{
public:
	Plansza();
	int8_t* zapisz();
	static int ktoryGracz( int poleId );
	static bool czyPilka( int pionekId );
	static bool czyPilkarzyk( int pionekId );
	int dajPozycje( int pionekId );
	bool czyPuste( int x, int y );
	bool czyPuste(int pozycja);
	std::vector<int> dajWszystkiePuste();
	std::vector<int> dajSasiedniePuste(int pilkarzId );
	std::vector<int> dajPodania( int pionekId );
	std::vector<int> dajDruzyne( int poleId );
	std::vector<int> dajRuchy(int pionekId );
	int dajIdPodajacego( int pilkaId );
	int czyjRuch();
	void nastepnyGracz();
	void przesun( int pionekId, int pozycja );
	bool remis();
	int winCheck();
	bool doubleWinCheck();
	bool unfairGameCheck(int gracz);
	unsigned long long hashCode();


private:
//[0-6] - nr pól pionków gracza 0; [7-13] - gracza 1, [14]/[15] - pilki gracz 0/1,
//[16] - 0/1 - ktory gracz zaczyna

	int8_t dane[17];
	//int8_t pola[7][7];
	//bool polaWypelnione;
};

#endif // PLANSZA_H

#include "gra.h"

Gra::Gra()
{
	//TO DO w zaleznosci od konfigu gry:

}

Gra::~Gra()
{
	delete gracze[0];
	delete gracze[1];
}

bool Gra::isValidMove( int pionekId, int pos )
{
	//sprawdzam czy dobry gracz
	if ( plansza->czyjRuch() != plansza->ktoryGracz( pionekId ) ) return false;

	//sprawdzam czy ma dostepne jeszcze ruchy
	if ( ruchy.size() > 3 ) return false;

	//sprawdzam czy nie nie przekracza ilosci podan/przesuniec
	int podanWTurze = 0;
	int przesuniecWTurze = 0;

	for ( int i = 0; i < 3; i++ )
		if ( plansza->czyPilka( pionekId ) )
			podanWTurze++;
		else //przesuniecie pilkarzyka
			przesuniecWTurze++;

	if ( podanWTurze == 1 && plansza->czyPilka( pionekId) ) return false;
	if ( przesuniecWTurze == 2 && plansza->czyPilkarzyk( pionekId) ) return false;


	//sprawdzam czy podany ruch jest dostepny (w tym miejscu powinien zawsze byc)
	std::vector<int>dobreRuchy = plansza->dajRuchy( pionekId );
	Q_ASSERT( std::find( dobreRuchy.begin(), dobreRuchy.end(), pos )
			  != dobreRuchy.end() );

	return true;
}

std::vector<int> Gra::findValidMoves( int pionekId )
{
	int czyjRuch = plansza->czyjRuch();

	//sprawdzam czy zapytano z obecnego gracza
	if ( czyjRuch != plansza->ktoryGracz( pionekId ) )
		return std::vector<int>();

	//pyta gracza o jego dostepne ruchy
	return gracze[ czyjRuch ]->dajDostepneRuchy( pionekId );
}


void Gra::zatwierdz()
{
	//czy jakies ruchy wykonane
	if( ruchy.empty() ) return;

	//historia.push_back( plansza->kopiuj() );
	ruchy.clear();

	plansza->nastepnyGracz();
	emit nastepnyGracz( plansza->czyjRuch() );
}


void Gra::move( int pionekId, int pozycja )
{
	//nie przyjdzie zawolanie z UI z pionkow kompa, bo nie wyswietla sie validMoves dla niego, wiec jest spoko
	Q_ASSERT( isValidMove( pionekId, pozycja  ) );

	//ruchy.push_back(
	//			ruch( pionekId, plansza->dajPozycje(pionekId), pozycja ) );


	//if( !ruchy.empty() ) emit odblikujzatwierdz

	//nie odsylam do czlowieka bo nie ma po co..
	//gracz[ plansza->ktoryGracz() ].move( pionekId, pos );
	physicalMove( pionekId, pozycja );
}



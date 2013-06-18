#include "aistan.h"

AIstan::AIstan()
{

}

AIstan::AIstan(Plansza *plansza, przesuniecWTurze, podanWTurze)
{
	for( int i=0; i<16; i++ )
		this->dane[ i ] = plansza->dane[ i ];

	this->przesuniecWTurze = przesuniecWTurze;
	this->podanWTurze = podanWTurze;
}

AIstan::~AIstan()
{
	foreach ( AIstan* stan, sons )
	{
		delete stan;
	}
}

QVector<AIstan> AIstan::generatorStanow()
{
	int gracz = czyjRuch();
	std::vector< AIstan> wynik;

	//jesli na planszy jest wygrana, to nie ma dostepnych ruchow
	if ( stan.winCheck() != -1 );
		return std::vector< AIstan>();

	//dostepne podanie z tego stanu
	if ( podanWTurze < 1 )
		for ( int i = 7 * gracz; i < 7 + 7 * gracz; i++ )
		{
			std::vector<int> dostepne = dajRuchy( i );
			while ( !dostepne.empty() )
			{
				AIstan nowy( this, przesuniecWTurze, podanWTurze + 1 );
				nowy.przesun( i, dostepne.back() );
				dostepne.pop_back();
				wynik.push_back( nowy );
			}
		}

	//dostepne przesuniecia z tego stanu
	if ( przesuniecWTurze < 2 )
		for ( int i = 7 * gracz; i < 7 + 7 * gracz; i++ )
		{
			std::vector<int> dostepne = dajRuchy( i );
			while ( !dostepne.empty() )
			{
				AIstan nowy( this, przesuniecWTurze + 1, podanWTurze );
				nowy.przesun( i, dostepne.back() );
				dostepne.pop_back();
				wynik.push_back( nowy );
			}
		}

	//zatiwerdzenie tury (nie wykonanie zadnego ruchu, przelaczenie na gracza)
	if ( przesuniecWTurze + podanWTurze > 0 )
	{
		AIstan nowy( this, 0, 0);
		nowy.nastepnyGracz();
		wynik.push_back( nowy );
	}

	return wynik;
}

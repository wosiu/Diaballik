#include "aistan.h"

AIstan::AIstan()
{
	v = INF;
	alfa = -INF;
	beta = INF;
	przesuniecWTurze = podanWTurze = 0;
}

AIstan::AIstan(Plansza *plansza, int przesuniecWTurze, int podanWTurze)
{
	//AIstan();

	for( int i=0; i<17; i++ )
		this->dane[ i ] = plansza->dane[ i ];

	this->przesuniecWTurze = przesuniecWTurze;
	this->podanWTurze = podanWTurze;
}

AIstan::~AIstan()
{
	while ( !sons.empty() )
	{
		AIstan* del = sons.back();
		delete del;
		sons.pop_back();
	}
}


void AIstan::dodajStan( int idPrzesunietego, int przesuniecWTurze, int podanWTurze  )
{
	std::vector<int> dostepne = dajRuchy( idPrzesunietego );

	while ( !dostepne.empty() )
	{
		AIstan* nowy = new AIstan( this, przesuniecWTurze, podanWTurze );
		nowy->przesun( idPrzesunietego, dostepne.back() );
		dostepne.pop_back();
		sons.push_back( nowy );
	}
}


void AIstan::generatorSynow()
{
	int gracz = czyjRuch();
	Q_ASSERT( gracz == 0 || gracz == 1 );

	//jesli na planszy jest wygrana, to nie ma dostepnych ruchow
	if ( winCheck() != -1 ) return;

	//dostepne podanie z tego stanu
	if ( podanWTurze < 1 )
		dodajStan( 14 + gracz, przesuniecWTurze, podanWTurze + 1 );

	//dostepne przesuniecia z tego stanu
	if ( przesuniecWTurze < 2 )
		for ( int i = 7 * gracz; i < 7 + 7 * gracz; i++ )
			dodajStan( i, przesuniecWTurze + 1, podanWTurze );

	//zatiwerdzenie tury (nie wykonanie zadnego ruchu w tym obiegu,
	//przelaczenie na przeciwnika
	if ( przesuniecWTurze + podanWTurze > 0 )
	{
		AIstan* nowy = new AIstan( this, 0, 0);
		nowy->nastepnyGracz();
		sons.push_back( nowy );
	}
}

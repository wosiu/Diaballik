#include "plansza.h"

Plansza::Plansza()
{
	for ( int i = 0; i < 7; i++ )
	{
		dane[i] = i;
		dane[i + 7] = i + 42;
	}
	//pilka 0
	dane[ 15 ] = 3;
	//pilka 1
	dane[ 16 ] = 45;
}

int8_t* Plansza::zapisz()
{
	return dane;
}

int Plansza::dajPozycje( int pionekId )
{
	Q_ASSERT( 0 <= pionekId && pionekId <= 15 );
	return dane[ pionekId ];
}

std::vector<int> Plansza::dajWszystkiePuste()
{
	bool color[49];
	for ( int i = 0; i < 49; i++ ) color[i] = true;
	for ( int i = 0; i < 14; i++ ) color[ dane[i] ] = false;

	std::vector<int>res;
	for ( int i = 0; i < 49; i++ )
		if ( color[i] )
			res.push_back( i );

	Q_ASSERT( res.size() == 49 - 14 );
	return res;
}

//gracz = -1
//TODO
/*
std::vector<int> Plansza::dajDruzyne( int pilkaId )
{
	std::vector<int>res( dane, dane + 14 );
	sort( res.begin(), res.end() );

	Q_ASSERT( res.size() == 14 );
	return res;
}*/

bool Plansza::czyPuste(int x, int y)
{
	if( x < 0 || x > 6 || y < 0 || y > 6 ) return false;
	int poz = x + y * 7;

	for ( int i = 0; i < 14; i++ )
		if( dane[i] == poz )
			return false;

	return true;
}

std::vector<int> Plansza::dajSasiedniePuste( int pilkarzId )
{
	Q_ASSERT( 0 <= pilkarzId && pilkarzId <= 15 );

	std::vector<int>res;

	int poz = dane[ pilkarzId ];
	int x = poz % 7;
	int y = poz / 7;

	if ( czyPuste( x - 1, y ) )	res.push_back( poz - 1 );
	if ( czyPuste( x, y - 1 ) )	res.push_back( poz - 7 );
	if ( czyPuste( x + 1, y ) )	res.push_back( poz + 1 );
	if ( czyPuste( x, y + 7 ) )	res.push_back( poz + 7 );

	return res;
}

std::vector<int> Plansza::dajPodania( int pionekId )
{
	//return NULL;
}




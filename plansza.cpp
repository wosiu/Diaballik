#include "plansza.h"

//wersory kierunkowe
const QPair <int,int> DIR[8] = {
qMakePair(0, 1), //GORA
qMakePair(1, 0), //PRAWO
qMakePair(0, -1), //DOL
qMakePair(-1, 0), //LEWO
qMakePair(1, 1), //GORA-PRAWO SKOS
qMakePair(1, -1), //DOL-PRAWO SKOS
qMakePair(-1, -1), //DOL-LEWO SKOS
qMakePair(-1, 1), //GORA-LEWO SKOS
};


Plansza::Plansza()
{
	for ( int i = 0; i < 7; i++ )
	{
		dane[i] = i;
		dane[i + 7] = i + 42;
	}

	dane[ 14 ] = 3; 	//pilka 0 na pozycje 3
	dane[ 15 ] = 45;	//pilka 1 na pozycje 45

	dane[16] = 0; //zaczyna gracz 0
}


//Plansza(tab), Plansza(vector) TO DO

bool Plansza::czyPilka( int pionekId )
{
	return ( pionekId == 14 || pionekId == 15 );
}

bool Plansza::czyPilkarzyk( int pionekId )
{
	if( pionekId > 15 || pionekId < 0 ) return false;
	return !czyPilka( pionekId );
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
	std::vector<int>res;
	bool color[49];
	for ( int i = 0; i < 49; i++ ) color[i] = true;
	for ( int i = 0; i < 14; i++ ) color[ dane[i] ] = false;

	for ( int i = 0; i < 49; i++ )
		if ( color[i] )
			res.push_back( i );

	Q_ASSERT( res.size() == 49 - 14 );
	return res;
}


std::vector<int> Plansza::dajDruzyne( int poleId )
{
	std::vector<int>res( dane + ktoryGracz( poleId ) * 7,
						 dane + 7 + ktoryGracz( poleId ) * 7 );
	sort( res.begin(), res.end() );

	Q_ASSERT( res.size() == 14 );
	return res;
}


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
	Q_ASSERT( 0 <= pilkarzId && pilkarzId < 14 );

	std::vector<int>res;

	int poz = dane[ pilkarzId ];
	int x = poz % 7;
	int y = poz / 7;
	int nx, ny;

	//sprawdzam cztery podstawowe kierunki
	for ( int i = 0; i < 4; i++ )
	{
		nx = x + DIR[i].first;
		ny = y + DIR[i].second;

		if ( czyPuste( nx, ny ) ) res.push_back( nx + ny * 7 );
	}
	return res;
}


int Plansza::ktoryGracz( int poleId )
{
	return (int) !( poleId < 7 || poleId == 14 );
}


std::vector<int> Plansza::dajPodania( int pilkaId )
{
	Q_ASSERT( pilkaId == 14 || pilkaId == 15 );

	//wypelniam plansze kwadratową
	int8_t pola[7][7];
	for ( int i = 0; i < 7; i++ )
		for ( int j = 0; j < 7; j++ )
			pola[i][j] = -1;

	for ( int i = 0; i < 14; i++ )
		pola[ dane[i] % 7 ][ dane[i] / 7  ] = i;

	int xp = dane[ pilkaId ] % 7;
	int yp = dane[ pilkaId ] / 7;

	std::vector<int>res;

	//sprawdzam wszystkie 8 kierunkow
	for( int i = 0; i < 8; i++ )
	{
		int dx = DIR[ i ].first;
		int dy = DIR[ i ].second;

		for ( int d = 1;
			  0 <= (xp + dx * d) && (xp + dx * d) < 7  &&
			  0 <= (yp + dy * d) && (yp + dy * d) < 7;
			  d++ )
		{
			int nx = xp + dx * d;
			int ny = yp + dy * d;

			//jesli nie jest puste
			if ( pola[ nx ][ ny ] != -1 )
			{
				//sprawdzam czy to pionek obecnego gracza
				if ( ktoryGracz( pola[ nx ][ ny ] ) == ktoryGracz( pilkaId ) )
					res.push_back( dane[ pola[ nx ][ ny ] ] );
				//w przeciwnym razie przerywam, bo nie moge podawac nad przeciwnikiem
				else
					break;
			}
		}
	}

	return res;
}


std::vector<int> Plansza::dajRuchy( int pionekId )
{
	if( czyPilka( pionekId ) )
		return dajPodania( pionekId );

	return dajSasiedniePuste( pionekId );
}

int Plansza::czyjRuch()
{
	return dane[16];
}


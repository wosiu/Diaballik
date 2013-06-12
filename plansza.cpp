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
	if ( x < 0 || x > 6 || y < 0 || y > 6 ) return false;
	int poz = x + y * 7;

	for ( int i = 0; i < 16; i++ )
		if( dane[i] == poz )
			return false;

	return true;
}

bool Plansza::czyPuste(int pozycja)
{
	return czyPuste( pozycja % 7, pozycja / 7 );
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

		//oddalam sie o 1 kwadrat w danym kierunku
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
	//jesli to pilka
	if( czyPilka( pionekId ) )
		return dajPodania( pionekId );

	//jesli to pionek i jest pod pilka, to nie moge go ruszac
	if( dane[ pionekId ] == dane[ 14 + ktoryGracz( pionekId ) ] )
		return std::vector<int>();

	//jesli pionek
	return dajSasiedniePuste( pionekId );
}

int Plansza::czyjRuch()
{
	return dane[16];
}

void Plansza::nastepnyGracz()
{
	dane[16] = ( dane[16] + 1 ) % 2;
}

void Plansza::przesun( int pionekId, int pozycja )
{
	//zakladamy poprawnosc wywolania!
	dane[ pionekId ] = pozycja;
}

int Plansza::winCheck()
{
	//do tej stuacji nei powinno dojsc
	Q_ASSERT( !doubleWinCheck() );

	//sprawdzamy unfair game
	bool unfrgm0 = unfairGameCheck( 0 );
	bool unfrgm1 = unfairGameCheck( 1 );

	//jesli oboje unfair game
	//if ( remis() )
	if ( unfrgm0 == true && unfrgm1 == true )
		return -1;

	if ( unfrgm0 )
		return 1;

	if ( unfrgm1 )
		return 0;

	//jesli pilka gracza znalazla sie po drugiej stronie
	if ( dane[ 14 ] >= 42 )
		return 0;

	if ( dane[ 15 ] < 7 )
		return 1;

	//nikt nie wygral
	return -1;
}

bool Plansza::doubleWinCheck()
{
	return  dane[ 14 ] >= 42  &&  dane[ 15 ] < 7;
}

bool Plansza::remis()
{
	return ( unfairGameCheck( 0 ) == true && unfairGameCheck( 1 ) == true );
}

//true -> unfair
bool Plansza::unfairGameCheck( int gracz )
{
	//sprawdzamy czy w ogole jest na planszy sciana nie do przejscia stworzona
	//przez danego gracza:
	std::vector <int> pozycje (7, -1);

	//sprawdzam x:
	for ( int i = 0; i < 7; i++ )
	{
		int j = i + 7 * gracz;

		//jakis pionek stoi juz na tym x => przynajmniej 2 stoją na tym samym x
		//=> jest luka pomiedzy pionkami jednego gracza
		if ( pozycje[ dane[ j ] % 7 ] != -1 )
			return false;

		pozycje[ dane[ j ] % 7 ] =  dane[ j ];
	}

	//sprawdzam y:
	for ( int i = 0; i < 6; i++ )
		//jezeli rozia sie wiecej niz jednym wierszem
		if ( std::abs( pozycje[i] / 7 - pozycje[i+1] / 7 ) > 1 )
			return false;

	//skoro dotrwalismy tutaj, tzn ze jest sciana nie do przejscia
	//sprawdzamy czy min 3 pionki przeciwnika stoją przy niej
	int przeciwnicy = 0; //licznik stykajacych sie z linia nie do przejscia przeciwnikow

	for ( int i = 0; i < 7; i++ )
	{
		//sprawdzam pole nad, jesli jest zajete to na pewno przez przeciwnika
		//poniewaz wszyscy moi zawodnicy znajduja sie w "scianie" (maja rozne x)
		if ( pozycje[ i ] / 7 > 0 )
			przeciwnicy += (int) (!czyPuste( pozycje[ i ] - 7 ));

		//pod analogicznie
		if ( pozycje[ i ] / 7 < 6 )
			przeciwnicy += (int) (!czyPuste( pozycje[ i ] + 7 ));

		if ( przeciwnicy >= 3 )
			return true;
	}

	//jesli przeszlo powyzsze, to jest fair
	return false;
}


unsigned long long Plansza::hashCode()
{
	unsigned long long prime[17] = {1000000007, 1000000014000000049, 14069101319555514199, 2173964608407773537, 7232439895113528231, 1423083718267137937, 9015250055358150391, 3633867129218868929, 3321970296312322375, 16054152209726863089, 4648997779380988567, 6980550577642475553, 13284617912389565159, 1913936733860700753, 17872352148200380983, 1978898776512519553, 7048929689792206983};
	unsigned long long res = 0;

	for ( int i = 0; i < 17; i++ )
		res += dane[ i ] * prime[ i ];

	return res;
}

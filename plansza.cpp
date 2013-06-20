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
	for ( int i = 0; i < rozmiarDruzyny; i++ )
	{
		dane[i] = i;
		dane[i + rozmiarDruzyny] = i + 42;
	}

	dane[ 14 ] = 3; 	//pilka 0 na pozycje 3
	dane[ 15 ] = 45;	//pilka 1 na pozycje 45

	dane[16] = 0; //zaczyna gracz 0
}


bool Plansza::czyPilka( int pionekId )
{
	return ( pionekId == 14 || pionekId == 15 );
}

bool Plansza::czyPilkarzyk( int pionekId )
{
	if( pionekId > 15 || pionekId < 0 ) return false;
	return !czyPilka( pionekId );
}


int Plansza::dajPozycje( int pionekId )
{
	Q_ASSERT( 0 <= pionekId && pionekId <= 15 );
	return dane[ pionekId ];
}


std::vector<int> Plansza::dajWszystkiePuste()
{
	std::vector<int>res;
	bool color[iloscPol];
	for ( int i = 0; i < iloscPol; i++ ) color[i] = true;
	for ( int i = 0; i < 14; i++ ) color[ dane[i] ] = false;

	for ( int i = 0; i < iloscPol; i++ )
		if ( color[i] )
			res.push_back( i );

	Q_ASSERT( res.size() == iloscPol - 14 );
	return res;
}


std::vector<int> Plansza::dajDruzyne( int poleId )
{
	std::vector<int>res( dane + ktoryGracz( poleId ) * rozmiarDruzyny,
						 dane + rozmiarDruzyny + ktoryGracz( poleId ) * rozmiarDruzyny );
	sort( res.begin(), res.end() );

	Q_ASSERT( (int)res.size() == rozmiarDruzyny );
	return res;
}


bool Plansza::czyPuste(int x, int y)
{
	if ( x < 0 || x > 6 || y < 0 || y > 6 ) return false;
	int poz = x + y * rozmiarDruzyny;

	for ( int i = 0; i < 16; i++ )
		if( dane[i] == poz )
			return false;

	return true;
}

bool Plansza::czyPuste(int pozycja)
{
	if ( pozycja < 0 || pozycja > iloscPol - 1 ) return false;

	return czyPuste( pozycja % rozmiarDruzyny, pozycja / rozmiarDruzyny );
}

int Plansza::pozycjaToId( int pozycja )
{
	for ( int i = 0; i < 16; i++ )
		if ( dane[i] == pozycja)
			return i;

	return -1;
}

//zwraca true, jesli na danej pozycji moge ustawic dany pionek
//nie sprawdza pod katem prawidlowosci przesuniecia
bool Plansza::czyDostepne( int pionekId, int pozycja )
{
	int idDocelowe = pozycjaToId ( pozycja );

	if ( czyPilka( pionekId ) )
	{
		//podana pozycja jest pusta, wiec nie mozemy postawic tam pilki
		if ( idDocelowe == -1 ) return false;
		//na podanej pozycji nie stoi pilkarzyk, a pilki na pilke nie moge postawic
		if ( ! czyPilkarzyk( idDocelowe ) ) return false;
		//na podanej pozycji stoi pilkarzyk, ale druzyny przeciwnej
		if ( ktoryGracz( pionekId ) != ktoryGracz( idDocelowe ) ) return false;
	}
	//pilkarzyka nie mozemy postawic na miejsce zajete ( czyPuste( pozycja ) != true )
	else if ( idDocelowe != -1 ) return false;

	return true;
}


std::vector<int> Plansza::dajSasiedniePuste( int pilkarzId )
{
	Q_ASSERT( 0 <= pilkarzId && pilkarzId < 14 );

	std::vector<int>res;

	int poz = dane[ pilkarzId ];
	int x = poz % rozmiarDruzyny;
	int y = poz / rozmiarDruzyny;
	int nx, ny;

	//sprawdzam cztery podstawowe kierunki
	for ( int i = 0; i < 4; i++ )
	{
		nx = x + DIR[i].first;
		ny = y + DIR[i].second;

		if ( czyPuste( nx, ny ) ) res.push_back( nx + ny * rozmiarDruzyny );
	}
	return res;
}


int Plansza::ktoryGracz( int poleId )
{
	return (int) !( poleId < rozmiarDruzyny || poleId == 14 );
}


std::vector<int> Plansza::dajPodania( int pilkaId )
{
	Q_ASSERT( pilkaId == 14 || pilkaId == 15 );

	//wypelniam plansze kwadratową
	int8_t pola[rozmiarDruzyny][rozmiarDruzyny];
	for ( int i = 0; i < rozmiarDruzyny; i++ )
		for ( int j = 0; j < rozmiarDruzyny; j++ )
			pola[i][j] = -1;

	for ( int i = 0; i < 14; i++ )
		pola[ dane[i] % rozmiarDruzyny ][ dane[i] / rozmiarDruzyny  ] = i;

	int xp = dane[ pilkaId ] % rozmiarDruzyny;
	int yp = dane[ pilkaId ] / rozmiarDruzyny;

	std::vector<int>res;

	//sprawdzam wszystkie 8 kierunkow
	for( int i = 0; i < 8; i++ )
	{
		int dx = DIR[ i ].first;
		int dy = DIR[ i ].second;

		//oddalam sie o 1 kwadrat w danym kierunku
		for ( int d = 1;
			  0 <= (xp + dx * d) && (xp + dx * d) < rozmiarDruzyny  &&
			  0 <= (yp + dy * d) && (yp + dy * d) < rozmiarDruzyny;
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
	if ( czyPilka( pionekId ) )
		return dajPodania( pionekId );

	//jesli to pionek i jest pod pilka, to nie moge go ruszac
	if ( dane[ pionekId ] == dane[ 14 + ktoryGracz( pionekId ) ] )
		return std::vector<int>();

	Q_ASSERT ( czyPilkarzyk( pionekId ) );
	return dajSasiedniePuste( pionekId );
}

int Plansza::dajIdPodajacego(int pilkaId)
{
	Q_ASSERT ( czyPilka( pilkaId ) );

	int gracz = ktoryGracz( pilkaId );

	for ( int i = rozmiarDruzyny * gracz ;
		  i < rozmiarDruzyny + rozmiarDruzyny * gracz; i++ )
		if( dane[ i ] == dane[ pilkaId ] )
			return i;

	Q_ASSERT ( false );
	return -1;
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

	if ( dane[ 15 ] < rozmiarDruzyny )
		return 1;

	//nikt nie wygral
	return -1;
}

bool Plansza::doubleWinCheck()
{
	return  dane[ 14 ] >= 42  &&  dane[ 15 ] < rozmiarDruzyny;
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
	std::vector <int> pozycje (rozmiarDruzyny, -1);

	//sprawdzam x:
	for ( int i = 0; i < rozmiarDruzyny; i++ )
	{
		int j = i + rozmiarDruzyny * gracz;

		//jakis pionek stoi juz na tym x => przynajmniej 2 stoją na tym samym x
		//=> jest luka pomiedzy pionkami jednego gracza
		if ( pozycje[ dane[ j ] % rozmiarDruzyny ] != -1 )
			return false;

		pozycje[ dane[ j ] % rozmiarDruzyny ] =  dane[ j ];
	}

	//sprawdzam y:
	for ( int i = 0; i < 6; i++ )
		//jezeli rozia sie wiecej niz jednym wierszem
		if ( std::abs( pozycje[i] / rozmiarDruzyny -
					   pozycje[i+1] / rozmiarDruzyny ) > 1 )
			return false;

	//skoro dotrwalismy tutaj, tzn ze jest sciana nie do przejscia
	//sprawdzamy czy min 3 pionki przeciwnika stoją przy niej
	int przeciwnicy = 0; //licznik stykajacych sie z linia nie do przejscia przeciwnikow

	for ( int i = 0; i < rozmiarDruzyny; i++ )
	{
		//sprawdzam pole nad, jesli jest zajete to na pewno przez przeciwnika
		//poniewaz wszyscy moi zawodnicy znajduja sie w "scianie" (maja rozne x)
		if ( pozycje[ i ] / rozmiarDruzyny > 0 )
			przeciwnicy += (int) (!czyPuste( pozycje[ i ] - rozmiarDruzyny ));

		//pod analogicznie
		if ( pozycje[ i ] / rozmiarDruzyny < 6 )
			przeciwnicy += (int) (!czyPuste( pozycje[ i ] + rozmiarDruzyny ));

		if ( przeciwnicy >= 3 )
			return true;
	}

	//jesli przeszlo powyzsze, to jest fair
	return false;
}


unsigned long long Plansza::hashCode()
{
	unsigned long long prime[17] = {1000000007ULL, 1000000014000000049ULL, 14069101319555514199ULL, 2173964608407773537ULL, 7232439895113528231ULL, 1423083718267137937ULL, 9015250055358150391ULL, 3633867129218868929ULL, 3321970296312322375ULL, 16054152209726863089ULL, 4648997779380988567ULL, 6980550577642475553ULL, 13284617912389565159ULL, 1913936733860700753ULL, 17872352148200380983ULL, 1978898776512519553ULL, 7048929689792206983ULL};
	unsigned long long res = 0;

	//bez info o graczu
	for ( int i = 0; i < 16; i++ )
		res += dane[ i ] * prime[ i ];

	return res;
}

void Plansza::usunPionki()
{
	for ( int i = 0; i < 16; i++ )
		dane[ i ] = -1;
}

std::vector<ruch> Plansza::znajdzRoznice( Plansza* plansza )
{
	std::vector<ruch> roznice;

	for ( int i = 0; i < 16; i++ )
		if ( this->dane[i] != plansza->dane[i] )
			roznice.push_back( ruch( i, dane[i], plansza->dane[i] ) );

	Q_ASSERT ( roznice.size() <= 1 );

	return roznice;
}

int Plansza::dajlOdlOdStart( int poleId )
{
	int liniaStartowa = ktoryGracz( poleId ) * 6;
	return std::abs( liniaStartowa - ( dane[ poleId ] / 7 ) );
}

QString Plansza::debug()
{
	QString deb = "Plansza debug:";
	for( int i = 0; i < 17; i++ )
		deb += QString::number( i ) + ":" + QString::number( dane[i] ) + "; ";
	return deb;
}

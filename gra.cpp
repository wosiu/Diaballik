#include "gra.h"

Gra::Gra()
{
	historyIterator = -1;
	inicjuj();
}

Gra::Gra(Tryb *innyTryb )
{
	plansza = innyTryb->plansza;
	planszaPoczatkowa = innyTryb->planszaPoczatkowa;

	podanWTurze = innyTryb->podanWTurze;
	przesuniecWTurze = innyTryb->przesuniecWTurze;
	typGracza[0] = innyTryb->typGracza[0];
	typGracza[1] = innyTryb->typGracza[1];

	history = innyTryb->history;
	inicjuj();
	historyIterator = innyTryb->historyIterator;
}


void Gra::inicjuj()
{
	//emit undoAble( true );
	//emit redoAble( true );
	isMoveLocked = false;
}

bool Gra::isEndGame()
{
	Q_ASSERT( !plansza.doubleWinCheck() );

	if ( plansza.remis() )
	{
		emit winDetector( 2 );
		return true;
	}

	int winner = plansza.winCheck();

	if ( winner != -1 )
	{
		emit winDetector( winner );
		return true;
	}

	return false;
}


void Gra::turaStart()
{
	//jesli na planszy nie wykryto nietypowych stanow (wygrana, unfair game)
	if ( !isEndGame() )
	{
		zliczRuchyWTurze();
		emit wykonaneRuchy( przesuniecWTurze, podanWTurze );
		//przelacznie na nastepnego gracza w UI
		emit nowaTura( plansza.czyjRuch() );
	}
	else
	{
		//aby nie mozna bylo wykonywa ruchow po zatwierdzeniu wygranej:
		przesuniecWTurze = 2;
		podanWTurze = 1;
	}
}


Plansza Gra::dajPlanszePoczatkowa()
{
	return planszaPoczatkowa;
}


Tryb::TYPGRACZA Gra::dajTypGracza( int graczId )
{
	return typGracza[ graczId ];
}


//sprawdzanie pod kątem nie przekroczenia limitow ruchow
bool Gra::isValidMove( int pionekId, int pos )
{
	//czy o ruch pyta wlasciwy gracz
	if ( plansza.czyjRuch() != plansza.ktoryGracz( pionekId ) ) return false;

	//sprawdzam czy ma dostepne jeszcze ruchy
	Q_ASSERT ( podanWTurze + przesuniecWTurze <= 3);

	//oprocz limitow ruchow ponizsze tez sprawdza czy na planszy poprzednia
	//runda wygrana (patrz turaStart)
	if ( podanWTurze + przesuniecWTurze == 3 ) return false;

	//sprawdzam czy nie nie przekracza ilosci podan/przesuniec
	if ( podanWTurze == 1 && plansza.czyPilka( pionekId) ) return false;
	if ( przesuniecWTurze == 2 && plansza.czyPilkarzyk( pionekId) ) return false;

	//sprawdzam czy podany ruch jest dostepny (w tym miejscu powinien zawsze byc)
	//chyba ze wczytujemy historie z pliku, w ktorym ktoś mogl grzebac recznie
	std::vector<int>dobreRuchy = plansza.dajRuchy( pionekId );

	if ( std::find( dobreRuchy.begin(), dobreRuchy.end(), pos )
			  == dobreRuchy.end() ) return false;

	return true;
}


std::vector<int> Gra::validateAllMoves( int pionekId )
{
	//pyta plansze o dostepne ruchy danego pionka
	std::vector<int> res;
	std::vector<int> mozliweRuchy = plansza.dajRuchy( pionekId );

	//sprawdzam dostepne ruchy mechaniczne pod kątem zasad gry
	for ( int i = 0; i < (int)mozliweRuchy.size(); i++ )
		if	( isValidMove( pionekId, mozliweRuchy[i] ))
			res.push_back( mozliweRuchy[i] );

	return res;
}


//uzywane na zewnatrz (UI) do inforamcji gracza (czlowieka) jakie ma mozliwosci ruchu
std::vector<int> Gra::findValidMoves( int pionekId )
{
	//sprawdzam czy zapytano o pionek obecnego gracza
	int czyjRuch = plansza.czyjRuch();

	if ( czyjRuch != plansza.ktoryGracz( pionekId ) )
		return std::vector<int>();

	//sprawdzam czy to nie jest zapytanie o pionki komputera
	//(dostepnych ruchow dla takowych nie chcemy zaznaczac na planszy
	//po kliknieciu na nie)
	if ( typGracza[ czyjRuch ] == KOMPUTER )
		return std::vector<int>();

	return validateAllMoves( pionekId );
}


//zatwierdzenie ruchu (uzywane na zewnatrz i przez komputer)
void Gra::zatwierdz()
{
	//czy jakies ruchy wykonane
	if ( podanWTurze == 0 && przesuniecWTurze == 0 )
	{
		emit uwaga( "Aby zatwierdzić turę, musi być wykonany przynajmniej jeden ruch!" );
		return;
	}

	plansza.nastepnyGracz();
	turaStart();
}


void Gra::move( int pionekId, int pozycja )
{
	Q_ASSERT( isValidMove( pionekId, pozycja  ) );

	//dorzucamy ruch do historii
	addToHistory( ruch( pionekId, plansza.dajPozycje(pionekId), pozycja ) );

	//zwiekszymy licznik przesuniec / podan rundy
	if ( plansza.czyPilka( pionekId ) )	podanWTurze++;
	else								przesuniecWTurze++;

	//wykonuje faktyczny ruch na planszy i emituje sygnal do UI
	physicalMove( pionekId, pozycja );
	emit wykonaneRuchy( przesuniecWTurze, podanWTurze );
}


// obsluguje ruchy czlowieka
void Gra::moveDetector( int pionekId, int pozycja )
{
	Q_ASSERT( isValidMove( pionekId, pozycja  ) );
	//nie przyjdzie zawolanie z UI pionkow kompa, bo nie wyswietla sie
	//validMoves dla niego, wiec uzytkownik nie bedzie mial dostepu do takich ruchow
	Q_ASSERT( typGracza[ plansza.ktoryGracz( pionekId ) ] == CZLOWIEK );

	move( pionekId, pozycja );
}



/* OBSLUGA HISTORII */

void Gra::addToHistory( ruch r )
{
	//porzucam historie od tego momentu do konca, bo wykonano jakis ruch
	history.erase( history.begin() + historyIterator + 1, history.end() );

	historyIterator++;
	history.push_back( r );
}


//ustawia gracza na wskazywany przez historyIterator pionek
bool Gra::undo()
{
	if ( historyIterator < 0 )
	{
		emit uwaga("Brak ruchów do cofnięcia.");
		return false;
	}

	ruch r = history[ historyIterator ];
	Q_ASSERT( plansza.dajPozycje( r.pionekId ) == r.dokad );

	physicalMove ( r.pionekId , r.skad );
	historyIterator--;
	poprawGraczaWzgledemHistorii();

	return true;
}


bool Gra::redo()
{
	if ( historyIterator + 1 >= (int)history.size() )
	{
		emit uwaga("Brak ruchów do powtorzenia.");
		return false;
	}

	historyIterator++;

	ruch r = history[ historyIterator ];
	Q_ASSERT( plansza.dajPozycje( r.pionekId ) == r.skad );

	physicalMove( r.pionekId , r.dokad );
	poprawGraczaWzgledemHistorii();

	return true;
}


void Gra::zliczRuchyWTurze()
{
	podanWTurze = przesuniecWTurze = 0;

	//jesli sa zakolejkowane jakies ruchy:
	if( historyIterator != -1 )
	{
		//pobieram gracza, do ktorego nalezy ostatni
		//(obecnie wskazywany, wykonany odzwierciedlony na planszy) ruch w historii
		int gracz = plansza.ktoryGracz( history[ historyIterator ].pionekId );

		//tura nalezy do niego, wiec sprawdzamy jego ruchy
		if( gracz == plansza.czyjRuch() )
		{
			//zliczam ruchy tego gracza w rundzie do momentu historyIterator]
			for( int i = historyIterator; i >= 0
				 && plansza.ktoryGracz( history[ i ].pionekId ) == gracz
				 ; i-- )
			{
				int pionekId = history[ i ].pionekId;

				if ( plansza.czyPilka( pionekId) ) podanWTurze++;
				else if ( plansza.czyPilkarzyk( pionekId) ) przesuniecWTurze++;
			}
		}
	}

	emit wykonaneRuchy( przesuniecWTurze, podanWTurze );
}


void Gra::poprawGraczaWzgledemHistorii()
{
	//żadne ruchy nie są zakolejkowane, wiec nie wykonano ruchow
	if ( historyIterator == -1 )
		plansza = planszaPoczatkowa;
	//w p.p.
	else
	{
		//gracz cofanego / powtarzanego ruchu
		int graczRuchu = plansza.ktoryGracz( history[ historyIterator ].pionekId );

		//gracz rozwazanego ruchu ( cofanego / powtarzanego ) jest taki sam,
		//jak aktaulny na planszy, wiec nic nie robimy
		if ( graczRuchu == plansza.czyjRuch() ) {}
		//w p.p. symulujemy zmiane tury
		else
		{
			plansza.nastepnyGracz();
			emit graczUpdate( graczRuchu );
		}
	}

	zliczRuchyWTurze();
}

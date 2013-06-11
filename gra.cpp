#include "gra.h"

Gra::Gra()
{
	//TO DO w zaleznosci od konfigu gry:
	typGracza[1]=CZLOWIEK;
	typGracza[0]=KOMPUTER;

	historyIterator = -1;

	/*Plansza a = *dajPlansze();
	a.nastepnyGracz();
	qDebug() << a.czyjRuch();
	qDebug() << dajPlansze()->czyjRuch();*/
	//pausa = lse;

	//ai = new AI();

	//ten connect nie dzioła
	//connect(this, SIGNAL( nowaTura(int) ), this, SLOT( komputerGraj(int) ) );
	//emit nowaTura( plansza->czyjRuch() );
}

Gra::~Gra()
{
	/*while( history.empty() )
	{
		delete history.back();
	}*/
}

void Gra::turaStart()
{
	//jesli na planszy wykryto wygraną
	if ( plansza->winCheck() != -1 )
	{
		Q_ASSERT( plansza->winCheck() == plansza->czyjRuch() );
		emit winDetector( plansza->czyjRuch() );
	}
	else //w p.p. kontynuujemy rozgrywke
	{
		podanWTurze = przesuniecWTurze = 0;
		//przelacznie na nastepnego gracza
		plansza->nastepnyGracz();
		emit nowaTura( plansza->czyjRuch() );
		komputerGraj( plansza->czyjRuch() );
	}

	//stare:
	//emit nowaTura( plansza->czyjRuch() );
	//komputerGraj( plansza->czyjRuch() );
}

Tryb::TYPGRACZA Gra::dajTypGracza( int graczId )
{
	return typGracza[ graczId ];
}


//sprawdzanie pod kątem nie przekroczenia limitow ruchow
bool Gra::isValidMove( int pionekId, int pos )
{
	//sprawdzam czy dobry gracz
	if ( plansza->czyjRuch() != plansza->ktoryGracz( pionekId ) ) return false;

	//sprawdzam czy ma dostepne jeszcze ruchy
	if ( ruchy.size() > 3 ) return false;

	//sprawdzam czy nie nie przekracza ilosci podan/przesuniec
	if ( podanWTurze == 1 && plansza->czyPilka( pionekId) ) return false;
	if ( przesuniecWTurze == 2 && plansza->czyPilkarzyk( pionekId) ) return false;

	//sprawdzam czy podany ruch jest dostepny (w tym miejscu powinien zawsze byc)
	//chyba ze wczytujemy historie z pliku, w ktorym ktoś mogl grzebac recznie
	std::vector<int>dobreRuchy = plansza->dajRuchy( pionekId );
	if ( std::find( dobreRuchy.begin(), dobreRuchy.end(), pos )
			  != dobreRuchy.end() ) return false;


	return true;
}

std::vector<int> Gra::validateAllMoves( int pionekId )
{
	//pyta plansze o dostepne ruchy danego pionka
	std::vector<int> res;
	std::vector<int> mozliweRuchy = plansza->dajRuchy( pionekId );

	//sprawdzam dostepne ruchy pod kątem pozostalej ilosci ruchow
	for ( int i = 0; i < mozliweRuchy.size(); i++ )
		if	( isValidMove( pionekId, mozliweRuchy[i] ))
			res.push_back( mozliweRuchy[i] );

	return res;
}

//uzywane na zewnatrz (UI) do inforamcji gracza (czlowieka) jakie ma mozliwosci ruchu
std::vector<int> Gra::findValidMoves( int pionekId )
{
	qDebug() << "findValidMoves::Gra ( pionekId = "<< pionekId << " )";

	//sprawdzam czy zapytano o pionek obecnego gracza
	int czyjRuch = plansza->czyjRuch();

	if ( czyjRuch != plansza->ktoryGracz( pionekId ) )
		return std::vector<int>();

	//sprawdzam czy to nie jest zapytanie o pionki komputera
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
	//while( pausa ) {}

	turaStart();
}

//TO DO! do trybu z tym (i z inicjacją w konstruktorze):
/*void Gra::setPausa( bool isPausa )
{
	pausa = isPausa;
}*/

void Gra::move( int pionekId, int pozycja )
{
	Q_ASSERT( isValidMove( pionekId, pozycja  ) );

	//jesli nikt nie wygral lub ta tura nalezy do gracza, ktory ktoryms ruchem
	//w tej turze wygral (to jak ma jeszcze jakies wolne ruchy, to moze zagrac,
	//dopoki nie zatwierdzi
	Q_ASSERT( plansza->winCheck() == -1 || plansza->winCheck() == plansza->czyjRuch() );

	//dorzucamy ruch do ruchow wykonanych w tej turze przez obecnego gracza
	addToHistory( ruch( pionekId, plansza->dajPozycje(pionekId), pozycja ) );

	//zostal wykonany ruch, wiec czyscimy historie plansza od stanu obecnego do konca
	eraseHistoryTail();

	//wykonuje faktyczny ruch na planszy i emituje sygnal do UI
	physicalMove( pionekId, pozycja );
}

//obsluguje ruchy czlowieka
void Gra::moveDetector( int pionekId, int pozycja )
{
	Q_ASSERT( isValidMove( pionekId, pozycja  ) );
	//nie przyjdzie zawolanie z UI pionkow kompa, bo nie wyswietla sie
	//validMoves dla niego, wiec uzytkownik nie bedzie mial dostepu do takich ruchow
	Q_ASSERT( typGracza[ plansza->ktoryGracz( pionekId ) ] == CZLOWIEK );

	move( pionekId, pozycja );
}


void Gra::komputerGraj( int gracz )
{
	//sprawdzam czy zaczeta tura nalezy do komputera
	if( typGracza[ gracz ] != KOMPUTER ) return;

	qDebug() << "komputerGraj start jako gracz: " << gracz;

	//prymitywne AI:
	int licznikRuchow = 0;
	while ( licznikRuchow < 2 )
	{
		int pionek = gracz * 7 + qrand() % 7;
		std::vector <int> dostepneRuchy = validateAllMoves( pionek );

		if ( dostepneRuchy.empty() ) continue;

		std::random_shuffle( dostepneRuchy.begin(), dostepneRuchy.end() );

		move( pionek, dostepneRuchy[0] );
		licznikRuchow++;
	}
	//move( gracz * 7 + 2, 37);
	//move( gracz * 7 + 2, 44);

	//TO DO: move( AI.dajPodpowiedz( obecnyGracz )

	//po wszystkim: zatwierdz(). nie emit nowaTura! (zeby sie zapisaly i wyczyscily ruchy, itd..)
	//ten zatwierdz uzaleznic od if (konfig.wzbudzanie kliknieciem )
	zatwierdz();
}


/* OBSLUGA HISTORII */
void Gra::addToHistory( ruch r )
{
	//porzucam historie od tego momentu do konca, bo wykonano jakis ruch
	history.erase( history.begin() + historyIterator + 1, history.end() );

	historyIterator++;
	history.push_back( r );

	if ( plansza->czyPilka( r.pionekId ) )	podanWTurze++;
	else									przesuniecWTurze++;
}

bool Gra::undo()
{
	if ( historyIterator < 0 )
	{https://www.youtube.com/watch?feature=player_embedded&v=kLO1djacsfg#!
		emit uwaga("Brak ruchów do cofnięcia.");
		return false;
	}

	ruch r = history[ historyIterator ];
	Q_ASSERT( plansza->dajPozycje( r.pionekId ) == r.dokad );


	/*if ( plansza->czyPilka( r.pionekId ) )	podanWTurze--;
	else									przesuniecWTurze--;
	Q_ASSERT( podanWTurze >= 0 && przesuniecWTurze >= 0 );*/


	Q_ASSERT ( isValidMove( r.pionekId , r.skad ) );
	physicalMove( r.pionekId , r.skad );
	historyIterator--;

	return true;
}

bool Gra::redo()
{
	if ( historyIterator + 1 >= history.size() )
	{
		emit uwaga("Brak ruchów do powtorzenia.");
		return false;
	}

	historyIterator++;
	ruch r = history[ historyIterator ];

	Q_ASSERT( plansza->dajPozycje( r.pionekId ) == r.skad );
	physicalMove( r.pionekId , r.dokad );

	return true;
}



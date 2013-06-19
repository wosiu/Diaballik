#include "gra.h"

#define debon 1
#define deb(burak) if(debon) {qDebug()<<"DEB-> "<<#burak<<": "<<burak;}
#define debv(burak) if(debon) {qDebug()<<"DEB-> "<<#burak<<": \t"; for(unsigned int zyx=0;zyx<burak.size();zyx++) cout<<burak[zyx]<<" "; cout<<endl;}



Gra::Gra()
{
	historyIterator = -1;
	inicjuj();
}

Gra::Gra( Tryb *innyTryb )
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

/*Gra::Gra(const Gra innaGra )
{
	plansza = innaGra.plansza;
	plansza.nastepnyGracz(); //poniewaz turaStart() zmieni z powrotem na dobrego
	podanWTurze = innaGra.podanWTurze;
	przesuniecWTurze = innaGra.przesuniecWTurze;
	typGracza[1] = innaGra.typGracza[1];
	typGracza[0] = innaGra.typGracza[0];
	inicjuj();
}*/


Gra::~Gra()
{
}

void Gra::inicjuj()
{
	emit undoAble( true );
	emit redoAble( true );
	isMoveLocked = false;
}

bool Gra::isEndGame()
{
	Q_ASSERT( !plansza.doubleWinCheck() );

	if ( plansza.remis() )
	{
		//emit remisDetector();
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
	//static int roundCounter = 0;
	qDebug() << " =========================================== ";
	//qDebug() << "Gra::turaStart(): tura = " << roundCounter++;

	//jesli na planszy nie wykryto nietypowych stanow (wygrana, unfair game)
	if ( !isEndGame() )
	{
		//przesuniecWTurze = podanWTurze = 0; //ew przeniesc do zatwierdz, zeby sie nie wykonywalo przy odpaleniu gry po raz pierwszy
		zliczRuchyWTurze();
		emit wykonaneRuchy( przesuniecWTurze, podanWTurze );
		//przelacznie na nastepnego gracza
		emit nowaTura( plansza.czyjRuch() );
		//komputerGraj( plansza.czyjRuch() );
	}
	else
	{
		//aby nie mozna bylo wykonywa ruchow po zatwierdzeniu wygranej:
		przesuniecWTurze = 2;
		podanWTurze = 1;
	}

	//jesli bedzie trza blokowac przyciski to zrobic int historyWalker(-1..1) i tam emitowac prawdzajac historyIterator
	emit undoAble( true );
	emit redoAble( true );

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
	//qDebug() << "isValidMove::Gra ( pionekId = "<< pionekId << ", pozycja "<< pos <<"  )";

	//TO DO: musi byc do wczytywania rozgrywki
	//(nie prawda ->) nie powinno byc do AI ( bo se bedzie liczyc w tle )
	//bo bedzie operowala na swoich planszach
	//ALE do zwyklej gry niepotrzebne, bo czy dobry gracz, sprawdzam w FindValidMoves
	//ale tez nie przeszkadza
	if ( plansza.czyjRuch() != plansza.ktoryGracz( pionekId ) ) return false;

	//sprawdzam czy ma dostepne jeszcze ruchy
	Q_ASSERT ( podanWTurze + przesuniecWTurze <= 3);

	//oprocz limitow ruchow powyzsze tez sprawdza czy na planszy poprzednia
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

	//sprawdzam czy ktos wygral w tej turze lub ta tura nalezy do gracza, ktory ktoryms ruchem
	//w tej turze wygra jak zatwierdzi (to jak ma jeszcze jakies wolne ruchy, to moze kontynuowac
	//int winner = plansza.winCheck();
	//if ( winner != -1 && winner != plansza.czyjRuch() ) return false;
	//tu nie moze byc sprawdzania zwyciestwa bo to syf.. zwyciestwo / przegrana / remis
	//ma byc stwierdzana po zatwierdzeniu tury a nie w jej trakcie !!!


	return true;
}

std::vector<int> Gra::validateAllMoves( int pionekId )
{
	//pyta plansze o dostepne ruchy danego pionka
	std::vector<int> res;
	std::vector<int> mozliweRuchy = plansza.dajRuchy( pionekId );

	//sprawdzam dostepne ruchy pod kątem pozostalej ilosci ruchow
	for ( int i = 0; i < (int)mozliweRuchy.size(); i++ )
		if	( isValidMove( pionekId, mozliweRuchy[i] ))
			res.push_back( mozliweRuchy[i] );

	return res;
}

//uzywane na zewnatrz (UI) do inforamcji gracza (czlowieka) jakie ma mozliwosci ruchu
std::vector<int> Gra::findValidMoves( int pionekId )
{
	//qDebug() << "findValidMoves::Gra ( pionekId = "<< pionekId << " )";

	//sprawdzam czy zapytano o pionek obecnego gracza
	int czyjRuch = plansza.czyjRuch();

	if ( czyjRuch != plansza.ktoryGracz( pionekId ) )
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

	//w tura start:

	//przesuniecWTurze = podanWTurze = 0; //- to moze byc ale nie powinno nic zmieniac bo w zliczRuchywturze sie powinno wyzerowac samo
	plansza.nastepnyGracz();
	turaStart();
}

//TO DO! do trybu z tym (i z inicjacją w konstruktorze):
/*void Gra::setPausa( bool isPausa )
{
	pausa = isPausa;
}*/

void Gra::move( int pionekId, int pozycja )
{
	//qDebug() << "move::Gra ( "<< pionekId << ", "<< pozycja <<"  )";

	Q_ASSERT( isValidMove( pionekId, pozycja  ) );

	//poszlo do isValid:
	//jesli nikt nie wygral lub ta tura nalezy do gracza, ktory ktoryms ruchem
	//w tej turze wygral (to jak ma jeszcze jakies wolne ruchy, to moze zagrac,
	//dopoki nie zatwierdzi
	//Q_ASSERT( plansza.winCheck() == -1 || plansza.winCheck() == plansza.czyjRuch() );

	//dorzucamy ruch do historii
	addToHistory( ruch( pionekId, plansza.dajPozycje(pionekId), pozycja ) );

	//zwiekszymy licznik przesuniec / podan rundy
	if ( plansza.czyPilka( pionekId ) )	podanWTurze++;
	else								przesuniecWTurze++;

	//wykonuje faktyczny ruch na planszy i emituje sygnal do UI
	physicalMove( pionekId, pozycja );
	emit wykonaneRuchy( przesuniecWTurze, podanWTurze );
}


/* OBSLUGA HISTORII */
void Gra::addToHistory( ruch r )
{
	//qDebug() << "addToHistory::Gra ( "<< "pionekId = " << r.pionekId  << ", skad = " << r.skad << ", dokad = " << r.dokad <<" )";

	//porzucam historie od tego momentu do konca, bo wykonano jakis ruch
	history.erase( history.begin() + historyIterator + 1, history.end() );

	historyIterator++;
	history.push_back( r );
}

//uwaga: undo / redo uzyte przez CZLOWIEK cofa zawsze jeden jego ruch
//nawet jesli bedzie to wymagalo cofniecia calej tury KOMPUTERa

//ustawia gracza na wskazywany przez historyIterator pionek
bool Gra::undo()
{
	if ( typGracza[0] == KOMPUTER && typGracza[1] == KOMPUTER )
	{
		emit uwaga("Gra Komputer vs Komputer - cofanie ruchów niedostępne.");
		return false;
	}

	if ( historyIterator < 0 )
	{
		emit uwaga("Brak ruchów do cofnięcia.");
		return false;
	}


	/*if ( typGracza[ plansza.czyjRuch() ] == KOMPUTER && historyIterator < 3 )
	{
		//ale jesli cofamy ruchy komputera, a przed nim nie ma nic
		//to nie mozemy ich cofnac (takie zachowanie nei ma sensu)S
			bool isHumanOnStart = false;
			//sprawdzam wiec czy ktorys z ruchow 0,1,(2) nalezy do czlowieka
			//bo jak tak, to moge cofnąc
			for ( int i = 0; i < historyIterator; i++ )
				if ( typGracza[ plansza.ktoryGracz( history[ i ].pionekId ) ]
					 == CZLOWIEK )
				{
					isHumanOnStart = true;
					break;
				}

			//nie ma ruchow czlowika na poczatku historii,
			//wiec nie ma sensu cofac ruchow komputera:
			if ( !isHumanOnStart )
			{
				emit( "Do tego miejsca rozgrywki zostały wykonane ruchy jedynie przez Komputer. Nie masz dostępu do cofnięcia ich. Kontynuuj grę lub powtórz ruchy jeśli dostępne.");
				return false;
			}
	}*/


	ruch r = history[ historyIterator ];
	Q_ASSERT( plansza.dajPozycje( r.pionekId ) == r.dokad );

	physicalMove ( r.pionekId , r.skad );
	historyIterator--;
	poprawGraczaWzgledemHistorii();

	//jesli w wyniku cofniecia obecnie ma wykonywac ruch komputer,
	//to jego wszystkie ruchy w tej turze zostają także cofniete
	//a takze ostatni ruch w turze przed komputerem CZLOWIEK'a
	if ( typGracza[ plansza.czyjRuch() ] == KOMPUTER )
	{
		//ale jesli nie ma co cofać w turze przed komputem,
		//to po jego cofnietych ruchach nakazujemy mu ponownie grac
		//minus w stosunku do porpzedniego rozwiazania: stracimy historie rozgrywki
		//ale ma to sens (odzwierciedla rzeczywistosc)
		if ( historyIterator <  0 )
			emit nowaTura( plansza.czyjRuch() );
		//	komputerGraj(); //jesli komputer gra, to gdy w UI jest idznaczone auto play to ten poczatkowy ruch po cofnieciu komp i tak wykona sam
		else //powtarzamy tak jak opisane wyzej
			undo();
	}

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

	//jesli w wyniku powtorzenia obecnie ma wykonywac ruch komputer,
	//to jego wszystkie ruchy w tej turze zostają także powtorzone
	//a takze pierwszu ruch w turze po komputerze
	if ( typGracza[ plansza.czyjRuch() ] == KOMPUTER )
	{
		//ale jesli nie ma co powtarzac w turze po komputerze,
		//to po prostu zaczynamy nowa runde
		if ( historyIterator + 1 >= (int)history.size() )
			zatwierdz();
		else //powtarzamy tak jak opisane wyzej
			redo();
	}

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


/* OBSLUGRA RUCHOW GRACZY */

// obsluguje ruchy czlowieka
void Gra::moveDetector( int pionekId, int pozycja )
{
	Q_ASSERT( isValidMove( pionekId, pozycja  ) );
	//nie przyjdzie zawolanie z UI pionkow kompa, bo nie wyswietla sie
	//validMoves dla niego, wiec uzytkownik nie bedzie mial dostepu do takich ruchow
	Q_ASSERT( typGracza[ plansza.ktoryGracz( pionekId ) ] == CZLOWIEK );

	move( pionekId, pozycja );
}

//obsluguje ruchy komputera
void Gra::komputerGraj()
{
	int gracz = plansza.czyjRuch();

	//sprawdzam czy zaczeta tura nalezy do komputera
	if ( typGracza[ gracz ] != KOMPUTER ) return;

	//qDebug() <<"Gra::grajKomputer() IN";

	/*if ( przesuniecWTurze == 2 )
	{
		zatwierdz();
		return;
	}*/

	if ( isMoveLocked )
	{
		emit uwaga( "Ruch komputera w trakcie obliczeń." );
		return;
	}
	isMoveLocked = true;

	//qDebug() << "komputerGraj start jako gracz: " << gracz;

	AI aitest;
	AIstan* stan = new AIstan( &plansza, przesuniecWTurze, podanWTurze );
	//AIstan stan( plansza, 0, 0 );
	ruch r = aitest.dajHinta( stan );
	delete stan;

	qDebug() << "proponowany ruch dla planszy:" << r.pionekId << r.skad << r.dokad;

	if ( r.czyRuch() )
	{
		Tryb::move( r );
		isMoveLocked = false;
	}
	else
	{
		isMoveLocked = false;
		zatwierdz();
	}
	//move( gracz * 7 + 2, 37);
	//move( gracz * 7 + 2, 44);

	//TO DO: move( AI.dajPodpowiedz( obecnyGracz )

	//po wszystkim: zatwierdz(). nie emit nowaTura! (zeby sie zapisaly i wyczyscily ruchy, itd..)
	//ten zatwierdz uzaleznic od if (konfig.wzbudzanie kliknieciem )

	//zatwierdz();

}

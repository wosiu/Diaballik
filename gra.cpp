#include "gra.h"

Gra::Gra()
{
	//TO DO w zaleznosci od konfigu gry:
	typGracza[0]=CZLOWIEK;
	typGracza[1]=CZLOWIEK;

	//stale:
	//TODO:
	//connect(this, SIGNAL( nowaTura(int) ), this, SLOT( komputerGraj(int) ) );
	//emit nowaTura( plansza->czyjRuch() );
}

Gra::~Gra()
{
}

int Gra::dajTypGracza( int graczId )
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
	int podanWTurze = 0;
	int przesuniecWTurze = 0;

	for ( int i = 0; i < ruchy.size() ; i++ )
		if ( plansza->czyPilka( ruchy[i].pionekId ) )
			podanWTurze++;
		else //przesuniecie pilkarzyka
			przesuniecWTurze++;

	if ( podanWTurze == 1 && plansza->czyPilka( pionekId) ) return false;
	if ( przesuniecWTurze == 2 && plansza->czyPilkarzyk( pionekId) ) return false;

	//sprawdzam czy podany ruch jest dostepny (w tym miejscu powinien zawsze byc)
	/*std::vector<int>dobreRuchy = plansza->dajRuchy( pionekId );
	Q_ASSERT( std::find( dobreRuchy.begin(), dobreRuchy.end(), pos )
			  != dobreRuchy.end() );
	*/
	return true;
}

//uzywane na zewnatrz (UI) do inforamcji gracza (czlowieka) jakie ma mozliwosci ruchu
std::vector<int> Gra::findValidMoves( int pionekId )
{
	qDebug() << "findValidMoves::Gra ( pionekId = "<< pionekId << " )";

	//odklikuję - odznaczam dostepne obszary po ponownym klknieciu tego samego pionka
	/*static int ostatniPionekId;

	if ( ostatniPionekId == pionekId )
	{
		ostatniPionekId = -1;
		return std::vector<int>();
	}
	ostatniPionekId = pionekId;*/ //przeniesione do grafiki (MainWindow::setValidMoves)


	//sprawdzam czy zapytano o pionek obecnego gracza
	int czyjRuch = plansza->czyjRuch();

	if ( czyjRuch != plansza->ktoryGracz( pionekId ) )
		return std::vector<int>();

	//sprawdzam czy to nie jest zapytanie o pionki komputera
	if ( typGracza[ czyjRuch ] == KOMPUTER )
		return std::vector<int>();

	//pyta plansze o dostepne ruchy danego pionka
	std::vector<int> res;
	std::vector<int> mozliweRuchy = plansza->dajRuchy( pionekId );

	//sprawdzam dostepne ruchy pod kątem pozostaluch ilosci ruchow
	for ( int i = 0; i < mozliweRuchy.size(); i++ )
		if	( isValidMove( pionekId, mozliweRuchy[i] ))
			res.push_back( mozliweRuchy[i] );

	return res;
}

//zatwierdzenie ruchu (uzywane na zewnatrz i przez komputer)
void Gra::zatwierdz()
{
	//czy jakies ruchy wykonane
	if( ruchy.empty() )
	{
		emit uwaga( "Aby zatwierdzic swoją turę, wykonaj przynajmniej jeden ruch!" );
		return;
	}

	//TO DO: monit do powyzszego, ze nie wykonano ruchow, lub blokowac przycisk "Zatwierdz"

	//historia.push_back( plansza->kopiuj() );
	ruchy.clear();

	plansza->nastepnyGracz();

	emit nowaTura( plansza->czyjRuch() );
}


void Gra::move( int pionekId, int pozycja )
{
	Q_ASSERT( isValidMove( pionekId, pozycja  ) );

	ruchy.push_back(
				ruch( pionekId, plansza->dajPozycje(pionekId), pozycja ) );

	//if( !ruchy.empty() ) emit odblikujzatwierdz

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
	qDebug("komputerGraj start!");

	//sprawdzam czy zaczeta tura nalezy do komputera
	if( typGracza[ gracz ] != KOMPUTER ) return;

	//TO DO: move( AI.dajPodpowiedz( obecnyGracz )
	qDebug("ruch kompa");

	//po wszystkim: zatwierdz(). nie emir nowaTura! (zeby sie zapisaly i wyczyscily ruchy, itd..)
}

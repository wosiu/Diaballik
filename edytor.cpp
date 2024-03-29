#include "edytor.h"

Edytor::Edytor()
{
	inicjuj();
}


Edytor::Edytor( Tryb *innyTryb )
{
	planszaPoczatkowa = plansza = innyTryb->plansza;

	podanWTurze = innyTryb->podanWTurze;
	przesuniecWTurze = innyTryb->przesuniecWTurze;
	typGracza[1] = innyTryb->typGracza[1];
	typGracza[0] = innyTryb->typGracza[0];

	inicjuj();
}


void Edytor::inicjuj()
{
}


std::vector<int> Edytor::findValidMoves( int pionekId )
{
	std::vector<int>res = plansza.dajWszystkiePuste();;
	std::vector<int>tmp;

	if ( plansza.czyPilka( pionekId ) )
	{
		//jesli pilka to dokladam jeszcze pilkarzy z druzyny
		tmp = plansza.dajDruzyne( pionekId );
		res.insert( res.end(), tmp.begin(), tmp.end() );
	}

	return res;
}


Tryb::TYPGRACZA Edytor::dajTypGracza( int graczId )
{
	return EDYTOR;
}


//nie korzystamy z tego w edytorze
bool Edytor::isValidMove(int pionekId, int pos)
{
	return true;
}


//zmienia gracza rozpoczynajacego na przeciwnego
void Edytor::turaStart()
{
	emit wykonaneRuchy( 0, 0 );
	emit nowaTura( plansza.czyjRuch() );

	emit undoAble( false );
	emit redoAble( false );
}


Plansza Edytor::dajPlanszePoczatkowa()
{
	return plansza;
}


void Edytor::zatwierdz()
{
	if ( plansza.doubleWinCheck() )
	{
		emit uwaga( "Niepoprawny stan planszy: obaj gracze wygrywają. Popraw i zatwierdź." );
		return;
	}

	planszaPoczatkowa = plansza;
	Tryb *nowyTryb = new Gra( this );
	emit zmianaTrybu( nowyTryb );
	qDebug("zakoncz edycje");
}


void Edytor::move( int pionekId, int pozycja )
{
	//jesli to pilka oraz miejsce docelowe jest puste
	if ( plansza.czyPilka( pionekId ) && plansza.czyPuste( pozycja ) )
		//to chcemy takze przesunac pionek pod nią
		physicalMove( plansza.dajIdPodajacego( pionekId ), pozycja );

	physicalMove( pionekId, pozycja );
}


void Edytor::moveDetector( int pionekId, int pozycja )
{
	move( pionekId, pozycja );
}


bool Edytor::undo()
{
	emit uwaga( "Cofanie ruchów niedostępne w trybie edycji." );
	return false;
}


bool Edytor::redo()
{
	emit uwaga( "Powtarzanie ruchów niedostępne w trybie edycji." );
	return false;
}


void Edytor::komputerGraj()
{
}

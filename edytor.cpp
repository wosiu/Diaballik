#include "edytor.h"

Edytor::Edytor()
{
}

Edytor::Edytor(Plansza plansza)
{
	this->plansza = &plansza;
}

/*Edytor::Edytor(Tryb *innyTryb)
{
	plansza = innyTryb->plansza;
}*/

std::vector<int> Edytor::findValidMoves( int pionekId )
{
	std::vector<int>res = plansza->dajWszystkiePuste();;
	std::vector<int>tmp;

	if ( plansza->czyPilka( pionekId ) )
	{
		//jesli pilka to dokladam jeszcze pilkarzy z druzyny
		tmp = plansza->dajDruzyne( pionekId );
		res.insert( res.end(), tmp.begin(), tmp.end() );
	}

	return res;
}

Tryb::TYPGRACZA Edytor::dajTypGracza(int graczId)
{
	return EDYTOR;
}

bool Edytor::isValidMove(int pionekId, int pos)
{
	Q_ASSERT(false);
}

//zmienia gracza rozpoczynajacego na przeciwnego
void Edytor::turaStart()
{
	plansza->nastepnyGracz();
	emit wykonaneRuchy(0,0);
	emit nowaTura( plansza->czyjRuch() );
}


void Edytor::zatwierdz()
{
	if ( plansza->doubleWinCheck() )
		emit uwaga( "Niepoprawny stan planszy: obaj gracze wygrywają. Popraw i zatwierdź." );

	qDebug("zakoncz edycje");
}


void Edytor::move( int pionekId, int pozycja )
{
	//jesli to pilka oraz miejsce docelowe jest puste
	if ( plansza->czyPilka( pionekId ) && plansza->czyPuste( pozycja ) )
		//to chcemy takze przesunac pionek pod nią
		physicalMove( plansza->dajIdPodajacego( pionekId ), pozycja );

	physicalMove( pionekId, pozycja );
}

void Edytor::moveDetector( int pionekId, int pozycja )
{
	move( pionekId, pozycja );
}

bool Edytor::undo()
{
	emit uwaga( "Cofanie ruchów niedostępne w trybie edycji." );
}

bool Edytor::redo()
{
	emit uwaga( "Powtarzanie ruchów niedostępne w trybie edycji." );
}



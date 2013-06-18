#include "ai.h"

AI::AI(QObject *parent) :
	QThread(parent)
{

}

void AI::run()
{
	//wystartuj obliczanie wezlow
}

//QVector <Tryb::ruch> AI::dajHinta( AIstan *plansza )
ruch AI::dajHinta( AIstan *poczatkowy )
{
	//unsigned long long hash = plansza->hashCode();

	//a moze by tak uzyc:  QList<AIstan*> values = map.values(hash);
	//i jesli jest wiecej niz jeden to zasadzic equals?
	//itHTS = hashToStan.find( hash );

	int h = 5;
	Plansza wynik = alfabeta( poczatkowy, -INF, +INF, h, true );

	std::vector<ruch>roznice = poczatkowy.znajdzRoznice( &wynik );

	Q_ASSERT ( roznice.size() < 3 );

	return roznice.front();


	//jesli hash wystapil = uklad wystapuje w drzewie,
	//od niego startujemy alfa bete
	//if ( itHTS != hashToStan.end() )
	//{
	//	alfabeta( itHTS.value(), itHTS.value().alfa, itHTS.value().beta, h );
	//}
}


AIstan alfabeta( AIstan parent, int alfa, int beta, int h, bool max )
{
	if ( h == 0 ) return ocen( &parent, graczId );

	//int r;
	AIstan r;
	vector <AIstan> sons = parent.generatorStanow();

	//jesli jestesmy w ruchu gracza ktory szuka hinta
	if ( max )
	{
		r.v = -INF; // pas

		foreach ( s, sons )
		{
			//najpierw jesli mam do wykonania jeszcze jakies ruchy, to
			//posylam max w dol
			if ( s.czyjRuch == parent.czyjRuch() ) max=!max; //robie na false, bo w wyloaniu zrobi sie true

			AIstan x = alfabeta( s, max( r.v, alfa ), beta, h-1, !max );
			if ( x.v >= beta ) return x;
			if ( x.v >= r.v) r = x;
		}
	}
	else
	{
		r.v = +INF;

		if ( s.czyjRuch == parent.czyjRuch() ) max=!max; //robie na true, bo w wyloaniu zrobi sie false

		foreach( s, sons )
		{
			AIstan x = alfabeta( s, alfa, min( r.v, beta ), h-1, !max );
			if ( x.v <= a ) return x;
			if ( x.v <= r.v) r = x;
		}
	}

	return r;
}


int AI::ocen( AIstan *stan, int graczId )
{
	int isWin = stan.winCheck();

	//stan, w ktorym nikt obecnie nie wygrywa nikt nie wygrywa
	if ( isWin == -1 )
	{
		int res=0;
		int startY = gracz*6;

		//odleglosc od startu
		for ( int i = 7 * gracz; i < 7 + 7 * gracz; i++ )
		{
			int odl = stan.dajlOdlegloscOdLiniStartowej( i );

			if ( odl == 6 ) odl += 10; //premia za stanie pionka na mecie
			res += 10*odl;
		}

		//premia: mozliwosc zagrania pilka do jakiegos gracza
		res += stan.dajRuchy( 14 + gracz ).size();
		//TO DO: a moze zrobic wyjebana premie za mozliwosc podania do gracza na mecie?

		//odleglosc przeciwnika od startu na minus
		int przeciwnik = ( grac + 1 ) % 2;

		return res - ocen( stan, przeciwnik );
	}

	//stan w ktorym wygrywa gracz, ktory pytal o hinta
	if ( isWin == graczId ) resturn INF;

	//stan w ktorym wygrywa przeciwny gracz
	return -INF;
}

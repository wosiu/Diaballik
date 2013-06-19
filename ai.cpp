#include "ai.h"

AI::AI(QObject *parent) :QThread(parent)
//AI::AI()
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

	//TO DO: uzależnić h od entropii planszy:
	//maksymalna glebokosc drzewa gry:
	int h = 6; //8 OK, 6-7 optymalnie

	wywolanyGracz = poczatkowy->czyjRuch();

	alfabeta( poczatkowy, poczatkowy->alfa, poczatkowy->beta, h, true );

	AIstan* najlepszy;
	int max = -INF;

	foreach ( AIstan* stan, poczatkowy->sons )
	{
		//qDebug() << stan->debug();
		if ( stan->v >= max )
		{
			max = stan->v;
			najlepszy = stan;
		}
	}


	/*if ( roznice.size() != 1 )
	{
		qDebug() << poczatkowy->debug();
		qDebug() << najlepszy->debug();
		for ( int i = 0; i < roznice.size(); i++ )
			qDebug() << roznice[i].pionekId << roznice[i].skad << roznice[i].dokad;
	}*/

	//jesli nie mial dostapnych ruchow, to np odnotowano wygraną
	if ( poczatkowy->sons.size() == 0 )
		return ruch(-1,0,0); //zatwierdz

	//szukamy roznice pomiedzy plansza wejsciowa a ta podpowiedziana przez AI
	std::vector<ruch>roznice = poczatkowy->znajdzRoznice( najlepszy );
	Q_ASSERT ( roznice.size() < 3 ); //miedzy nimi moze byc max 3 roznice


	//AI podpowiada zatwierdzenie tury
	if ( najlepszy->czyjRuch() != poczatkowy->czyjRuch() )
	{
		//skoro zatwierdzil, to nie wykonal zadnych ruchow
		Q_ASSERT( roznice.size() == 0 );

		return ruch(-1,0,0); //zatwierdz
	}


	//nie moze byc brak roznic (pas bylby rozwazony przez AI na
	//dwoch nastepujacych po sobie glebokosciach
	//tak samo jak 2 rozne ruchy
	Q_ASSERT ( roznice.size() == 1 );

	return roznice.front();

	//jesli hash wystapil = uklad wystapuje w drzewie,
	//od niego startujemy alfa bete
	//if ( itHTS != hashToStan.end() )
	//{
	//	alfabeta( itHTS.value(), itHTS.value().alfa, itHTS.value().beta, h );
	//}
}


int AI::alfabeta( AIstan* parent, int alfa, int beta, int h, bool max )
{
	//qDebug() << "AI::alfabeta( AIstan* parent, int alfa, int beta, int h = "<< h << ", bool max = " << max << ")";

	//jesli zuzylismy dostepna glebokosc obliczen
	if ( h == 0 )
		return parent->v = ocen( parent, wywolanyGracz );

	parent->generatorSynow();

	//jesli wskazany ojciec nie ma synow (brak mozliwosci ruszenia np z powodu
	//czyjejs wygranej
	if ( parent->sons.empty() )
		return parent->v = ocen( parent, wywolanyGracz );


	//jesli jestesmy w ruchu gracza ktory szuka hinta
	if ( max )
	{
		parent->v = -INF; // pas
		//to do: uzywac bety zamiast powyzszego?

		foreach ( AIstan* s, parent->sons )
		{
			//najpierw jesli mam do wykonania jeszcze jakies ruchy, to
			//posylam max w dol
			if ( s->czyjRuch() == parent->czyjRuch() ) max = !max; //robie na false, bo w wyloaniu zrobi sie true

			int x = alfabeta( s, qMax( parent->v, alfa ), beta, h-1, !max );

			if ( x >= beta ) { parent->v = x; return x; }
			if ( x >= parent->v ) parent->v = x;
		}
	}
	else
	{
		parent->v = +INF;

		foreach ( AIstan* s, parent->sons )
		{
			if ( s->czyjRuch() == parent->czyjRuch() ) max = !max; //robie na false, bo w wyloaniu zrobi sie true

			int x = alfabeta( s, alfa, qMin( parent->v, beta ), h-1, !max );
			if ( x <= alfa ) { parent->v = x; return x; };
			if ( x <= parent->v ) parent->v = x;
		}
	}

	return parent->v;
}


int AI::ocenaHeurystyczna( AIstan *stan, int graczId )
{
	int res = 0;

	//odleglosc od startu
	for ( int i = 7 * graczId; i < 7 + 7 * graczId; i++ )
	{
		int odl = stan->dajlOdlOdStart( i );

		odl *= odl;
		//if ( odl == 6 ) odl += 15; //premia za stanie pionka na mecie
		res += odl * 10;
	}

	//premia: mozliwosc zagrania pilka do gracza, ktory jest na lini przeciwnika
	/*std::vector <int> pilkaRuchy = stan->dajRuchy( 14 + graczId );
	for ( int i = 0; i < pilkaRuchy.size(); i++ )
		//jesli pilka ma podanie do pionka ktory stoi na lini przeciwnika
		if ( pilkaRuchy[i] / 7 == ((graczId + 1) % 2) * 6 )
			//premia
			res += 1000;*/
	//powyzsze: fajne, ale gra trwa dluzej (choc faktycznie komp jest madrzejszy)

	return res;
}


int AI::ocen( AIstan *stan, int graczId )
{
	//qDebug() << "AI::ocen( AIstan *stan " << stan->czyjRuch() << ", int graczId = "<< graczId << ")";

	int isWin = stan->winCheck();

	//stan, w ktorym nikt obecnie nie wygrywa nikt nie wygrywa
	if ( isWin == -1 )
	{
		int przeciwnik = ( graczId + 1 ) % 2;
		return ocenaHeurystyczna( stan, graczId ) - ocenaHeurystyczna( stan, przeciwnik );
	}

	//stan w ktorym wygrywa gracz, ktory pytal o hinta
	if ( isWin == graczId ) return INF;

	//stan w ktorym wygrywa przeciwny gracz
	return -INF;

}



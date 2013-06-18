#ifndef AI_H
#define AI_H

#include <QThread>
#include <QMap>

#include "tryb.h"
#include "aistan.h"
#include "ruch.h"

class AI : public QThread
{
	Q_OBJECT
public:
	explicit AI(QObject *parent = 0);
	void run();
	ruch dajHinta( AIstan *poczatkowy );
	//QVector < AIstan > generatorStanow( Plansza* plansza );

private:
	//QMap < unsigned long long, AIstan* > hashToStan;
	//QMap < unsigned long long, AIstan* >::iterator itHTS;
	//void garbage();
	const int INF = 1073741824;
	int ocen( AIstan* stan , int graczId );
	AIstan alfabeta( AIstan stan, int alfa, int beta, int h, bool max );
	//AIstan *root;
	//Aistan *present;
	int wywolanyGracz;

//pytac kosz o hash, jak w koszu, to usuwam z kosza, szukam w hashTosant i usuwam
//AIstan'y laczyc z removeStan tutaj, podczas usuwania jakiegos stanu emituje, ze jest usuwany
//i usuwamy go z hashTostan

signals:

public slots:
	//removeStan(Aistan* stan);


};

#endif // AI_H

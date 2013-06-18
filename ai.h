#ifndef AI_H
#define AI_H

#include <QThread>
#include <QMap>

#include "tryb.h"
#include "aistan.h

class AI : public QThread
{
	Q_OBJECT
public:
	explicit AI(QObject *parent = 0);
	void run();
	Tryb::ruch dajHinta( Plansza* plansza );
	//QVector < AIstan > generatorStanow( Plansza* plansza );

private:
	//QMap < unsigned long long, AIstan* > hashToStan;
	//QMap < unsigned long long, AIstan* >::iterator itHTS;
	//void garbage();
	int ocen(AIstan* stan , int graczId);
	int alfabeta( AIstan *stan, int alfa, int beta );
	//AIstan *root;
	//Aistan *present;

//pytac kosz o hash, jak w koszu, to usuwam z kosza, szukam w hashTosant i usuwam
//AIstan'y laczyc z removeStan tutaj, podczas usuwania jakiegos stanu emituje, ze jest usuwany
//i usuwamy go z hashTostan

signals:

public slots:
	//removeStan(Aistan* stan);


};

#endif // AI_H

#ifndef AI_H
#define AI_H

#include <QThread>
#include <QMap>

#include "aistan.h"
#include "ruch.h"

class AI : public QThread
{
	Q_OBJECT
public:
	explicit AI( Plansza* plansza, int przesuniecWTurze, int podanWTurze,
				 QObject* parent = 0 );
	~AI();
	void run();
	ruch dajHinta();
	ruch hint;

private:
	const int INF = 1073741824;
	int ocen( AIstan* stan , int graczId );
	int ocenaHeurystyczna(AIstan *stan, int graczId);
	int alfabeta( AIstan* stan, int alfa, int beta, int h, bool max );
	int wywolanyGracz;
	AIstan* poczatkowy;

signals:

public slots:
	//removeStan(Aistan* stan);

};

#endif // AI_H

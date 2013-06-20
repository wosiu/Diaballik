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
	bool done;
	bool running;

private:
	const int INF = 1 << 30;
	int ocen( AIstan* stan , int graczId );
	int ocenaHeurystyczna(AIstan *stan, int graczId);
	int alfabeta( AIstan* stan, int alfa, int beta, int h, bool max );
	int wywolanyGracz;
	AIstan* poczatkowy;

signals:

public slots:
	void stop();

};

#endif // AI_H

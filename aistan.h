#ifndef AISTAN_H
#define AISTAN_H
#include <QVector>
#include "plansza.h"

class AIstan : public Plansza
{
public:
	AIstan();
	AIstan( Plansza* plansza, int przesuniecWTurze, int podanWTurze );
	~AIstan();
	//int minimalValue, maximalValue;
	int v;
	int przesuniecWTurze, podanWTurze;
	void generatorSynow();
	QVector <AIstan*> sons;
	int alfa,beta; //inicjowac -inf, inf

private:
	const int INF = 1073741824;
};

#endif // AISTAN_H

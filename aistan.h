#ifndef AISTAN_H
#define AISTAN_H
#include <QVector>
#include "plansza.h"

class AIstan : public Plansza
{
public:
	AIstan();
	AIstan( Plansza* plansza, przesuniecWTurze, podanWTurze );
	~AIstan();
	//int minimalValue, maximalValue;
	int value;
	int przesuniecWTurze, podanWTurze;
	QVector < AIstan > generatorStanow();

private:
	//QVector <AIstan*> sons;
	//int alfa,beta; //inicjowac -inf, inf
};

#endif // AISTAN_H

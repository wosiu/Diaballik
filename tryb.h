#ifndef TRYB_H
#define TRYB_H

#include <vector>
#include <QObject>

#include "plansza.h"

class Tryb : public QObject
{
	Q_OBJECT

public:
	Tryb();
	~Tryb();
	virtual bool isValidMove( int pionekId, int pos );
	virtual std::vector <int> findValidMoves( int pionekId );

protected:
	Plansza *plansza;

public slots:
	void userMoveDetected( int pionekId, int pos );

signals:
	void moved( int pionekId, int pos );
};

#endif // TRYB_H

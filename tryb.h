#ifndef TRYB_H
#define TRYB_H

#include <cstdint>
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
	void physicalMove( int pionekId, int pozycja );

protected:
	Plansza *plansza;
	struct ruch;

public slots:
	virtual void move( int pionekId, int pos );
	virtual void zatwierdz();
	//bool undo();
	//bool redo();

signals:
	void moved( int pionekId, int pos );
	void nastepnyGracz( int gracz );
};

#endif // TRYB_H

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

	struct ruch {
		int skad,dokad,pionekId;
		ruch(int _pionekId, int _skad, int _dokad) {
			skad = _skad; dokad = _dokad; pionekId = _pionekId;
		}
	};

protected:
	Plansza *plansza;

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

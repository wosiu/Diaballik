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
		int pionekId,skad,dokad;
		ruch(int _pionekId, int _skad, int _dokad) {
			pionekId = _pionekId, skad = _skad; dokad = _dokad;
		}
	};

protected:
	Plansza *plansza;

public slots:
	virtual void zatwierdz();
	virtual void move( int pionekId, int pozycja );
	virtual void moveDetector(int pionekId , int pozycja);
	//bool undo();
	//bool redo();

signals:
	void moved( int pionekId, int pos );
	//jak pozbyc sie ponizszego z tej klasy, zeby bylo tylko w podklasie?
	virtual void nowaTura( int gracz );
};


#endif // TRYB_H

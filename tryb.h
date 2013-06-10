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
	enum TYPGRACZA { CZLOWIEK, KOMPUTER, EDYTOR };
	/*static const int CZLOWIEK;
	static const int KOMPUTER;
	static const int EDYTOR;*/

	virtual bool isValidMove( int pionekId, int pos );
	virtual std::vector <int> findValidMoves( int pionekId );
	void physicalMove( int pionekId, int pozycja );
	virtual TYPGRACZA dajTypGracza( int graczId ) = 0; //czemu to nie moze byc virtualne?
	virtual void start() = 0;

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
	void nowaTura( int gracz );
	void uwaga( QString tresc );
};


#endif // TRYB_H

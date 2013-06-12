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

	virtual bool isValidMove( int pionekId, int pos ) = 0;
	virtual std::vector <int> findValidMoves( int pionekId ) = 0;
	void physicalMove( int pionekId, int pozycja );
	virtual TYPGRACZA dajTypGracza( int graczId ) = 0; //czemu to nie moze byc virtualne?
	virtual void turaStart() = 0;

	struct ruch {
		int pionekId,skad,dokad;
		ruch(int _pionekId, int _skad, int _dokad) {
			pionekId = _pionekId, skad = _skad; dokad = _dokad;
		}
	};

protected:
	Plansza *plansza;

public slots:
	virtual void zatwierdz() = 0;
	virtual void move( int pionekId, int pozycja ) = 0;
	virtual void moveDetector(int pionekId , int pozycja) = 0;
	virtual bool undo() = 0;
	virtual bool redo() = 0;

signals:
	void moved( int pionekId, int pos );
	//jak pozbyc sie ponizszego z tej klasy, zeby bylo tylko w podklasie?
	void nowaTura( int gracz );
	void winDetector( int gracz );
	void uwaga( QString tresc );
	//void remisDetector();

};


#endif // TRYB_H

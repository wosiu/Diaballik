#ifndef TRYB_H
#define TRYB_H

#include <vector>

#include <QObject>

class Tryb : public QObject
{
	Q_OBJECT

public:
	Tryb();
	virtual bool isValidMove( int pionekId, int pos );
	virtual std::vector <int> findValidMoves( int pionekId );

public slots:
	void userMoveDetected( int pionekId, int pos );

signals:
	void moved( int pionekId, int pos );
};

#endif // TRYB_H

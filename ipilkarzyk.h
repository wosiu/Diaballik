#ifndef IPILKARZYK_H
#define IPILKARZYK_H

#include "ipole.h"

class IPilkarzyk : public IPole
{
	Q_OBJECT
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
	IPilkarzyk( QObject *parent, int gracz, int id );
	static const int rozmiar;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
						 QWidget *widget);

	static int przesuniecieWzgledne();



};

#endif // IPILKARZYK_H

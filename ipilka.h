#ifndef IPILKA_H
#define IPILKA_H

#include "ipole.h"

class IPilka : public IPole
{
public:
	IPilka(QObject *parent = 0, int gracz = 0, int id = 0 );
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
						 QWidget *widget);
	const static int rozmiar;

private:
	int przesuniecieWzgledne();
};

#endif // IPILKA_H

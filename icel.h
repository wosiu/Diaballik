#ifndef ICEL_H
#define ICEL_H

#include "ipilkarzyk.h"

class ICel : public IPole
{


public:
	ICel(QObject *parent = 0, int poz = 0);
	ICel(int poz = 0);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget);

//tutaj id z nadklasy pamieta pozycję

};

#endif // ICEL_H

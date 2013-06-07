#include "icel.h"

ICel::ICel(QObject *parent, int poz):
	IPole(parent)
{
	id = poz;
}

ICel::ICel(int poz)
{
	id = poz;
}


void ICel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
					 QWidget *widget)
{

	QPen pen( Qt::green );
	pen.setWidth( 3 );
	painter->setPen( pen );
	//painter->setBrush( QBrush(Qt::green, Qt::DiagCrossPattern) );

	painter->drawEllipse(IPilkarzyk::przesuniecieWzgledne(),
						 IPilkarzyk::przesuniecieWzgledne(),
						 IPilkarzyk::rozmiar,IPilkarzyk::rozmiar);
}


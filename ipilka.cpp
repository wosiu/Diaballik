#include "ipilka.h"

const int IPilka::rozmiar = 30;


IPilka::IPilka(QObject *parent, int gracz, int id) :
	IPole(parent)
{
	this->graczId = gracz;
	this->id = id;
}


int IPilka::przesuniecieWzgledne()
{
	return ( IPole::rozmiar - IPilka::rozmiar ) / 2;
}


void IPilka::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
					 QWidget *widget)
{
	if( graczId == 0 )	painter->setBrush( QBrush(Qt::white, Qt::Dense4Pattern) );
	else				painter->setBrush( QBrush(Qt::black, Qt::Dense4Pattern) );

	painter->drawEllipse(przesuniecieWzgledne(),przesuniecieWzgledne(),
						 this->rozmiar,this->rozmiar);
}


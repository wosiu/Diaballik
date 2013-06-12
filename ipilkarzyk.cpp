#include "ipilkarzyk.h"

const int IPilkarzyk::rozmiar = 40;

IPilkarzyk::IPilkarzyk(QObject *parent, int gracz, int id ) :
	IPole(parent)
{
	this->graczId = gracz; //odpowiedzialne za kolor
	this->id = id; //odpowiedzialne za pozycje
}


int IPilkarzyk::przesuniecieWzgledne()
{
	return (IPole::rozmiar - IPilkarzyk::rozmiar ) / 2;
}

void IPilkarzyk::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
					 QWidget *widget)
{
	if( graczId == 0 )		painter->setBrush( QBrush(Qt::black) );
	else if( graczId == 1 )	painter->setBrush( QBrush(Qt::white) );
	else					painter->setBrush( QBrush(Qt::red) );
	//(QColor(0xFF, 0xFF, 0xFF, highlightLevel), Qt::SolidPattern);

	painter->drawEllipse(przesuniecieWzgledne(),przesuniecieWzgledne(),
						 this->rozmiar,this->rozmiar);
}

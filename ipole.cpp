#include "ipole.h"

const int IPole::rozmiar = 50;

IPole::IPole(QObject *parent) :
	QObject(parent)
{
	locked = false;
}

QRectF IPole::boundingRect() const
{
	return QRectF(0,0,rozmiar,rozmiar);
}

void IPole::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
						 QWidget *widget)
{
	painter->drawRect( boundingRect() );
}

void IPole::moveable(bool lock)
{
	locked = true;
}

void IPole::unlock()
{
	locked = false;
}

bool IPole::getLock()
{
	return locked;
}

/* Przesuwa IPole na planszy o [rozmiar]
 * move(1,0) - w prawo
 * move(-1,0) - w lewo
 * itd.. */
void IPole::move(int x, int y)
{
	//if ( locked ) return;
	locked = true;

	QPropertyAnimation* animation = new QPropertyAnimation(this,"pos");

	connect( animation, SIGNAL(finished()), this, SLOT(unlock()) );
	connect( animation, SIGNAL(finished()), this, SIGNAL(finished()) );

	animation->setDuration(350);
	animation->setEndValue( this->mapToParent( x*IPole::rozmiar, y*IPole::rozmiar ) );
	animation->setEasingCurve(QEasingCurve::InOutSine);
	animation->start( QPropertyAnimation::DeleteWhenStopped );
}

void IPole::mousePressEvent(QGraphicsSceneMouseEvent *event )
{
	if ( locked ) return;
	emit clicked( this->id );
}

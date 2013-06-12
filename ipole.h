#ifndef IPOLE_H
#define IPOLE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation> //needed?
#include <QSequentialAnimationGroup>

class IPole : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)
	Q_INTERFACES( QGraphicsItem )

public:
	const static int rozmiar;
	explicit IPole(QObject *parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
						 QWidget *widget);
	void move(int x, int y);
	void moveable(bool lock);
	bool getLock();

private:
	/*static*/ bool locked;
	//QSequentialAnimationGroup movesQueue;

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	int graczId;
	int id;

private slots:
		void unlock();

signals:
		void clicked(int clickedId);
		//void animationFinished();
};


#endif // IPOLE_H

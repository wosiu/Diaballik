#ifndef KOLEJKAANIMACJI_H
#define KOLEJKAANIMACJI_H

#include <list>
#include <QPropertyAnimation>

class KolejkaAnimacji
{
public:
	KolejkaAnimacji();
	void add( QPropertyAnimation* animacja );

private:
	std::list < QPropertyAnimation* > que;
	void run();

private slots:
	void next();
};

#endif // KOLEJKAANIMACJI_H

#ifndef IPLANSZA_H
#define IPLANSZA_H

#include <QGraphicsScene>
#include "ipilkarzyk.h"
#include "ipilka.h"
#include "icel.h"
#include "plansza.h"
#include <vector>

class IPlansza : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit IPlansza(QObject *parent = 0);
	void dodajDostepnePole( int pos );
	void ustawPionek( bool gracz, int id, int x, int y );
	bool getLock();
	void clear();
	void dodajPionki();
	void dodajPionki( Plansza* plansza );
	void ustawPionki();
	void ustawPionki( Plansza* plansza );


private:
	IPole* pionki[ 16 ];
	int toPixels(int x) {return x * IPole::rozmiar;}
	int fromPixels(int px) {return px / IPole::rozmiar;}
	void rysujPodklad();
	void stworzPionki();
	void polaczPionki();
	void ustawPole( IPole *pole, int pos );

	bool locked;
	int clickedId;
	std::vector< ICel* > dostepneRuchy;
	//int lastClickedId; //do "odznaczania" - zrealizowane w MainWindow::setValidMoves()


signals:
	//sygnal o kliknieciu na pionek
	void clicked( int poleId  );
	//sygnal o kliknieciu na dostepne pole do ruchu = wykonanie ruchu na planszy
	void chose( int clickedId, int pos );

private slots:
	//zbiera sygnaly z pionkow
	void clickDetector( int poleId );
	//zbiera sygnaly z wolnych pol
	void moveDetector( int pos );

public slots:
	void move( int pionekId, int dx, int dy);
	void move( int pionekId, int poz);
	void czyscDostepneRuchy();

};

#endif // IPLANSZA_H

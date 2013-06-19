#include <QApplication>
#include "mainwindow.h"
#include <QTextCodec>


#include "watek.h"
#include "ai.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTextCodec::setCodecForCStrings( QTextCodec::codecForName("UTF-8") );

/*
	Plansza* plansza = new Plansza();
	plansza->przesun(14,3);
	plansza->przesun(0,7);

	AI aitest;
	AIstan* stan = new AIstan( plansza, 1, 1 );
	//AIstan stan( plansza, 0, 0 );
	ruch r = aitest.dajHinta( stan );

	qDebug() << "proponowany ruch dla planszy:" << r.pionekId << r.skad << r.dokad;

	delete stan;

	qDebug() << "git";
	return 0;
*/
	MainWindow w;
	w.show();

	return a.exec();
}

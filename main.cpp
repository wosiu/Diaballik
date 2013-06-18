#include <QApplication>
#include "mainwindow.h"
#include <QTextCodec>


#include "watek.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTextCodec::setCodecForCStrings( QTextCodec::codecForName("UTF-8") );

	//MainWindow w;
	//w.show();

	watek w;
	w.start();

	for( int i = 0; i< 10000; i++ )
	{
		w.count();
	}
	//w.terminate();

	return 0;
	//return a.exec();
}

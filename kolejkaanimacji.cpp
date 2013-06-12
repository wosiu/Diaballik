#include "kolejkaanimacji.h"

KolejkaAnimacji::KolejkaAnimacji()
{
}
/*
void KolejkaAnimacji::add( QPropertyAnimation* animacja )
{
	que.push_back( animacja );
	connect( animacja, SIGNAL(finished()), this, SLOT(next()) );

	if ( !running ) run();
}

void KolejkaAnimacji::next()
{
	que.front()->destroy;
	delete que.front();

	if ( !que.empty() ) run();
}

void KolejkaAnimacji::run()
{
	running = true;
	if ( !que.empty() )
		que.front()->start();

}
*/

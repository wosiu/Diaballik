#include "ruch.h"

ruch::ruch()
{
}


ruch::ruch(int8_t _pionekId, int8_t _skad, int8_t _dokad)
{
	pionekId = _pionekId, skad = _skad; dokad = _dokad;
}

//jelsi ruch nie spelnia ponizszych warunkow to moze byc to np "Zatwierdz"
bool ruch::czyRuch()
{
	return ( pionekId >= 0 && pionekId < 16 && 0 <= skad && skad < 49
			 &&  0 <= dokad && dokad < 49 );
}

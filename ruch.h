#ifndef RUCH_H
#define RUCH_H
#include <cstdint>

class ruch
{
public:
	int8_t pionekId,skad,dokad;

	ruch();
	ruch(int8_t _pionekId, int8_t _skad, int8_t _dokad);
	bool czyRuch();
};

#endif // RUCH_H

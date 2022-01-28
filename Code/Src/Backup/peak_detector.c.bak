/*
 * peak_detector.c
 *
 *  Created on: Jul 6, 2020
 *      Author: Zening
 */


#include "peak_detector.h"

void init_PeakDetector(PeakDetector* pPD, uint16_t capacity)
{
	pPD->begin = pPD->data;
	pPD->end = pPD->data + capacity;
	pPD->head =pPD->begin;
	pPD->tail = pPD->begin;
	pPD->empty = (pPD->end != pPD->begin)?(true):(false);
	pPD->length = 0;
	pPD->capacity = capacity;
}

void append_PeakDetector(PeakDetector* pPD, uint16_t v)
{
	*pPD->tail = v;
	// inc header if tail conflits (except when empty)
	if((!pPD->empty) && (pPD->tail == pPD->head))
	{
		pPD->head++;
		if(pPD->head == pPD->end)
			pPD->head = pPD->begin;
		pPD->length--;
	}
	// inc tail pointer
	pPD->tail++;
	if(pPD->tail == pPD->end)
		pPD->tail = pPD->begin;
	pPD->empty = false;
	pPD->length++;
}


uint16_t max_PeakDetector(const PeakDetector* pPD)
{
	uint16_t i;
	uint16_t* p = pPD->head;
	uint16_t max_v = 0;
	for(i = 0; i < pPD->length; ++i)
	{
		if(*p > max_v) max_v = *p;
		if(++p == pPD->end) p = pPD->begin;
	}
	return max_v;
}

uint16_t min_PeakDetector(const PeakDetector* pPD)
{
	uint16_t i;
	uint16_t* p = pPD->head;
	uint16_t min_v = UINT16_MAX;
	for(i = 0; i < pPD->length; ++i)
	{
		if(*p < min_v) min_v = *p;
		if(++p == pPD->end) p = pPD->begin;
	}
	return min_v;

}
uint16_t avg_top_quantile_PeakDetector(const PeakDetector* pPD, float quantile)
{
	uint16_t sorted[LEN_PEAK_DETECTOR_WINDOW];
	if(pPD->empty) return 0;
	uint16_t nQuantile = pPD->length * quantile + 1;
	if(nQuantile >= pPD->length) nQuantile = pPD->length;
	// copy to sorted
	uint16_t* p = pPD->head;
	for(int i = 0; i < pPD->length; ++i)
	{
		sorted[i] = *(p++);
		if (p == pPD->end)
			p = pPD->begin;
	}
	// buuble sort
	uint32_t sum = 0;
	for(int i = 0; i < nQuantile; ++i)
	{
		for(int j = pPD->length - 1; j > i; --j)
		{
			if(sorted[j] > sorted[j-1])
			{
				uint16_t c = sorted[j-1];
				sorted[j-1] = sorted[j];
				sorted[j] = c;
			}
		}
		sum += sorted[i];
	}
	// average of top quantile
	return sum / nQuantile;
}

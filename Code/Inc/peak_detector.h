/*
 * peak_detector.h
 *
 *  Created on: 2020年6月26日
 *      Author: Zening
 */

#ifndef PEAK_DETECTOR_H_
#define PEAK_DETECTOR_H_

#include "stdint.h"
#include "stdbool.h"

#define LEN_PEAK_DETECTOR_WINDOW 11

typedef struct {
	uint16_t data[LEN_PEAK_DETECTOR_WINDOW];
	uint16_t* begin;
	uint16_t* end;
	uint16_t* head;
	uint16_t* tail;
	bool empty;
	uint16_t length;
	uint16_t capacity;
}PeakDetector;

void init_PeakDetector(PeakDetector* pPD, uint16_t capacity);

void append_PeakDetector(PeakDetector* pPD, uint16_t v);

uint16_t max_PeakDetector(const PeakDetector* pPD);

uint16_t min_PeakDetector(const PeakDetector* pPD);

uint16_t avg_top_quantile_PeakDetector(const PeakDetector* pPD, float quantile);
#endif /* PEAK_DETECTOR_H_ */

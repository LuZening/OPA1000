/*
 * MultiLinearValueMapper.h
 *
 *  Created on: 2020年8月29日
 *      Author: Zening
 */

#ifndef MULTILINEARVALUEMAPPER_H_
#define MULTILINEARVALUEMAPPER_H_

// TODO: 测试MultiLinearValueMapper

#define MLVM_MAX_POINTS 6
#include <stdbool.h>
#include <inttypes.h>
#include "main.h"

typedef struct
{
	float AnchorsX[MLVM_MAX_POINTS];
	float AnchorsY[MLVM_MAX_POINTS];
	float Slopes[MLVM_MAX_POINTS];
	float Intercepts[MLVM_MAX_POINTS];
	uint8_t nAnchors;
	bool isSynthesized;
	GPIO_TypeDef* portGPIO;
	uint16_t pinGPIO;
} MultiLinearValueMapper_t;

void init_multi_linear_value_mapper(MultiLinearValueMapper_t* p, uint8_t nAnchors);

void MLVM_add_point(MultiLinearValueMapper_t* p, float x, float y);

void MLVM_synthesize(MultiLinearValueMapper_t* p);

float MLVM_map(MultiLinearValueMapper_t *p ,float x);

void MLVM_clear(MultiLinearValueMapper_t *p);

#endif /* MULTILINEARVALUEMAPPER_H_ */

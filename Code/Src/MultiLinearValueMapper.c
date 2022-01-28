/*
 * MultiLinearValueMapper.c
 *
 *  Created on: 2020年8月29日
 *      Author: Zening
 */

#include "MultiLinearValueMapper.h"

void init_multi_linear_value_mapper(MultiLinearValueMapper_t* p, uint8_t nAnchors)
{
	p->nAnchors = nAnchors;
	p->isSynthesized = false;
}

void MLVM_add_point(MultiLinearValueMapper_t* p, float x, float y)
{
	uint8_t i, j;
	if (p->nAnchors >= MLVM_MAX_POINTS) return;
	for (i = 0; i < p->nAnchors; ++i)
	{
		if (x < p->AnchorsX[i])
			break;
	}
	for (j = p->nAnchors; j > i; --j)
	{
		p->AnchorsX[j] = p->AnchorsX[j - 1];
		p->AnchorsY[j] = p->AnchorsY[j - 1];
	}
	p->AnchorsX[i] = x;
	p->AnchorsY[i] = y;
	p->isSynthesized = false;
	p->nAnchors++;
}


void MLVM_synthesize(MultiLinearValueMapper_t* p)
{
	uint8_t i;
	float x0 = 0., y0 = 0.;
	for (i = 0; i < p->nAnchors; ++i)
	{
		// y = ki * (x - xi) + yi <=> y = ki * x + (yi - ki xi)
		// ki = (yi - y_i-1) / (x_i - x_i-1)
		float x1 = p->AnchorsX[i], y1 = p->AnchorsY[i];
		if (i > 0)
		{
			x0 = p->AnchorsX[i - 1];
			y0 = p->AnchorsY[i - 1];
		}
		float k = (y1 - y0) / (x1 - x0); // slope
		float b = y1 - k * x1; // intercept
		p->Slopes[i] = k;
		p->Intercepts[i] = b;
	}
	p->isSynthesized = true;
}

float MLVM_map(MultiLinearValueMapper_t* p, float x)
{
	uint8_t begin = 0, end = p->nAnchors;
	uint8_t mid;
	uint8_t idx = end;
	// 找到第一个大于等于的值的位置
	while (begin < end)
	{
		mid = ((end - begin) / 2) + begin;
		float val = p->AnchorsX[mid];
		if (val > x)
			end = mid;
		else if (val < x)
			begin = mid + 1;
		else
		{
			idx = mid;
			break;
		}
	}
	if (begin >= end)
		idx = end;
	if (idx >= p->nAnchors)
		idx = p->nAnchors - 1;
	float k = p->Slopes[idx];
	float b0 = p->Intercepts[idx];
	return k * x + b0;
}

void MLVM_clear(MultiLinearValueMapper_t* p)
{
	p->nAnchors = 0;
	p->isSynthesized = false;
}

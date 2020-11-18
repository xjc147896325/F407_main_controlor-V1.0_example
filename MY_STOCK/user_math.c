#include "user_math.h"

void absLimit(float* Val, float Limit)
{
	if(*Val > Limit)
		*Val = Limit;
	if(*Val < -Limit)
		*Val = -Limit;
}
void absLimit_int16(int16_t * Val, int16_t Limit)
{
	if(*Val > Limit)
		*Val = Limit;
	if(*Val < -Limit)
		*Val = -Limit;
}
void absLimit_int32(int32_t * Val, int32_t Limit)
{
	if(*Val > Limit)
		*Val = Limit;
	if(*Val < -Limit)
		*Val = -Limit;
}

float Min_f(float x, float y)
{
	return x < y ? x : y;
}

float Max_f(float x, float y)
{
	return x > y ? x : y;
}

float MinAbs_f(float x, float y)
{
	return __fabs(x) < __fabs(y) ? x : y;
}

float MaxAbs_f(float x, float y)
{
	return __fabs(x) > __fabs(y) ? x : y;
}

float InvSqrt(float Val)
{
	float HalfVal = 0.5f * Val;
	float y = Val;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (HalfVal * y * y));
	return y;
}

uint8_t ValueInRange_u(uint32_t Value, uint32_t Min, uint32_t Max)
{
	if((Value >= Min) && (Value <= Max))
		return 1;
	return 0;
}

uint8_t ValueInRange_i(int32_t Value, int32_t Min, int32_t Max)
{
	if((Value >= Min) && (Value <= Max))
		return 1;
	return 0;
}

uint8_t ValueInRange_f(float Value, float Min, float Max)
{
	if((Value >= Min) && (Value <= Max))
		return 1;
	return 0;
}
/**
  *  @brief  柔化 步进
  *  @param  dstVal 设定值
             srcVal 当前值
             step   步进值
  *  @retval
  *
  *
  */



float FlexibelValue(float dstVal, float srcVal, float step)
{
	float RealVal;
	if(srcVal < dstVal)
		if(srcVal + step > dstVal)
			RealVal = dstVal;
		else
			RealVal = srcVal + step;
	else
		if(srcVal - step < dstVal)
			RealVal = dstVal;
		else
			RealVal = srcVal - step;
	return RealVal;
}
/**
  *  @brief  柔化 步进
  *  @param  dstVal 设定值
             srcVal 当前值
             step   步进值
  *  @retval
  *
  *
  */

int16_t FlexibelValue_int16(int16_t dstVal, int16_t srcVal, int16_t step)
{
	int16_t RealVal;
	if(srcVal < dstVal)
		if(srcVal + step > dstVal)
			RealVal = dstVal;
		else
			RealVal = srcVal + step;
	else
		if(srcVal - step < dstVal)
			RealVal = dstVal;
		else
			RealVal = srcVal - step;
	return RealVal;
}












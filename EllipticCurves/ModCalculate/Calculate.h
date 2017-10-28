#pragma once

#include <cstdint>
#include <array>

//b=1 - 5 шагов; 2-5; 3-3; 4-5; 6-3; 7-7; 9-5 good; 
//5-24 bad;

int64_t inverse(const int64_t _value, const int64_t _mod);

int64_t divide(const int64_t _a, const int64_t _b, const int64_t _mod);

int64_t greatestCommonDivisor(const int64_t _a, const int64_t _b);

bool onCurve(int64_t _fieldSize, int64_t _curveA, int64_t _curveB, const int64_t x, const int64_t y);

int64_t tryMultiplyPoint(int64_t _fieldSize, int64_t _curveA, int64_t _curveB, int64_t &_changedPointX, int64_t &_changedPointY, const int64_t _multiplier);
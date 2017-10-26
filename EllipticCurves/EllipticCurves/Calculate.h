#pragma once

#include <cstdint>
#include <array>

const int
fieldSize = 289,//167,//289,//185,//161,
curveA = 3,
curveB = 5;
//b=1 - 6 шагов; 2-6; 3-4; 4-6; 6-4; 7-8; 9-6 good; 
//5-24 bad;

uint64_t inverse(const uint64_t _value, const uint64_t _mod);

uint64_t divide(const uint64_t _a, const uint64_t _b, const uint64_t _mod);

uint64_t greatestCommonDivisor(const uint64_t _a, const uint64_t _b);

bool onCurve(const uint64_t x, const uint64_t y);

uint64_t tryMultiplyPoint(uint64_t &_changedPointX, uint64_t &_changedPointY, const uint64_t _multiplier);
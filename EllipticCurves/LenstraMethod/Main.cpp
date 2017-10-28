#include <iostream>
#include <string>

#include "../ModCalculate/Calculate.h"

using namespace std;

int main()
{
	int64_t tryMultiplyPoint(int64_t _fieldSize, int64_t _curveA, int64_t _curveB, int64_t &_changedPointX, int64_t &_changedPointY, const int64_t _multiplier);

	int64_t
		fieldSize = 10007ULL * 10007 * 10007,
		x0 = 13,
		y0 = 5,
		pointX = x0,
		pointY = y0,
		curveA = 3,
		curve_B = (y0 * y0 - x0 * x0 * x0 - curveA * x0) % fieldSize,
		curveB = (curve_B > 0 ? curve_B : curve_B + fieldSize),
		divisor = 1,
		k = 2;

	while (divisor == 1)
	{
		divisor = tryMultiplyPoint(fieldSize, curveA, curveB, pointX, pointY, k);
		k++;

		if (divisor == 0)
		{
			string message = "Point at infinity reached! Multiplier: " + to_string(k - 1);
			cout << message << endl;
		}
		else if (divisor > 1)
		{
			string message = "Divisor is " + std::to_string(divisor) + ", multiplier: " + to_string(k - 1);
			cout << message << endl;
		}
	}
	 
	//std::cout << curveB << std::endl;

	system("pause");
}
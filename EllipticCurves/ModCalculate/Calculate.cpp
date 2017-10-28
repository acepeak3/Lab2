#include"Calculate.h"

int64_t inverse(const int64_t _value, const int64_t _mod) 
{
	int64_t n = _value, m = _mod, a = 1, b = 0, c = 0, d = 1;

	if (n >= m)
	{
		n %= m;
	}

	bool evenStep = true;

	while ((n != 1) && (m != 1))
	{
		if (evenStep)
		{
			const int64_t k = m / n;
			m %= n;
			a += c*k;
			b += d*k;
		}
		else
		{
			const int64_t k = n / m;
			n %= m;
			c += a*k;
			d += b*k;
		}

		evenStep = !evenStep;
	}

	return (evenStep ? d : _mod - b);

}

int64_t divide(const int64_t _a, const int64_t _b, const int64_t _mod)
{
	const int64_t result = inverse(_b, _mod) * _a;

	return (int64_t)(result % _mod);
}

int64_t greatestCommonDivisor(const int64_t _a, const int64_t _b) 
{
	int64_t
		a = _a,
		b = _b;

	if (_b > _a)
	{
		a = _b;
		b = _a;
	}

	while (b > 1)
	{
		const int64_t c = a % b;
		a = b;
		b = c;
	}

	return (b == 0 ? a : b);
}

bool onCurve(int64_t _fieldSize, int64_t _curveA, int64_t _curveB, const int64_t x, const int64_t y) 
{
	return ((y * y - x * x * x - _curveA * x - _curveB) % _fieldSize == 0);
}

int64_t tryDoublePoint(int64_t _fieldSize, int64_t _curveA, int64_t _curveB, int64_t &_changedPointX, int64_t &_changedPointY)
{
	int64_t denominator = 2 * _changedPointY;
	denominator %= _fieldSize;
	int64_t gcd = greatestCommonDivisor(_fieldSize, denominator);

	if (gcd > 1)
	{
		return gcd;
	}
	else
	{
		int64_t lambda = divide(3 * _changedPointX * _changedPointX + _curveA, 2 * _changedPointY, _fieldSize);
		int64_t changedPointX = (lambda * lambda - 2 * _changedPointX) % _fieldSize;
		int64_t changedPointY = ((_changedPointX - changedPointX) * lambda - _changedPointY) % _fieldSize;
		_changedPointX = (changedPointX < 0 ? changedPointX + _fieldSize : changedPointX);
		_changedPointY = (changedPointY < 0 ? changedPointY + _fieldSize : changedPointY);

		/*if (!onCurve(_changedPointX, _changedPointY)) // проверка что мы находимся на кривой
		{
		int t = 0;
		}*/


		return 1;
	}
}

int64_t tryAddPoint(int64_t _fieldSize, int64_t _curveA, int64_t _curveB, int64_t &_changedPointX, int64_t &_changedPointY, const int64_t _pointToAddX, const int64_t _pointToAddY)
{
	if ((_changedPointX == _pointToAddX) && (_changedPointY == _pointToAddY))
	{
		return tryDoublePoint(_fieldSize, _curveA, _curveB, _changedPointX, _changedPointY);
	}
	else
	{
		int64_t delta = (_changedPointY - _pointToAddY) % _fieldSize;
		const int64_t deltaY = (delta < 0 ? delta + _fieldSize : delta);
		delta = (_changedPointX - _pointToAddX) % _fieldSize;
		const int64_t deltaX = (delta < 0 ? delta + _fieldSize : delta);

		if (deltaX == 0) 
		{
			return 0;
		}

		const int64_t gcd = greatestCommonDivisor(_fieldSize, deltaX);
		if (gcd > 1)
		{
			return gcd;
		}
		else 
		{
			int64_t lambda = divide(deltaY, deltaX, _fieldSize);
			int64_t changedPointX = (lambda * lambda - _changedPointX - _pointToAddX) % _fieldSize;
			int64_t changedPointY = (lambda * (_changedPointX - changedPointX) - _changedPointY) % _fieldSize;
			_changedPointX = (changedPointX < 0 ? changedPointX + _fieldSize : changedPointX);
			_changedPointY = (changedPointY < 0 ? changedPointY + _fieldSize : changedPointY);

			/*if (!onCurve(_changedPointX, _changedPointY)) // проверка что мы находимся на кривой
			{
			int t = 0;
			}*/

			return 1;
		}
	}
}

int64_t tryMultiplyPoint(int64_t _fieldSize, int64_t _curveA, int64_t _curveB, int64_t &_changedPointX, int64_t &_changedPointY, const int64_t _multiplier) 
{
	int64_t
		power2MultipliedPointX = _changedPointX,
		power2MultipliedPointY = _changedPointY;

	bool firstBitSet = false;

	int64_t
		divisor = 1,
		rest = _multiplier;

	while ((divisor == 1) && (rest > 0)) 
	{
		const int64_t binaryDigit = rest % 2;
		rest /= 2;

		if (binaryDigit == 1)
		{
			if (!firstBitSet) 
			{
				firstBitSet = true;
				_changedPointX = power2MultipliedPointX;
				_changedPointY = power2MultipliedPointY;
			}
			else
			{
				divisor = tryAddPoint(_fieldSize, _curveA, _curveB, _changedPointX, _changedPointY, power2MultipliedPointX, power2MultipliedPointY);
			}
		}

		if (rest > 0)
		{
			divisor = tryDoublePoint(_fieldSize, _curveA, _curveB, power2MultipliedPointX, power2MultipliedPointY);
		}
	}

	return divisor;
}
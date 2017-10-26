#include "Calculate.h"

uint64_t inverse(const uint64_t _value, const uint64_t _mod)
{
	uint64_t n = _value, m = _mod, a = 1, b = 0, c = 0, d = 1;

	if (n >= m)
	{
		n %= m;
	}

	bool evenStep = true;

	while ((n != 1) && (m != 1))
	{
		if (evenStep)
		{
			const uint64_t k = m / n;
			m %= n;
			a += c*k;
			b += d*k;
		}
		else
		{
			const uint64_t k = n / m;
			n %= m;
			c += a*k;
			d += b*k;
		}

		evenStep = !evenStep;
	}

	return (evenStep ? d : _mod - b);

}

uint64_t divide(const uint64_t _a, const uint64_t _b, const uint64_t _mod)
{
	return (inverse(_b, _mod) * _a) % _mod;
}

uint64_t greatestCommonDivisor(const uint64_t _a, const uint64_t _b)
{
	uint64_t
		a = _a,
		b = _b;

	if (_b > _a)
	{
		a = _b;
		b = _a;
	}	

	while (b > 1)
	{
		const uint64_t c = a % b;
		a = b;
		b = c;
	}

	return (b == 0 ? a : b);
}

bool onCurve(const uint64_t x, const uint64_t y) 
{
	int64_t diff = (int64_t)y * y - x * x * x - curveA * x - curveB;
	return (diff % fieldSize == 0);
}

uint64_t tryDoublePoint(uint64_t &_changedPointX, uint64_t &_changedPointY)
{
	uint64_t denominator = 2 * _changedPointY;
	denominator %= fieldSize;
	uint64_t gcd = greatestCommonDivisor(fieldSize, denominator);

	if (gcd > 1)
	{
		return gcd;
	}
	else
	{
		int64_t lambda = divide(3 * _changedPointX * _changedPointX + curveA, 2 * _changedPointY, fieldSize);
		int64_t diff = lambda * lambda - 2 * _changedPointX;
		int64_t changedPointX = diff % fieldSize;
		diff = (_changedPointX - changedPointX) * lambda - _changedPointY;
		int64_t changedPointY = diff % fieldSize;
		_changedPointX = (changedPointX < 0 ? changedPointX + fieldSize : changedPointX);
		_changedPointY = (changedPointY < 0 ? changedPointY + fieldSize : changedPointY);

		if (!onCurve(_changedPointX, _changedPointY))
		{
			int t = 0;
		}

		return 1;
	}
}

uint64_t tryAddPoint(uint64_t &_changedPointX, uint64_t &_changedPointY, const uint64_t _pointToAddX, const uint64_t _pointToAddY)
{
	if ((_changedPointX == _pointToAddX) && (_changedPointY == _pointToAddY))
	{
		return tryDoublePoint(_changedPointX, _changedPointY);
	}
	else
	{
		int64_t diff = (int64_t)_changedPointY - _pointToAddY;
		int64_t delta = diff % fieldSize;
		const uint64_t deltaY = (delta < 0 ? delta + fieldSize : delta);
		diff = (int64_t)_changedPointX - _pointToAddX;
		delta = diff % fieldSize;
		const uint64_t deltaX = (delta < 0 ? delta + fieldSize : delta);

		if (deltaX == 0)
		{
			return 0;
		}

		const uint64_t gcd = greatestCommonDivisor(fieldSize, deltaX);
		if (gcd > 1)
		{
			return gcd;
		}
		else
		{
			int64_t lambda = divide(deltaY, deltaX, fieldSize);
			int64_t diff = lambda * lambda - _changedPointX - _pointToAddX;
			int64_t changedPointX = diff % fieldSize;
			diff = lambda * (_changedPointX - changedPointX) - _changedPointY;
			int64_t changedPointY = diff % fieldSize;
			_changedPointX = (changedPointX < 0 ? changedPointX + fieldSize : changedPointX);
			_changedPointY = (changedPointY < 0 ? changedPointY + fieldSize : changedPointY);

			if (!onCurve(_changedPointX, _changedPointY))
			{
				int t = 0;
			}

			return 1;
		}
	}
}

uint64_t tryMultiplyPoint(uint64_t &_changedPointX, uint64_t &_changedPointY, const uint64_t _multiplier)
{
	uint64_t 
		power2MultipliedPointX = _changedPointX,
		power2MultipliedPointY = _changedPointY;

	bool firstBitSet = false;

	uint64_t
		divisor = 1,
		rest = _multiplier,
		k = 0;

	while ((divisor == 1) && (rest > 0))
	{
		const uint64_t binaryDigit = rest % 2;
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
				divisor = tryAddPoint(_changedPointX, _changedPointY, power2MultipliedPointX, power2MultipliedPointY);
			}
		}

		if (rest > 0)
		{
			divisor = tryDoublePoint(power2MultipliedPointX, power2MultipliedPointY);
			k++;
		}
	}	

	return divisor;
}
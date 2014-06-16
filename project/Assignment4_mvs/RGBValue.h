#ifndef RGBVALUE_H
#define RGBVALUE_H

//image class just dumped to hide...
class RGBValue
{
public:
	RGBValue(float rI = 0, float gI = 0, float bI = 0)
		: r(rI)
		, g(gI)
		, b(bI)
	{
		if (r>1)
			r = 1.0;
		if (g>1)
			g = 1.0;
		if (b>1)
			b = 1.0;

		if (r<0)
			r = 0.0;
		if (g<0)
			g = 0.0;
		if (b<0)
			b = 0.0;
	};

	float operator[](int i) const
	{
		switch (i)
		{
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		default:
			return r;
		}
	}
	float & operator[](int i)
	{
		switch (i)
		{
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		default:
			return r;
		}
	}
	float r, b, g;
};

#endif
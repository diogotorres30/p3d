#include "Color.h"

void Color::operator += (const Color &v)
{
	r += v.r;
	g += v.g;
	b += v.b;
}

void Color::operator *= (const float s)
{
	r *= s;
	g *= s;
	b *= s;
}

Color operator+ (const Color &v, const Color &u)
{
	return Color(v.r + u.r, v.g + u.g, v.b + u.b);
}

Color operator* (const Color &v, const float &num)
{
	return Color(v.r * num, v.g * num, v.b * num);
}

Color operator* (const float &num, const Color &v)
{
	return Color(num * v.r, num * v.g, num * v.b);
}

Color operator* (const Color &v, const Color &u)
{
	return Color(v.r * u.r, v.g * u.g, v.b * u.b);
}
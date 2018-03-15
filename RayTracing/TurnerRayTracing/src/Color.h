#ifndef __COLOR_H__
#define __COLOR_H__

struct Color
{
	public:
		float r, g, b;
		Color() {};

		Color(float red, float green, float blue) 
		{
			r = red;
			g = green;
			b = blue;
		};
		void operator += (const Color &v);
		void operator *= (const float s);
};

Color operator+ (const Color &v, const Color &u);
Color operator* (const Color &v, const float &num);
Color operator* (const float &num, const Color &v);
Color operator* (const Color &v, const Color &u);

#endif // !__COLOR_H__
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
};

#endif // !__COLOR_H__
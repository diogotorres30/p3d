#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "MathAux.h"
#include "Color.h"

class Light
{
	protected:
		Vector3 position;
		Color color;

	public:
		Light() {};
		Light(Vector3 pos, Color col);
		~Light() {};
		Vector3 getPosition() { return position; };
		Color getColor() { return color; };
};

#endif // !__LIGHT_H__


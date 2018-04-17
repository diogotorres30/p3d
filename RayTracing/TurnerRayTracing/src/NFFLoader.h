#ifndef __NFFLOADER_H__
#define __NFFLOADER_H__

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <vector>

#include "Mesh.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Light.h"
#include "Camera.h"
#include "MathAux.h"
#include "Scene.h"
#include "BoudingBox.h"


class NFFLoader
{

	private:
        Scene *scene;
		Material *material;
        BoudingBox *boudingBox;
		Camera *camera;

	public:
		NFFLoader();
		~NFFLoader() {};
		void parseLine(std::string sin);
		Scene *createScene(std::string &filename);
		
		void parseViewing(std::string &sin);
		void parseFrom(std::string sin);
		void parseAt(std::string sin);
		void parseUp(std::string sin);
		void parseAngle(std::string sin);
		void parseHither(std::string sin);
		void parseResolution(std::string sin);
		
		void parseBackground(std::string sin);
		void parseLight(std::string sin);
		void parseMaterial(std::string sin);
		void parseCone(std::string sin);
		void parseSphere(std::string sin);
		void parsePolygon(std::string line1, std::string line2, std::string line3);
		void parsePolygonalPatch(std::string sin);
		void parsePlane(std::string sin);
};

#endif // !__NFFLOADER_H__

//"v" - viewing vectors and angles
	//"from" Fx Fy Fz
	//"at" Ax Ay Az
	//"up" Ux Uy Uz
	//"angle" angle
	//"hither" hither
	//"resolution" xres yres
//"b" - background color
//"l" - positional light location
//"f" - object material properties
//"c" - cone or cylinder primitive
//"s" - sphere primitive
//"p" - polygon primitive
//"pp" - polygonal patch primitive

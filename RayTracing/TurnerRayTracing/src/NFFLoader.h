#ifndef __NFFLOADER_H__
#define __NFFLOADER_H__

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

class NFFLoader
{
	private:
		
	public:
		virtual ~NFFLoader();
		void parseLine(std::stringstream& sin);
		void parseViewing(std::stringstream& sin);

		void parseCone(std::stringstream& sin);
		void parseCone(std::stringstream& sin);
		void parseCone(std::stringstream& sin);
		void parseCone(std::stringstream& sin);
		void parseCone(std::stringstream& sin);
		void parseCone(std::stringstream& sin);
		void parseCone(std::stringstream& sin);

		void parseBackground(std::stringstream& sin);
		void parseLight(std::stringstream& sin);
		void parseMaterial(std::stringstream& sin);
		void parseCone(std::stringstream& sin);
		void parseSphere(std::stringstream& sin);
		void parsePolygon(std::stringstream& sin);
		void parsePolygonalPatch(std::stringstream& sin);
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
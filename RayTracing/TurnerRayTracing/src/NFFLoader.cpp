#include "NFFLoader.h"

void NFFLoader::parseLine(std::stringstream& sin)
{
	std::string s;
	sin >> s;
	if (s.compare("v") == 0) parseVertex(sin);
	else if (s.compare("vt") == 0) parseTexcoord(sin);
	else if (s.compare("vn") == 0) parseNormal(sin);
	else if (s.compare("usemtl") == 0) parseMesh(sin);
	//else if (s.compare("s") == 0) {
	//	sin >> s;
	//	if (s.compare("1") == 0) smoothedFace = true;
	//	else smoothedFace = false;
	//}
	else if (s.compare("f") == 0 /*&& smoothedFace*/) parseFace(sin);
}
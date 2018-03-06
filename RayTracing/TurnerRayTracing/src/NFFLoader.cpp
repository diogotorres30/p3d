#include "NFFLoader.h"

NFFLoader::NFFLoader() 
{
	std::string sin;
	std::ifstream nff_input("nff_input.nff");
	for (std::string line; std::getline(nff_input, line);) 
	{
		sin = line.substr(0, line.find(" "));
		if (line.compare("v") == 0) parseViewing(sin);
		else if (sin.compare("b") == 0) parseBackground(line);
		else if (sin.compare("l") == 0) parseLight(line);
		else if (sin.compare("f") == 0) parseMaterial(line);
		else if (sin.compare("c") == 0) parseCone(line);
		else if (sin.compare("s") == 0) parseSphere(line);
		else if (sin.compare("p") == 0) parsePolygon(line);
		else if (sin.compare("pp") == 0) parsePolygonalPatch(line);
		else if (sin.compare("pl") == 0) parsePlane(line);
	}
}

void NFFLoader::parseViewing(std::string s) 
{
	std::istringstream iss(s);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	//Depois e chamar a respectiva utilizando as posicoes correctas do vector 'results' criado acima
}

void NFFLoader::parseFrom(std::string s)
{

}

void NFFLoader::parseAt(std::string s)
{
}

void NFFLoader::parseUp(std::string s)
{
}

void NFFLoader::parseAngle(std::string s)
{
}

void NFFLoader::parseHither(std::string s)
{
}

void NFFLoader::parseResolution(std::string s)
{
}

void NFFLoader::parseBackground(std::string s)
{
	std::istringstream iss(s);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

}

void NFFLoader::parseLight(std::string s)
{
}

void NFFLoader::parseMaterial(std::string s)
{
}

void NFFLoader::parseCone(std::string s)
{
}

void NFFLoader::parseSphere(std::string s)
{
}

void NFFLoader::parsePolygon(std::string s)
{
}

void NFFLoader::parsePolygonalPatch(std::string s)
{
}

void NFFLoader::parsePlane(std::string s)
{
}


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
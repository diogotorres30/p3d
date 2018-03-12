#include "NFFLoader.h"

NFFLoader::NFFLoader()
{
	scene = NULL;
	material = NULL;
	camera = NULL;
}

Scene *NFFLoader::createScene(std::string &filename)
{
    scene = new Scene();

    std::string sin;
    std::ifstream nff_input(filename);
    for (std::string line; std::getline(nff_input, line);)
    {
        sin = line.substr(0, line.find(" "));
        if (line.compare("v") == 0) parseViewing(line);
        else if (sin.compare("from") == 0) parseFrom(line);
        else if (sin.compare("at") == 0) parseAt(line);
        else if (sin.compare("up") == 0) parseUp(line);
        else if (sin.compare("angle") == 0) parseAngle(line);
        else if (sin.compare("hither") == 0) parseHither(line);
        else if (sin.compare("resolution") == 0) parseResolution(line);
        else if (sin.compare("b") == 0) parseBackground(line);
        else if (sin.compare("l") == 0) parseLight(line);
        else if (sin.compare("f") == 0) parseMaterial(line);
        else if (sin.compare("c") == 0) parseCone(line);
        else if (sin.compare("s") == 0) parseSphere(line);
        else if (sin.compare("p") == 0) parsePolygon(line);
        else if (sin.compare("pp") == 0) parsePolygonalPatch(line);
        else if (sin.compare("pl") == 0) parsePlane(line);

    }
    return scene;
}

void NFFLoader::parseViewing(std::string &sin)
{
	camera = new Camera();

	/*std::istringstream iss(s);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());*/

	//Depois e chamar a respectiva utilizando as posicoes correctas do vector 'results' criado acima


}

void NFFLoader::parseFrom(std::string s)
{
	std::istringstream iss(s);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string::size_type sz;	
	Vector3 from = Vector3(std::stof(results[1], &sz), std::stof(results[2], &sz), std::stof(results[3], &sz));
	camera->setFrom(from);
}

void NFFLoader::parseAt(std::string s)
{
	std::istringstream iss(s);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string::size_type sz;
	Vector3 at = Vector3(std::stof(results[1], &sz), std::stof(results[2], &sz), std::stof(results[3], &sz));
	camera->setAt(at);
}

void NFFLoader::parseUp(std::string s)
{
	std::istringstream iss(s);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string::size_type sz;
	Vector3 up = Vector3(std::stof(results[1], &sz), std::stof(results[2], &sz), std::stof(results[3], &sz));
	camera->setUp(up);
}

void NFFLoader::parseAngle(std::string s)
{
	std::istringstream iss(s);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string::size_type sz;
	camera->setAngle(std::stof(results[1], &sz));
}

void NFFLoader::parseHither(std::string s)
{
	std::istringstream iss(s);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string::size_type sz;
	camera->setHither(std::stof(results[1], &sz));
}

void NFFLoader::parseResolution(std::string s)
{
	std::istringstream iss(s);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string::size_type sz;
	camera->setResolution(std::stof(results[1], &sz), std::stof(results[2], &sz));
}

void NFFLoader::parseBackground(std::string s)
{
	std::istringstream iss(s);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string::size_type sz;
	Color up = Color(std::stof(results[1], &sz), std::stof(results[2], &sz), std::stof(results[3], &sz));
	scene->addBackground(up);

	//create color with the given values and add it to the scene
	/*Color c = new Color();
	scene->addBackground(c);*/
}

void NFFLoader::parseLight(std::string s)
{
	std::istringstream iss(s);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string::size_type sz;

	Vector3 pos = Vector3(std::stof(results[1], &sz), std::stof(results[2], &sz), std::stof(results[3], &sz));
	Color col = Color(std::stof(results[3], &sz), std::stof(results[4], &sz), std::stof(results[5], &sz));
	scene->addLight(col);
	//create light and add it to the scene
	//Light *light = new Light();
	//scene->addLight(light);
}

void NFFLoader::parseMaterial(std::string s)
{
	//create material with the given values, add it to the scene and store in a variable to use in the creation of new meshs
	//Material *mat = new Material();
	//material = mat;
	//scene->addMaterial(mat);
}

void NFFLoader::parseCone(std::string s)
{
	//Do later if needed
}

void NFFLoader::parseSphere(std::string s)
{
	//create sphere and add it to the scene as a mesh with the appropriate material
	//Sphere *sphere = new Sphere();
	//scene->addMesh(sphere, material);
}

void NFFLoader::parsePolygon(std::string s)
{
	//Do later if needed
}

void NFFLoader::parsePolygonalPatch(std::string s)
{
	//Do later if needed
}

void NFFLoader::parsePlane(std::string s)
{
	//create plane and add it to the scene as a mesh with the appropriate material
	//Plane *plane = new Plane();
	//scene->addMesh(plane, material);
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

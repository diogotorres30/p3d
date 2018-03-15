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

    std::string sin, line1, line2, line3;
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
        else if (sin.compare("p") == 0)
        {
            std::getline(nff_input, line1);
            std::getline(nff_input, line2);
            std::getline(nff_input, line3);
            parsePolygon(line1, line2, line3);
            
        }
        else if (sin.compare("pp") == 0) parsePolygonalPatch(line);
        else if (sin.compare("pl") == 0) parsePlane(line);
    }
    return scene;
}

void NFFLoader::parseViewing(std::string &sin)
{
	camera = new Camera();
	scene->setCamera(camera);
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
	camera->setFov(std::stof(results[1], &sz));
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
	camera->setResolution(std::stoi(results[1], &sz), std::stoi(results[2], &sz));
}

void NFFLoader::parseBackground(std::string s)
{
	std::istringstream iss(s);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string::size_type sz;
	Color up = Color(std::stof(results[1], &sz), std::stof(results[2], &sz), std::stof(results[3], &sz));
	scene->addBackground(up);
	//create color with the given values and add it to the scene
}

void NFFLoader::parseLight(std::string s)
{
	std::istringstream iss(s);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string::size_type sz;
	Vector3 pos = Vector3(std::stof(results[1], &sz), std::stof(results[2], &sz), std::stof(results[3], &sz));
	Color col = Color(std::stof(results[4], &sz), std::stof(results[5], &sz), std::stof(results[6], &sz));
	Light *l = new Light(pos, col);
	scene->addLight(l);
	//create light and add it to the scene
}

void NFFLoader::parseMaterial(std::string s)
{
	std::istringstream iss(s);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string::size_type sz;
	Color col = Color(std::stof(results[1], &sz), std::stof(results[2], &sz), std::stof(results[3], &sz));
	Material *mat = new Material(col, std::stof(results[4], &sz), std::stof(results[5], &sz), std::stof(results[6], &sz), std::stof(results[7], &sz), std::stof(results[8], &sz));
	scene->addMaterial(mat);
	//create material with the given values, add it to the scene and store in a variable to use in the creation of new meshs
}

void NFFLoader::parseCone(std::string s)
{
	//Do later if needed
}

void NFFLoader::parseSphere(std::string s)
{
	std::istringstream iss(s);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string::size_type sz;
	Vector3 center = Vector3(std::stof(results[1], &sz), std::stof(results[2], &sz), std::stof(results[3], &sz));
	//create sphere and add it to the scene as a mesh with the appropriate material
	Sphere *sphere = new Sphere(center, std::stof(results[4], &sz),scene->getMaterials().back());
	scene->addMesh(sphere);
}

void NFFLoader::parsePolygon(std::string line1, std::string line2, std::string line3)
{
    std::istringstream iss(line1);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
    std::string::size_type sz;
    Vector3 p1 = Vector3(std::stof(results[0], &sz), std::stof(results[1], &sz), std::stof(results[2], &sz));
    std::istringstream iss1(line2);
    std::vector<std::string> results1((std::istream_iterator<std::string>(iss1)), std::istream_iterator<std::string>());
    Vector3 p2 = Vector3(std::stof(results1[0], &sz), std::stof(results1[1], &sz), std::stof(results1[2], &sz));
    std::istringstream iss2(line3);
    std::vector<std::string> results2((std::istream_iterator<std::string>(iss2)), std::istream_iterator<std::string>());
    Vector3 p3 = Vector3(std::stof(results2[0], &sz), std::stof(results2[1], &sz), std::stof(results2[2], &sz));
}

void NFFLoader::parsePolygonalPatch(std::string s)
{
	//Do later if needed
}

void NFFLoader::parsePlane(std::string s)
{
	std::istringstream iss(s);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string::size_type sz;
	Vector3 a = Vector3(std::stof(results[1], &sz), std::stof(results[2], &sz), std::stof(results[3], &sz));
	Vector3 b = Vector3(std::stof(results[4], &sz), std::stof(results[5], &sz), std::stof(results[6], &sz));
	Vector3 c = Vector3(std::stof(results[7], &sz), std::stof(results[8], &sz), std::stof(results[9], &sz));
	//create plane and add it to the scene as a mesh with the appropriate material
	Plane *plane = new Plane(a,b,c,scene->getMaterials().back());
	scene->addMesh(plane);
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

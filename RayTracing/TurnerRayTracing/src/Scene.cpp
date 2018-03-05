#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::setCamera(Camera *cam)
{
	camera = cam;
}

int Scene::load_nff(std::string& filename)
{
	//NFFLoader loader = NFFLoader();

	//TODO: missing NFFLoader implementation

	return 0;
}
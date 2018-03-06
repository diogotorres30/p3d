#include "Scene.h"

Scene::Scene()
{
    Camera *camera = nullptr;
    std::vector<Mesh*> meshes = std::vector<Mesh*>();
    std::vector<Material*> materials = std::vector<Material*>();
    std::vector<Light*> lights = std::vector<Light*>();
}

void Scene::setCamera(Camera *cam)
{
    camera = cam;
}

void Scene::addMesh(Mesh *mesh)
{
    meshes.push_back(mesh);
}

void Scene::addMaterial(Material *material)
{
    materials.push_back(material);
}

void Scene::addBackground(const Color &color)
{
    background = color;
}

void Scene::addCamera(Camera *cam)
{
    camera = cam;
}

void Scene::addLight(Light *light)
{
    lights.push_back(light);
}


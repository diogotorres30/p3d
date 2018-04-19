#ifndef Scene_h
#define Scene_h

#include <stdio.h>
#include <vector>

#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"
#include "Box.h"
#include "Grid.h"

class Scene
{
    private:
        Camera *camera;
        std::vector<Mesh*> meshes;
        std::vector<Material*> materials;
        std::vector<Light*> lights;
		Grid *accelerationStructure = nullptr;
        Color background;
    
    public:
        Scene();
		~Scene() {};
        Camera *getCamera() { return camera; };
		std::vector<Mesh*> getMeshes() { return meshes; };
		std::vector<Material*> getMaterials() { return materials; };
		std::vector<Light*> getLights() { return lights; };
		Color getBackground() { return background; };
		Grid *getAccelerationStructure() { return accelerationStructure; };
		void setAccelerationStructure(Grid *grid) { accelerationStructure = grid; };
		void createAccelerationStructure();
        void setCamera(Camera *cam);
        void addMesh(Mesh *mesh);
        void addMaterial(Material *material);
        void addBackground(const Color &color);
        void addCamera(Camera *cam);
        void addLight(Light *light);
};


#endif /* Scene_h */

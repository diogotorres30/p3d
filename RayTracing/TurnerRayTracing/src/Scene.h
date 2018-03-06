#ifndef Scene_h
#define Scene_h

#include <stdio.h>
#include <vector>

#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"

class Scene
{
    private:
        Camera *camera;
        std::vector<Mesh*> meshes;
        std::vector<Material*> materials;
        std::vector<Light*> lights;
        Color background;
    
    public:
        Scene();
        ~Scene();
        Camera *getCamera() { return camera; };
        void setCamera(Camera *cam);
        void addMesh(Mesh *mesh);
        void addMaterial(Material *material);
        void addBackground(const Color &color);
        void addCamera(Camera *cam);
        void addLight(Light *light);
};


#endif /* Scene_h */

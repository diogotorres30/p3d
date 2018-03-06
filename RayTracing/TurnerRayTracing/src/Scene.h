#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>

#include "NFFLoader.h"
#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Material.h"

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

#endif // !__SCENE_H__
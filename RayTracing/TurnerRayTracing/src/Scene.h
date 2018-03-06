#ifndef __SCENE_H__
#define __SCENE_H__

#include "NFFLoader.h"
#include "Camera.h"
#include "Color.h"

class Scene
{
	private:
		Camera *camera = NULL;
		//missing mesh implementation
		//Mesh *meshes;
		Color background;

	public:
		Scene();
		~Scene();
		Camera *getCamera() { return camera; };
		void setCamera(Camera *cam);
		int load_nff(std::string& filename);
};

#endif // !__SCENE_H__
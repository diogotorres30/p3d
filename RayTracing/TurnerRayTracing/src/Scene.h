#ifndef __SCENE_H__
#define __SCENE_H__

#include "Camera.h"

class Scene
{
	private:
		Camera camera;


	public:
		Scene();
		~Scene();
		Camera GetCamera();
		void setCamera(Camera cam);
};

#endif // !__SCENE_H__
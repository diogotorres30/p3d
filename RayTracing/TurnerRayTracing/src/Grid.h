#ifndef __GRID_H__
#define __GRID_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

#include "BoundingBox.h"
#include "Box.h"

class Grid
{
	private:
		Box *BB;
		std::vector<Box*> uniformGrid;
		Vector3 p0, p1;
		float Nx, Ny, Nz;

	public:
		Grid();
		Grid(std::vector<Mesh*> meshes);
		~Grid() {};
		std::vector<Box*> getUniformGrid() { return uniformGrid; };
		Mesh *intersect(Ray ray, float &nearestT);
};


#endif /* __GRID_H__ */

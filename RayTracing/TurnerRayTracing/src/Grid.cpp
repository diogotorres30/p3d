#include "Grid.h"

Grid::Grid() {}

Grid::Grid(std::vector<Mesh*> meshes)
{
	Vector3 pMin = Vector3(HUGE_VALUE);
	Vector3 pMax = Vector3(EPSILON);
	BoundingBox *box;

	for (std::vector<Mesh*>::iterator itMesh = meshes.begin(); itMesh != meshes.end(); ++itMesh)
	{
		box = (*itMesh)->getBoundingBox();
		if (box != nullptr) {
			
			if (box->getXMin() < pMin.x)
				pMin.x = box->getXMin();
			if (box->getYMin() < pMin.y)
				pMin.y = box->getYMin();
			if (box->getZMin() < pMin.z)
				pMin.z = box->getZMin();

			if (box->getXMax() > pMax.x)
				pMax.x = box->getXMax();
			if (box->getYMax() > pMax.y)
				pMax.y = box->getYMax();
			if (box->getZMax() > pMax.z)
				pMax.z = box->getZMax();
		}
	}

	pMin -= EPSILON;
	pMax += EPSILON;

	p0 = pMin;
	p1 = pMax;

	BB = new Box(pMin.x, pMax.x, pMin.y, pMax.y, pMin.z, pMax.z);

	int N = meshes.size();

	//multiplying factor
	int m = 2;

	float wx = p1.x - p0.x;
	float wy = p1.y - p0.y;
	float wz = p1.z - p0.z;

	float s = cbrt((wx*wy*wz)/N);

	Nx = trunc(m * (wx / s)) + 1;
	Ny = trunc(m * (wy / s)) + 1;
	Nz = trunc(m * (wz / s)) + 1;

	int TotalN = Nx * Ny * Nz;

	float cellX = (wx / Nx);
	float cellY = (wy / Ny);
	float cellZ = (wz / Nz);

	for (int i = 0; i < Nz; i++)
	{
		for (int j = 0; j < Ny; j++)
		{
			for (int k = 0; k < Nx; k++)
			{
				uniformGrid.push_back(new Box(p0.x + (k * cellX), p0.x + ((k * cellX) + cellX),
											p0.y + (j * cellY), p0.y + ((j * cellY) + cellY),
											p0.z + (i * cellZ), p0.z + ((i * cellZ) + cellZ)));
			}
		}
	}

	int index;

	for (std::vector<Mesh*>::iterator itMesh = meshes.begin(); itMesh != meshes.end(); ++itMesh)
	{
		if ((*itMesh)->getBoundingBox() != nullptr)
		{
			Mesh *ptr = (*itMesh);
			int iXMin = clamp((((*itMesh)->getBoundingBox()->getXMin() - p0.x) * (Nx / (p1.x - p0.x))), 0, (Nx - 1));
			int iYMin = clamp((((*itMesh)->getBoundingBox()->getYMin() - p0.y) * (Ny / (p1.y - p0.y))), 0, (Ny - 1));
			int iZMin = clamp((((*itMesh)->getBoundingBox()->getZMin() - p0.z) * (Nz / (p1.z - p0.z))), 0, (Nz - 1));
			int iXMax = clamp((((*itMesh)->getBoundingBox()->getXMax() - p0.x) * (Nx / (p1.x - p0.x))), 0, (Nx - 1));
			int iYMax = clamp((((*itMesh)->getBoundingBox()->getYMax() - p0.y) * (Ny / (p1.y - p0.y))), 0, (Ny - 1));
			int iZMax = clamp((((*itMesh)->getBoundingBox()->getZMax() - p0.z) * (Nz / (p1.z - p0.z))), 0, (Nz - 1));

			for (int iz = iZMin; iz <= iZMax; iz++)
			{
				for (int iy = iYMin; iy <= iYMax; iy++)
				{
					for (int ix = iXMin; ix <= iXMax; ix++)
					{
						index = ix + Nx * iy + Nx * Ny * iz;
						uniformGrid[index]->getMeshes()->push_back(ptr);
					}
				}
			}
		}
	}
}

Mesh *Grid::intersect(Ray ray, float &nearestT)
{
	Mesh *nearestMesh = nullptr;
	int ix, iy, iz;

	float t = BB->intersect(ray);

	if (t > 0.0f)
	{
		if ((ray.origin > p0) && (ray.origin < p1))
		{
			ix = clamp(((ray.origin.x - p0.x) * (Nx / (p1.x - p0.x))), 0, (Nx - 1));
			iy = clamp(((ray.origin.y - p0.y) * (Ny / (p1.y - p0.y))), 0, (Ny - 1));
			iz = clamp(((ray.origin.z - p0.z) * (Nz / (p1.z - p0.z))), 0, (Nz - 1));
		}
		else
		{
			ray.intersectionPoint(t);

			ix = clamp(((ray.point.x - p0.x) * (Nx / (p1.x - p0.x))), 0, (Nx - 1));
			iy = clamp(((ray.point.y - p0.y) * (Ny / (p1.y - p0.y))), 0, (Ny - 1));
			iz = clamp(((ray.point.z - p0.z) * (Nz / (p1.z - p0.z))), 0, (Nz - 1));

		}
	}

	//// ray parameter increments per cell in the x, y, and z directions
	//double dtx = (tx_max - tx_min) / Nx;
	//double dty = (ty_max - ty_min) / Ny;
	//double dtz = (tz_max - tz_min) / Nz;
	//double tx_next, ty_next, tz_next;
	//int ix_step, iy_step, iz_step;
	//int ix_stop, iy_stop, iz_stop;

	////A ray has direction (dx, dy). Possible cases for direction xx’:
	//if (ray.direction.x > 0) {
	//	tx_next = tx_min + (ix + 1) * dtx;
	//	ix_step = +1;
	//	ix_stop = Nx;
	//}
	//else {
	//	tx_next = tx_min + (Nx - ix) * dtx;
	//	ix_step = -1;
	//	ix_stop = -1;
	//}
	//if (ray.direction.x == 0.0) {
	//	tx_next = HUGE_VALUE; //WHY?
	//	ix_step = -1; // just to initialize. Never used
	//	ix_stop = -1;
	//}

	int index = ix + Nx * iy + Nx * Ny * iz;
	
	while (nearestMesh == nullptr)
	{
		std::vector<Mesh*> *meshes = uniformGrid[index]->getMeshes();
		for (std::vector<Mesh*>::iterator itMesh = meshes->begin(); itMesh != meshes->end(); ++itMesh)
		{
			t = (*itMesh)->getBoundingBox()->intersect(ray);
			if (t > 0.0f && t < nearestT)
			{
				t = (*itMesh)->intersect(ray);
				if (t > 0.0f && t < nearestT)
				{
					nearestT = t;
					nearestMesh = (*itMesh);
				}
			}
		}

		if (nearestMesh != nullptr)
		{
			return nearestMesh;
		}


	}

	return nearestMesh;
}
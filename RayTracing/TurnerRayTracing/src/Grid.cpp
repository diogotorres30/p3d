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

	BB = new BoundingBox(pMin.x, pMax.x, pMin.y, pMax.y, pMin.z, pMax.z);

	//the size except for the plane
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
		else
		{

		}
	}
}

Mesh *Grid::intersect(Ray &ray, float &nearestT)
{
	Mesh *nearestMesh = nullptr;
	int index;
	int ix = 0, iy = 0, iz = 0;

	float t = BB->intersect(ray);
	//Vector2 v = BB->intersectTwo(ray);

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
	else
	{
 		nearestT = HUGE_VALUE;
		return nullptr;
	}

	float tminx = (p0.x - ray.origin.x) / ray.direction.x;
	float tminy = (p0.y - ray.origin.y) / ray.direction.y;
	float tminz = (p0.z - ray.origin.z) / ray.direction.z;

	float tmaxx = (p1.x - ray.origin.x) / ray.direction.x;
	float tmaxy = (p1.y - ray.origin.y) / ray.direction.y;
	float tmaxz = (p1.z - ray.origin.z) / ray.direction.z;


	if(tminx > tmaxx)
		std::swap(tminx, tmaxx);
	if (tminy > tmaxy)
		std::swap(tminy, tmaxy);
	if (tminz > tmaxz)
		std::swap(tminz, tmaxz);


	// ray parameter increments per cell in the x, y, and z directions
	float dtx = (tmaxx - tminx) / Nx;
	float dty = (tmaxy - tminy) / Ny;
	float dtz = (tmaxz - tminz) / Nz;
	float tx_next, ty_next, tz_next;
	int ix_step, iy_step, iz_step;
	int ix_stop, iy_stop, iz_stop;


	//A ray has direction (dx, dy). Possible cases for direction xx’:
	if (ray.direction.x > 0.0f) {
		tx_next = tminx + (ix + 1) * dtx;
		ix_step = 1;
		ix_stop = Nx;
	}
	else {
		tx_next = tminx + (Nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}
	if (ray.direction.x == 0.0f) {
		tx_next = HUGE_VALUE; //WHY?
		ix_step = -1; // just to initialize. Never used
		ix_stop = -1;
	}

	//A ray has direction (dx, dy). Possible cases for direction xx’:
	if (ray.direction.y > 0.0f) {
		ty_next = tminy + (iy + 1) * dty;
		iy_step = 1;
		iy_stop = Ny;
	}
	else {
		ty_next = tminy + (Ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}
	if (ray.direction.y == 0.0f) {
		ty_next = HUGE_VALUE; //WHY?
		iy_step = -1; // just to initialize. Never used
		iy_stop = -1;
	}

	//A ray has direction (dx, dy). Possible cases for direction xx’:
	if (ray.direction.z > 0.0f) {
		tz_next = tminz + (iz + 1) * dtz;
		iz_step = 1;
		iz_stop = Nz;
	}
	else {
		tz_next = tminz + (Nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}
	if (ray.direction.z == 0.0f) {
		tz_next = HUGE_VALUE; //WHY?
		iz_step = -1; // just to initialize. Never used
		iz_stop = -1;
	}
	
	while (true)
	{
		nearestT = HUGE_VALUE;
		t = 0.0f;

		index = ix + (Nx * iy) + (Nx * Ny * iz);
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

		if (tx_next < ty_next && tx_next < tz_next)
		{
			if (nearestMesh != nullptr && nearestT < tx_next)
			{
				return nearestMesh;
			}

			tx_next += dtx;
			ix += ix_step;

			if (ix == ix_stop)
			{
				nearestT = HUGE_VALUE;
				return nullptr;
			}
				
		}
		else
		{
			if (ty_next < tz_next)
			{
				if (nearestMesh != nullptr && nearestT < ty_next)
				{
					return nearestMesh;
				}

				ty_next += dty;
				iy += iy_step;

				if (iy == iy_stop)
				{
					nearestT = HUGE_VALUE;
					return nullptr;
				}

			}
			else
			{
				if (nearestMesh != nullptr && nearestT < tz_next)
				{
					return nearestMesh;
				}

				tz_next += dtz;
				iz += iz_step;

				if (iz == iz_stop)
				{
					nearestT = HUGE_VALUE;
					return nullptr;
				}
			}
		}
	}

}
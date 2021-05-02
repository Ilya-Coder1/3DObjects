#pragma once
#include "Graphics.h"
#include <vector>
#include <string>
#include <list>
#include <array>

using namespace std;

class Object3D
{
private:
	struct Coords
	{
		double x;
		double y;
		double z;
	};
	vector<vector<Coords>> m_polygons{};

	bool isSorted = false;

	int partition(int p, int r);
	void quickSortImpl(int p, int r);

public:
	Object3D(string name);

	Object3D& rotate(double byX, double byY, double byZ);

	Object3D& scale(int ratio);

	Object3D& move(double byX, double byY);

	double calcMedZ(int index);

	void quickSort();

	Object3D& draw();
};


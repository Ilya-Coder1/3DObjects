#include "Object3D.h"

Object3D::Object3D(string name)
{
	ifstream file(name);
	string str;
	Coords crd;
	vector<Coords> coords;
	vector<Coords> c;
	string index;

	while (!file.eof())
	{
		file >> str;
		if (str == "v")
		{
			file >> crd.x >> crd.y >> crd.z;
			coords.push_back(crd);
		}

		if (str == "f")
		{
			c.clear();
			getline(file, str, '\n');
			str.erase(0, 1);
			while (!str.empty())
			{
				if (str.find(' ') != -1)
				{
					index = str.substr(0, str.find(' '));
					str.erase(0, str.find(' ') + 1);
					if (index.find('/') != -1)
						index.erase(index.find('/'));
					c.push_back(coords[stoi(index) - 1]);
				}
				else
				{
					if (str.find('/') != -1)
						str.erase(str.find('/'));
					c.push_back(coords[stoi(str) - 1]);
					str.clear();
				}
			}
			m_polygons.push_back(c);
		}
	}
	file.close();
}

Object3D& Object3D::rotate(double byX, double byY, double byZ)
{
	byX *= (3.1415f / 180);
	byY *= (3.1415f / 180);
	byZ *= (3.1415f / 180);

	double mX[4][4]{
	{ 1, 0, 0, 0 },
	{ 0, cos(byX), -sin(byX), 0 },
	{ 0, sin(byX), cos(byX), 0 },
	{ 0, 0, 0, 1 }
	};

	double mY[4][4]{
		{ cos(byY), 0, -sin(byY), 0 },
		{ 0, 1, 0, 0 },
		{ sin(byY), 0, cos(byY), 0 },
		{ 0, 0, 0, 1 }
	};

	double mZ[4][4]{
		{ cos(byZ), sin(byZ), 0, 0 },
		{ -sin(byZ), cos(byZ), 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};

	double res;
	array<array<double, 4>, 4> resXY;

	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < 4; i++)
		{
			res = 0;
			for (int j = 0; j < 4; j++)
			{
				res += mX[k][j] * mY[j][i];
			}
			resXY[k][i] = res;
		}
	}

	res = 0;
	array<array<double, 4>, 4> resXYZ;

	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < 4; i++)
		{
			res = 0;
			for (int j = 0; j < 4; j++)
			{
				res += resXY[k][j] * mZ[j][i];
			}
			resXYZ[k][i] = res;
		}
	}

	array<double, 4> crd;
	res = 0;
	array<double, 4> resCrd;

	for (int i = 0; i < m_polygons.size(); i++)
	{
		for (int j = 0; j < m_polygons[i].size(); j++)
		{
			crd = { m_polygons[i][j].x, m_polygons[i][j].y, m_polygons[i][j].z, 1 };

			for (int k = 0; k < 4; k++)
			{
				res = 0;
				for (int h = 0; h < 4; h++)
				{
					res += crd[h] * resXYZ[k][h];
				}
				resCrd[k] = res;
			}
			resCrd[0] /= resCrd[3];
			resCrd[1] /= resCrd[3];
			resCrd[2] /= resCrd[3];

			m_polygons[i][j].x = resCrd[0];
			m_polygons[i][j].y = resCrd[1];
			m_polygons[i][j].z = resCrd[2];
		}
	}
	return *this;
}

Object3D& Object3D::scale(int ratio)
{
	for (auto iter = m_polygons.begin(); iter != m_polygons.end(); iter++)
	{
		for (int i = 0; i < iter->size(); i++)
		{
			(*iter)[i].x *= ratio;
			(*iter)[i].y *= ratio;
			(*iter)[i].z *= ratio;
		}
	}
	return *this;
}

Object3D& Object3D::move(double byX, double byY)
{
	for (auto iter = m_polygons.begin(); iter != m_polygons.end(); iter++)
	{
		for (auto iter1 = iter->begin(); iter1 != iter->end(); iter1++)
		{
			iter1->x += byX;
			iter1->y += byY;
		}
	}
	return *this;
}

double Object3D::calcMedZ(int index)
{
	double medZ = 0;
	for (auto iter = m_polygons[index].begin(); iter != m_polygons[index].end(); iter++)
	{
		medZ += iter->z;
	}
	medZ /= m_polygons[index].size();
	return medZ;
}

int Object3D::partition(int p, int r)
{
	double x = calcMedZ(r);
	int i = p - 1;
	int j;
	for (j = p; j < r; j++)
	{
		if (calcMedZ(j) <= x)
		{
			i++;
			swap(m_polygons[i], m_polygons[j]);
		}
	}
	swap(m_polygons[r], m_polygons[i + 1]);
	return i + 1;
}

void Object3D::quickSortImpl(int p, int r)
{
	int q;
	if (p < r)
	{
		q = partition(p, r);
		quickSortImpl(p, q - 1);
		quickSortImpl(q + 1, r);
	}
}

void Object3D::quickSort()
{
	if (!m_polygons.empty())
		quickSortImpl(0, m_polygons.size() - 1);
	isSorted = true;
}

Object3D& Object3D::draw()
{
	if (!isSorted)
	{
		quickSort();
	}

	vector<POINT> temp;
	POINT pt;
	for (int i = 0; i < m_polygons.size(); i++)
	{
		temp.clear();
		for (int j = 0; j < m_polygons[i].size(); j++)
		{
			pt.x = (LONG)(m_polygons[i][j].x + 0.5f);
			pt.y = Graphics::GetHeg() - (LONG)(m_polygons[i][j].y + 0.5f);
			temp.push_back(pt);
		}
		Polygon(Graphics::dc, &temp[0], temp.size());
	}
	return *this;
}
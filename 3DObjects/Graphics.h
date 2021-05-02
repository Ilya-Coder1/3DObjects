#pragma once

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

extern HWND hWnd;

class Graphics
{
private:
	static HPEN hPen;
	static HBRUSH hBrush;
	static int wid, heg;
public:
	static HDC dc;
	static void Set_pen(COLORREF, int);
	static void Set_brush(COLORREF, bool);
	static void Line(int, int, int, int);
	static void Point(int, int, COLORREF);
	static void Osi(int, int);
	static int GetWid() { return wid; }
	static int GetHeg() { return heg; }

	static void InitGraphics(HWND);
	~Graphics();
};

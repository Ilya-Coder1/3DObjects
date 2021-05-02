#include <iostream>
#include "Object3D.h"
#include "Graphics.h"

int main()
{
	::SetConsoleTitle("Graphics");
	::system("mode con cols=200 lines=60");
	HWND hWnd = ::FindWindow(NULL, "Graphics");
	Sleep(100);
	Graphics::InitGraphics(hWnd);

	Graphics::Set_pen(RGB(255, 0, 0), 2);
	Graphics::Set_brush(RGB(0, 0, 0), false);

	Object3D lego_man("LEGO_Man.obj");
	lego_man.rotate(30, 30, 0).scale(80).move(200, 200);

	//Object3D skull("12140_Skull_v3_L2.obj");
	//skull.rotate(100, 180, 180).scale(30).move(1200, 100);
	//skull.draw();

	while (true)
	{
		lego_man.draw().move(20, 0);
		system("cls");
	}

}

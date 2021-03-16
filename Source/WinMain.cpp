#include "Application.h"


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow) 
{
	try {
		Application app;
		return static_cast<int>(app.StartGameLoop());
	}
	catch (int e)
	{
		return e;
	}
}
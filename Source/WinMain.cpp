#include "Application.h"


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow) 
{
	std::shared_ptr<Application> pApp = Application::CreateObject();
	if (!pApp) return -1;

	uint64_t returnCode = pApp->StartGameLoop();
	pApp->DestroyObject();

	return static_cast<int32_t>(returnCode);
}
#include <Windows.h>
#include "Config.h"
#include "Engine.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) 
{
	Cfg::ParseConfig();
	Config cfg = Cfg::GetCfg();
	Engine eng;
	eng.Initialize(hInstance, "ShaderJam", "ShaderJamWC", cfg.width, cfg.height);
	while (eng.ProcessMessage()) {
		eng.Update();
	}
	return 0;
}
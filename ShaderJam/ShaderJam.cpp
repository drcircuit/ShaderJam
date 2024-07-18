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
	Engine::ShaderFiles shaders;
	shaders.vertexShaderFile = StringConverter::StringToWide(cfg.vertexShader);
	shaders.effectShaderFile = StringConverter::StringToWide(cfg.effectShader);
	shaders.postShaderFile = StringConverter::StringToWide(cfg.postShader);
	
	if(eng.Initialize(hInstance, "ShaderJam", "ShaderJamWC", cfg.width, cfg.height, shaders))
	while (eng.ProcessMessage()) {
		eng.Update();
		eng.Draw();
	}
	return 0;
}
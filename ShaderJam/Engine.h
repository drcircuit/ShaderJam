#pragma once
#include "WindowContainer.h"
class Engine:WindowContainer {
	
public:
	struct ShaderFiles {
		std::wstring vertexShaderFile;
		std::wstring effectShaderFile;
		std::wstring postShaderFile;
	};
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height, ShaderFiles shaders);
	bool ProcessMessage();
	void Update();
	void Draw();
private:
	bool recompileShaders = false;
	ShaderFiles shaders;
};
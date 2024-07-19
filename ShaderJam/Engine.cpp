#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height, ShaderFiles shaders) {
	this->shaders = shaders;
	if (!this->rWindow.Initialize(this, hInstance, window_title, window_class, width, height)) {
		return false;	
	};
	if (!gfx.Initialize(this->rWindow.GetHWND(), width, height, shaders.vertexShaderFile, shaders.effectShaderFile, shaders.postShaderFile)) {
		return false;
	}
}

bool Engine::ProcessMessage() {
	return this->rWindow.ProcessMessage();
}
void Engine::Update() {

	while (!keyboard.KeyBufferIsEmpty()){
		KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
		if (kbe.IsPress()) {
			switch (keycode) {
			case VK_ESCAPE:
				exit(0);
				break;
			
			case VK_F5:
				recompileShaders = true;
				break;
			}
		}
	}
	while (!mouse.EventBufferIsEmpty()) {
		mouse.ReadEvent();
	}
	if (keyboard.KeyIsPressed(VK_ESCAPE)) {
		PostMessage(this->rWindow.GetHWND(), WM_DESTROY, NULL, NULL);
	}
	if (recompileShaders) {
		gfx.Recompile(this->shaders.vertexShaderFile, this->shaders.effectShaderFile, this->shaders.postShaderFile);
		recompileShaders = false;
	}
}
void Engine::Draw() {
	gfx.Render();
}
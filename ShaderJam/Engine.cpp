#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height, ShaderFiles shaders) {
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
	while (!keyboard.CharBufferIsEmpty()) {
		unsigned char ch = keyboard.ReadChar();
		
	}
	while (!keyboard.KeyBufferIsEmpty()){
		KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
	}
	while (!mouse.EventBufferIsEmpty()) {
		MouseEvent me = mouse.ReadEvent();
		if (me.GetType() == MouseEvent::EventType::RAW_MOVE) {
			std::string msg = "RAW: ";
			msg += "X: " + std::to_string(me.GetX());
			msg += ", Y: " + std::to_string(me.GetY())+"\n";
			//OutputDebugStringA(msg.c_str());
		}
	}
}
void Engine::Draw() {
	gfx.Render();
}
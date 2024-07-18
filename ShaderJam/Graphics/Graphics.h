#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
class Graphics {
public:
	bool Initialize(HWND hwnd, int width, int height, std::wstring vertexShaderFile, std::wstring effectShaderFile, std::wstring postShaderFile);
	void Render();
private:
	bool InitializeDirectX(HWND hwnd, int width, int height);
	bool InitializeShaders(std::wstring vertexShaderFile, std::wstring effectShaderFile, std::wstring postShaderFile);
	// Yay smart pointers!
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	VertexShader vertexShader;
	PixelShader effectShader;
	PixelShader postShader;
};
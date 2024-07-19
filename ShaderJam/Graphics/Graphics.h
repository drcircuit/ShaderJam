#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"
#include "..\\Timer.h"

class Graphics {
public:
	bool Initialize(HWND hwnd, int width, int height, std::wstring vertexShaderFile, std::wstring effectShaderFile, std::wstring postShaderFile);
	bool Recompile(std::wstring vertexShaderFile, std::wstring effectShaderFile, std::wstring postShaderFile);
	void Render();
private:
	bool InitializeDirectX(HWND hwnd, int width, int height);
	bool InitializeShaders(std::wstring vertexShaderFile, std::wstring effectShaderFile, std::wstring postShaderFile);
	bool InitializeScene(int width, int height);
	bool InitializeEffectTexture(int width, int height);
	// Yay smart pointers!
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> effectRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> effectShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> effectTexture;
	VertexShader vertexShader;
	PixelShader effectShader;
	PixelShader postShader;
	Timer timer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
};
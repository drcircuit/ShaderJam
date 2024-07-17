#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")

class Graphics {
public:
	bool Initialize(HWND hwnd, int width, int height);
private:
	bool InitializeDirectX(HWND hwnd, int width, int height);

	// Yay smart pointers!
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
};
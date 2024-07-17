#include "Graphics.h"
bool Graphics::Initialize(HWND hwnd, int width, int height) {
	if (!InitializeDirectX(hwnd, width, height))
		return false;
	return true;
}

void Graphics::Render()
{
	float color[] = { 0.0f, 0.5f, 0.8f, 1.0f };
	this->context->ClearRenderTargetView(this->renderTargetView.Get(), color);
	this->swapchain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd, int width, int height) {
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();
	// If there are no adapters, throw an error
	if (adapters.size() < 1) {
		ErrorLogger::Log("No DXGI Adapters found.");
		return false;
	}
	AdapterData adapter = AdapterReader::GetBestAdapter();
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;

	scd.OutputWindow = hwnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		adapter.pAdapter, // The adapter we are using (the best one)
		D3D_DRIVER_TYPE_UNKNOWN, // We are not specifying the driver type, so we are letting DirectX choose
		NULL, // We are not using a software driver, so we are passing NULL
		NULL, // We are not using any flags, so we are passing NULL
		NULL, // We are not specifying the feature levels, so we are passing NULL
		0, // We are not specifying the number of feature levels, so we are passing NULL
		D3D11_SDK_VERSION, // The SDK version we are using
		&scd, // The swapchain we are creating
		this->swapchain.GetAddressOf(), // The device we are creating
		this->device.GetAddressOf(), // The device context we are creating
		NULL,
		this->context.GetAddressOf() // The context we are creating
	);
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to create device and swapchain.");
		return false;
	}
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "GetBuffer failed.");
		return false;
	}
	hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to create render target view.");
		return false;
	}
	this->context->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), NULL);
	return true;
}
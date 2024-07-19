#include "Graphics.h"
bool Graphics::Initialize(HWND hwnd, int width, int height, std::wstring vertexShaderFile, std::wstring effectShaderFile, std::wstring postShaderFile) {
	if (!InitializeDirectX(hwnd, width, height))
		return false;
	if (!InitializeEffectTexture(width, height))
		return false;
	if(!InitializeShaders(vertexShaderFile, effectShaderFile, postShaderFile))
		return false;
	if(!InitializeScene(width, height))
		return false;
	timer.Initialize();
	return true;
}

bool Graphics::Recompile(std::wstring vertexShaderFile, std::wstring effectShaderFile, std::wstring postShaderFile)
{
	if(!InitializeShaders(vertexShaderFile, effectShaderFile, postShaderFile))
		return false;
	return true;
}

void Graphics::Render()
{
	float time = timer.Ellapsed();
	float color[] = { 0.0f, 0.5f, 0.8f, 1.0f };

	// First pass: Render to offscreen texture
	this->context->OMSetRenderTargets(1, this->effectRenderTargetView.GetAddressOf(), nullptr);
	this->context->ClearRenderTargetView(this->effectRenderTargetView.Get(), color);
	this->effectShader.SetTime(time);
	PixelShader::EffectData* data = this->effectShader.GetDataPointer();
	ID3D11Buffer* buffer = this->effectShader.GetDataBuffer();
	this->context->UpdateSubresource(buffer, 0, NULL, data, 0, 0);
	this->context->PSSetConstantBuffers(0, 1, &buffer);
	// Set shaders and draw
	this->context->IASetInputLayout(this->vertexShader.GetInputLayout());
	this->context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	this->context->VSSetShader(this->vertexShader.GetShader(), NULL, 0);
	this->context->PSSetShader(this->effectShader.GetShader(), NULL, 0);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	this->context->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), &stride, &offset);
	this->context->Draw(4, 0);

	// Second pass: Render to the back buffer
	this->context->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), nullptr);
	this->context->ClearRenderTargetView(this->renderTargetView.Get(), color);


	this->context->PSSetShaderResources(0, 1, this->effectShaderResourceView.GetAddressOf());
	this->postShader.SetTime(time);
	buffer = this->postShader.GetDataBuffer();
	this->context->UpdateSubresource(buffer, 0, NULL, data, 0, 0);
	this->context->PSSetConstantBuffers(0, 1, &buffer);

	this->context->PSSetShader(this->postShader.GetShader(), NULL, 0);
	this->context->Draw(4, 0); // Assuming same vertex setup for full screen quad

	this->swapchain->Present(1, 0);
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

	UINT numLevels = 0;
	hr = this->device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 1, &numLevels);

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

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;

	this->context->RSSetViewports(1, &viewport);
	return true;
}

bool Graphics::InitializeShaders(std::wstring vertexShaderFile, std::wstring effectShaderFile, std::wstring postShaderFile)
{
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{
			"POSITION", // The name of the element
			0, // The index of the element
			DXGI_FORMAT_R32G32_FLOAT, // The format of the element
			0, // The input slot
			0, // The byte offset
			D3D11_INPUT_PER_VERTEX_DATA, // The input slot class
			0 // The instance data step rate
		}, 
			{
			"TEXCOORD", // The name of the element
			0, // The index of the element
			DXGI_FORMAT_R32G32_FLOAT, // The format of the element
			0, // The input slot
			D3D11_APPEND_ALIGNED_ELEMENT, // The byte offset
			D3D11_INPUT_PER_VERTEX_DATA, // The input slot class
			0 // The instance data step rate
		},
		// supply IRESOLUTION
		{
			"IRESOLUTION", // The name of the element
			0, // The index of the element
			DXGI_FORMAT_R32G32_FLOAT, // The format of the element
			0, // The input slot
			D3D11_APPEND_ALIGNED_ELEMENT, // The byte offset
			D3D11_INPUT_PER_VERTEX_DATA, // The input slot class
			0 // The instance data step rate
}
	};
	if(!this->vertexShader.Initialize(this->device,vertexShaderFile, layout, ARRAYSIZE(layout)))
		return false;
	if(!this->effectShader.Initialize(this->device, this->context, effectShaderFile))
		return false;
	if(!this->postShader.Initialize(this->device, this->context, postShaderFile))
		return false;
	return true;
}

bool Graphics::InitializeScene(int width, int height)
{
	//full screen quad
	float w = static_cast<float>(width);
	float h = static_cast<float>(height);
	Vertex quad[] = {
		Vertex(-1.0f, -1.0f, 0.0f, 0.0f,w,h),
		Vertex(-1.0f, 1.0f, 0.0f, h,w,h),
		Vertex(1.0f, -1.0f, w, 0.f,w,h),
		Vertex(1.0f, 1.0f, w, h,w,h)
	};
	D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(quad);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(D3D11_SUBRESOURCE_DATA));
	vertexData.pSysMem = quad;

	HRESULT hr = this->device->CreateBuffer(&vertexBufferDesc, &vertexData, this->vertexBuffer.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to create vertex buffer.");
		return false;
	}

	return true;
}

bool Graphics::InitializeEffectTexture(int width, int height)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT hr = this->device->CreateTexture2D(&textureDesc, NULL, this->effectTexture.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to create effect texture.");
		return false;
	}
	hr = this->device->CreateRenderTargetView(this->effectTexture.Get(), NULL, this->effectRenderTargetView.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to create effect render target view.");
		return false;
	}
	hr = this->device->CreateShaderResourceView(this->effectTexture.Get(), NULL, this->effectShaderResourceView.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to create effect shader resource view.");
		return false;
	}
	return true;
}

#include "Shaders.h"


bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements) {
	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), shader_buffer.GetAddressOf());
	ID3D10Blob* error_buffer = nullptr;
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, L"Failed to load shader: " + shaderpath);
		return false;
	}
	// if extension is .cso, use CreateVertexShader, if .hlsl, use CompileFromFile
	if (shaderpath.substr(shaderpath.size() - 4, 4) == L".cso")
	{
		hr = device->CreateVertexShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), NULL, shader.GetAddressOf());
		if (FAILED(hr)) {
			std::wstring errorMsg = L"Failed to compile shader: " + shaderpath + L"\n";
			if (error_buffer) {
				errorMsg += StringConverter::StringToWide(static_cast<char*>(error_buffer->GetBufferPointer()));
				error_buffer->Release();
			}
			ErrorLogger::Log(hr, errorMsg);
			return false;
		}
	}
	if(shaderpath.substr(shaderpath.size() - 5, 5) == L".hlsl")
	{
		hr = D3DCompileFromFile(shaderpath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, shader_buffer.GetAddressOf(), &error_buffer);
		if (FAILED(hr)) {
			std::wstring errorMsg = L"Failed to compile shader: " + shaderpath + L"\n";
			if (error_buffer) {
				errorMsg += StringConverter::StringToWide(static_cast<char*>(error_buffer->GetBufferPointer()));
				error_buffer->Release();
			}
			ErrorLogger::Log(hr, errorMsg);
			return false;
		}
		hr = device->CreateVertexShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), NULL, shader.GetAddressOf());
		if (FAILED(hr)) {
			ErrorLogger::Log(hr, L"Failed to create vertex shader: " + shaderpath);
			return false;
		}
		hr = device->CreateInputLayout(
			layout, // The layout of the input
			numElements, // The number of elements in the layout
			this->shader_buffer->GetBufferPointer(), // The buffer containing the vertex shader
			this->shader_buffer->GetBufferSize(), // The size of the buffer
			this->inputLayout.GetAddressOf() // The input layout
		);
	}
	else
	{
		ErrorLogger::Log(hr, L"Unknown shader file extension: " + shaderpath);
		return false;
	}
	
	return true;
}

ID3D11VertexShader* VertexShader::GetShader() {
	return this->shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer() {
	return this->shader_buffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return this->inputLayout.Get();
}

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath) {
	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), shader_buffer.GetAddressOf());
	ID3D10Blob* error_buffer = nullptr;
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, L"Failed to load shader: " + shaderpath);
		return false;
	}
	// if extension is .cso, use CreateVertexShader, if .hlsl, use CompileFromFile
	if (shaderpath.substr(shaderpath.size() - 4, 4) == L".cso")
	{
		hr = device->CreatePixelShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), NULL, shader.GetAddressOf());
		if (FAILED(hr)) {
			std::wstring errorMsg = L"Failed to compile shader: " + shaderpath + L"\n";
			if (error_buffer) {
				errorMsg += StringConverter::StringToWide(static_cast<char*>(error_buffer->GetBufferPointer()));
				error_buffer->Release();
			}
			ErrorLogger::Log(hr, errorMsg);
			return false;
		}
	}if (shaderpath.substr(shaderpath.size() - 5, 5) == L".hlsl")
	{
		hr = D3DCompileFromFile(shaderpath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, shader_buffer.GetAddressOf(), &error_buffer);
		if (FAILED(hr)) {
			std::wstring errorMsg = L"Failed to compile shader: " + shaderpath + L"\n";
			if (error_buffer) {
				errorMsg += StringConverter::StringToWide(static_cast<char*>(error_buffer->GetBufferPointer()));
				error_buffer->Release();
			}
			ErrorLogger::Log(hr, errorMsg);
			return false;
		}
		hr = device->CreatePixelShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), NULL, shader.GetAddressOf());
		if (FAILED(hr)) {
			ErrorLogger::Log(hr, L"Failed to create vertex shader: " + shaderpath);
			return false;
		}
	}
	else
	{
		ErrorLogger::Log(hr, L"Unknown shader file extension: " + shaderpath);
		return false;
	}
	return true;
}

ID3D11PixelShader* PixelShader::GetShader() {
	return this->shader.Get();
}

ID3D10Blob* PixelShader::GetBuffer() {
	return this->shader_buffer.Get();
}
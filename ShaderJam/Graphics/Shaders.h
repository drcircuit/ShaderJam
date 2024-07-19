#pragma once
#include "..\\ErrorLogger.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#pragma comment(lib, "D3DCompiler.lib")
class VertexShader {
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements);
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

};

class PixelShader {
public:
	struct EffectData {
		float time;
		float Padding[3];
	};
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>&context, std::wstring shaderpath);
	ID3D11PixelShader* GetShader();
	ID3D11Buffer* GetDataBuffer();
	ID3D10Blob* GetBuffer();
	EffectData* GetDataPointer();
	void SetTextureResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);
	void SetTime(float time);
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> dataBuffer;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	EffectData data;
};;
#include "AdapterReader.h"

std::vector<AdapterData> AdapterReader::adapters;
std::vector<AdapterData> AdapterReader::GetAdapters() {
	adapters.clear();
	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;
	UINT i = 0;
	
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to create DXGIFactory for enumerating adapters.");
		exit(-1);
	}
	IDXGIAdapter* pAdapter;
	while (SUCCEEDED(pFactory->EnumAdapters(i, &pAdapter))) {
		adapters.push_back(AdapterData(pAdapter));
		i += 1;
	}
	return adapters;
}
AdapterData AdapterReader::GetBestAdapter()
{
	AdapterData adapter = adapters.at(0);
	for (int i = 0; i < adapters.size(); i++)
	{
		if (adapters[i].description.DedicatedVideoMemory > adapter.description.DedicatedVideoMemory)
		{
			adapter = adapters[i];
		}
	}
	return adapter;
}
AdapterData::AdapterData(IDXGIAdapter* pAdapter)
{
	this->pAdapter = pAdapter;
	HRESULT hr = pAdapter->GetDesc(&this->description);
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to Get Description for IDXGIAdapter.");
	}
}
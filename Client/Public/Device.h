#pragma once

#include "Client_Defines.h"

using namespace Microsoft::WRL;

BEGIN(Client)

class Device
{
	DECLARE_SINGLETON(Device)

private:

	 Device();
	~Device() = default;

public:

	HRESULT Init();

public:

	void Flush();

private:

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

private:

	ID3D12Device* m_pDevice = nullptr;

private:

	_uint m_iRTV_DescriptorSize         = 0;
	_uint m_iDSV_DescriptorSize         = 0;
	_uint m_iCBV_SRV_UAV_DescriptorSize = 0;
	_uint m_iMSAAQuality				= 0;
	_uint m_iSwapChainBufferCount		= 2;
	_bool m_bMSAAState					= false;

private:

	DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

private:
	
	ComPtr<IDXGIFactory4>        m_pGiFactory = nullptr;
	ComPtr<IDXGISwapChain>       m_pSwapChain = nullptr;
	ComPtr<ID3D12DescriptorHeap> m_pRTVHeap   = nullptr;
	ComPtr<ID3D12DescriptorHeap> m_pDSVHeap   = nullptr;
	ComPtr<ID3D12DescriptorHeap> m_pCBVHeap   = nullptr;

private:

	std::vector<ComPtr<ID3D12Resource>> m_pSwapChainBuffer;

};

END
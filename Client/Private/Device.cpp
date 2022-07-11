#include "pch.h"
#include "Device.h"
#include "Fence.h"
#include "CmdQueue.h"
#include "RootSignature.h"

BEGIN(Client)

IMPLEMENT_SINGLETON(Device);

Device::Device()
{

}

HRESULT Device::Init()
{
	// DebugLayer 汲沥 ------------------------------------------------------------------------

#if defined(DEBUG) || defined(_DEBUG)

	ID3D12Debug* pDebugController;
	{
		IsFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugController)));

		pDebugController->EnableDebugLayer();
	}

#endif
	//
	//
	// 
	// GIFactory 积己 -------------------------------------------------------------------------

	IsFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_pGiFactory)));

	//
	// 
	// 
	// Device 积己 ----------------------------------------------------------------------------

	if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice))))
	{
		IDXGIAdapter* pWarpAdapter;
		{
			IsFailed(m_pGiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));
			IsFailed(D3D12CreateDevice(pWarpAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice)));
		}
	}

	//
	// 
	// 
	// Fence 积己 -----------------------------------------------------------------------------

	Fence::GetInstance()->Init(m_pDevice);

	//
	// 
	// 
	// DescriptorSize 汲沥 --------------------------------------------------------------------

	m_iRTV_DescriptorSize		  = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_iDSV_DescriptorSize		  = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_iCBV_SRV_UAV_DescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//
	// 
	// 
	// Quality Level 汲沥 ---------------------------------------------------------------------

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevelDesc;
	{
		QualityLevelDesc.Format			  = m_BackBufferFormat;
		QualityLevelDesc.SampleCount	  = 4;
		QualityLevelDesc.Flags			  = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		QualityLevelDesc.NumQualityLevels = 0;

		IsFailed(m_pDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &QualityLevelDesc, sizeof(QualityLevelDesc)));

		m_iMSAAQuality = QualityLevelDesc.NumQualityLevels;

		assert(m_iMSAAQuality > 0 && "Unexpceted MASS Quality Level.");
	}

	//
	//
	//
	// LogAdapter 汲沥 ------------------------------------------------------------------------

	LogAdapters();

	//
	//
	//
	// CmdQueue 积己 --------------------------------------------------------------------------

	CmdQueue::GetInstance()->Init(m_pDevice);

	//
	//
	//
	// SwapChain 积己 -------------------------------------------------------------------------

	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	{
		SwapChainDesc.BufferDesc.Width			         = g_Width;
		SwapChainDesc.BufferDesc.Height			         = g_Height;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator   = 60;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		SwapChainDesc.BufferDesc.Format					 = m_BackBufferFormat;
		SwapChainDesc.BufferDesc.ScanlineOrdering		 = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		SwapChainDesc.BufferDesc.Scaling				 = DXGI_MODE_SCALING_UNSPECIFIED;
		SwapChainDesc.SampleDesc.Count					 = m_bMSAAState ? 4 : 1;
		SwapChainDesc.SampleDesc.Quality				 = m_bMSAAState ? (m_iMSAAQuality - 1) : 0;
		SwapChainDesc.BufferUsage						 = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount						 = m_iSwapChainBufferCount;
		SwapChainDesc.OutputWindow						 = g_hWnd;
		SwapChainDesc.Windowed							 = true;
		SwapChainDesc.SwapEffect						 = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.Flags								 = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IsFailed(m_pGiFactory->CreateSwapChain(CmdQueue::GetInstance()->Get(), &SwapChainDesc, m_pSwapChain.GetAddressOf()));
	}

	//
	//
	//
	// DescriptorHeaps 积己 -------------------------------------------------------------------

	D3D12_DESCRIPTOR_HEAP_DESC RTVDesc;
	{
		RTVDesc.NumDescriptors = m_iSwapChainBufferCount;
		RTVDesc.Type		   = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		RTVDesc.Flags		   = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		RTVDesc.NodeMask	   = 0;

		IsFailed(m_pDevice->CreateDescriptorHeap(&RTVDesc, IID_PPV_ARGS(m_pRTVHeap.GetAddressOf())));
	}

	D3D12_DESCRIPTOR_HEAP_DESC DSVDesc;
	{
		DSVDesc.NumDescriptors = 1;
		DSVDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		DSVDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		DSVDesc.NodeMask       = 0;

		IsFailed(m_pDevice->CreateDescriptorHeap(&DSVDesc, IID_PPV_ARGS(m_pDSVHeap.GetAddressOf())));
	}

	D3D12_DESCRIPTOR_HEAP_DESC CBVDesc;
	{
		CBVDesc.NumDescriptors = 1024;
		CBVDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		CBVDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		CBVDesc.NodeMask       = 0;

		IsFailed(m_pDevice->CreateDescriptorHeap(&CBVDesc, IID_PPV_ARGS(m_pCBVHeap.GetAddressOf())));
	}

	//
	//
	//
	// RootSignature 积己

	RootSignature::GetInstance()->Init(m_pDevice);

	return S_OK;
}

void Device::Flush()
{
	CmdQueue::GetInstance()->Flush();
}

void Device::LogAdapters()
{
	UINT i = 0;
	IDXGIAdapter* adapter = nullptr;
	std::vector<IDXGIAdapter*> adapterList;
	while (m_pGiFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wstring text = L"***Adapter: ";
		text += desc.Description;
		text += L"\n";

		OutputDebugString(text.c_str());

		adapterList.push_back(adapter);

		++i;
	}

	for (size_t i = 0; i < adapterList.size(); ++i)
	{
		LogAdapterOutputs(adapterList[i]);

		adapterList[i]->Release();
		adapterList[i] = 0;
	}
}

void Device::LogAdapterOutputs(IDXGIAdapter* adapter)
{
	UINT i = 0;
	IDXGIOutput* output = nullptr;
	while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		std::wstring text = L"***Output: ";
		text += desc.DeviceName;
		text += L"\n";
		OutputDebugString(text.c_str());

		LogOutputDisplayModes(output, m_BackBufferFormat);

		output->Release();
		output = 0;

		++i;
	}
}

void Device::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;

	// Call with nullptr to get list count.
	output->GetDisplayModeList(format, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]);

	for (auto& x : modeList)
	{
		UINT n = x.RefreshRate.Numerator;
		UINT d = x.RefreshRate.Denominator;
		std::wstring text =
			L"Width = " + std::to_wstring(x.Width) + L" " +
			L"Height = " + std::to_wstring(x.Height) + L" " +
			L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
			L"\n";

		::OutputDebugString(text.c_str());
	}
}

END
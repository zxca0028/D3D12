#pragma once

#include "Client_Defines.h"

using namespace Microsoft::WRL;

BEGIN(Client)

class RootSignature
{
	DECLARE_SINGLETON(RootSignature)

private:

	 RootSignature();
	~RootSignature() = default;

public:

	HRESULT Init(ID3D12Device* pDevice);

private:

	ComPtr<ID3D12RootSignature> m_pRootSignature = nullptr;

};

END	
#pragma once

#include "Client_Defines.h"

using namespace Microsoft::WRL;

BEGIN(Client)

class Fence
{
	DECLARE_SINGLETON(Fence)

private:

	 Fence();
	~Fence() = default;

public:

	HRESULT Init(ID3D12Device* pDevice);
	void WaitForFenceValue();

	void IncreaseFenceValue()
	{
		++m_iCurrentFenceValue;
	}
	_uint GetFenceValue()
	{
		return m_iCurrentFenceValue;
	}
	ID3D12Fence* Get() const
	{
		return m_pFence.Get();
	}


private:

	ComPtr<ID3D12Fence> m_pFence = nullptr;

private:

	_uint m_iCurrentFenceValue = 0;

};

END
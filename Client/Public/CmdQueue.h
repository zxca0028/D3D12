#pragma once

#include "Client_Defines.h"

using namespace Microsoft::WRL;

BEGIN(Client)

class CmdQueue
{
	DECLARE_SINGLETON(CmdQueue)

private:

	 CmdQueue();
	~CmdQueue() = default;

public:

	HRESULT Init(ID3D12Device* pDevice);

public:

	void Flush();

public:

	ID3D12CommandQueue* Get() const
	{
		return m_pCmdQueue.Get();
	}

private:

	ComPtr<ID3D12CommandQueue>        m_pCmdQueue = nullptr;
	ComPtr<ID3D12CommandAllocator>    m_pCmdAlloc = nullptr;
	ComPtr<ID3D12GraphicsCommandList> m_pCmdList  = nullptr;

};

END
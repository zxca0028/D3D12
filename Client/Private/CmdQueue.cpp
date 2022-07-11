#include "pch.h"
#include "CmdQueue.h"
#include "Fence.h"

BEGIN(Client)

IMPLEMENT_SINGLETON(CmdQueue)

CmdQueue::CmdQueue()
{

}

HRESULT CmdQueue::Init(ID3D12Device* pDevice)
{
	D3D12_COMMAND_QUEUE_DESC CommandQueueDesc = {};
	{
		CommandQueueDesc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;
		CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

		IsFailed(pDevice->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(&m_pCmdQueue)));

		IsFailed(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCmdAlloc)));

		IsFailed(pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCmdAlloc.Get(), nullptr, IID_PPV_ARGS(m_pCmdList.GetAddressOf())));
	
		m_pCmdList->Close();
	}

	return S_OK;
}

void CmdQueue::Flush()
{
	Fence::GetInstance()->IncreaseFenceValue();
	m_pCmdQueue->Signal(Fence::GetInstance()->Get(), Fence::GetInstance()->GetFenceValue());
	Fence::GetInstance()->WaitForFenceValue();
}

END
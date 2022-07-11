#include "pch.h"
#include "Fence.h"

BEGIN(Client)

IMPLEMENT_SINGLETON(Fence)

Fence::Fence()
{

}

HRESULT Fence::Init(ID3D12Device* pDevice)
{
    IsFailed(pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence)));

    return S_OK;
}

void Fence::WaitForFenceValue()
{
    if (m_pFence->GetCompletedValue() < m_iCurrentFenceValue)
    {
        HANDLE EventHandle = CreateEventEx(nullptr, 0, 0, EVENT_ALL_ACCESS);

        IsFailed(m_pFence->SetEventOnCompletion(m_iCurrentFenceValue, EventHandle));

        WaitForSingleObject(EventHandle, INFINITE);
        CloseHandle(EventHandle);
    }
}

END
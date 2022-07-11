#include "pch.h"
#include "RootSignature.h"

BEGIN(Client)

IMPLEMENT_SINGLETON(RootSignature)

RootSignature::RootSignature()
{

}

HRESULT RootSignature::Init(ID3D12Device* pDevice)
{
    return S_OK;
}

END
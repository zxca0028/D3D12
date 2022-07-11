#include "pch.h"
#include "Timer.h"

BEGIN(Client)

Timer::Timer()
{
}

HRESULT Timer::Init()
{
    QueryPerformanceFrequency(&m_tCPUTick);

    QueryPerformanceCounter(&m_tBegin);
    QueryPerformanceCounter(&m_tEnd);

    return S_OK;
}

_float Timer::DeltaTime()
{
    QueryPerformanceCounter(&m_tEnd);

    m_fDeltaTime = _float(m_tEnd.QuadPart - m_tBegin.QuadPart) / m_tCPUTick.QuadPart;

    m_tBegin = m_tEnd;

    return m_fDeltaTime;
}

std::shared_ptr<Client::Timer> Timer::Create()
{
    std::shared_ptr<Client::Timer> pInstance = std::make_shared<Client::Timer>();

    IsNullptr(pInstance);

    IsFailed(pInstance->Init());

    return pInstance;
}

END
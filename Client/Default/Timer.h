#pragma once

#include "Client_Defines.h"

BEGIN(Client)

class Timer final
{
public:

	 Timer();
	~Timer() = default;

public:

	HRESULT Init();
	_float  DeltaTime();

private:

	_float		  m_fDeltaTime = 0.0f;
	LARGE_INTEGER m_tCPUTick   = {};
	LARGE_INTEGER m_tBegin     = {};
	LARGE_INTEGER m_tEnd       = {};

public:

	static std::shared_ptr<Client::Timer> Create();

};

END
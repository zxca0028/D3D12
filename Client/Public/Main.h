#pragma once

#include "Client_Defines.h"

BEGIN(Client)

class Main final
{
public:

	 Main();
	~Main() = default;

public:

	HRESULT Init();
	_int    Tick    (_float DeltaTime);
	_int    LateTick(_float DeltaTime);
	HRESULT Render();

public:

	static std::unique_ptr<Client::Main> Create();

};

END
#include "pch.h"
#include "Main.h"
#include "Device.h"

BEGIN(Client)

Main::Main()
{

}

HRESULT Main::Init()
{
	Device::GetInstance()->Init();

	return S_OK;
}

_int Main::Tick(_float DeltaTime)
{
	return _int();
}

_int Main::LateTick(_float DeltaTime)
{
	return _int();
}

HRESULT Main::Render()
{
	return S_OK;
}

std::unique_ptr<Client::Main> Main::Create()
{
	std::unique_ptr<Main> pInstance = std::make_unique<Main>();

	IsNullptr(pInstance);

	IsFailed(pInstance->Init());

	return pInstance;
}

END
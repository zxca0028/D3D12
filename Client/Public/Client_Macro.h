#pragma once

namespace Client
{
#define MSGBOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("Error"), MB_OK)

#define BEGIN(NAME) namespace NAME {
#define END }

#define DECLARE_SINGLETON(Class)					   \
public:												   \
													   \
	static Class* GetInstance()						   \
	{												   \
		if (nullptr == pInstance)					   \
		{											   \
			pInstance = new Class;				       \
		}											   \
													   \
		return pInstance;							   \
	}												   \
private:											   \
													   \
		static Class* pInstance;

#define IMPLEMENT_SINGLETON(Class)					   \
Class* Class::pInstance = nullptr;

}
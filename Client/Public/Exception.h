#pragma once

namespace Client
{
	class Exception final
	{
	public:

		Exception() = default;
		Exception(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber);

	public:

		std::wstring ToString() const;

	public:

		HRESULT ErrorCode = S_OK;
		std::wstring FunctionName;
		std::wstring FileName;
		int LineNumber = -1;

	};

	inline std::wstring AnsiToWString(const std::string& str)
	{
		WCHAR buffer[512];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
		return std::wstring(buffer);
	}

#ifndef IsFailed
#define IsFailed(x)                                                   \
{                                                                     \
    HRESULT hr__ = (x);                                               \
    std::wstring wfn = AnsiToWString(__FILE__);                       \
    if(FAILED(hr__)) { throw Exception(hr__, L#x, wfn, __LINE__); }   \
}
#endif

#ifndef IsNullptr
#define IsNullptr(x)						                               \
{											                               \
    std::wstring wfn = AnsiToWString(__FILE__);                            \
	if (nullptr == x) { throw Exception(E_POINTER, L#x, wfn, __LINE__); }  \
}
#endif

}
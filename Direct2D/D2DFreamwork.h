#pragma once

#include <d2d1.h>
#include <exception>
#include <sstream>
#include <wrl/client.h>

#pragma comment (lib, "d2d1.lib")

class com_exception : public std::exception
{
private:
	HRESULT result;

public:
	com_exception(HRESULT hr) : result{ hr } {};
	virtual const char* what() const override
	{
		static char buffer[128];

		sprintf_s(buffer, 128, "Fail with HRESULT of 0x%08X", result);
		return buffer;
	}
};

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw com_exception(hr);
	}
}

class D2DFreamwork
{
protected:
	Microsoft::WRL::ComPtr<ID2D1Factory> mspD2D1Factory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mspRenderTarget{};

public:
	virtual HRESULT Initialize(HWND hwnd);
	virtual void Release();
	virtual void Render();

	static void ShowErrorMsg(LPCWSTR msg, LPCWSTR title = L"Error");
};
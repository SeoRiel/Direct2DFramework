#include "D2DFreamwork.h"

HRESULT D2DFreamwork::Initialize(HWND hwnd)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, mspD2D1Factory.GetAddressOf());
	ThrowIfFailed(hr);

	RECT wr;
	GetClientRect(hwnd, &wr);
	hr = mspD2D1Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)), mspRenderTarget.GetAddressOf());
	ThrowIfFailed(hr);

	return S_OK;
}

void D2DFreamwork::Release()
{
}

void D2DFreamwork::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	// TODO : Draw

	mspRenderTarget->EndDraw();
}

void D2DFreamwork::ShowErrorMsg(LPCWSTR msg, LPCWSTR title)
{
	OutputDebugString(msg);
	MessageBox(nullptr, msg, title, MB_OK | MB_ICONEXCLAMATION);
}

#include <Windows.h>
#include "D2DFreamwork.h"

const wchar_t gClassName[] = L"MyWindowClass";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

D2DFreamwork myFramework;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;

	if (RegisterClassEx(&wc) == false)
	{
		D2DFreamwork::ShowErrorMsg(L"Failed To Register window class!");
		// MessageBox(nullptr, L"Failed To Register window class!", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	RECT wr = { 0, 0, 1024, 768 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

	HWND hwnd = CreateWindowEx(0, gClassName, L"Direct2D", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, 0, 0, hInstance, 0);

	if (hwnd == 0)
	{
		D2DFreamwork::ShowErrorMsg(L"Failed To Create window!");
		// MessageBox(nullptr, L"Failed To Create window!", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	try
	{
		ThrowIfFailed(myFramework.Initialize(hwnd));
	}
	catch (com_exception& e)
	{
		static wchar_t wstr[128]{};
		size_t length;

		mbstowcs_s(&length, wstr, e.what(), 128);

		D2DFreamwork::ShowErrorMsg(wstr);
	}

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	// 일단 무한 반복 : Game Loop
	MSG msg;
	while (true)
	{
		// 메세지가 있는지 살펴 봄
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// 메세지가 있으면 처리
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// 종료 메세지라면 반복 break;
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		// 메세지가 없을면
		else
		{
			// 게임의 그래픽 갱신
			myFramework.Render();
		}
	}

	// 4. Resource delete - close
	myFramework.Release();

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
		break;
	}

	return 0;
}
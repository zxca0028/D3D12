#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "Main.h"
#include "Timer.h"

#define MAX_LOADSTRING 100

HINSTANCE g_hInst;
HWND      g_hWnd;
WCHAR     szTitle[MAX_LOADSTRING];
WCHAR     szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;
    
    try
    {
        auto pMain  = Client::Main::Create();
        auto pTimer = Client::Timer::Create();

        float fTimeAcc = 0.0f;

        while (true)
        {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                if (WM_QUIT == msg.message)
                {
                    break;
                }
                if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }

            fTimeAcc += pTimer->DeltaTime();

            if (fTimeAcc > 1 / 60.0f)
            {
                pMain->Tick(pTimer->DeltaTime());
                pMain->LateTick(pTimer->DeltaTime());
                pMain->Render();

                fTimeAcc = 0.0f;
            }
        }
    }
    catch (Client::Exception& Exp)
    {
        MessageBox(nullptr, Exp.ToString().c_str(), TEXT("Exception Occured!"), MB_OK);

        return 0;
    }


    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName  = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    g_hInst = hInstance;

    RECT rcWindow = { 0, 0, g_Width, g_Height };
    AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, false);

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    g_hWnd = hWnd;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:

        switch (wParam)
        {
        case VK_ESCAPE:

            DestroyWindow(hWnd);
            break;
        }

        break;

    case WM_DESTROY:

        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<windowsx.h>
#include<commctrl.h>
#include<stdio.h>
#include"resource.h"

CONST CHAR g_sz_MY_WINDOW_CLASS[] = "Mouse";
INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateToolTipForRect(HWND hwndParent, CHAR* sz_message);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow) {

    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.style = 0;

    wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(hInstance, IDI_APPLICATION);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

    wc.hInstance = hInstance;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_sz_MY_WINDOW_CLASS;

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Класс незаригестрирован", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    HWND hwnd = CreateWindowEx(
        NULL,
        g_sz_MY_WINDOW_CLASS,
        g_sz_MY_WINDOW_CLASS,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == 0) {
        MessageBox(NULL, "Окно не было созданно", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);


    MSG msg;
    while (GetMessage(&msg, 0, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}


INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    INT mouse_X, mouse_Y;
    TRACKMOUSEEVENT tg;
    tg.cbSize = sizeof(tg);
    tg.dwFlags = TME_HOVER/*| TME_LEAVE*/;
    tg.hwndTrack = hwnd;
    tg.dwHoverTime = HOVER_DEFAULT;
    TrackMouseEvent(&tg);
    SendMessage(GetDlgItem(hwnd, IDC_TOOLTIP1), TTM_ACTIVATE, true, 0);
    switch (uMsg)
    {
    case WM_CREATE: {} break;
    case WM_COMMAND: {} break;
    case WM_MOUSEHOVER: {
        mouse_X = GET_X_LPARAM(lParam);
        mouse_Y = GET_Y_LPARAM(lParam);
        CONST INT SIZE = 256;
        CHAR sz_message[SIZE]{};
        sprintf(sz_message, "Координаты % d * %d.", mouse_X, mouse_Y);
        CreateToolTipForRect(hwnd, sz_message);
    }break;
    case WM_DESTROY: PostQuitMessage(0); break;
    case WM_CLOSE:
        if (MessageBox(hwnd, "Вы действительно хотите закрыть окно?", "Question", MB_YESNO | MB_ICONQUESTION) == IDYES) {
            DestroyWindow(hwnd);
        }               break;
    default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}


void CreateToolTipForRect(HWND hwndParent, CHAR* sz_message)
{
    HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
        WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        hwndParent, NULL, GetModuleHandle(NULL), NULL);
    SetWindowPos(hwndTT, HWND_TOPMOST, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    TOOLINFO ti = { 0 };
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_SUBCLASS;
    ti.hwnd = hwndParent;
    ti.hinst = GetModuleHandle(NULL);
    ti.lpszText = (LPSTR)sz_message;
    GetClientRect(hwndParent, &ti.rect);
    SendMessage(hwndTT, TTM_SETDELAYTIME, TTDT_RESHOW, 1500);
    SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
    //    DestroyWindow(hwndTT);
}
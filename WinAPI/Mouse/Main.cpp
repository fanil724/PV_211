#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<windowsx.h>
#include<commctrl.h>
#include<stdio.h>
#include"resource.h"

CONST CHAR g_sz_MY_WINDOW_CLASS[] = "Mouse";
INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


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


    /* INITCOMMONCONTROLSEX icc;
     InitCommonControlsEx(&icc);
     icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
     icc.dwICC = ICC_BAR_CLASSES;*/

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

    HWND tooltip_hwnd = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, "TooltipIM", WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        hwnd, (HMENU)IDC_TOOLTIP1, (HINSTANCE)GetModuleHandle(NULL), NULL);
    RECT rect;
    GetWindowRect(hwnd, &rect);
    SetWindowPos(tooltip_hwnd, HWND_TOPMOST,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    SendMessage(tooltip_hwnd, TTM_SETMAXTIPWIDTH, 0, 200);
    SendMessage(tooltip_hwnd, TTM_SETDELAYTIME, TTDT_AUTOPOP, 1500);

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}


INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    INT mouse_X, mouse_Y;
    TOOLINFO ti = { 0 };
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_SUBCLASS;
    ti.hwnd = hwnd;
    ti.hinst = (HINSTANCE)GetModuleHandle(NULL);
    ti.uId = 1;
    TRACKMOUSEEVENT tg;
    tg.cbSize = sizeof(tg);    tg.dwFlags = TME_HOVER/*| TME_LEAVE*/;    tg.hwndTrack = hwnd;    tg.dwHoverTime = HOVER_DEFAULT;
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
        RECT rect;
        GetWindowRect(hwnd, &rect);
        ti.lpszText = sz_message;
        ti.rect.left = rect.left;
        ti.rect.top = rect.top;
        ti.rect.right = rect.right;
        ti.rect.bottom = rect.bottom;
        ti.lpszText = sz_message;
        SendMessage(GetDlgItem(hwnd, IDC_TOOLTIP1), TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
        //TrackMouseEvent(&tg);
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
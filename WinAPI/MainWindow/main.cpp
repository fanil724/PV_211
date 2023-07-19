#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include"resource.h"
#include<stdio.h>

CONST CHAR g_sz_MY_WINDOW_CLASS[] = "MyFirstWindow";

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void SetSize(HWND hwnd, RECT rect);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow) {

    //1. ����������� ������ ����
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);      //������ ������ � ������( cb-Count Bytes)
    wc.cbClsExtra = 0;  // Class ExtraBytes �������������� ����� ������
    wc.cbWndExtra = 0;  //window Extra bytes �������������� ����� ����
    wc.style = 0;

    // wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_STAR));//������������ � ������ �����
    // wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_PALM));//������������� � ��������� ����
    wc.hIcon = (HICON)LoadImage(hInstance, "Palm.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
    wc.hIconSm = (HICON)LoadImage(hInstance, "Star.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
    //wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
    wc.hCursor = (HCURSOR)LoadImage(hInstance, "Protoss.cur", IMAGE_CURSOR, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

    wc.hInstance = hInstance;
    wc.lpfnWndProc = (WNDPROC)WndProc; //TODO: ������� ��������� ����
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_sz_MY_WINDOW_CLASS;

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "����� �����������������", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    //2. �������� ����

    HWND hwnd = CreateWindowEx(
        NULL,
        g_sz_MY_WINDOW_CLASS,//��� ������ ����
        g_sz_MY_WINDOW_CLASS,//��������� ����
        WS_OVERLAPPEDWINDOW, //����� ���� ��� �������� ���� ��������� ������ �����
        //WS_OVERLAPPEDWINDOW -���� �������� ������ ������� �������� � ���� �������� ����
        CW_USEDEFAULT, CW_USEDEFAULT,//��������� ������� ����
        CW_USEDEFAULT, CW_USEDEFAULT,//������ ������
        NULL,//parrent window
        NULL,//��� �������� ���� ����, ��� ��������� ���� �� ��� �������
        hInstance,
        NULL
    );
    if (hwnd == 0) {
        MessageBox(NULL, "���� �� ���� ��������", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);//������ ����� ����������� ����
    UpdateWindow(hwnd);//��������� ���������� ����
    //3. ������ ����� ���������
    MSG msg;
    while (GetMessage(&msg, 0, 0, 0) > 0) {
        TranslateMessage(&msg);//����������� ��������� ����������� ������ � ���������� ���������
        DispatchMessage(&msg);//���������� ��������� � ��������� ����. ��������� ����� �� GETMessage();
    }

    return msg.wParam;
}


INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg)
    {
    case WM_CREATE: {
        INT X = GetSystemMetrics(0) / 8;
        INT Y = GetSystemMetrics(1) / 8;
        SetWindowPos(hwnd, 0, X, Y, (GetSystemMetrics(0) / 4 * 3), (GetSystemMetrics(1) / 4 * 3), 0); ;//��������� ������� ����
        RECT rect;
        GetWindowRect(hwnd, &rect);
        SetSize(hwnd, rect);
    }
                  break;
    case WM_COMMAND: {} break;
    case WM_SIZE:
    case WM_MOVE: {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        SetSize(hwnd, rect);
    }                        break;
    case WM_DESTROY: PostQuitMessage(0); break;
    case WM_CLOSE:
        if (MessageBox(hwnd, "�� ������������� ������ ������� ����?", "Question", MB_YESNO | MB_ICONQUESTION) == IDYES) {
            DestroyWindow(hwnd);//������� ���������� ����
        }
        //��������� ����� ������� �������� ��������� WM_DESTROY
        break;
    default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}


void SetSize(HWND hwnd, RECT rect) {
    CONST INT SIZE = 256;
    INT horizontal = rect.left;
    INT vertical = rect.top;
    INT width = rect.bottom - rect.top;
    INT height = rect.right - rect.left;
    CHAR sz_message[SIZE]{};
    sprintf(sz_message, "%s (������� %d* %d, ������� %d* %d)", g_sz_MY_WINDOW_CLASS,
        horizontal, vertical, height, width);
    SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_message);
}
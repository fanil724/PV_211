#include<Windows.h>

CONST CHAR g_sz_MY_WINDOW_CLASS[] = "MeFirstWindow";

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow) {

    //1. ����������� ������ ����
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);      //������ ������ � ������( cb-Count Bytes)
    wc.cbClsExtra = 0;  // Class ExtraBytes �������������� ����� ������
    wc.cbWndExtra = 0;  //window Extra bytes �������������� ����� ����
    wc.style = 0;

    wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
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
    //3. ������ ����� ���������
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}


INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    {
    case WM_CREATE: {}
                  break;
    case WM_COMMAND: {}
                   break;
    case WM_DESTROY: PostQuitMessage(0); break;
    case WM_CLOSE: DestroyWindow(hwnd); break;
    default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
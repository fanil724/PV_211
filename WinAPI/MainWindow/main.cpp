#include<Windows.h>

CONST CHAR g_sz_MY_WINDOW_CLASS[] = "MeFirstWindow";

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow) {

    //1. Регистрация класса окна
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);      //размер класса в байтах( cb-Count Bytes)
    wc.cbClsExtra = 0;  // Class ExtraBytes дополнительные байты класса
    wc.cbWndExtra = 0;  //window Extra bytes дополнительные байты окна
    wc.style = 0;

    wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

    wc.hInstance = hInstance;
    wc.lpfnWndProc = (WNDPROC)WndProc; //TODO: указать процедуру окна
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_sz_MY_WINDOW_CLASS;

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Класс незаригестрирован", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    //2. Создание окна

    HWND hwnd = CreateWindowEx(
        NULL,
        g_sz_MY_WINDOW_CLASS,//имя класса окна
        g_sz_MY_WINDOW_CLASS,//заголовок окна
        WS_OVERLAPPEDWINDOW, //стиль окна для главного окна программы всегда будет
        //WS_OVERLAPPEDWINDOW -окно врехнего уровня которое включает в себя дочерние окна
        CW_USEDEFAULT, CW_USEDEFAULT,//начальная позиция окна
        CW_USEDEFAULT, CW_USEDEFAULT,//ширина высота
        NULL,//parrent window
        NULL,//для галвного окна меню, для дочернего окна ид его ресурса
        hInstance,
        NULL
    );
    if (hwnd == 0) {
        MessageBox(NULL, "Окно не было созданно", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }
    //3. Запуск цикла сообщений
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
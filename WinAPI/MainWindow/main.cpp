#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include"resource.h"
#include<stdio.h>

CONST CHAR g_sz_MY_WINDOW_CLASS[] = "Сalculator";
CONST INT g_i_BTN_SIZE = 50;     //g global, i int
CONST INT g_i_DISTANCE = 10;     //растояние между кнопками
CONST INT g_i_START_X = 10;      //отступ от начала окна
CONST INT g_i_START_Y = 10;      //отступ от начала окна
CONST INT g_i_DISPLAY_WIDHT = (g_i_BTN_SIZE + g_i_DISTANCE) * 5 - g_i_DISTANCE;
CONST INT g_i_DISPLAY_HEIGHT = 45;

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void SetSize(HWND hwnd, RECT rect);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow) {

    //1. Регистрация класса окна
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);      //размер класса в байтах( cb-Count Bytes)
    wc.cbClsExtra = 0;  // Class ExtraBytes дополнительные байты класса
    wc.cbWndExtra = 0;  //window Extra bytes дополнительные байты окна
    wc.style = 0;

    // wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_STAR));//отображается в панели задач
    // wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_PALM));//отоброажается в заголовке окна
    wc.hIcon = (HICON)LoadImage(hInstance, "Icon/Cal.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
    wc.hIconSm = (HICON)LoadImage(hInstance, "Icon/Cal.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
    wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW));
    // wc.hCursor = (HCURSOR)LoadImage(hInstance, "Protoss.cur", IMAGE_CURSOR, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
    wc.hbrBackground = (HBRUSH)COLOR_ACTIVEBORDER;

    wc.hInstance = hInstance;
    wc.lpfnWndProc = (WNDPROC)WndProc; //TODO: указать процедуру окна
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_sz_MY_WINDOW_CLASS;

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Класс незаригестрирован", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    //2. Создание окна
    INT window_width = g_i_START_X * 2 + g_i_DISPLAY_WIDHT + 16;
    INT window_height = g_i_START_Y * 2 + g_i_DISPLAY_HEIGHT + (g_i_BTN_SIZE + g_i_DISTANCE) * 4 + 37;

    HWND hwnd = CreateWindowEx(
        NULL,
        g_sz_MY_WINDOW_CLASS,//имя класса окна
        g_sz_MY_WINDOW_CLASS,//заголовок окна
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, //стиль окна для главного окна программы всегда будет
        //WS_OVERLAPPEDWINDOW -окно врехнего уровня которое включает в себя дочерние окна
        g_i_START_X, g_i_START_Y,//начальная позиция окна
        window_width, window_height,//ширина высота
        NULL,//parrent window
        NULL,//для галвного окна меню, для дочернего окна ид его ресурса
        hInstance,
        NULL
    );

    if (hwnd == 0) {
        MessageBox(NULL, "Окно не было созданно", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);//задает режим отображения окна
    UpdateWindow(hwnd);//выполняет прорисовку окна
    //3. Запуск цикла сообщений
    MSG msg;
    while (GetMessage(&msg, 0, 0, 0) > 0) {
        TranslateMessage(&msg);//транслирует сообщения виртуальных клавиш в символьные сообщения
        DispatchMessage(&msg);//отправляет сообщения в процедуре окна. Сообщения берет от GETMessage();
    }

    return msg.wParam;
}


INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg)
    {
    case WM_CREATE: {
        /* INT X = GetSystemMetrics(0) / 8;
         INT Y = GetSystemMetrics(1) / 8;*/
         //SetWindowPos(hwnd, 0, X, Y, (GetSystemMetrics(0) / 4 * 3), (GetSystemMetrics(1) / 4 * 3), 0); ;//установка размера окна
         //RECT rect;
         //GetWindowRect(hwnd, &rect);
         //SetSize(hwnd, rect);

        HWND hEdit = CreateWindowEx
        (
            NULL,
            "Edit",
            "0",
            WS_CHILD | WS_VISIBLE | ES_RIGHT | WS_BORDER,
            g_i_START_X, g_i_START_Y,
            g_i_DISPLAY_WIDHT, g_i_DISPLAY_HEIGHT,
            hwnd,
            (HMENU)IDC_EDIT,
            GetModuleHandle(NULL),
            NULL
        );
        CHAR sz_btn_name[] = "0";
        INT number = 1;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                sz_btn_name[0] = number + 48;
                CreateWindowEx(
                    NULL,
                    "Button",
                    sz_btn_name,
                    WS_CHILD | WS_VISIBLE | BS_ICON | BS_PUSHBUTTON,
                    g_i_START_X + (g_i_BTN_SIZE + g_i_DISTANCE) * j, g_i_START_Y + g_i_DISTANCE + (g_i_BTN_SIZE + g_i_DISTANCE) * (2 - i) + g_i_DISPLAY_HEIGHT,
                    g_i_BTN_SIZE, g_i_BTN_SIZE,
                    hwnd,
                    (HMENU)(1000 + number),
                    GetModuleHandle(NULL),
                    NULL
                );
                number++;
            }
        }
        CreateWindowEx(NULL, "Button", "0",
            WS_CHILD | WS_VISIBLE | BS_ICON | BS_PUSHBUTTON,
            g_i_START_X, g_i_DISTANCE + g_i_START_Y + (g_i_BTN_SIZE + g_i_DISTANCE) * 3 + g_i_DISPLAY_HEIGHT,
            g_i_BTN_SIZE * 2 + g_i_DISTANCE, g_i_BTN_SIZE,
            hwnd, (HMENU)IDC_BUTTON_0, GetModuleHandle(NULL), NULL
        );
        CreateWindowEx(NULL, "BUTTON", ".",
            WS_CHILD | WS_VISIBLE | BS_ICON | BS_PUSHBUTTON,
            g_i_START_X + (g_i_BTN_SIZE + g_i_DISTANCE) * 2, g_i_DISTANCE + g_i_START_Y + (g_i_BTN_SIZE + g_i_DISTANCE) * 3 + g_i_DISPLAY_HEIGHT,
            g_i_BTN_SIZE, g_i_BTN_SIZE,
            hwnd, (HMENU)IDC_BUTTON_POINT, GetModuleHandle(NULL), NULL
        );
        CreateWindowEx(NULL, "BUTTON", "/",
            WS_CHILD | WS_VISIBLE | BS_ICON | BS_PUSHBUTTON,
            g_i_START_X + (g_i_BTN_SIZE + g_i_DISTANCE) * 3, g_i_DISTANCE + g_i_START_Y + g_i_DISPLAY_HEIGHT,
            g_i_BTN_SIZE, g_i_BTN_SIZE,
            hwnd, (HMENU)IDC_BUTTON_SLASH, GetModuleHandle(NULL), NULL
        );
        CreateWindowEx(NULL, "BUTTON", "*",
            WS_CHILD | WS_VISIBLE | BS_ICON | BS_PUSHBUTTON,
            g_i_START_X + (g_i_BTN_SIZE + g_i_DISTANCE) * 3, g_i_DISTANCE + g_i_START_Y + (g_i_BTN_SIZE + g_i_DISTANCE) + g_i_DISPLAY_HEIGHT,
            g_i_BTN_SIZE, g_i_BTN_SIZE,
            hwnd, (HMENU)IDC_BUTTON_ASTER, GetModuleHandle(NULL), NULL
        );
        CreateWindowEx(NULL, "BUTTON", "-",
            WS_CHILD | WS_VISIBLE | BS_ICON | BS_PUSHBUTTON,
            g_i_START_X + (g_i_BTN_SIZE + g_i_DISTANCE) * 3, g_i_DISTANCE + g_i_START_Y + (g_i_BTN_SIZE + g_i_DISTANCE) * 2 + g_i_DISPLAY_HEIGHT,
            g_i_BTN_SIZE, g_i_BTN_SIZE,
            hwnd, (HMENU)IDC_BUTTON_MINUS, GetModuleHandle(NULL), NULL
        );
        CreateWindowEx(NULL, "BUTTON", "+",
            WS_CHILD | WS_VISIBLE | BS_ICON | BS_PUSHBUTTON,
            g_i_START_X + (g_i_BTN_SIZE + g_i_DISTANCE) * 3, g_i_DISTANCE + g_i_START_Y + (g_i_BTN_SIZE + g_i_DISTANCE) * 3 + g_i_DISPLAY_HEIGHT,
            g_i_BTN_SIZE, g_i_BTN_SIZE,
            hwnd, (HMENU)IDC_BUTTON_PLUS, GetModuleHandle(NULL), NULL
        );
        CreateWindowEx(NULL, "BUTTON", "C",
            WS_CHILD | WS_VISIBLE | BS_ICON | BS_PUSHBUTTON,
            g_i_START_X + (g_i_BTN_SIZE + g_i_DISTANCE) * 4, g_i_DISTANCE + g_i_START_Y + g_i_DISPLAY_HEIGHT,
            g_i_BTN_SIZE, g_i_BTN_SIZE * 2 + g_i_DISTANCE,
            hwnd, (HMENU)IDC_BUTTON_CLAER, GetModuleHandle(NULL), NULL
        );
        CreateWindowEx(NULL, "BUTTON", "=",
            WS_CHILD | WS_VISIBLE | BS_ICON | BS_PUSHBUTTON,
            g_i_START_X + (g_i_BTN_SIZE + g_i_DISTANCE) * 4, g_i_DISTANCE + g_i_START_Y + (g_i_BTN_SIZE + g_i_DISTANCE) * 2 + g_i_DISPLAY_HEIGHT,
            g_i_BTN_SIZE, g_i_BTN_SIZE * 2 + g_i_DISTANCE,
            hwnd, (HMENU)IDC_BUTTON_EQUAL, GetModuleHandle(NULL), NULL
        );

        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_1), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/One.ico", IMAGE_ICON,
                LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE));
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_2), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/Two.ico", IMAGE_ICON,
                LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE));
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_3), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/Free.ico", IMAGE_ICON,
                LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE));
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_4), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/Four.ico", IMAGE_ICON,
                LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE));
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_5), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/Five.ico", IMAGE_ICON,
                LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE));
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_6), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/Six.ico", IMAGE_ICON,
                LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE));
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_7), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/Seven.ico", IMAGE_ICON,
                LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE));
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_8), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/Vosem.ico", IMAGE_ICON,
                LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE));
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_9), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/Nine.ico", IMAGE_ICON,
                LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE));
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_0), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/Zero.ico", IMAGE_ICON,
                g_i_BTN_SIZE * 2 + g_i_DISTANCE, LR_DEFAULTSIZE, LR_LOADFROMFILE));
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_POINT), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/Point.ico", IMAGE_ICON,
                g_i_BTN_SIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE));
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_MINUS), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/Minus.ico", IMAGE_ICON,
                g_i_BTN_SIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE));
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/Plus.ico", IMAGE_ICON,
                g_i_BTN_SIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE));
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/Aster.ico", IMAGE_ICON,
                g_i_BTN_SIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE));
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_SLASH), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/Slash.ico", IMAGE_ICON,
                g_i_BTN_SIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE));
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_CLAER), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/C.ico", IMAGE_ICON,
                g_i_BTN_SIZE + g_i_DISTANCE, g_i_BTN_SIZE * 2 + g_i_DISTANCE, LR_LOADFROMFILE));
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/Equal.ico", IMAGE_ICON,
                g_i_BTN_SIZE + g_i_DISTANCE, g_i_BTN_SIZE * 2 + g_i_DISTANCE, LR_LOADFROMFILE));
    }       break;
    case WM_COMMAND: {
        CONST INT SIZE = 256;
        CHAR SZ_buffer[SIZE] = {};
        HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
        CHAR sz_digit[2] = {};
        static DOUBLE a = 0;
        static DOUBLE b = 0;
        static bool stored = false;
        static bool input = false;
        static char operation = '0';
        if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9) {
            input = true;
            if (stored && operation != '0') {
                SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"");
            }
            stored = false;
            SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)SZ_buffer);
            if (SZ_buffer[1] != '.') {
                while (SZ_buffer[0] == '0') {
                    for (int i = 0; i < SZ_buffer[i]; i++) {
                        SZ_buffer[i] = SZ_buffer[i + 1];
                    }
                }
            }
            sz_digit[0] = LOWORD(wParam) - 1000 + '0';
            strcat(SZ_buffer, sz_digit);
            SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)SZ_buffer);
        }
        if (LOWORD(wParam) == IDC_BUTTON_POINT) {
            SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)SZ_buffer);
            if (strchr(SZ_buffer, '.')) break;
            sz_digit[0] = '.';
            strcat(SZ_buffer, sz_digit);
            SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)SZ_buffer);
        }
        if (LOWORD(wParam) == IDC_BUTTON_CLAER) {
            SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"0");
            a = 0; stored = false;
        }
        if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH) {
            if (a == 0) {
                SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)SZ_buffer);
                a = strtod(SZ_buffer, NULL);
            }
            stored = true;
            input = false;
            SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_EQUAL, 0);
            switch (LOWORD(wParam))
            {
            case IDC_BUTTON_PLUS:operation = '+'; break;
            case IDC_BUTTON_MINUS:operation = '-'; break;
            case IDC_BUTTON_SLASH:operation = '/'; break;
            case IDC_BUTTON_ASTER:operation = '*'; break;
            }
        }
        if (LOWORD(wParam) == IDC_BUTTON_EQUAL) {
            if (input) {
                SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)SZ_buffer);
                b = strtod(SZ_buffer, NULL);
            }
            input = false;
            switch (operation)
            {
            case '+': a += b; break;
            case '-': a -= b; break;
            case '*': a *= b; break;
            case '/': a /= b; break;
            }
            sprintf(SZ_buffer, "%g", a);
            SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)SZ_buffer);
        }
    } break;
    case WM_SIZE:
    case WM_MOVE: {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        SetSize(hwnd, rect);
    }                        break;
    case WM_DESTROY: PostQuitMessage(0); break;
    case WM_CLOSE:
        // if (MessageBox(hwnd, "Вы действительно хотите закрыть окно?", "Question", MB_YESNO | MB_ICONQUESTION) == IDYES) {
        DestroyWindow(hwnd);
        //}
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
    sprintf(sz_message, "%s(%d*%d, %d*%d)", g_sz_MY_WINDOW_CLASS,
        horizontal, vertical, height, width);
    SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_message);
}
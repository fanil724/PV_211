#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include"resource.h"
#include<stdio.h>
#include<winuser.h>
#include<Commctrl.h>


CONST CHAR g_sz_MY_WINDOW_CLASS[] = "�alculator";
CONST INT g_i_BTN_SIZE = 50;     //g global, i int
CONST INT g_i_DISTANCE = 10;     //��������� ����� ��������
CONST INT g_i_START_X = 10;      //������ �� ������ ����
CONST INT g_i_START_Y = 10;      //������ �� ������ ����
CONST INT g_i_DISPLAY_WIDHT = (g_i_BTN_SIZE + g_i_DISTANCE) * 5 - g_i_DISTANCE;
CONST INT g_i_DISPLAY_HEIGHT = 45;
CONST INT g_i_DISPLAY_FONT_HEIGHT = g_i_DISPLAY_HEIGHT - 2;
CONST INT g_i_DISPLAY_FONT_WIDHT = g_i_DISPLAY_HEIGHT / 2.5;


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
    wc.hIcon = (HICON)LoadImage(hInstance, "Icon/Cal.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
    wc.hIconSm = (HICON)LoadImage(hInstance, "Icon/Cal.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
    wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW));
    // wc.hCursor = (HCURSOR)LoadImage(hInstance, "Protoss.cur", IMAGE_CURSOR, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
    wc.hbrBackground = (HBRUSH)COLOR_ACTIVEBORDER;
    //wc.hbrBackground = CreateSolidBrush(RGB(0,0,200));


    wc.hInstance = hInstance;
    wc.lpfnWndProc = (WNDPROC)WndProc; //TODO: ������� ��������� ����
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_sz_MY_WINDOW_CLASS;

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "����� �����������������", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    //2. �������� ����
    INT window_width = g_i_START_X * 2 + g_i_DISPLAY_WIDHT + 16;
    INT window_height = g_i_START_Y * 2 + g_i_DISPLAY_HEIGHT + (g_i_BTN_SIZE + g_i_DISTANCE) * 4 + 37;

    HWND hwnd = CreateWindowEx(
        NULL,
        g_sz_MY_WINDOW_CLASS,//��� ������ ����
        g_sz_MY_WINDOW_CLASS,//��������� ����
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, //����� ���� ��� �������� ���� ��������� ������ �����
        //WS_OVERLAPPEDWINDOW -���� �������� ������ ������� �������� � ���� �������� ����
        g_i_START_X, g_i_START_Y,//��������� ������� ����
        window_width, window_height,//������ ������
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
        /* INT X = GetSystemMetrics(0) / 8;
         INT Y = GetSystemMetrics(1) / 8;*/
         //SetWindowPos(hwnd, 0, X, Y, (GetSystemMetrics(0) / 4 * 3), (GetSystemMetrics(1) / 4 * 3), 0); ;//��������� ������� ����
         //RECT rect;
         //GetWindowRect(hwnd, &rect);
         //SetSize(hwnd, rect);
        HWND hEdit = CreateWindowEx
        (
            NULL,
            "Edit",
            "0",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT /*| ES_READONLY*/,
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
        CONST INT SIZE = 256;
        CHAR sz_icon_name[SIZE]{};
        for (int number = 1; number < 10; number++) {
            sprintf(sz_icon_name, "Icon/%d.ico", number);
            SendMessage(GetDlgItem(hwnd, 1000 + number), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
                (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), sz_icon_name, IMAGE_ICON,
                    LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE));
        }
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON_0), BM_SETIMAGE, (WPARAM)IMAGE_ICON,
            (LPARAM)(HICON)LoadImage(GetModuleHandle(NULL), "Icon/0.ico", IMAGE_ICON,
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
        HFONT hFont = CreateFont(g_i_DISPLAY_FONT_HEIGHT, g_i_DISPLAY_FONT_WIDHT, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
            OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
        SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
    }       break;
    case WM_CTLCOLOREDIT: {
        if ((HWND)lParam == GetDlgItem(hwnd, IDC_EDIT))
        {
            SetBkColor((HDC)wParam, RGB(0, 120, 255));
            SetTextColor((HDC)wParam, 0xffffff);
            return (INT_PTR)GetStockObject(NULL_BRUSH);
        }
    } break;
    case WM_COMMAND: {
        SetFocus(hwnd);
        CONST INT SIZE = 256;
        CHAR SZ_buffer[SIZE] = {};
        HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
        CHAR sz_digit[2] = {};
        static DOUBLE a = 0;
        static DOUBLE b = 0;
        static bool stored = false;
        static bool input = false;
        static bool operation_input = false;
        static char operation = 0;
        //static char old_operation = 0;
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
            a = 0; b = 0; stored = false; input = false; operation_input = false;
            operation = 0;
            //SetFocus(hwnd);
        }
        if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH) {
            SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)SZ_buffer);
            b = strtod(SZ_buffer, NULL);
            if (a == 0) {
                a = b;
            }
            stored = true;
            if (input && operation_input) {
                SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_EQUAL, 0);
            }
            input = false;
            switch (LOWORD(wParam))
            {
            case IDC_BUTTON_PLUS:operation = '+'; break;
            case IDC_BUTTON_MINUS:operation = '-'; break;
            case IDC_BUTTON_SLASH:operation = '/'; break;
            case IDC_BUTTON_ASTER:operation = '*'; break;
            }
            operation_input = true;
        }
        if (LOWORD(wParam) == IDC_BUTTON_EQUAL) {
            SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)SZ_buffer);
            if (input) {
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
            operation_input = false;
            sprintf(SZ_buffer, "%g", a);
            SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)SZ_buffer);
        }
    } break;
    case WM_KEYDOWN: {
        /* char symbol[2]{};
         symbol[0] = LOWORD(wParam);*/
         //MessageBox(hwnd, symbol, "Symbol", MB_OK);
        switch (LOWORD(wParam))
        {
        case VK_OEM_PLUS:
        case VK_ADD:SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_PLUS, 0); break;

        case VK_SUBTRACT:
        case VK_OEM_MINUS:SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_MINUS, 0); break;

        case VK_OEM_2:
        case VK_DIVIDE:SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_SLASH, 0); break;

        case VK_MULTIPLY:SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_ASTER, 0); break;

        case VK_RETURN:SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_EQUAL, 0); break;

        case VK_BACK:
        case VK_DELETE:SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_CLAER, 0); break;

        case VK_ESCAPE:SendMessage(hwnd, WM_CLOSE, 0, 0); break;
        }
        if (wParam == VK_OEM_PERIOD || wParam == VK_DECIMAL)SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_POINT, 0);
        if (GetKeyState(VK_SHIFT) < 0) {
            if (wParam == 0x38)SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_ASTER, 0);
        }
        else {
            if (wParam >= 0x30 && wParam <= 0x39) SendMessage(hwnd, WM_COMMAND, wParam - 0x30 + 1000, 0);
        }
        if (wParam >= 0x60 && wParam <= 0x69) { SendMessage(hwnd, WM_COMMAND, wParam - 0x60 + 1000, 0); }
    }break;
        //case WM_SIZE:
    case WM_MOVE: {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        SetSize(hwnd, rect);
    }   break;
    case WM_DESTROY: PostQuitMessage(0); break;
    case WM_CLOSE:
        DestroyWindow(hwnd);

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

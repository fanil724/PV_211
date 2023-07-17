#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<winuser.h>
#include<cstdio>
#include"resource.h"
#include<vector>
#include<string>

CONST CHAR* values[] = { "Завести", "машину,", "и", "обеспечить", "сжигание", "топлива", "двигаталем", "каждую","секунду." };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow) {
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
    return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HWND hList = GetDlgItem(hwnd, IDC_LIST1);
    std::vector<std::string> listen;
    for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        listen.push_back(values[i]);
    }
    switch (uMsg)
    {
    case WM_INITDIALOG: {
        HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
        SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
        SendMessage(GetDlgItem(hwnd, IDC_BUTTON1), WM_SETTEXT, 0, (LPARAM)"Добавить запись");
        SendMessage(GetDlgItem(hwnd, IDC_CHECK1), WM_SETTEXT, 0, (LPARAM)"Сортировка");
        for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)values[i]);
        }
    }break;
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDOK: {
            CONST INT SIZE = 256;
            CHAR sz_buffer[SIZE]{};
            int i = SendMessage(hList, LB_GETCURSEL, 0, 0);
            SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);
            CHAR sz_message[SIZE]{};
            sprintf(sz_message, "Вы выбрали элемент № %i, со значением \"%s\"", i, sz_buffer);
            MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
        }break;
        case IDC_BUTTON1: {
            CONST INT SIZE = 256;
            CHAR sz_buffer[SIZE]{};
            CHAR sz_buf[SIZE]{};
            HWND hEditControl = GetDlgItem(hwnd, IDC_EDIT1);
            SendMessage(hEditControl, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
            int count = SendMessage(hList, LB_GETCOUNT, 0, 0);
            bool isEmpty = false;
            if (sz_buffer == NULL) {
                MessageBox(hwnd, "Строка пустая!", "Info", MB_OK | MB_ICONINFORMATION);
            }
            else {
                for (int i = 0; i < count; i++) {
                    SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buf);
                    if (strcmp(sz_buf, sz_buffer) == 0) {
                        isEmpty = true;
                    }
                }
                if (isEmpty) {
                    MessageBox(hwnd, "Данное слово уже есть в списке", "Info", MB_OK | MB_ICONINFORMATION);
                }
                else {
                    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
                    listen.push_back(sz_buffer);
                }
            }
        }break;
        case IDC_CHECK1: {
            INT value = SendMessage(GetDlgItem(hwnd, IDC_CHECK1), BM_GETCHECK, 0, 0);
            DestroyWindow(GetDlgItem(hwnd, IDC_LIST1));
            if (value == 0) {
                hList = CreateWindow("listbox", NULL, WS_CHILD | WS_VISIBLE,
                    7, 11, 442, 200, hwnd, (HMENU)IDC_LIST1, NULL, NULL);
                SendMessage(hList, WM_SETREDRAW, FALSE, 0L);
                for (int i = 0; i != listen.size(); i++)
                {
                    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)(LPSTR)listen[i].c_str());
                }

                SendMessage(hList, WM_SETREDRAW, TRUE, 0L);
                InvalidateRect(hList, NULL, TRUE);
            }
            else {
                hList = CreateWindow("listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_SORT,
                    7, 11, 442, 200, hwnd, (HMENU)IDC_LIST1, NULL, NULL);
                SendMessage(hList, WM_SETREDRAW, FALSE, 0L);
                for (auto i : listen)
                {
                    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)(LPSTR)i.c_str());
                }
                SendMessage(hList, WM_SETREDRAW, TRUE, 0L);
                InvalidateRect(hList, NULL, TRUE);
            }
        }break;
        case IDCANCEL: {EndDialog(hwnd, 0); } break;
        }
    }break;
    case WM_CLOSE:EndDialog(hwnd, 0); break;
    }
    return false;
}
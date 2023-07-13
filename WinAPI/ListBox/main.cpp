#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<cstdio>
#include"resource.h"


CONST CHAR* values[] = { "�������", "������,", "�", "����������", "��������", "�������", "����������", "������","�������." };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow) {
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
    return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    {
    case WM_INITDIALOG: {
        HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
        SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
        HWND hList = GetDlgItem(hwnd, IDC_LIST1);
        for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)values[i]);
        }
    }break;
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDOK: {
            HWND hList = GetDlgItem(hwnd, IDC_LIST1);
            CONST INT SIZE = 256;
            CHAR sz_buffer[SIZE]{};
            int i = SendMessage(hList, LB_GETCURSEL, 0, 0);
            SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);
            CHAR sz_message[SIZE]{};
            sprintf(sz_message, "�� ������� ������� � %i, �� ��������� \"%s\"", i, sz_buffer);
            MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
        }break;
        case IDCANCEL: {EndDialog(hwnd, 0); } break;
        }
    }break;
    case WM_CLOSE:EndDialog(hwnd, 0); break;
    }
    return false;
}
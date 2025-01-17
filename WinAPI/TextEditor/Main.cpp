﻿#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include"resource.h"
#include<RichEdit.H>
#include<Shellapi.h>
#include<wingdi.h>
#include<stdio.h>
#include <string>

CONST CHAR g_sz_CLASS_NAME[] = "Text Editor PV_211";
CONST CHAR* g_sz_SIZE[] = { "8","9","10","12","14","16","18","20","22","24","26","28","36","48","72" };
CONST CHAR* g_sz_INSCRIPTION[] = { "обычный","курсив","полужирный","полужирный курсив" };
CONST CHAR* g_sz_FONT[] = { "Times New Roman","Georgia","Arial","Arial Black","Tahoma",
"Verdana","Trebuchet MS","Lucida Sans Unicode","Impact","Comic Sans MS","Courier New","Lucida Console" };


HFONT g_hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
COLORREF g_RGB_Text = RGB(0, 0, 0);
COLORREF g_rgbBackground = RGB(255, 255, 255);
COLORREF g_rgbCustom[16] = {};


LRESULT CALLBACK WndPRoc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcFont(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL LoadTextFileToEdit(HWND hEdit, LPCSTR lpszFileName);
BOOL SaveTextFileFromEdit(HWND hEsit, LPCSTR lpszFileName);
VOID SelectFont(HWND hwnd);
VOID SelectColor(HWND hwnd);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.style = 0;

    wc.hIcon = (HICON)LoadImage(hInstance, "Icon/TXT.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
    wc.hIconSm = (HICON)LoadImage(hInstance, "Icon/TXT.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
    wc.hCursor = (HCURSOR)LoadCursor(hInstance, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndPRoc;
    wc.lpszClassName = g_sz_CLASS_NAME;
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Class registration failed,", "Info", MB_OK | MB_ICONERROR);
    }

    HWND hwnd = CreateWindowEx(
        WS_EX_ACCEPTFILES,
        g_sz_CLASS_NAME,
        g_sz_CLASS_NAME,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        MessageBox(NULL, "Window greation  failed,", "Error", MB_OK | MB_ICONERROR);
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

LRESULT CALLBACK WndPRoc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static CHAR lpszFileName[MAX_PATH] = {};
    static LPSTR lpszFileContent = NULL;

    switch (uMsg)
    {
    case WM_CREATE: {
        LoadLibrary("riched20.dll");
        RECT window_rect;
        RECT client_rect;
        GetWindowRect(hwnd, &window_rect);
        GetClientRect(hwnd, &client_rect);
        HWND hEdit = CreateWindowEx(
            NULL, RICHEDIT_CLASS, "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOHSCROLL,
            0, 0,
            client_rect.right, client_rect.bottom,
            hwnd,
            (HMENU)IDC_EDIT,
            GetModuleHandle(NULL),
            NULL
        );

        if (strstr(GetCommandLineA(), "exe")) {
            LoadTextFileToEdit(hEdit, strstr(GetCommandLineA(), "exe") + 5);
            DWORD dwTextLenght = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
            GlobalFree(lpszFileContent);
            lpszFileContent = (LPSTR)GlobalAlloc(GPTR, dwTextLenght + 1);
            SendMessage(hEdit, WM_GETTEXT, dwTextLenght + 1, (LPARAM)lpszFileContent);
        }

    }break;
    case WM_DROPFILES: {

        HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
        DWORD dwTextLengt = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
        LPSTR lpszText = new CHAR[dwTextLengt + 1]{};
        SendMessage(hEdit, WM_GETTEXT, dwTextLengt + 1, (LPARAM)lpszText);
        if (lpszFileContent && strcmp(lpszFileContent, lpszText) != 0 || lpszFileContent == NULL && lpszText[0]) {
            switch (MessageBox(hwnd, "Сохранить изменения?", "Вопрос", MB_YESNOCANCEL | MB_ICONQUESTION)) {
            case IDYES: SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, 0);
            case IDNO: {
                CHAR szFileName[MAX_PATH] = {};
                DragQueryFile((HDROP)wParam, 0, szFileName, MAX_PATH);
                HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
                LoadTextFileToEdit(GetDlgItem(hwnd, IDC_EDIT), szFileName);
                DWORD dwTextLenght = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
                GlobalFree(lpszFileContent);
                lpszFileContent = (LPSTR)GlobalAlloc(GPTR, dwTextLenght + 1);
                SendMessage(hEdit, WM_GETTEXT, dwTextLenght + 1, (LPARAM)lpszFileContent);
                DragFinish((HDROP)wParam);
            }
            case IDCANCEL:break;
            }
        }
        else {
            CHAR szFileName[MAX_PATH] = {};
            DragQueryFile((HDROP)wParam, 0, szFileName, MAX_PATH);
            HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
            LoadTextFileToEdit(GetDlgItem(hwnd, IDC_EDIT), szFileName);
            DWORD dwTextLenght = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
            GlobalFree(lpszFileContent);
            lpszFileContent = (LPSTR)GlobalAlloc(GPTR, dwTextLenght + 1);
            SendMessage(hEdit, WM_GETTEXT, dwTextLenght + 1, (LPARAM)lpszFileContent);
            DragFinish((HDROP)wParam);
        }
    }break;
    case WM_COMMAND: {
        switch (LOWORD(wParam))
        {
        case ID_FILE_NEW: {
            HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
            DWORD dwTextLengt = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
            LPSTR lpszText = new CHAR[dwTextLengt + 1]{};
            SendMessage(hEdit, WM_GETTEXT, dwTextLengt + 1, (LPARAM)lpszText);
            if (lpszFileContent && strcmp(lpszFileContent, lpszText) != 0 || lpszFileContent == NULL && lpszText[0]) {
                switch (MessageBox(hwnd, "Сохранить изменения?", "Вопрос", MB_YESNOCANCEL | MB_ICONQUESTION)) {
                case IDYES: SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, 0);
                case IDNO: {
                    GlobalFree(lpszFileContent);
                    lpszFileContent = nullptr;
                    lpszFileName[0] = 0;
                    SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"");
                }
                case IDCANCEL:break;
                }
            }
            else {
                GlobalFree(lpszFileContent);
                lpszFileContent = nullptr;
                lpszFileName[0] = 0;
                SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"");
                SetFocus(hEdit);
            }
        }break;
        case ID_FILE_OPEN: {
            OPENFILENAME ofn;
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFilter = "Text files: (*.txt)\0*.txt\0AllFiles (*.*)\0*.*\0 ";
            ofn.lpstrFile = lpszFileName;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
            ofn.lpstrDefExt = "txt";

            if (GetOpenFileName(&ofn)) {
                HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
                LoadTextFileToEdit(hEdit, lpszFileName);
                DWORD dwTextLenght = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
                GlobalFree(lpszFileContent);
                lpszFileContent = (LPSTR)GlobalAlloc(GPTR, dwTextLenght + 1);
                SendMessage(hEdit, WM_GETTEXT, dwTextLenght + 1, (LPARAM)lpszFileContent);
                SetFocus(hEdit);
            }

        }break;
        case ID_FILE_SAVE: {
            HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
            if (lpszFileName[0] == 0) SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVEAS, 0);
            else {
                SaveTextFileFromEdit(hEdit, lpszFileName);
                DWORD dwTextLenght = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
                //GlobalFree(lpszFileContent);
                lpszFileContent = (LPSTR)GlobalAlloc(GPTR, dwTextLenght + 1);
                SendMessage(hEdit, WM_GETTEXT, dwTextLenght + 1, (LPARAM)lpszFileContent);
            }
        }break;
        case ID_FILE_SAVEAS: {

            OPENFILENAME ofn;
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFilter = "Text files: (*.txt)\0*.txt\0AllFiles (*.*)\0*.*\0";
            ofn.lpstrFile = lpszFileName;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
            ofn.lpstrDefExt = "txt";


            if (GetSaveFileName(&ofn))SaveTextFileFromEdit(GetDlgItem(hwnd, IDC_EDIT), ofn.lpstrFile);
        }break;
        case ID_FILE_EXIT:SendMessage(hwnd, WM_CLOSE, 0, 0); break;
        case ID_FORMAT_FONT: {
            SelectFont(hwnd);
        }break;
        case ID_FORMAT_COLOR:SelectColor(hwnd); break;
        }
    } break;
    case WM_DESTROY:  PostQuitMessage(0); break;
    case WM_CLOSE: {
        BOOL close = FALSE;
        HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
        DWORD dwTextLengt = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
        LPSTR lpszText = (LPSTR)GlobalAlloc(GPTR, dwTextLengt + 1);
        if (lpszText != NULL) {
            SendMessage(hEdit, WM_GETTEXT, dwTextLengt + 1, (LPARAM)lpszText);
            if (lpszFileContent && strcmp(lpszFileContent, lpszText) != 0 || lpszFileContent == NULL && lpszText[0]) {
                switch (MessageBox(hwnd, "Сохранить изменения?", "Вопрос", MB_YESNOCANCEL | MB_ICONQUESTION)) {
                case IDYES: SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, 0);
                case IDNO: close = TRUE; break;
                }
            }
            else {
                close = TRUE;
            }
            GlobalFree(lpszText);
            GlobalFree(lpszFileContent);
        }
        if (close)DestroyWindow(hwnd);
    }break;
    default:return DefWindowProc(hwnd, uMsg, wParam, lParam); break;
    }
    return 0;
}

BOOL CALLBACK DlgProcFont(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    {
    case WM_INITDIALOG: {
        LoadLibrary("Gdi32.dll");
        CONST INT SIZE = 256;
        HWND hListSize = GetDlgItem(hwnd, IDC_LIST3);
        for (int i = 0; i < sizeof(g_sz_SIZE) / sizeof(g_sz_SIZE[0]); i++)
        {
            SendMessage(hListSize, LB_ADDSTRING, 0, (LPARAM)g_sz_SIZE[i]);
        }
        HWND hListInscription = GetDlgItem(hwnd, IDC_LIST2);
        for (int i = 0; i < sizeof(g_sz_INSCRIPTION) / sizeof(g_sz_INSCRIPTION[0]); i++)
        {
            SendMessage(hListInscription, LB_ADDSTRING, 0, (LPARAM)g_sz_INSCRIPTION[i]);
        }
        HWND hListFont = GetDlgItem(hwnd, IDC_LIST1);
        for (int i = 0; i < sizeof(g_sz_FONT) / sizeof(g_sz_FONT[0]); i++)
        {
            SendMessage(hListFont, LB_ADDSTRING, 0, (LPARAM)g_sz_FONT[i]);
        }

        LOGFONT lf;
        ZeroMemory(&lf, sizeof(LOGFONT));
        HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        GetObject(hFont, sizeof(LOGFONT), &lf);
        CHAR Height[SIZE] = {};
        CHAR Inscription[SIZE] = {};
        sprintf(Height, "%d", lf.lfHeight);
        SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, 0, (LPARAM)Height);
        SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)lf.lfFaceName);
        if (lf.lfItalic) {
            if (lf.lfWeight == FW_BOLD)
                SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)g_sz_INSCRIPTION[3]);
            SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)g_sz_INSCRIPTION[1]);
        }
        else if (lf.lfWeight == FW_BOLD) {
            SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)g_sz_INSCRIPTION[2]);
        }
        else {
            SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)g_sz_INSCRIPTION[0]);
        }
        SendMessage(hListSize, LB_SETCURSEL, SendMessage(hListSize, LB_FINDSTRING, -1, (LPARAM)Height), 0);
        SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_GETTEXT, SIZE, (LPARAM)Inscription);
        SendMessage(hListInscription, LB_SETCURSEL, SendMessage(hListInscription, LB_FINDSTRING, -1, (LPARAM)Inscription), 0);
        SendMessage(hListFont, LB_SETCURSEL, SendMessage(hListFont, LB_FINDSTRING, -1, (LPARAM)lf.lfFaceName), 0);
    }break;
    case WM_COMMAND: {
        switch (LOWORD(wParam))
        {
        case IDOK: {
            CONST INT SIZE = 20;
            CHAR sz_SIZE[SIZE] = {};
            CHAR sz_FONT[SIZE] = {};
            CHAR sz_Inscription[SIZE] = {};
            SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_GETTEXT, SIZE, (LPARAM)sz_SIZE);
            SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_GETTEXT, SIZE, (LPARAM)sz_FONT);
            SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_GETTEXT, SIZE, (LPARAM)sz_Inscription);
            BOOL Italic = FALSE;
            INT Weight = 400;
            if (strcmp(sz_Inscription, "обычный") == 0) {
                Italic = false;
                Weight = FW_NORMAL;
            }
            if (strcmp(sz_Inscription, "курсив") == 0) {
                Italic = TRUE;
                Weight = FW_NORMAL;
            }
            if (strcmp(sz_Inscription, "полужирный") == 0) {
                Italic = false;
                Weight = FW_BOLD;
            }
            if (strcmp(sz_Inscription, "полужирный курсив") == 0) {
                Italic = TRUE;
                Weight = FW_BOLD;
            }
            HFONT hFont = CreateFont(std::stoi(sz_SIZE), 0, 0, 0, Weight, Italic, FALSE, FALSE, ANSI_CHARSET,
                OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE, sz_FONT);
            HWND hParent = GetParent(hwnd);
            SendMessage(GetDlgItem(hParent, IDC_EDIT), WM_SETFONT, (WPARAM)hFont, TRUE);
        }
        case IDCANCEL: EndDialog(hwnd, 0); break;
        }
        HWND hList = (HWND)lParam;
        if (LBN_SELCHANGE) {
            CONST INT SIZE = 64;
            if (hList == GetDlgItem(hwnd, IDC_LIST3)) {
                CHAR sz_Size[SIZE] = {};
                SendMessage(GetDlgItem(hwnd, IDC_LIST3), LB_GETTEXT, SendMessage(GetDlgItem(hwnd, IDC_LIST3), LB_GETCURSEL, 0, 0), (LPARAM)sz_Size);
                SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, SIZE, (LPARAM)sz_Size);
            }
            if (hList == GetDlgItem(hwnd, IDC_LIST2)) {
                CHAR sz_Inscription[SIZE] = {};
                SendMessage(GetDlgItem(hwnd, IDC_LIST2), LB_GETTEXT, SendMessage(GetDlgItem(hwnd, IDC_LIST2), LB_GETCURSEL, 0, 0), (LPARAM)sz_Inscription);
                SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, SIZE, (LPARAM)sz_Inscription);
            }
            if (hList == GetDlgItem(hwnd, IDC_LIST1)) {
                CHAR sz_Font[SIZE] = {};
                SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_GETTEXT, SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_GETCURSEL, 0, 0), (LPARAM)sz_Font);
                SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, SIZE, (LPARAM)sz_Font);
            }
        }break;
    }
    case WM_CLOSE: EndDialog(hwnd, 0);
    }
    return FALSE;
}

BOOL LoadTextFileToEdit(HWND hEdit, LPCSTR lpszFileName) {
    BOOL bSuccess = FALSE;
    HANDLE hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD dwFileSize = GetFileSize(hFile, NULL);
        if (dwFileSize != UINT_MAX) {
            LPSTR lpszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);

            if (lpszFileText) {
                DWORD dwRead;
                if (ReadFile(hFile, lpszFileText, dwFileSize, &dwRead, NULL)) {
                    lpszFileText[dwFileSize] = 0;

                    if (SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)lpszFileText)) bSuccess = TRUE;
                }
                GlobalFree(lpszFileText);

            }
            CloseHandle(hFile);
        }
    }
    return bSuccess;
}

BOOL SaveTextFileFromEdit(HWND hEdit, LPCSTR lpszFileName) {
    BOOL bSuccess = FALSE;
    HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD dwTextLenght = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
        if (dwTextLenght > 0) {
            LPSTR lpszText = (LPSTR)GlobalAlloc(GPTR, dwTextLenght + 1);
            if (lpszText) {
                if (SendMessage(hEdit, WM_GETTEXT, dwTextLenght + 1, (LPARAM)lpszText)) {
                    DWORD dwWritten;
                    if (WriteFile(hFile, lpszText, dwTextLenght, &dwWritten, NULL))bSuccess = TRUE;
                }
                GlobalFree(lpszText);
            }
        }
        CloseHandle(hFile);
    }
    return bSuccess;
}

VOID SelectFont(HWND hwnd) {
    CHOOSEFONT cf;
    LOGFONT lf;
    HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);

    ZeroMemory(&cf, sizeof(cf));
    ZeroMemory(&lf, sizeof(lf));

    GetObject(g_hFont, sizeof(LOGFONT), &lf);

    cf.lStructSize = sizeof(cf);
    cf.hwndOwner = hwnd;

    cf.Flags = CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
    cf.hInstance = GetModuleHandle(NULL);
    cf.lpLogFont = &lf;
    cf.rgbColors = g_RGB_Text;

    if (ChooseFont(&cf))
    {
        HFONT hf = CreateFontIndirect(&lf);
        if (hf)
        {
            g_hFont = hf;
        }
        else
        {
            MessageBox(hwnd, "Font creation failed", "Error", MB_OK | MB_ICONERROR);
        }
        g_RGB_Text = cf.rgbColors;
    }
    SendMessage(hEdit, WM_SETFONT, (WPARAM)g_hFont, TRUE);
    SetFocus(hEdit);
}
VOID SelectColor(HWND hwnd) {
    HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
    HDC hdc = GetDC(hwnd);
    CHOOSECOLOR cc = { sizeof(CHOOSECOLOR) };
    cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR;
    cc.hwndOwner = hwnd;
    cc.rgbResult = g_rgbBackground;
    cc.lpCustColors = g_rgbCustom;
    if (ChooseColor(&cc))g_rgbBackground = cc.rgbResult;

    CHARFORMAT ch;
    ZeroMemory(&ch, sizeof(CHARFORMAT));
    ch.cbSize = sizeof(ch);
    ch.dwMask = CFM_COLOR;
    ch.crTextColor = g_rgbBackground;
    SendMessage(hEdit, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&ch);
    SetFocus(hEdit);
}

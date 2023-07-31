//#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include"resource.h"
#include<RichEdit.H>

CONST CHAR g_sz_CLASS_NAME[] = "Text Editor PV_211";
LRESULT CALLBACK WndPRoc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL LoadTextFileToEdit(HWND hEdit, LPCSTR lpszFileName);
BOOL SaveTextFileFromEdit(HWND hEsit, LPCSTR lpszFileName);

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
        NULL,
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

        //  HWND hStatus = CreateWindowEx(0,STATUSCLASSNAME);

    }break;
    case WM_COMMAND: {
        switch (LOWORD(wParam))
        {
        case ID_FILE_OPEN: {
            CHAR szFileName[MAX_PATH] = {};

            OPENFILENAME ofn;
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFilter = "Text files: (*.txt)\0*.txt\0AllFiles (*.*)\0*.*\0 ";
            ofn.lpstrFile = szFileName;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
            ofn.lpstrDefExt = "txt";

            if (GetOpenFileName(&ofn)) LoadTextFileToEdit(GetDlgItem(hwnd, IDC_EDIT), szFileName);
        }break;
        case ID_FILE_SAVEAS: {
            CHAR szFileName[MAX_PATH] = {};
            OPENFILENAME ofn;
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFilter = "Text files: (*.txt)\0*.txt\0AllFiles (*.*)\0*.*\0";
            ofn.lpstrFile = szFileName;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
            ofn.lpstrDefExt = "txt";

            if (GetSaveFileName(&ofn))SaveTextFileFromEdit(GetDlgItem(hwnd, IDC_EDIT), ofn.lpstrFile);
        }break;
        case ID_FILE_EXIT:SendMessage(hwnd, WM_CLOSE, 0, 0); break;
        }
    } break;
    case WM_DESTROY:  PostQuitMessage(0); break;
    case WM_CLOSE:DestroyWindow(hwnd); break;
    default:return DefWindowProc(hwnd, uMsg, wParam, lParam); break;
    }
    return 0;
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


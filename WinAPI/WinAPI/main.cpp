#include<Windows.h>
#include<winuser.h>
#include"resource.h"

const CHAR g_sz_LOGIN_INVATE[] = "Введите имя пользователя";
const CHAR g_sz_PASSWORD_INVATE[] = "Введите пароль";

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//-----------------------------------------------------------------------------------------------------------------------
//HWND - handler to window(лбработчик окна). для того чтобы обратиться к окну можно только через его обработчик (handler).
//uint uMsg - сообщения отправляемые в окно.
// WPARAM, LPARAM -- параметры сообщения зависят от самого сообщения.
//-----------------------------------------------------------------------------------------------------------------------

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//MessageBox(NULL, "Привет Мир! Я окно сообщения :-)", "Info", MB_YESNOCANCEL | MB_ICONINFORMATION);
	//hInstance - запущенной программы всегда можно получить при помощи команды GetModuleHandle(NULL);

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc, 0);

	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:	//Создаем элемменты окна
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
		SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WM_SETTEXT, 0, (LPARAM)g_sz_LOGIN_INVATE);
		SendMessage(GetDlgItem(hwnd, IDC_EDIT_PASSWORD), WM_SETTEXT, 0, (LPARAM)g_sz_PASSWORD_INVATE);
	}
	break;
	case WM_COMMAND:	//Обрабатываем команды нажатия на кнопки, наведение мыши........
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_COPY: {
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
			const int SIZE = 256;
			char sz_buffer[SIZE] = {}; //sz_ string zero(null Terminated line 
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);// обращаемся к окну, и сообщаем ему что хотим прочитать его текст 
			SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}		break;
		case IDOK:		MessageBox(hwnd, "Была нажата кнопка OK", "Info", MB_OK | MB_ICONINFORMATION); break;
		case IDCANCEL:	EndDialog(hwnd, 0); break;
		case IDC_EDIT_LOGIN: {
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			const int SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			if (HIWORD(wParam) == EN_SETFOCUS) {
				if (strcmp(sz_buffer, g_sz_LOGIN_INVATE) == 0) { SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)""); }
			}
			if (HIWORD(wParam) == EN_KILLFOCUS) {
				if (strlen(sz_buffer) == 0) {
					SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)g_sz_LOGIN_INVATE);
				}
			}
		} break;
		case IDC_EDIT_PASSWORD: {
			HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
			const int SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditPassword, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			if (HIWORD(wParam) == EN_SETFOCUS) {
				if (strcmp(sz_buffer, g_sz_PASSWORD_INVATE) == 0) { SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)""); }
			}
			if (HIWORD(wParam) == EN_KILLFOCUS) {
				if (strlen(sz_buffer) == 0) {
					SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)g_sz_PASSWORD_INVATE);
				}
			}
		} break;
		}break;
	case WM_CLOSE: EndDialog(hwnd, 0);
	}
	return FALSE;
}
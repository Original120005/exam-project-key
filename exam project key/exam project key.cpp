#pragma comment(lib,"comctl32")
#include<windows.h>
#include <tchar.h>
#include <conio.h>
#include <windowsX.h>
#include <commctrl.h>
#include <time.h>
#include"resource.h"

#define WM_ICON WM_APP
#define ID_TRAYICON WM_USER
#define WM_USER_SHELLICON (WM_USER + 1)

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

PNOTIFYICONDATA pNID;
NOTIFYICONDATA data;

int points = 0;
int error = 0;

TCHAR lvl1[] = TEXT("àîàî îàîà ààîî îîàà îààî àîîà àîàî îàîà");
TCHAR lvl2[] = TEXT("âëâë ëâëâ ââëë ëëââ âëëâ ëââë âëëâ");
TCHAR lvl3[] = TEXT("ïðïð ðïðï ïïðð ððïï ðïïð ïððï ððïï ðïïð");
TCHAR lvl4[] = TEXT("êåêå åêåê êêåå ååêê êååê åêêå êååê");
TCHAR lvl5[] = TEXT("íãíã ãíãí ííãã ããíí íããí ãííã ãíãí");
TCHAR str_lvl[10];

HWND hTime, hLevel, hDialog, hText, hEnterText, hFinish;
HWND hEdit1, hEdit2, hEdit3;
HWND hButton16;
HWND hModalDialog, hResultDialog;

HICON hIcon;

HANDLE hMutex;
HANDLE hThread;

TCHAR str_time[50];
TCHAR str_result[100];
TCHAR str_text[50];
TCHAR str_timer[50];

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow) {
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

void Cls_OnSize(HWND hwnd, UINT state, int cx, int cy) {
	if (state == SIZE_MINIMIZED) {
		ShowWindow(hwnd, SW_HIDE); 
		Shell_NotifyIcon(NIM_ADD, pNID); 
	}
}

void OnTrayIcon(WPARAM wp, LPARAM lp) {
	if (lp == WM_LBUTTONDBLCLK) {
		Shell_NotifyIcon(NIM_DELETE, pNID);
		ShowWindow(hDialog, SW_NORMAL);
		SetForegroundWindow(hDialog);
	}
}

DWORD WINAPI TimeFunc(LPVOID lp) {
	HWND hProgress = (HWND)lp;
	struct Time {
		int Minutes = 0;
		int Seconds = 0;
	} Time;



	while (TRUE) {
		wsprintf(str_time, TEXT("%d:%d"), Time.Minutes, Time.Seconds);
		SetWindowText(hTime, str_time);
		if (Time.Seconds >= 60) {
			Time.Seconds = 00;
			Time.Minutes++;
		}
		else {
			Time.Seconds++;
		}
		Sleep(1000);
	}
	return 0;
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG: {
		hTime = GetDlgItem(hWnd, IDC_TIME);
		hEdit1 = GetDlgItem(hWnd, IDC_EDIT1);
		hEdit2 = GetDlgItem(hWnd, IDC_EDIT2);
		hEdit3 = GetDlgItem(hWnd, IDC_EDIT3);
		hLevel = GetDlgItem(hWnd, IDC_LEVEL);
		hText = GetDlgItem(hWnd, IDC_TEXT);
		hEnterText = GetDlgItem(hWnd, IDC_ENTERTEXT);
		hFinish = GetDlgItem(hWnd, IDOK);
		hDialog = hWnd;
		hButton16 = GetDlgItem(hWnd, IDC_BUTTON16);

		
		HINSTANCE hInst = GetModuleHandle(NULL);
		pNID->cbSize = sizeof(NOTIFYICONDATA); 
		memset(pNID, 0, sizeof(NOTIFYICONDATA)); 
		pNID->hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)); 
		pNID->hWnd = hDialog; 	
		pNID->uCallbackMessage = WM_ICON;
		pNID->uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_INFO;
		pNID->uID = ID_TRAYICON;


		data.cbSize = sizeof(NOTIFYICONDATA);
		data.hWnd = hWnd;
		data.uID = ID_TRAYICON;
		data.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		data.uCallbackMessage = WM_USER_SHELLICON;
		data.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
		data.uVersion = NOTIFYICON_VERSION;
		Shell_NotifyIcon(NIM_ADD, &data);


		TCHAR GUID[] = TEXT("{P5J783E0-270D-4dclf-9BFG4-99FD247E793DGDFGFB}");
		hMutex = CreateMutex(NULL, FALSE, GUID);
		DWORD dwAnswer = WaitForSingleObject(hMutex, 0);
		if (dwAnswer == WAIT_TIMEOUT) {
			MessageBox(hWnd, TEXT("çàïóñê íà áîëåå îäíîé êîïèè íà  ïðèëîæåíèÿ!!!"), TEXT("Òðåíàæåð ïå÷àòè íà êëàâèàòóðå íà" ), MB_OK | MB_ICONINFORMATION);
			EndDialog(hWnd, 0);
		}
	}
					  break;

	case WM_COMMAND: {
		if (LPARAM(wParam) == IDC_START) {
			LRESULT result1 = SendDlgItemMessage(hWnd, IDC_RADIO1, BM_GETCHECK, 0, 0);
			LRESULT result2 = SendDlgItemMessage(hWnd, IDC_RADIO2, BM_GETCHECK, 0, 0);
			LRESULT result3 = SendDlgItemMessage(hWnd, IDC_RADIO3, BM_GETCHECK, 0, 0);
			LRESULT result4 = SendDlgItemMessage(hWnd, IDC_RADIO4, BM_GETCHECK, 0, 0);
			LRESULT result5 = SendDlgItemMessage(hWnd, IDC_RADIO5, BM_GETCHECK, 0, 0);

			if (result1 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl1);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("1")));
				wsprintf(str_lvl, TEXT("%s"), TEXT("1"));
			}
			if (result2 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl2);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("2")));
				wsprintf(str_lvl, TEXT("%s"), TEXT("2"));
			}
			if (result3 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl3);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("3")));
				wsprintf(str_lvl, TEXT("%s"), TEXT("3"));
			}
			if (result4 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl4);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("4")));
				wsprintf(str_lvl, TEXT("%s"), TEXT("4"));
			}
			if (result5 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl5);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("5")));
				wsprintf(str_lvl, TEXT("%s"), TEXT("5"));
			}

			hThread = CreateThread(NULL, 0, TimeFunc, hTime, 0, NULL);

			TCHAR str_1[10] = TEXT("1");
			TCHAR str_2[10] = TEXT("2");
			TCHAR str_3[10] = TEXT("3");
			TCHAR str_4[10] = TEXT("4");
			TCHAR str_5[10] = TEXT("5");

			int compare = _tcscmp(str_lvl, str_1);
			if (compare == 0) {
				wsprintf(str_text, TEXT("%s"), lvl1);
			}
			else {
				compare = _tcscmp(str_lvl, str_2);
				if (compare == 0) {
					wsprintf(str_text, TEXT("%s"), lvl2);
				}
				else {
					compare = _tcscmp(str_lvl, str_3);
					if (compare == 0) {
						wsprintf(str_text, TEXT("%s"), lvl3);
					}
					else {
						compare = _tcscmp(str_lvl, str_4);
						if (compare == 0) {
							wsprintf(str_text, TEXT("%s"), lvl4);
						}
						else {
							compare = _tcscmp(str_lvl, str_5);
							wsprintf(str_text, TEXT("%s"), lvl5);
						}
					}
				}
			}

		}

		if (LPARAM(wParam) == IDOK) {
			points = 0;
			error = 0;

			int count_result = GetWindowTextLength(hEnterText);
			GetWindowText(hEnterText, str_result, count_result + 1);
			for (int i = 0; i < 30; i++) {
				if (str_result[i] == str_text[i]) {
					points++;
				}
				else {
					error++;
				}
			}
			TCHAR str_points[10];
			wsprintf(str_points, TEXT("%d"), points);
			TCHAR str_error[10];
			wsprintf(str_error, TEXT("%d"), error);

			CloseHandle(hThread);
			EndDialog(hModalDialog, 0);
			hResultDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG3), hWnd, DLGPROC(DlgProc));
			ShowWindow(hResultDialog, SW_RESTORE);
			SendMessage(hEdit1, WM_SETTEXT, 0, LPARAM(str_time));
			SendMessage(hEdit2, WM_SETTEXT, 0, LPARAM(str_error));
			SendMessage(hEdit3, WM_SETTEXT, 0, LPARAM(str_points));

		}

	}
				   break;

	case WM_SIZE: {
		if (message == SIZE_MINIMIZED) {
			ShowWindow(hWnd, SW_HIDE); 
			Shell_NotifyIcon(NIM_ADD, pNID); 
		}
	}
				break;
				

	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	if (message == WM_ICON) {
		OnTrayIcon(wParam, lParam);
		return TRUE;
	}
	return FALSE;
}
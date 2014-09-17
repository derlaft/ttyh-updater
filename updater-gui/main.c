#include <windows.h>
#include <commctrl.h>
#include <winuser.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

LPCTSTR g_szClassName = "ttyhupdater";

#define IDC_MAIN_LABEL 100
#define IDC_MAIN_PROGRESS 101

#define WINDOW_HEIGHT 100
#define WINDOW_WIDTH 500

#ifndef IDC_STATIC
  #define IDC_STATIC                   -1
#endif

#define PAUSE 100

#define MARKER_MSG "msg "
#define MARKER_PERCENT "per "
#define MARKER_ERR "err "

HWND hProgress;
HWND hEdit;
HWND hwnd;

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CREATE: ;

            hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, ("Static"), ("\nStarting to update launcher"),
                        WS_CHILD | WS_VISIBLE | SS_CENTER,
                        -2, -2, WINDOW_WIDTH + 1, WINDOW_HEIGHT/2 + 2,
			hwnd, (HMENU) IDC_MAIN_LABEL, GetModuleHandle(NULL), NULL);

            hProgress = CreateWindowEx(WS_EX_CLIENTEDGE, ("msctls_progress32"), (""),
                        WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
                        -2, WINDOW_HEIGHT/2 - 1, WINDOW_WIDTH, WINDOW_HEIGHT/2 - 1,
			hwnd, (HMENU) IDC_MAIN_PROGRESS, GetModuleHandle(NULL), NULL);

	    SendMessage(hProgress, PBM_SETSTEP, 1, 0);
	    SendMessage(hProgress, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0,100));
	    SendMessage(hProgress, PBM_SETPOS, 0, 0);

            if (hProgress == NULL || hEdit == NULL)
                MessageBox(hwnd, ("Could not start. Wat?"), ("Error"), MB_OK | MB_ICONERROR);

        break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

DWORD WINAPI ThreadProc() {

  FILE *fp;
  char buf[256];

  fp = popen ("busybox.exe sh updater.sh", "r");

  if (fp == NULL) {
	  MessageBox(hwnd, ("Could not start. Wat?"), ("Error"), MB_OK | MB_ICONERROR);
	  ExitThread(1);
  }

  while (fgets(buf, sizeof(buf) - 1, fp)) {

	  if (strstr(buf, MARKER_MSG) == buf) {
		  char msg[256] = "\n";
		  strcat(msg, buf+strlen(MARKER_MSG));
		  SetWindowText(hEdit, (msg));
	  }
	  else if (strstr(buf, MARKER_PERCENT) == buf) {
		 SendMessage(hProgress, PBM_SETPOS, atoi(buf+strlen(MARKER_PERCENT)), 0); 
	  }
	  else if (strstr(buf, MARKER_ERR) == buf) {
		 MessageBox(hwnd, (buf+strlen(MARKER_ERR)), ("Error"), MB_OK | MB_ICONERROR);
		 ExitThread(1);
	  }
  }

  printf("man\n");

  pclose(fp);

  ExitThread(0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{

INITCOMMONCONTROLSEX icc;
icc.dwSize = sizeof(icc);
icc.dwICC = ICC_WIN95_CLASSES | ICC_STANDARD_CLASSES;
InitCommonControlsEx(&icc);

    WNDCLASSEX wc;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    RECT area;
    area.left = 0;
    area.top = 0;
    area.right = WINDOW_WIDTH;
    area.bottom = WINDOW_HEIGHT;

    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

    if (!AdjustWindowRect(
    	&area,
	style,
	FALSE
    )) {
	MessageBox(NULL, ("Window adjusting Failed!"), ("Error!"),
			MB_ICONEXCLAMATION | MB_OK);
    }


    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        ("Updating launcher"),
        style,
        CW_USEDEFAULT, CW_USEDEFAULT, area.right, area.bottom + 10,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, ("Window Creation Failed!"), ("Error!"),
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    CreateThread(NULL, 0, &ThreadProc, NULL, 0, NULL);

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

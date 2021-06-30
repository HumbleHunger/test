#include<stdafx.h>
#include<Util.h>
#include<FreecellApp.h>
#include<ardDrawer.h>
#include<ard.h>
#include<onstant.h>
#include<amePanel.h>
#include<commctrl.h>

__ANT_USING_NAMESPACE

#define MAX_LOADSTRING 128

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name


//////////////////////////////////////////////////////////////////////////
static bool bEnabled = false;
static int gameNumber;
static GamePanel gamePanel;
//////////////////////////////////////////////////////////////////////////


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Select(HWND, UINT, WPARAM, LPARAM );
INT_PTR CALLBACK	CardView(HWND, UINT, WPARAM, LPARAM );
BOOL	CALLBACK	EnumChildProc(HWND, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	InitCommonControls();

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FREECELL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FREECELL));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			//if (!IsDialogMessage(msg.hwnd, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			//}
		}
	}

	return (int) msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FREECELL));
	wcex.hCursor		= LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_ARROW));
	wcex.hbrBackground	= CreateSolidBrush((RGB(0, 127, 0))); //set window background
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_FREECELL);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
	  NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   
   ShowWindow(hWnd, nCmdShow);
   //ShowWindow(hDlg, SW_SHOWNA);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	HDC hdc;
	int retVal;
	VARIABLE_DECLARATION_FOR_CENTER_WINDOW

	switch (message)
	{
	case WM_CREATE:
		CENTER_WINDOW(hWnd)
		//TRANSPARENT_WINDOW(hWnd, 70)
		gamePanel.SetInstance(hInst);
		gamePanel.SetWnd(hWnd);
		gamePanel.EnableRePlayMenu(false);
		gamePanel.EnableUndoMenu(false);
		gamePanel.EnableAiMenu(false);
		break;

	case WM_LBUTTONDBLCLK:
		FREECELL_DRAW_BEGIN(hWnd, hdc)
		gamePanel.OnLeftButtonDoubleClick(wParam, lParam);
		FREECELL_DRAW_END(hWnd, hdc)
		return 0;

	case WM_LBUTTONDOWN:
		FREECELL_DRAW_BEGIN(hWnd, hdc)
		gamePanel.OnLeftButtonDown(wParam, lParam);
		FREECELL_DRAW_END(hWnd, hdc)
		return 0;

	case WM_RBUTTONDOWN:
		FREECELL_DRAW_BEGIN(hWnd, hdc)
		SetCapture(hWnd);
		gamePanel.OnRightButtonDown(wParam, lParam);
		FREECELL_DRAW_END(hWnd, hdc)
		return 0;

	case WM_RBUTTONUP:
		FREECELL_DRAW_BEGIN(hWnd, hdc)
		gamePanel.OnRightButtonUp(wParam, lParam);
		ReleaseCapture();
		FREECELL_DRAW_END(hWnd, hdc)
		return 0;

	case WM_MOUSEMOVE:
		FREECELL_DRAW_BEGIN(hWnd, hdc)
		gamePanel.OnMouseMove(wParam, lParam);
		FREECELL_DRAW_END(hWnd, hdc)
		return 0;

	case WM_GETMINMAXINFO: //change default maximize action
		gamePanel.OnGetMinMaxInfo(wParam, lParam);
		return 0;

	case WM_SIZING: //forbidden change window width
		gamePanel.OnSizing(wParam, lParam);
		return TRUE;

	case WM_AIWORKING:
		gamePanel.OnAIWorking();
		return 0;

	case WM_AIFINISHED:
		gamePanel.OnAIFinished();
		return 0;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_HELP: //帮助
			ShellExecute(hWnd, "open", "freecell.chm", NULL, NULL, SW_SHOWNORMAL);
			break;

		case IDM_ABOUT: //关于
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case IDM_PLAY: //开局
			if (!gamePanel.IsGameStarted() || gamePanel.ShowWarningMessageBox(WARNING_RESTART) == IDYES) {
					FREECELL_DRAW_BEGIN(hWnd, hdc)
					gamePanel.OnStart();
					FREECELL_DRAW_END(hWnd, hdc)
			}
			break;

		case IDM_SELECT: //选局
			if (!gamePanel.IsGameStarted() || gamePanel.ShowWarningMessageBox(WARNING_RESTART) == IDYES) {
				FREECELL_DRAW_BEGIN(hWnd, hdc)
				retVal = (int)DialogBox(hInst, MAKEINTRESOURCE(IDD_SELECTBOX), hWnd, Select);
				if (retVal == IDOK)
					gamePanel.OnStart(gameNumber);
				FREECELL_DRAW_END(hWnd, hdc)
			}
			break;

		case IDM_REPLAY: //重玩
			if (!gamePanel.IsGameStarted() || gamePanel.ShowWarningMessageBox(WARNING_RESTART) == IDYES) {
				FREECELL_DRAW_BEGIN(hWnd, hdc)
				gamePanel.OnReStart();
				FREECELL_DRAW_END(hWnd, hdc)
			}
			break;

		case IDM_UNDO: //撤销
			// TODO:
			break;

		case IDM_AI: //启动AI
			FREECELL_DRAW_BEGIN(hWnd, hdc)
			gamePanel.OnReStart();
			gamePanel.AIPlay();
			FREECELL_DRAW_END(hWnd, hdc)
			break;

		case IDM_MOVE_HINT: //移牌提示
			bEnabled = !bEnabled;
			gamePanel.EnableMoveHint(bEnabled);
			break;

		case IDM_EXIT: //退出
			gamePanel.OnClose();
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_ERASEBKGND:
		return 1;

	case WM_PAINT:
		gamePanel.OnPaint();
		break;

	case WM_CLOSE:
		gamePanel.OnClose();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	VARIABLE_DECLARATION_FOR_CENTER_WINDOW

	switch (message)
	{
	case WM_INITDIALOG:
		CENTER_WINDOW(hDlg)
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Message handler for Select box.
INT_PTR CALLBACK Select(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	VARIABLE_DECLARATION_FOR_CENTER_WINDOW
	BOOL fError;

	switch (message)
	{
	case WM_INITDIALOG:
		CENTER_WINDOW(hDlg)
		//将牌局文本框初始为随机值
		SetDlgItemInt(hDlg,
			IDC_EDIT_GAMENUMBER,
			gamePanel.GetRandomGameNumber(),
			FALSE);

		//全选牌局文本框中的字符
		SendDlgItemMessage(hDlg, IDC_EDIT_GAMENUMBER, EM_SETSEL, 0, -1);

		if (GetDlgCtrlID((HWND)wParam) != IDC_EDIT_GAMENUMBER) 
		{
			SetFocus(GetDlgItem(hDlg, IDC_EDIT_GAMENUMBER));
			return (INT_PTR)FALSE;
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if (LOWORD(wParam) == IDOK) {
				gameNumber = GetDlgItemInt(hDlg,
					IDC_EDIT_GAMENUMBER, &fError, FALSE);
				if (gameNumber < MIN_GAME_NUMBER ||
					gameNumber > MAX_GAME_NUMBER) {
						SetDlgItemInt(hDlg,
							IDC_EDIT_GAMENUMBER,
							1,
							FALSE);
						SendMessage(GetDlgItem(hDlg, IDC_EDIT_GAMENUMBER), EM_SETSEL, 0, -1);
						return (INT_PTR)TRUE;
				}
			}
			EndDialog(hDlg, LOWORD(wParam));
		}
		return (INT_PTR)TRUE;

	}
	return (INT_PTR)FALSE;
}

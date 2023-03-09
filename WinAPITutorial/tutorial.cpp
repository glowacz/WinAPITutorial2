#include "framework.h"
#include "tutorial.h"
#include <string>
#include <list>

#define MAX_LOADSTRING 100

using namespace std;

static list < POINT > pointsList;

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDC_TUTORIAL, szWindowClass, MAX_LOADSTRING);
	
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

	std::wstring title(L"tutorial\0");
	title.copy(szTitle, title.size());

	std::wstring className(L"tutorial\0");
	className.copy(szWindowClass, className.size());

	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	//HACCEL hAccelTable = LoadAccelerators(hInstance, nullptr);
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TUTORIAL));

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return static_cast<int>(msg.wParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.hCursor = nullptr;
	//wcex.hbrBackground = reinterpret_cast <HBRUSH>(COLOR_WINDOW + 1);
	wcex.hbrBackground = reinterpret_cast <HBRUSH>(COLOR_ACTIVECAPTION + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TUTORIAL);
	
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TUTORIAL));
	
	
	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 400, 400, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	//HWND hWnd;

	//int size = 300;

	//for (int i = 0; i < 3; i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE, i*size, j*size, size, size, nullptr, nullptr, hInstance, nullptr);
	//	}
	//}

	//// Set WS_EX_LAYERED on this window
	//SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	//// Make this window 50% alpha
	//SetLayeredWindowAttributes(hWnd, 0, (255 * 50) / 100, LWA_ALPHA);
	
	// Show this window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	return TRUE;
}

void GetTextInfoForMouseMsg(HWND hWnd, WPARAM wParam, LPARAM lParam, const TCHAR* msgName, TCHAR* buf, int bufSize)
{
	short x = (short)LOWORD(lParam);
	short y = (short)HIWORD(lParam);
	POINT pt = { x , y };
	ClientToScreen(hWnd, &pt);

	_stprintf_s(buf, bufSize, _T("%s x: %d, y: %d, vk:"),
		msgName, x, y);
	if ((wParam == MK_LBUTTON) != 0) {
		_tcscat_s(buf, bufSize, _T(" LEFT "));
	}
	if ((wParam == MK_MBUTTON) != 0) {
		_tcscat_s(buf, bufSize, _T(" MIDDLE "));
	}
	if ((wParam == MK_RBUTTON) != 0) {
		_tcscat_s(buf, bufSize, _T(" RIGHT "));
	}
}

void GetTextInfoForKeyMsg(WPARAM wParam, const TCHAR* msgName, TCHAR * buf, int bufSize)
{
	static int counter = 0;
	counter++;
	_stprintf_s(buf, bufSize, _T("%s key: %d ( counter : %d)"), msgName,
		wParam, counter);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const int bufSize = 256;
	TCHAR buf[bufSize];
	static HCURSOR cursor = NULL;
	static HDC offDC = NULL;
	static HBITMAP offOldBitmap = NULL;
	static HBITMAP offBitmap = NULL;

	switch (message)
	{
#pragma region popups
		case WM_COMMAND:
		{
			//RECT rc;
			//GetWindowRect(hWnd, &rc);
			//OffsetRect(&rc, 20, 0);
			//MoveWindow(hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE); // dumb

			int wmId = LOWORD(wParam);
			switch (wmId)
			{
				case IDM_ABOUT:
					 DialogBox(hInst , MAKEINTRESOURCE ( IDD_ABOUTBOX ), hWnd, About );
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				case IDM_SHOW:
					MessageBox(hWnd, LPCWSTR(L"duuuuuzoooo\r\nteeeeekstuuuu"), LPCWSTR(L"mojpierwszytytul"), MB_OK);
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
#pragma endregion popups
		
#pragma region old_paints
		//case WM_PAINT:
		//{
		//	PAINTSTRUCT ps;
		//	HDC hdc = BeginPaint(hWnd, &ps);
		//	TCHAR s[] = _T(" Hello world !");
		//	RECT rc;
		//	GetClientRect(hWnd, &rc);
		//	DrawText(hdc, s, (int)_tcslen(s), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//	EndPaint(hWnd, &ps);
		//}
		//break;

		//case WM_PAINT:
		//{
		//	PAINTSTRUCT ps;
		//	HDC hdc = BeginPaint(hWnd, &ps);
		//	HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		//	HPEN oldPen = (HPEN)SelectObject(hdc, pen); // SelectObject	returns a handle to the object being replaced
		//	MoveToEx(hdc, 0, 0, NULL);
		//	LineTo(hdc, 100, 100);
		//	SelectObject(hdc, oldPen);
		//	DeleteObject(pen); // DESTROYING A GDI OBJECT!!!!!!!
		//	EndPaint(hWnd, &ps);
		//}
		//break;

		//case WM_PAINT:
		//{
		//	PAINTSTRUCT ps;
		//	HDC hdc = BeginPaint(hWnd, &ps);
		//	HPEN pen = CreatePen(PS_DOT, 1, RGB(255, 0, 0));
		//	HPEN oldPen = (HPEN)SelectObject(hdc, pen);
		//	HBRUSH brush = CreateSolidBrush(RGB(0, 128, 0));
		//	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
		//	Rectangle(hdc, 20, 20, 120, 120);
		//	SelectObject(hdc, oldPen);
		//	DeleteObject(pen);
		//	SelectObject(hdc, oldBrush);
		//	DeleteObject(brush);
		//	EndPaint(hWnd, &ps);
		//}
		//break;

		//case WM_PAINT:
		//{
		//	PAINTSTRUCT ps;
		//	HDC hdc = BeginPaint(hWnd, &ps);
		//	TCHAR s[] = _T(" Hello world !");
		//	HFONT font = CreateFont(
		//		-MulDiv(24, GetDeviceCaps(hdc, LOGPIXELSY), 72), // Height
		//		0, // Width
		//		0, // Escapement
		//		0, // Orientation
		//		FW_BOLD, // Weight
		//		false, // Italic
		//		FALSE, // Underline
		//		0, // StrikeOut
		//		EASTEUROPE_CHARSET, // CharSet
		//		OUT_DEFAULT_PRECIS, // OutPrecision
		//		CLIP_DEFAULT_PRECIS, // ClipPrecision
		//		DEFAULT_QUALITY, // Quality
		//		DEFAULT_PITCH | FF_SWISS, // PitchAndFamily
		//		_T(" Verdana ")); // Facename
		//	HFONT oldFont = (HFONT)SelectObject(hdc, font);
		//	RECT rc;
		//	GetClientRect(hWnd, &rc);
		//	DrawText(hdc, s, (int)_tcslen(s), &rc,
		//		DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//	SelectObject(hdc, oldFont);
		//	DeleteObject(font);
		//	EndPaint(hWnd, &ps);
		//}
		//break;

		//case WM_PAINT:
		//{
		//	PAINTSTRUCT ps;
		//	HDC hdc = BeginPaint(hWnd, &ps);
		//	TCHAR s[] = _T(" Hello world !");
		//	HBITMAP bitmap = LoadBitmap(hInst,
		//		MAKEINTRESOURCE(IDB_BITMAP1));
		//	HDC memDC = CreateCompatibleDC(hdc);
		//	HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, bitmap);
		//	//BitBlt(hdc, 0, 0, 48, 48, memDC, 0, 0, SRCCOPY);
		//	StretchBlt(hdc, 20, 10, 200, 200, memDC,
		//		0, 0, 48, 48, SRCCOPY);
		//	SelectObject(memDC, oldBitmap);
		//	DeleteObject(bitmap);
		//	DeleteDC(memDC);
		//	EndPaint(hWnd, &ps);
		//}
		//break;

		//case WM_SIZE: //displays window size in the caption of the window
		//{
		//	// get the size of the client area
		//	int clientWidth = LOWORD(lParam);
		//	int clientHeight = HIWORD(lParam);
		//	// get the size of the window

		//	RECT rc;
		//	GetWindowRect(hWnd, &rc);
		//	// modify the caption of the window
		//	TCHAR s[256];
		//	_stprintf_s(s, 256,
		//		_T(" Window 's size : %d x %d Client area 's size : %d x %d"),
		//		rc.right - rc.left, rc.bottom - rc.top,
		//		clientWidth, clientHeight);
		//	SetWindowText(hWnd, s);
		//}
		//break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			HBRUSH brush = CreateSolidBrush(RGB(128, 128, 0));
			HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, &brush);
			list < POINT >::const_iterator iter = pointsList.begin();
			while (iter != pointsList.end()) {
				POINT pt = *iter;
				const int rad = 15;
				Ellipse(hdc, pt.x - 2*rad, pt.y - rad, pt.x + 2*rad, pt.y + rad);
				iter++;
			}
			SelectObject(hdc, oldBrush);
			DeleteObject(brush);
			EndPaint(hWnd, &ps);
		}
		break;

		//case WM_PAINT:
		//{
		//	PAINTSTRUCT ps;
		//	HDC hdc = BeginPaint(hWnd, &ps);
		//	RECT rc;
		//	GetClientRect(hWnd, &rc);
		//	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH) GetStockObject(GRAY_BRUSH) );
		//	Rectangle(hdc, 0, 0, rc.right, rc.bottom);
		//	SelectObject(hdc, (HBRUSH)GetStockObject(BLACK_BRUSH));
		//	const int margin = 50;
		//	Rectangle(hdc, margin, margin,
		//		rc.right - margin, rc.bottom - margin);
		//	SelectObject(hdc, oldBrush);
		//	EndPaint(hWnd, &ps);
		//}
		//break;
#pragma endregion old_paints

#pragma region bitmap_bg
	//case WM_CREATE:
	//{
	//	HDC hdc = GetDC(hWnd);
	//	offDC = CreateCompatibleDC(hdc);
	//	ReleaseDC(hWnd, hdc);
	//	break;
	//}
	//case WM_SIZE:
	//{
	//	int clientWidth = LOWORD(lParam);
	//	int clientHeight = HIWORD(lParam);
	//	HDC hdc = GetDC(hWnd);
	//	if (offOldBitmap != NULL) {
	//		SelectObject(offDC, offOldBitmap);
	//	}
	//	if (offBitmap != NULL) {
	//		DeleteObject(offBitmap);
	//	}
	//	offBitmap = CreateCompatibleBitmap(hdc, clientWidth, clientHeight);
	//	offOldBitmap = (HBITMAP)SelectObject(offDC, offBitmap);
	//	ReleaseDC(hWnd, hdc);

	//}
	//break;

	//case WM_PAINT:
	//{
	//	PAINTSTRUCT ps;
	//	HDC hdc = BeginPaint(hWnd, &ps);
	//	RECT rc;
	//	GetClientRect(hWnd, &rc);
	//	HBRUSH oldBrush = (HBRUSH)SelectObject(offDC,
	//		(HBRUSH)GetStockObject(GRAY_BRUSH));
	//	Rectangle(offDC, 0, 0, rc.right, rc.bottom);
	//	SelectObject(offDC, (HBRUSH)GetStockObject(BLACK_BRUSH));
	//	const int margin = 50;
	//	Rectangle(offDC, margin, margin,
	//		rc.right - margin, rc.bottom - margin);
	//	SelectObject(offDC, oldBrush);
	//	BitBlt(hdc, 0, 0, rc.right, rc.bottom, offDC, 0, 0, SRCCOPY);
	//	EndPaint(hWnd, &ps);
	//}
	//break;
	//case WM_ERASEBKGND:
	//	return 1;
	//case WM_DESTROY:
	//	if (offOldBitmap != NULL) {
	//		SelectObject(offDC, offOldBitmap);
	//	}
	//	if (offDC != NULL) {
	//		DeleteDC(offDC);
	//	}
	//	if (offBitmap != NULL) {
	//		DeleteObject(offBitmap);
	//	}
	//	PostQuitMessage(0);
	//break;
#pragma endregion bitmap_bg

#pragma region mouse buttons
	//case WM_LBUTTONDOWN:
	//{
	//	HDC hdc = GetDC(hWnd);
	//	HBRUSH brush = CreateSolidBrush(RGB(128, 0, 128));
	//	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
	//	short x = (short)LOWORD(lParam);
	//	short y = (short)HIWORD(lParam);
	//	const int rad = 25;
	//	Ellipse(hdc, x - 2 * rad, y - rad, x + 2 * rad, y + rad);
	//	SelectObject(hdc, oldBrush);
	//	DeleteObject(brush);
	//	ReleaseDC(hWnd, hdc);
	//}
	//break;
	/*case WM_LBUTTONDOWN:
		GetTextInfoForMouseMsg(hWnd, wParam, lParam, _T(" LBUTTONDOWN "),
			buf, bufSize);
		SetWindowText(hWnd, buf);
		SetCapture(hWnd);
		break;*/
	case WM_LBUTTONUP:
		ReleaseCapture();
		GetTextInfoForMouseMsg(hWnd, wParam, lParam, _T(" LBUTTONUP "), buf, bufSize);
		SetWindowText(hWnd, buf);
	break;
	case WM_RBUTTONDOWN:
		GetTextInfoForMouseMsg(hWnd, wParam, lParam, _T(" RBUTTONDOWN "), buf, bufSize);
		SetWindowText(hWnd, buf);
		SetCapture(hWnd);
	break;
	case WM_RBUTTONUP:
		ReleaseCapture();
		GetTextInfoForMouseMsg(hWnd, wParam, lParam, _T(" RBUTTONUP "), buf, bufSize);
		SetWindowText(hWnd, buf);
	break;
	case WM_LBUTTONDBLCLK:
		GetTextInfoForMouseMsg(hWnd, wParam, lParam, _T(" LBUTTONDBLCLK "), buf, bufSize);
		SetWindowText(hWnd, buf);
	break;
	case WM_MBUTTONDBLCLK:
		GetTextInfoForMouseMsg(hWnd, wParam, lParam, _T(" MBUTTONDBLCLK "), buf, bufSize);
		SetWindowText(hWnd, buf);
	break;
	case WM_RBUTTONDBLCLK:
		GetTextInfoForMouseMsg(hWnd, wParam, lParam, _T(" RBUTTONDBLCLK "), buf, bufSize);
		SetWindowText(hWnd, buf);
	break;

	case WM_LBUTTONDOWN:
	{
		GetTextInfoForMouseMsg(hWnd, wParam, lParam, _T(" LBUTTONDOWN "), buf, bufSize);
		SetWindowText(hWnd, buf);
		SetCapture(hWnd);

		POINT pt;
		pt.x = (short)LOWORD(lParam);
		pt.y = (short)HIWORD(lParam);
		pointsList.push_back(pt);
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd); // not absolutely necessary ???
	}
	break;
#pragma endregion mouse buttons	
	
#pragma region keyboard_keys
	case WM_CHAR:
		_stprintf_s(buf, bufSize, _T(" WM_CHAR : %c"), (TCHAR)wParam);
		SetWindowText(hWnd, buf);
	break;

	case WM_KEYDOWN:
		GetTextInfoForKeyMsg(wParam, _T(" KEYDOWN "), buf, bufSize);
		SetWindowText(hWnd, buf);
	break;
	case WM_KEYUP:
		GetTextInfoForKeyMsg(wParam, _T(" KEYUP "), buf, bufSize);
		SetWindowText(hWnd, buf);
	break;
#pragma endregion keyboard_keys
		
#pragma region sizing
		//case WM_GETMINMAXINFO: // idiotic funtionality ...
		//{
		//	MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
		//	minMaxInfo -> ptMaxSize.x = minMaxInfo -> ptMaxTrackSize.x = 500;
		//	minMaxInfo -> ptMaxSize.y = minMaxInfo -> ptMaxTrackSize.y = 200;
		//}
		//break;

		//case WM_SIZING: // idiotic funtionality ...
		//{
		//	RECT* rc = (RECT*)lParam;
		//	if (wParam == WMSZ_BOTTOM
		//		|| wParam == WMSZ_BOTTOMLEFT
		//		|| wParam == WMSZ_BOTTOMRIGHT
		//		|| wParam == WMSZ_TOP
		//		|| wParam == WMSZ_TOPLEFT
		//		|| wParam == WMSZ_TOPRIGHT)
		//	{
		//		rc -> right = rc -> left + rc -> bottom - rc -> top;
		//	}
		//	else {
		//		rc -> bottom = rc -> top + rc -> right - rc -> left;

		//	}
		//}
		//break;

		//case WM_CREATE: // message received before displaying the window, but after creation (these are separate functions)
		//	SetTimer(hWnd, 7, 250, NULL); // empty timer ... , doesn't notify any function after it passes 
		//break;

		//case WM_TIMER: // interesting, but kinda useless ...
		//{
		//	if (wParam == 7) // check timer id
		//	{
		//		const int minSize = 200;
		//		const int maxSize = 400;
		//		static int stepSize = 10;

		//		RECT rc;
		//		// get the center of the work area of the system
		//		SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
		//		int centerX = (rc.left + rc.right + 1) / 2;
		//		int centerY = (rc.top + rc.bottom + 1) / 2;
		//		// get current size of the window
		//		GetWindowRect(hWnd, &rc);
		//		int currentSize = max(rc.right - rc.left, rc.bottom - rc.top);
		//		// modify size of the window
		//		currentSize += stepSize;
		//		if (currentSize >= maxSize) {
		//			stepSize = -abs(stepSize);
		//		}
		//		else if (currentSize <= minSize) {
		//			stepSize = abs(stepSize);
		//		}
		//		MoveWindow(hWnd, centerX - currentSize / 2,
		//			centerY - currentSize / 2,
		//			currentSize, currentSize, TRUE);
		//	}
		//}
		//break;
#pragma endregion sizing

#pragma region cursor
//case WM_CREATE:
//	cursor = LoadCursor(NULL, IDC_CROSS);
//	break;
//case WM_SETCURSOR: // don't use without LoadCursor (above)
//	SetCursor(cursor);
//	return TRUE;
#pragma endregion cursor
	
#pragma region stuff
	//case WM_NCHITTEST: // dragging
	//	return HTCAPTION;
	//case WM_ERASEBKGND:
	//	return 1;
	//case WM_DESTROY:
	//	PostQuitMessage(0);
	//	break;
#pragma endregion stuff

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}	

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
			return static_cast <INT_PTR>(TRUE);

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return static_cast <INT_PTR>(TRUE);
			}
			break;
	}
	return static_cast <INT_PTR>(FALSE);
}


// 
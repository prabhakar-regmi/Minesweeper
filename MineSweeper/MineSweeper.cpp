// MineSweeper.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MineSweeper.h"
#include "GridWindow.h"
#include "Drawer.h"

#define MAX_LOADSTRING 100
#define DEF_BOARDSIZE 18
#define DIFFICULTY_PERC 0.10

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR title_bar_text[MAX_LOADSTRING];					// The title bar text
TCHAR main_window_class_name[MAX_LOADSTRING];			// the main window class name
GridWindow grid_window_;

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE instance);
bool InitInstance(HINSTANCE instance, int command_show);
LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM width_parameter, LPARAM length_parameter);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int board_size_ = DEF_BOARDSIZE;
int num_of_bombs_ = 30;

// Function definition to change the size of the board according to user input
void ChangeBoardSize(HWND window, int size, int number_of_bombs)
{
	if (size != board_size_)
	{
		board_size_ = size;
		num_of_bombs_ = number_of_bombs;
		grid_window_.Initialize(window, board_size_);
		InvalidateRect(window, NULL, true);
	}
}


int APIENTRY _tWinMain(_In_ HINSTANCE   instance,
                     _In_opt_ HINSTANCE prev_instance,
                     _In_ LPTSTR        command_line,
                     _In_ int           command_show)
{
	UNREFERENCED_PARAMETER(prev_instance);
	UNREFERENCED_PARAMETER(command_line);
	MSG message;
	HACCEL accel_table;

	// Initialize global strings
	LoadString(instance, IDS_APP_TITLE, title_bar_text, MAX_LOADSTRING);
	LoadString(instance, IDC_MINESWEEPER, main_window_class_name, MAX_LOADSTRING);
	MyRegisterClass(instance);

	// Perform application initialization:
	if (!InitInstance (instance, command_show))
		return false;

	accel_table = LoadAccelerators(instance, MAKEINTRESOURCE(IDC_MINESWEEPER));

	// Main message loop:
	while (GetMessage(&message, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(message.hwnd, accel_table, &message))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	return (int) message.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE instance)
{
	WNDCLASSEX window_class_information;

	window_class_information.cbSize = sizeof(WNDCLASSEX);

	window_class_information.style			= CS_HREDRAW | CS_VREDRAW;
	window_class_information.lpfnWndProc	= WndProc;
	window_class_information.cbClsExtra		= 0;
	window_class_information.cbWndExtra		= 0;
	window_class_information.hInstance		= instance;
	window_class_information.hIcon			= LoadIcon(instance, MAKEINTRESOURCE(IDI_MINESWEEPER));
	window_class_information.hCursor		= LoadCursor(NULL, IDC_ARROW);
	window_class_information.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	window_class_information.lpszMenuName	= MAKEINTRESOURCE(IDC_MINESWEEPER);
	window_class_information.lpszClassName	= main_window_class_name;
	window_class_information.hIconSm		= LoadIcon(window_class_information.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassEx(&window_class_information);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
bool InitInstance(HINSTANCE instance, int command_show)
{
   HWND window;

   hInst = instance; // Store instance handle in our global variable

   window = CreateWindow(main_window_class_name, title_bar_text, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, //x
	   CW_USEDEFAULT, //y
	   600, //width
	   600,
	   NULL, NULL, instance, NULL);

   if (!window) return false;
   
   grid_window_.Initialize(window, board_size_);
   ShowWindow(window, command_show);
   UpdateWindow(window);
   return true;
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
LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	int w_id, w_event;

	switch (message)
	{
	case WM_COMMAND:
		w_id    = LOWORD(w_param);
		w_event = HIWORD(w_param);
		// Parse the menu selections:
		switch (w_id)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), window, About);
			break;
		case IDM_EXIT:
			DestroyWindow(window);
			break;
		case IDM_PRAB_RESTART:
			grid_window_.Initialize(window, board_size_);
			InvalidateRect(window, NULL, true);
			break;
		case IDM_PRAB_SMALL_BOARD:
			num_of_bombs_ = 15;
			ChangeBoardSize(window,10, num_of_bombs_);
			break;
		case IDM_PRAB_MEDIUM_BOARD:
			num_of_bombs_ = 30;
			ChangeBoardSize(window, 18, num_of_bombs_);
			break;
		case IDM_PRAB_LARGE_BOARD:
			num_of_bombs_ = 60;
			ChangeBoardSize(window, 25, num_of_bombs_);
			break;
		default:
			return DefWindowProc(window, message, w_param, l_param);
		}
		break;
	case WM_PAINT:
		Drawer::DrawGridWindow( grid_window_ );
		return(0);
	case WM_LBUTTONUP:
	{
		if (grid_window_.HasLost()) break;
		long xPos = LOWORD(l_param);
		long yPos = HIWORD(l_param);
		if (grid_window_.OnClick(xPos, yPos)){
			RECT lRect = grid_window_.GetGridRect(xPos, yPos);
			InvalidateRect(window, &(grid_window_.GetRectToPrint()), true);
			InvalidateRect(window, &(grid_window_.GetRectForText()), true);
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		if (grid_window_.HasLost()) break;
		long xPos = LOWORD(l_param);
		long yPos = HIWORD(l_param);
		if (grid_window_.OnRightClick(xPos, yPos))
		{
			InvalidateRect(window, &(grid_window_.GetRectToPrint()), true);
			InvalidateRect(window, &(grid_window_.GetRectForText()), true);
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(window, message, w_param, l_param);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
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

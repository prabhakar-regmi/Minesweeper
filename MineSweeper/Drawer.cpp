#include "stdafx.h"
#include "GridWindow.h"
#include "Drawer.h"
#include <windowsx.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sstream>

bool Rectangle(HDC device, Measurement& measurement)
{
	return Rectangle(device, measurement.left_, measurement.top_, measurement.right_, measurement.bottom_);
}

void Drawer::DrawCell(const Cell& cell, HDC device, Measurement& measurement, bool is_lost)
{
	HBRUSH hNewBrush, hOldBrush;
	hNewBrush = CreateSolidBrush(RGB(200, 200, 200));   // to fill with grey
	hOldBrush = (HBRUSH)SelectObject(device, hNewBrush);

	if (cell.IsExposed() || is_lost)
		SelectObject(device, hOldBrush);
	else
		SelectObject(device, hNewBrush);

	Rectangle(device, measurement);

	SelectObject(device, hOldBrush);
	DeleteObject(hNewBrush);

	if (cell.HasBomb() && cell.IsExposed()) DrawCellBomb(cell, device, measurement);
	else if (cell.IsFlagged()) DrawCellFlag(cell, device, measurement);
	else if (cell.Neighbors() > 0 && cell.IsExposed()) WriteCellText(cell, device, measurement);
}

void Drawer::DrawCellBomb(const Cell& cell, HDC device, Measurement& measurement)
{
	int len = measurement.GetLength();
	double ratio = 0.3;
	double length_ratio = ((1 - ratio) * len) / 2;

	HBRUSH hNewBrush, hOldBrush;
	hNewBrush = CreateSolidBrush(RGB(0, 0, 0));   // to fill with black
	hOldBrush = (HBRUSH)SelectObject(device, hNewBrush);

	Ellipse(device,
		(long)(measurement.left_ + length_ratio),
		(long)(measurement.top_ + length_ratio),
		(long)(measurement.right_ - length_ratio),
		(long)(measurement.bottom_ - length_ratio));

	SelectObject(device, hOldBrush);
	DeleteObject(hNewBrush);
}

void Drawer::DrawCellFlag(const Cell& cell, HDC device, Measurement& measurement)
{
	int len = measurement.GetLength();
	double ratio = 0.08;
	double width = ratio * len;

	double x = (double)(measurement.left_) + ((double)len * (double)0.25);
	double y = (double)(measurement.top_) + ((double)len * (double)0.25);
	double y_end = y + ((double)len / (double)2.0) + 1;
	double x_end = x + ((double)len / (double)2.4);
	double x_max = x_end + ((double)len / (double)10.0);
	double y_mid = y + ((double)len / (double)6.0);
	double y_flag_end = y + ((double)len * 0.3);

	POINT points[10];
	points[0] = { (int)x, (int)y };
	points[1] = { (int)x_end, (int)y };
	points[2] = { (int)((x_max + x_end + 1) * 0.5),(int)((y_mid + y + 1) * 0.5) };
	points[3] = { (int)(x_max), (int)(y_mid) };
	points[4] = { (int)((x_max + x_end + 1) * 0.5), (int)((y_mid + y_flag_end + 1) * 0.5) };
	points[5] = { (int)x_end, (int)(y_flag_end + width / 2.0) };
	points[6] = { (int)x, (int)y_flag_end };
	points[7] = { (int)(x + ((double)len / (double)20.0)), (int)((y_mid + y_flag_end) * 0.5) };
	points[8] = { (int)(x + ((double)len / (double)10.0)), (int)(y_mid) };
	points[9] = { (int)(x + ((double)len / 15.0)), (int)((y + y_mid) / 2.0) };

	HBRUSH hOldBrush, hRedBrush;

	hRedBrush = CreateSolidBrush(RGB(254, 0, 0));   // to fill with red
	hOldBrush = SelectBrush(device, hRedBrush);

	HPEN hNewPen = CreatePen(PS_SOLID, (int)(width), RGB(0, 0, 0));
	HPEN hOldPen = SelectPen(device, hNewPen);

	LPPOINT lpPoint = new POINT();

	MoveToEx(device, (int)x, (int)(y - width / 2.0), lpPoint);
	LineTo(device, (int)x, (int)y_end);

	SelectObject(device, hNewPen);
	Polygon(device, points, 10);

	SelectPen(device, hOldPen);
	SelectBrush(device, hOldBrush);
	DeleteObject(hRedBrush);
	DeleteObject(hNewPen);
}

void Drawer::WriteCellText(const Cell& cell, HDC device, Measurement& measurement)
{
	RECT rc;
	rc.left = measurement.left_;
	rc.right = measurement.right_;
	rc.top = measurement.top_;
	rc.bottom = measurement.bottom_;

	int len = measurement.GetLength();
	double ratio = 0.5;
	int textsize = (int)((double)(len * ratio));

	//Convert int to LPCSTR (wchar_t*)
	std::string str = std::to_string(cell.Neighbors());
	const char* result = str.c_str();
	wchar_t* wString = new wchar_t[5];
	MultiByteToWideChar(CP_ACP, 0, result, -1, wString, 5);

	SetTextColor(device, RGB(0, 0, 0));
	HFONT hFont = CreateFont(textsize, textsize / 2, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0/*textsize/3*/, 0, LPCWSTR("SYSTEM_FIXED_FONT"));
	HFONT hTmp = (HFONT)SelectObject(device, hFont);
	SetBkMode(device, TRANSPARENT);

	DrawText(device, wString, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	DeleteObject(SelectObject(device, hTmp));
}

void Drawer::DrawInformation(GridWindow& grid_window, HDC hdc)
{
	std::string total_bombs = "TOTAL BOMBS = " + std::to_string(grid_window.NumBombs());
	std::string total_flagged = "TOTAL FLAGGED = " + std::to_string(grid_window.NumFlagged());

	wchar_t* total_bombs_str = new wchar_t[25];
	wchar_t* total_flagged_str = new wchar_t[25];

	MultiByteToWideChar(CP_ACP, 0, total_bombs.c_str(), -1, total_bombs_str, 25);
	MultiByteToWideChar(CP_ACP, 0, total_flagged.c_str(), -1, total_flagged_str, 25);

	SetTextColor(hdc, RGB(0, 0, 0));

	HFONT font = CreateFont(18, 7, 0, 0, FW_BOLD, false, false, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,
		(LPCWSTR)L"Arial");

	HFONT hFontOld = (HFONT)SelectObject(hdc, font);

	DrawText(hdc, total_bombs_str, -1, &(grid_window.GetRectForText()), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	DrawText(hdc, total_flagged_str, -1, &(grid_window.GetRectForText()), DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

	SelectObject(hdc, hFontOld);
	DeleteObject(font);
}

void Drawer::DrawGridWindow(GridWindow& grid_window)
{
	if (!grid_window.GetWindow()) return;

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(grid_window.GetWindow(), &ps);
	HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH hOldBrush;

	if (grid_window.HasLost()) hOldBrush = (HBRUSH)SelectObject(hdc, hRedBrush);

	for (int i = 0; i < grid_window.Size(); ++i)
	{
		for (int j = 0; j < grid_window.Size(); ++j)
		{
			Measurement measurement = grid_window.GetMeasurement(i, j);
			Drawer::DrawCell(grid_window.GetCellAt(i, j), hdc, measurement, grid_window.HasLost());
		}
	}

	DrawInformation(grid_window, hdc);
	EndPaint(grid_window.GetWindow(), &ps);
	auto& [title, detail] = grid_window.MessageToPrint();
	
	if (!title.empty()) {
		DisplayMessage(grid_window, detail, title);
	}
}
std::wstring ConvertToUnicode(const std::string& s)
{
	return std::wstring(s.begin(), s.end());
}

void Drawer::DisplayMessage(GridWindow& grid_window, std::string main_message, std::string menu_item)
{
	int msgboxID = MessageBox(grid_window.GetWindow(), 
		ConvertToUnicode(main_message).c_str(), 
		ConvertToUnicode(menu_item).c_str(), 
		MB_ICONINFORMATION | MB_RETRYCANCEL);
	switch (msgboxID)
	{
	case IDRETRY:
	{
		grid_window.Initialize(grid_window.GetWindow(), grid_window.Size());
		InvalidateRect(grid_window.GetWindow(), NULL, true);
		break;
	}
	case IDCANCEL:
		break;
	}
}


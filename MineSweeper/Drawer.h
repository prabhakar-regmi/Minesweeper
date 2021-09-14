#pragma once
#include "Measurement.h"
#include "Cell.h"
#include "GridWindow.h"

class Drawer
{
public:
	static void DrawGridWindow(GridWindow& grid_window);
private:
	static void DrawCell(const Cell& cell, HDC device, Measurement& measurement, bool is_lost);
	static void DrawCellBomb(const Cell& cell, HDC device, Measurement& measurement);
	static void DrawCellFlag(const Cell& cell, HDC device, Measurement& measurement);
	static void WriteCellText(const Cell& cell, HDC device, Measurement& measurement);
	static void DrawInformation(GridWindow& grid_window, HDC hdc);
	static void DisplayMessage(GridWindow& grid_window, std::string main_message, std::string menu_item);
};


#pragma once
#include "Grid.h"
#include "Measurement.h"

class GridWindow
{
public:
	GridWindow() = default;

	void Initialize(HWND window, int size);
	bool OnClick(long x, long y);
	bool OnRightClick(long x, long y);
	std::pair<std::string, std::string> MessageToPrint();

	// Getters
	RECT GetGridRect(long x, long y) const;
	RECT GetRectToPrint() const { return grid_to_print_; }
	RECT GetRectForText() const { return text_rect_; }
	bool HasLost() const { return grid_.HasLost(); }
	const HWND& GetWindow() const { return window_; }
	int Size() const { return size_; }
	Measurement GetMeasurement(int i, int j) const;
	const Cell& GetCellAt(int i, int j) const { return grid_.GetCellAt(i, j); }
	int NumBombs() const { return grid_.NumBombs(); }
	int NumFlagged() const { return grid_.NumFlagged(); }
	
private:
	std::pair<int, int> GetIndexFromPixel(long xx, long yy) const;
	RECT GetGridRectFromIndex(int i, int j) const;
	void UpdateGridAreaToPrint(RECT rectangle);

	// Private Getters
	int Top() const { return top_; }
	int Left() const { return left_; }
	int PixelLength() const { return pixel_length_; }

private:
	Grid grid_;
	RECT text_rect_ = { 0,0,0,0 };
	RECT grid_to_print_ = { 0,0,0,0 };
	HWND window_; // Window Instance

	int size_ = -1;
	long top_ = -1;
	long left_ = -1;
	long pixel_length_ = -1; // pixel size of each rectangle
};

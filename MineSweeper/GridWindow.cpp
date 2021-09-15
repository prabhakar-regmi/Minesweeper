#include "stdafx.h"
#include "GridWindow.h"
#include "Drawer.h"
#include "Measurement.h"
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sstream>
using namespace std;

void GridWindow::Initialize(HWND window, int size)
{
	window_ = window;
	size_ = size;

	RECT rectangle;
	GetClientRect(window_, &rectangle);
	int length = rectangle.right - rectangle.left;
	int width = rectangle.bottom - rectangle.top - 25;
	double grid_size = min(length, width);
	double offset = 0.01 * grid_size;
	double left_offset = ((length - grid_size) / 2.0) + (offset);
	double top_offset = ((width - grid_size) / 2.0) + (offset);
	double actual_length = (grid_size - (2.0 * offset)) / size;
	top_ = static_cast<long>(top_offset);
	left_ = static_cast<long>(left_offset);
	pixel_length_ = static_cast<long>(actual_length);
	text_rect_.top = static_cast<long>(top_ + grid_size);
	text_rect_.left = left_;
	text_rect_.right = static_cast<long>(left_ + grid_size);
	text_rect_.bottom = static_cast<long>(text_rect_.top + 15);

	// initialize the internal data structure
	grid_.Initialize(size, size);
}

std::pair<int, int> GridWindow::GetIndexFromPixel(long xx, long yy) const
{
	int lengthX = xx - left_;
	int lengthY = yy - top_;
	return { lengthX / pixel_length_, lengthY / pixel_length_ };
}

bool GridWindow::OnRightClick(long xx, long yy)
{
	auto [i,j] = GetIndexFromPixel(xx, yy);
	bool retVal = grid_.FlipFlag(i, j);
	if (retVal) grid_to_print_ = GetGridRectFromIndex(i, j);	
	return retVal;
}

RECT GridWindow::GetGridRectFromIndex(int i, int j) const
{
	RECT res = {0,0,0,0};
	res.left = left_ + i * pixel_length_;
	res.right = left_ + (i + 1) * pixel_length_;
	res.top = top_ + (j)* pixel_length_;
	res.bottom = top_ + (j + 1) * pixel_length_;
	return res;
}

RECT GridWindow::GetGridRect(long x, long y) const
{
	auto[i, j] = GetIndexFromPixel(x, y);
	if (i < 0 || j < 0)
		return { 0,0,0,0 };

	return GetGridRectFromIndex(i, j);
}

void GridWindow::UpdateGridAreaToPrint(RECT rect)
{
	// set the area to repaint. Doesn't make sense to paint the whole grid!
	if (rect.left != 0 && (rect.left < grid_to_print_.left || grid_to_print_.left == 0)) 
		grid_to_print_.left = rect.left;

	if (rect.right != 0 && (rect.right > grid_to_print_.right || grid_to_print_.right == 0))
		grid_to_print_.right = rect.right;

	if (rect.top != 0 && (rect.top < grid_to_print_.top || grid_to_print_.top == 0))
		grid_to_print_.top = rect.top;

	if (rect.bottom != 0 && (rect.bottom > grid_to_print_.bottom || grid_to_print_.bottom == 0))
		grid_to_print_.bottom = rect.bottom;
}

bool GridWindow::OnClick(long xx, long yy)
{
	grid_to_print_ = { 0, 0, 0, 0 };
	auto [i, j] = GetIndexFromPixel(xx, yy);
	auto [is_not_bomb, visited] = grid_.Open(i, j);
	if (visited.empty()) return false; // No cells visited because the click was invalid

	if (!is_not_bomb)
	{
		// it is a bomb - paint the whole area!
		grid_to_print_.left = Left();
		grid_to_print_.top = Top();
		grid_to_print_.bottom = Top() + PixelLength() * size_;
		grid_to_print_.right = Left() + PixelLength() * size_;
	}
	else
	{
		// it is not a bomb - paint only the affected area
		for (auto coord : visited)
		{
			UpdateGridAreaToPrint(GetGridRectFromIndex(coord[0], coord[1]));
		}
	}
	return true;
}

std::pair<std::string, std::string> GridWindow::MessageToPrint() const
{
	if (grid_.HasWon())
		return { "Congratulations...", "Congratulations. You have Won. Please play again." };
	else if (grid_.HasLost())
		return { "Sorry..", "Sorry. You have Lost. Please try again." };
	else
		return { "","" };
}

Measurement GridWindow::GetMeasurement(int i, int j) const {
	return Measurement(
		left_ + i * pixel_length_, // left
		top_ + j * pixel_length_, // top
		left_ + (i + 1) * pixel_length_, // right
		top_ + (j + 1) * pixel_length_ // bottom
	);
}
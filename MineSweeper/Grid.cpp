#include "stdafx.h"
#include "Grid.h"
#include <stdlib.h>
#include <time.h>
using namespace std;

inline pair<int, int> Grid::GetSize() 
{
	return { static_cast<int>(cells_.size()), static_cast<int>(cells_[0].size()) };
}

inline bool Grid::IndexOutOfBounds(int i, int j) 
{
	auto [rows, cols] = GetSize();
	return i < 0 || i >= rows || j < 0 || j >= cols;
}

void Grid::Initialize(int rows, int cols) 
{
	// Clear the previous setup
	cells_.clear();
	cells_.shrink_to_fit();
	cells_.resize(rows, vector<Cell>(cols));
	bombs_.clear();
	bombs_.shrink_to_fit();
	bombs_.reserve(rows * cols);
	has_won_ = false;
	has_lost_ = false;
	flagged_count_ = 0;

	// Distribute the bombs -- O ( rows * cols )
	DistributeBombs();

	// Count the neighbor bombs -- O(number Of Bombs)
	UpdateNeighbourBombCount();
}

void Grid::DistributeBombs()
{
	srand((unsigned int)(time(NULL)));

	// Percentage of grid with bombs
	double perc_bombs = static_cast<double>(rand()) / RAND_MAX;
	perc_bombs = perc_bombs * (max_bomb_percentage_ - min_bomb_percentage_) + min_bomb_percentage_;
	int bomb_count = 0;

	// Loop through to figure out if that cell will have bomb or not
	auto [rows, cols] = GetSize();
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			double rand_num = static_cast<double>(rand()) / RAND_MAX;
			if (rand_num <= perc_bombs) {
				bombs_.push_back({ i,j });
				cells_[i][j].AddBomb();
			}
		}
	}
}

void Grid::UpdateNeighbourBombCount()
{
	for (auto coords : bombs_) {
		for (auto dir : direction_)
		{
			int i = coords[0] + dir[0];
			int j = coords[1] + dir[1];
			if(IndexOutOfBounds(i,j) || cells_[i][j].HasBomb()) continue;
			cells_[i][j].IncreaseNeighborsCount();
		}
	}
}

bool Grid::FlipFlag(int i, int j)
{
	if (has_won_ || IndexOutOfBounds(i, j) || cells_[i][j].IsExposed())
		return false; // Nothing to do

	if (cells_[i][j].IsFlagged())
	{
		cells_[i][j].UnFlagCell();
		--flagged_count_;
	}
	else
	{
		cells_[i][j].FlagCell();
		++flagged_count_;
	}
	return true;
}

std::pair<bool, std::vector<std::vector<int>>> Grid::Open(int i, int j)
{
	if (has_lost_ || has_won_ || IndexOutOfBounds(i, j) || cells_[i][j].IsExposed() || cells_[i][j].IsFlagged())
		return { false,{} }; // Nothing to do

	vector<vector<int>> visited;
	bool is_not_bomb = OpenUsingDFS(i, j, visited);
	if (!is_not_bomb) {
		has_lost_ = true;
		visited = bombs_;
		for (auto idx : bombs_)
		{
			cells_[idx[0]][idx[1]].ExposeCell();
		}
	}
	else
	{
		CheckIfWon();
	}
	return { is_not_bomb, visited };
}

// Time complexity = O(rows * cols)
// Space Complexity = O(max(rows, cols))
bool Grid::OpenUsingDFS(int i, int j, std::vector<std::vector<int>>& visited)
{
	if (IndexOutOfBounds(i, j) || cells_[i][j].IsExposed()) return true; // true -> is not bomb
	
	// Check if the cell was flagged
	if (cells_[i][j].IsFlagged())
		--flagged_count_;

	// Expose the cell
	visited.push_back({ i,j });
	cells_[i][j].ExposeCell();

	// Check if the cell that we just exposed has a bomb
	if (visited.size() == 1 && cells_[i][j].HasBomb())
		return false; 

	// Check if the cell that we just exposed has a number in it
	if (cells_[i][j].Neighbors() > 0) return true;

	// This cell doesn't have a number in it
	// So, open all the neighboring cells until we reach cells with number
	for (auto dir : direction_)
	{
		int ii = i + dir[0];
		int jj = j + dir[1];
		OpenUsingDFS(ii, jj, visited);
	}
	return true;
}

inline long Grid::IndexSingle(int i, int j)
{
	return cells_.empty() ? -1 : i * static_cast<long>(cells_[0].size()) + j;
}

inline std::pair<int, int> Grid::IndexPair(long idx)
{
	return std::pair<int, int>();
}

bool Grid::CheckIfWon()
{
	if (has_won_) return has_won_;

	// Currently loops throgh everything - need to improve this!
	for (const auto& a : cells_)
	{
		for (const auto& curr_cell : a)
		{
			if (!curr_cell.HasBomb() && !curr_cell.IsExposed())
			{
				has_won_ = false;
				return has_won_;
			}
		}
	}
	has_won_ = true;
	return has_won_;
}

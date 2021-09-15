#pragma once
#include "Cell.h"
#include <vector>

class Grid
{
public:
	Grid() = default;
	void Initialize(int rows, int cols);

	// Open a cell - does a DFS if the cell is empty
	std::pair<bool, std::vector<std::vector<int>>> Open(int i, int j);

	// Cell operation at i and j
	bool FlipFlag(int i, int j);
	const Cell& GetCellAt(int i, int j) const { return cells_[i][j]; }

	// Getters
	bool HasWon() const { return has_won_; }
	bool HasLost() const { return has_lost_; }
	int NumBombs() const { return static_cast<int>(bombs_.size()); }
	int NumFlagged() const { return flagged_count_; }
	int NumLeftToExpose() const { return unexposed_cell_count_; }
	
private:
	void UpdateNeighbourBombCount();
	void DistributeBombs();
	bool OpenUsingDFS(int i, int j, std::vector<std::vector<int>>& visited);
	inline std::pair<int, int> GetSize();
	inline bool IndexOutOfBounds(int i, int j);

private:
	std::vector<std::vector<Cell>> cells_;
	std::vector<std::vector<int>> bombs_;
	const std::vector<std::vector<int>> direction_ = { {-1,-1},{-1,0},{-1,1},{0,-1}, {0,1}, {1,-1},{1,0}, {1,1} };
	double min_bomb_percentage_ = 0.05; // 5%
	double max_bomb_percentage_ = 0.3;  // 30%
	int flagged_count_ = 0;
	int unexposed_cell_count_ = 0;
	bool has_lost_ = false;
	bool has_won_ = false;
};


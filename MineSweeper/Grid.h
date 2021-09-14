#pragma once
#include "Cell.h"
#include <vector>
#include <unordered_set>

class Grid
{
public:
	Grid() = default;
	void Initialize(int rows, int cols);
	bool FlipFlag(int i, int j);
	std::pair<bool, std::vector<std::vector<int>>> Open(int i, int j);
	inline long IndexSingle(int i, int j);
	inline std::pair<int, int> IndexPair(long idx);
	bool CheckIfWon();
	bool HasLost() const { return has_lost_; }
	int NumBombs() const { return static_cast<int>(bombs_.size()); }
	int NumFlagged() const { return flagged_count_; }
	const Cell& GetCellAt(int i, int j) const { return cells_[i][j]; }

private:
	void UpdateNeighbourBombCount();
	void DistributeBombs();
	bool OpenUsingDFS(int i, int j, std::vector<std::vector<int>>& visited);
	inline std::pair<int, int> GetSize();
	inline bool IndexOutOfBounds(int i, int j);
	bool HasWon() const { return has_won_; }



	const std::vector<std::vector<int>> direction_ = { {-1,-1},{-1,0},{-1,1},{0,-1}, {0,1}, {1,-1},{1,0}, {1,1} };
private:
	std::vector<std::vector<Cell>> cells_;
	std::vector<std::vector<int>> bombs_;
	double min_bomb_percentage_ = 0.05; // 5%
	double max_bomb_percentage_ = 0.3;  // 30%
	int flagged_count_ = 0;
	bool has_lost_ = false;
	bool has_won_ = false;

};


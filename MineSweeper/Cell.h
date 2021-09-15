#pragma once

class Cell
{
public:
	enum State{EXPOSED, NOT_EXPOSED, FLAGGED};
	Cell() = default;
	void ExposeCell();
	void FlagCell();
	void UnFlagCell();
	void AddBomb();
	void IncreaseNeighborsCount();

	// Getters
	bool IsExposed() const;
	bool IsFlagged() const;
	bool HasBomb() const;
	int Neighbors() const;

private:
	int neighbors_ = 0;
	State cell_state_ = State::NOT_EXPOSED;
	bool has_bomb_ = false;
};


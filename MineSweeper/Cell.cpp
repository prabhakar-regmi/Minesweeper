#include "stdafx.h"
#include "Cell.h"

void Cell::ExposeCell()
{
	cell_state_ = State::EXPOSED;
}

void Cell::FlagCell()
{
	if (cell_state_ == State::NOT_EXPOSED) cell_state_ = State::FLAGGED;
}

void Cell::UnFlagCell()
{
	if (cell_state_ == State::FLAGGED) cell_state_ = State::NOT_EXPOSED;
}

void Cell::StartProcessing()
{
	cell_state_ = State::PROCESSING;
}

void Cell::AddBomb()
{
	has_bomb_ = true;
}

void Cell::IncreaseNeighborsCount()
{
	neighbors_++;
}

bool Cell::IsExposed() const
{
	return cell_state_ == State::EXPOSED;
}

bool Cell::IsFlagged() const
{
	return cell_state_ == State::FLAGGED;
}

bool Cell::IsNotProcessing() const
{
	return cell_state_ != State::PROCESSING;
}

bool Cell::HasBomb() const
{
	return has_bomb_;
}

bool Cell::HasNonBombNeighbor() const
{
	return (!has_bomb_ && IsNotProcessing() && !IsExposed());
}

int Cell::Neighbors() const
{
	return neighbors_;
}

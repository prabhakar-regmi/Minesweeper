#pragma once
struct Measurement
{
	const int left_;
	const int right_;
	const int top_;
	const int bottom_;
	Measurement(int left, int top, int right, int bottom):
		left_(left), top_(top), right_(right), bottom_(bottom)
	{}
	int GetLength() const { return right_ - left_; }
};

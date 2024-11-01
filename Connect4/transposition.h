#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#include <vector>
#include <cstdint>

namespace Transposition
{
	enum class ScoreType
	{
		Exact,
		LowerBound,
		UpperBound
	};

	struct TableEntry
	{
		int score{};
		int depth{};
		ScoreType type{};
	};

	// Initialise the Zobrist table
	void initialiseZobrist();

	// Compute the Zobrist hash for the current board state
	uint64_t computeHash(const std::vector<std::vector<char>>& board);

	// Check if the current hash is in the transposition table
	bool lookup(uint64_t hash, int& score, int alpha, int beta, int depth);

	// Store a score in the transposition table for a given hash
	void store(uint64_t hash, int score, ScoreType type, int depth);
}

#endif

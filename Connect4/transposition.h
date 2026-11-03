#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#include <cstdint>

namespace Transposition
{
	enum class ScoreType : std::uint8_t
	{
		Exact,
		LowerBound,
		UpperBound
	};

	struct TableEntry
	{
		std::uint64_t key{};       // full hash, to detect index collisions
		int score{};
		std::int8_t depth{ -1 };   // -1 = empty slot
		ScoreType type{};
		std::int8_t bestMove{ -1 }; // best column found at this node, -1 if none
	};

	// Initialise the Zobrist keys and allocate the table (safe to call more than once)
	void initialiseZobrist();

	// Remove every entry from the transposition table
	void clear();

	// Zobrist key for a piece of the given player (0 or 1) at (row, col).
	// XOR it into the hash when the piece is placed and again when it is removed.
	std::uint64_t pieceKey(int player, int row, int col);

	// Copy the entry for this hash into `out`. Returns false if there is none.
	bool probe(std::uint64_t hash, TableEntry& out);

	// Store a result for this hash
	void store(std::uint64_t hash, int depth, int score, ScoreType type, int bestMove);
}

#endif

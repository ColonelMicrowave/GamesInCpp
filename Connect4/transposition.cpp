#include <array>
#include <cstdint>
#include <limits>
#include <vector>
#include "random.h"
#include "transposition.h"
#include "config.h"

namespace
{
	// 2^21 entries * 16 bytes = 32 MB. Must be a power of two.
	constexpr std::size_t TABLE_SIZE{ std::size_t{ 1 } << 21 };

	std::array<std::array<std::uint64_t, Config::CELLS>, 2> zobristTable{};
	std::vector<Transposition::TableEntry> transpositionTable{};
	bool initialised{ false };

	Transposition::TableEntry& slotFor(std::uint64_t hash)
	{
		return transpositionTable[hash & (TABLE_SIZE - 1)];
	}
}

namespace Transposition
{
	void initialiseZobrist()
	{
		if (initialised)
			return;

		for (auto& playerKeys : zobristTable)
			for (auto& key : playerKeys)
				key = Random::get(std::uint64_t{ 0 }, std::numeric_limits<std::uint64_t>::max());

		transpositionTable.assign(TABLE_SIZE, TableEntry{});
		initialised = true;
	}

	void clear()
	{
		transpositionTable.assign(TABLE_SIZE, TableEntry{});
	}

	std::uint64_t pieceKey(int player, int row, int col)
	{
		return zobristTable[player][row * Config::COLS + col];
	}

	bool probe(std::uint64_t hash, TableEntry& out)
	{
		const TableEntry& entry{ slotFor(hash) };
		if (entry.depth < 0 || entry.key != hash)
			return false;
		out = entry;
		return true;
	}

	void store(std::uint64_t hash, int depth, int score, ScoreType type, int bestMove)
	{
		TableEntry& entry{ slotFor(hash) };

		// Overwrite an entry for a different position (it's probably from an older search).
		// For the same position, only overwrite with an equal or deeper search.
		if (entry.key == hash && entry.depth > depth)
			return;

		entry = { hash, score, static_cast<std::int8_t>(depth), type, static_cast<std::int8_t>(bestMove) };
	}
}

#include <unordered_map>
#include <limits>
#include <iostream>
#include "random.h"
#include "transposition.h"
#include "config.h"

namespace
{
	std::vector<std::vector<uint64_t>> zobristTable{};
	std::unordered_map<uint64_t, Transposition::TableEntry> transpositionTable{};
}

namespace Transposition
{
	void initialiseZobrist()
	{
		zobristTable.resize(2, std::vector<uint64_t>(Config::ROWS * Config::COLS));
		for (int player{ 0 }; player < 2; ++player)
		{
			for (int cell{ 0 }; cell < Config::ROWS * Config::COLS; ++cell)
			{
				zobristTable[player][cell] = Random::get(static_cast<uint64_t>(0), std::numeric_limits<uint64_t>::max());
			}
		}
	}

	uint64_t computeHash(const std::vector<std::vector<char>>& board)
	{
		uint64_t hash{ 0 };
		for (int row{ 0 }; row < Config::ROWS; ++row)
		{
			for (int col{ 0 }; col < Config::COLS; ++col)
			{
				if (board[row][col] == Config::PLAYER1)
					hash ^= zobristTable[0][row * Config::COLS + col];
				else if (board[row][col] == Config::PLAYER2)
					hash ^= zobristTable[1][row * Config::COLS + col];
			}
		}
		return hash;
	}

	bool lookup(uint64_t hash, int& score, int alpha, int beta, int depth)
	{
		auto it{ transpositionTable.find(hash) };
		if (it != transpositionTable.end())
		{
			const TableEntry& entry{ it->second };

			if (entry.depth >= depth)
			{
				if (entry.type == ScoreType::Exact)
				{
					score = entry.score;
					return true;
				}
				else if (entry.type == ScoreType::LowerBound && entry.score > alpha)
				{
					alpha = entry.score;
				}
				else if (entry.type == ScoreType::UpperBound && entry.score < beta)
				{
					beta = entry.score;
				}
				if (alpha >= beta)
				{
					score = entry.score;
					return true;
				}
			}
		}
		return false;
	}

	void store(uint64_t hash, int score, ScoreType type, int depth)
	{
		auto it{ transpositionTable.find(hash) };

		if (it == transpositionTable.end() || it->second.depth < depth)
		{
			transpositionTable[hash] = { score, depth, type };
		}
	}
}

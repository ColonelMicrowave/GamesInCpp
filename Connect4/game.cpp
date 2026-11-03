#include <iostream>
#include <array>
#include <initializer_list>
#include <algorithm>
#include <cstdint>
#include <limits>
#include <string>
#include "random.h"
#include "transposition.h"
#include "config.h"

void setColour(int colour)
{
	std::cout << "\033[" << colour << "m";
}

void resetColour()
{
	std::cout << "\033[0m";
}

namespace Score
{
	// Win/loss scores are WIN minus the number of plies from the root, so a
	// quicker win scores higher and a slower loss scores higher (less negative).
	inline constexpr int WIN{ 1'000'000 };
	inline constexpr int INF{ WIN + 1 };
	// Any score with |score| >= this is a forced win/loss, not a heuristic value
	inline constexpr int WIN_THRESHOLD{ WIN - Config::CELLS - 1 };

	bool isWinOrLoss(int score)
	{
		return score >= WIN_THRESHOLD || score <= -WIN_THRESHOLD;
	}

	// A win/loss score depends on the ply it was found at, but the same position can
	// be reached at a different ply. Store it as "distance from this node" in the
	// table, and convert it back to "distance from the root" when reading it.
	int toTable(int score, int ply)
	{
		if (score >= WIN_THRESHOLD) return score + ply;
		if (score <= -WIN_THRESHOLD) return score - ply;
		return score;
	}

	int fromTable(int score, int ply)
	{
		if (score >= WIN_THRESHOLD) return score - ply;
		if (score <= -WIN_THRESHOLD) return score + ply;
		return score;
	}
}

// Columns ordered from the centre outwards: 3, 2, 4, 1, 5, 0, 6 for 7 columns
constexpr std::array<int, Config::COLS> centreOrder()
{
	std::array<int, Config::COLS> order{};
	int idx{ 0 };
	order[idx++] = Config::COLS / 2;
	for (int offset{ 1 }; idx < Config::COLS; ++offset)
	{
		if (Config::COLS / 2 - offset >= 0)
			order[idx++] = Config::COLS / 2 - offset;
		if (Config::COLS / 2 + offset < Config::COLS && idx < Config::COLS)
			order[idx++] = Config::COLS / 2 + offset;
	}
	return order;
}

class Connect4
{
private:
	using Board = std::array<std::array<char, Config::COLS>, Config::ROWS>;
	using MoveList = std::array<int, Config::COLS>;

	Board board{};
	std::array<int, Config::COLS> nextRow{}; // lowest empty row in each column, -1 if full
	std::uint64_t hash{ 0 };                 // Zobrist hash, updated on every move
	int moveCount{ 0 };

	char aiPlayer{ Config::PLAYER2 };
	char humanPlayer{ Config::PLAYER1 };

	static constexpr MoveList CENTRE_ORDER{ centreOrder() };

public:
	Connect4()
	{
		Transposition::initialiseZobrist();
		Transposition::clear();
		for (auto& row : board)
			row.fill(Config::EMPTY);
		nextRow.fill(Config::ROWS - 1);
	}

	void displayBoard() const
	{
		std::cout << "  ";
		for (int col{ 0 }; col < Config::COLS; ++col)
			std::cout << col + 1 << " ";
		std::cout << "\n";

		for (const auto& row : board)
		{
			std::cout << "| ";
			for (char cell : row)
			{
				if (cell == Config::PLAYER1)
					setColour(31);
				else if (cell == Config::PLAYER2)
					setColour(33);
				std::cout << cell << " ";
				resetColour();
			}
			std::cout << "|\n";
		}
		std::cout << "  ";

		for (int col{ 0 }; col < Config::COLS; ++col)
			std::cout << "--";
		std::cout << "\n";
	}

	bool isValidMove(int col) const
	{
		return col >= 0 && col < Config::COLS && nextRow[col] >= 0;
	}

	bool makeMove(int col, char player)
	{
		if (!isValidMove(col))
			return false;

		const int row{ nextRow[col]-- };
		board[row][col] = player;
		hash ^= Transposition::pieceKey(player == Config::PLAYER1 ? 0 : 1, row, col);
		++moveCount;
		return true;
	}

	void undoMove(int col)
	{
		const int row{ ++nextRow[col] };
		const char player{ board[row][col] };
		hash ^= Transposition::pieceKey(player == Config::PLAYER1 ? 0 : 1, row, col);
		board[row][col] = Config::EMPTY;
		--moveCount;
	}

	// True if the top piece in `col` (i.e. the piece just played there) makes four in a row.
	// Only the lines through that piece need checking.
	bool lastMoveWon(int col) const
	{
		if (col < 0 || col >= Config::COLS)
			return false;

		const int row{ nextRow[col] + 1 };
		if (row >= Config::ROWS)
			return false; // column is empty

		const char player{ board[row][col] };
		constexpr int directions[4][2]{ { 0, 1 }, { 1, 0 }, { 1, 1 }, { 1, -1 } };

		for (const auto& dir : directions)
		{
			int count{ 1 };
			for (int sign : { 1, -1 })
			{
				int r{ row + sign * dir[0] };
				int c{ col + sign * dir[1] };
				while (r >= 0 && r < Config::ROWS && c >= 0 && c < Config::COLS && board[r][c] == player)
				{
					++count;
					r += sign * dir[0];
					c += sign * dir[1];
				}
			}
			if (count >= 4)
				return true;
		}
		return false;
	}

	bool isBoardFull() const
	{
		return moveCount == Config::CELLS;
	}

	// Heuristic score for a line of 4 cells that contains no four-in-a-row
	// (wins are detected before the board is ever evaluated)
	static int evaluationWindow(int aiCount, int humanCount)
	{
		const int emptyCount{ 4 - aiCount - humanCount };

		if (aiCount == 3 && emptyCount == 1) return 6;
		if (aiCount == 2 && emptyCount == 2) return 3;
		if (humanCount == 3 && emptyCount == 1) return -8;
		if (humanCount == 2 && emptyCount == 2) return -5;
		return 0;
	}

	// Heuristic score from the AI's point of view
	int evaluateBoard() const
	{
		int score{ 0 };

		// Score centre column
		for (int row{ 0 }; row < Config::ROWS; ++row)
		{
			if (board[row][Config::COLS / 2] == aiPlayer)
				score += 3;
			else if (board[row][Config::COLS / 2] == humanPlayer)
				score -= 3;
		}

		// Score every line of 4: horizontal, vertical and both diagonals
		constexpr int directions[4][2]{ { 0, 1 }, { 1, 0 }, { 1, 1 }, { -1, 1 } };
		for (const auto& dir : directions)
		{
			for (int row{ 0 }; row < Config::ROWS; ++row)
			{
				for (int col{ 0 }; col < Config::COLS; ++col)
				{
					const int endRow{ row + 3 * dir[0] };
					const int endCol{ col + 3 * dir[1] };
					if (endRow < 0 || endRow >= Config::ROWS || endCol >= Config::COLS)
						continue;

					int aiCount{ 0 };
					int humanCount{ 0 };
					for (int i{ 0 }; i < 4; ++i)
					{
						const char cell{ board[row + i * dir[0]][col + i * dir[1]] };
						if (cell == aiPlayer)
							++aiCount;
						else if (cell == humanPlayer)
							++humanCount;
					}
					score += evaluationWindow(aiCount, humanCount);
				}
			}
		}
		return score;
	}

	// Fill `moves` with the valid columns: `firstMove` (if valid) first, then centre-out.
	// Returns how many moves were written.
	int getOrderedMoves(int firstMove, MoveList& moves) const
	{
		int count{ 0 };
		if (isValidMove(firstMove))
			moves[count++] = firstMove;

		for (int col : CENTRE_ORDER)
			if (col != firstMove && isValidMove(col))
				moves[count++] = col;

		return count;
	}

	// depth = plies left to search, ply = plies from the root,
	// lastCol = the column the previous player just played in
	int minimax(int depth, int ply, int alpha, int beta, bool maximisingPlayer, int lastCol)
	{
		// Did the previous move win? If so the player to move here has lost.
		if (lastMoveWon(lastCol))
			return maximisingPlayer ? -(Score::WIN - ply) : (Score::WIN - ply);
		if (isBoardFull())
			return 0; // draw
		if (depth == 0)
			return evaluateBoard();

		// Remember the window before the table narrows it, to classify the result later
		const int alphaOrig{ alpha };
		const int betaOrig{ beta };

		// Transposition table lookup
		int ttMove{ -1 };
		Transposition::TableEntry entry{};
		if (Transposition::probe(hash, entry))
		{
			ttMove = entry.bestMove;
			if (entry.depth >= depth)
			{
				const int ttScore{ Score::fromTable(entry.score, ply) };
				if (entry.type == Transposition::ScoreType::Exact)
					return ttScore;
				if (entry.type == Transposition::ScoreType::LowerBound)
					alpha = std::max(alpha, ttScore);
				else
					beta = std::min(beta, ttScore);
				if (alpha >= beta)
					return ttScore;
			}
		}

		const char mover{ maximisingPlayer ? aiPlayer : humanPlayer };
		int best{ maximisingPlayer ? -Score::INF : Score::INF };
		int bestMove{ -1 };

		MoveList moves{};
		const int moveCountHere{ getOrderedMoves(ttMove, moves) };
		for (int i{ 0 }; i < moveCountHere; ++i)
		{
			const int col{ moves[i] };
			makeMove(col, mover);
			const int eval{ minimax(depth - 1, ply + 1, alpha, beta, !maximisingPlayer, col) };
			undoMove(col);

			if (maximisingPlayer)
			{
				if (eval > best)
				{
					best = eval;
					bestMove = col;
				}
				alpha = std::max(alpha, best);
			}
			else
			{
				if (eval < best)
				{
					best = eval;
					bestMove = col;
				}
				beta = std::min(beta, best);
			}

			if (alpha >= beta)
				break;
		}

		// Classify against the ORIGINAL window: at or below alpha it's an upper bound,
		// at or above beta it's a lower bound, otherwise it's exact.
		Transposition::ScoreType type{ Transposition::ScoreType::Exact };
		if (best <= alphaOrig)
			type = Transposition::ScoreType::UpperBound;
		else if (best >= betaOrig)
			type = Transposition::ScoreType::LowerBound;

		Transposition::store(hash, depth, Score::toTable(best, ply), type, bestMove);
		return best;
	}

	int getAIMove(char ai, char human)
	{
		aiPlayer = ai;
		humanPlayer = human;

		// Dynamic depth (in plies) based on the number of filled cells. Tweak if the AI is too slow;
		// the search is a lot faster than before, so you may be able to raise these.
		const int maxDepth
		{
			(moveCount < 2) ? 8 :
			(moveCount < 7) ? 10 :
			(moveCount < 10) ? 11 :
			(moveCount < 12) ? 13 :
			(moveCount < 16) ? 15 :
			(moveCount < 22) ? 19 : Config::CELLS
		};
		const int emptyCells{ Config::CELLS - moveCount };

		MoveList moves{};
		getOrderedMoves(-1, moves);
		int bestMove{ moves[0] };

		for (int depth{ 1 }; depth <= std::min(maxDepth, emptyCells); ++depth)
		{
			// Each iteration starts fresh: the deepest finished search decides the move
			int iterationBest{ -Score::INF };
			int iterationMove{ -1 };
			int alpha{ -Score::INF };

			// Try the previous iteration's best move first
			const int count{ getOrderedMoves(bestMove, moves) };
			for (int i{ 0 }; i < count; ++i)
			{
				const int col{ moves[i] };
				makeMove(col, aiPlayer);
				// Pass the best score so far as alpha so other root moves can be pruned
				const int score{ minimax(depth - 1, 1, alpha, Score::INF, false, col) };
				undoMove(col);

				if (score > iterationBest)
				{
					iterationBest = score;
					iterationMove = col;
				}
				alpha = std::max(alpha, iterationBest);
			}

			bestMove = iterationMove;

			// A forced win or loss has been found; searching deeper won't change it
			if (Score::isWinOrLoss(iterationBest))
				break;
		}

		return bestMove;
	}
};

void ignoreLine()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void playGame(bool aiEnabled = false, bool isPlayerOne = true)
{
	Connect4 game{};
	char currentPlayer{ Config::PLAYER1 };
	const char aiPlayer{ isPlayerOne ? Config::PLAYER2 : Config::PLAYER1 };
	const char humanPlayer{ isPlayerOne ? Config::PLAYER1 : Config::PLAYER2 };

	while (true)
	{
		game.displayBoard();
		int col{};

		if (aiEnabled && currentPlayer == aiPlayer)
		{
			// AI's turn
			col = game.getAIMove(aiPlayer, humanPlayer);
			game.makeMove(col, aiPlayer);
			setColour(35);
			std::cout << "AI plays in column " << col + 1 << "\n";
			resetColour();
		}
		else
		{
			// Player's turn
			while (true)
			{
				std::cout << "Player " << currentPlayer << ", enter your move (1-" << Config::COLS << "): ";
				int input{};
				std::cin >> input;

				if (!std::cin)
				{
					if (std::cin.eof())
						return; // input closed, nothing more to read
					std::cin.clear();
					ignoreLine();
					std::cout << "Invalid input. Please try again.\n";
					continue;
				}
				ignoreLine();

				if (input < 1 || input > Config::COLS)
				{
					std::cout << "Invalid input. Please try again.\n";
					continue;
				}

				col = input - 1;
				if (game.makeMove(col, currentPlayer))
					break;
				std::cout << "Column " << input << " is full. Please try again.\n";
			}
		}

		if (game.lastMoveWon(col))
		{
			game.displayBoard();
			setColour(currentPlayer == Config::PLAYER1 ? 31 : 33);
			std::cout << "Player " << currentPlayer << " wins!\n";
			resetColour();
			break;
		}

		if (game.isBoardFull())
		{
			game.displayBoard();
			setColour(34);
			std::cout << "It's a tie! The board is full.\n";
			resetColour();
			break;
		}

		currentPlayer = (currentPlayer == Config::PLAYER1) ? Config::PLAYER2 : Config::PLAYER1;
	}
}

bool askYesNo(const std::string& prompt)
{
	while (true)
	{
		std::cout << prompt << " (y/n): ";
		char choice{};
		std::cin >> choice;

		if (!std::cin)
		{
			if (std::cin.eof())
				return false; // input closed
			std::cin.clear();
			ignoreLine();
			std::cout << "Invalid input. Please try again.\n";
			continue;
		}

		ignoreLine();

		if (choice == 'y' || choice == 'Y')
			return true;
		if (choice == 'n' || choice == 'N')
			return false;
		std::cout << "Invalid input. Please try again.\n";
	}
}

int main()
{
	if (!askYesNo("Do you want to play against an AI?"))
		playGame(); // Human vs Human
	else
		playGame(true, askYesNo("Do you want to be player 1?"));
	return 0;
}

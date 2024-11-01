#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <limits>
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

class Connect4
{
private:
	std::vector<std::vector<char>> board;

public:
	Connect4()
	{
		Transposition::initialiseZobrist();
		board = std::vector<std::vector<char>>(Config::ROWS, std::vector<char>(Config::COLS, Config::EMPTY));
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
				{
					setColour(31);
					std::cout << cell << " ";
				}
				else if (cell == Config::PLAYER2)
				{
					setColour(33);
					std::cout << cell << " ";
				}
				else
				{
					std::cout << cell << " ";
				}
				resetColour();
			}
			std::cout << "|\n";
		}
		std::cout << "  ";

		for (int col{ 0 }; col < Config::COLS; ++col)
			std::cout << "--";
		std::cout << "\n";
	}

	bool makeMove(int col, char player)
	{
		if (col < 0 || col >= Config::COLS || board[0][col] != Config::EMPTY)
			return false; // Invalid move

		for (int row{ Config::ROWS - 1 }; row >= 0; --row)
		{
			if (board[row][col] == Config::EMPTY)
			{
				board[row][col] = player;
				return true;
			}
		}
		return false;
	}

	void undoMove(int col)
	{
		for (int row{ 0 }; row < Config::ROWS; ++row)
		{
			if (board[row][col] != Config::EMPTY)
			{
				board[row][col] = Config::EMPTY;
				break;
			}
		}
	}

	bool checkWin(char player)
	{
		//Horizontal check
		for (int row{ 0 }; row < Config::ROWS; ++row)
		{
			for (int col{ 0 }; col < Config::COLS - 3; ++col)
			{
				if (
					board[row][col]     == player &&
					board[row][col + 1] == player &&
					board[row][col + 2] == player &&
					board[row][col + 3] == player
					)
					return true;
			}
		}

		//Vertical check
		for (int row{ 0 }; row < Config::ROWS - 3; ++row)
		{
			for (int col{ 0 }; col < Config::COLS; ++col)
			{
				if (
					board[row][col]     == player &&
					board[row + 1][col] == player &&
					board[row + 2][col] == player &&
					board[row + 3][col] == player
					)
					return true;
			}
		}

		// Diagonal check (bottom left to top right)
		for (int row{ 3 }; row < Config::ROWS; ++row)
		{
			for (int col{ 0 }; col < Config::COLS - 3; ++col)
			{
				if (
					board[row][col]         == player &&
					board[row - 1][col + 1] == player &&
					board[row - 2][col + 2] == player &&
					board[row - 3][col + 3] == player
					)
					return true;
			}
		}

		// Diagonal check (top left to bottom right)
		for (int row{ 0 }; row < Config::ROWS - 3; ++row)
		{
			for (int col{ 0 }; col < Config::COLS - 3; ++col)
			{
				if (
					board[row][col]         == player &&
					board[row + 1][col + 1] == player &&
					board[row + 2][col + 2] == player &&
					board[row + 3][col + 3] == player
					)
					return true;
			}
		}
		return false;
	}

	bool isBoardFull() const
	{
		for (int col{ 0 }; col < Config::COLS; ++col)
		{
			if (board[0][col] == Config::EMPTY)
				return false;
		}
		return true;
	}

	int evaluationWindow(const std::vector<char>& window, char aiPlayer, char humanPlayer) const
	{
		int score{ 0 };
		int aiCount{ 0 };
		int humanCount{ 0 };
		int emptyCount{ 0 };

		for (char cell : window)
		{
			if (cell == Config::EMPTY)
				++emptyCount;
			else if (cell == aiPlayer)
				++aiCount;
			else if (cell == humanPlayer)
				++humanCount;
		}

		if (aiCount == 4) score += 1000000;
		else if (humanCount == 4) score -= 1000000;
		else if (aiCount == 3 && emptyCount == 1) score += 5;
		else if (aiCount == 2 && emptyCount == 2) score += 2;
		else if (humanCount == 3 && emptyCount == 1) score -= 4;
		else if (humanCount == 2 && emptyCount == 2) score -= 1;

		return score;
	}

	int evaluateBoard(char aiPlayer, char humanPlayer) const
	{
		int score{ 0 };

		// Score center column
		for (int row{ 0 }; row < Config::ROWS; ++row)
		{
			if (board[row][Config::COLS / 2] == aiPlayer)
				score += 3;
			else if (board[row][Config::COLS / 2] == humanPlayer)
				score -= 3;
		}

		// Score horizontal
		for (int row{ 0 }; row < Config::ROWS; ++row)
		{
			for (int col{ 0 }; col < Config::COLS - 3; ++col)
			{
				std::vector<char> window{};
				for (int i{ 0 }; i < 4; ++i)
					window.push_back(board[row][col + i]);
				score += evaluationWindow(window, aiPlayer, humanPlayer);
			}
		}

		// Score vertical
		for (int row{ 0 }; row < Config::ROWS - 3; ++row)
		{
			for (int col{ 0 }; col < Config::COLS; ++col)
			{
				std::vector<char> window{};
				for (int i{ 0 }; i < 4; ++i)
					window.push_back(board[row + i][col]);
				score += evaluationWindow(window, aiPlayer, humanPlayer);
			}
		}

		// Score diagonal (bottom left to top right)
		for (int row{ 3 }; row < Config::ROWS; ++row)
		{
			for (int col{ 0 }; col < Config::COLS - 3; ++col)
			{
				std::vector<char> window{};
				for (int i{ 0 }; i < 4; ++i)
					window.push_back(board[row - i][col + i]);
				score += evaluationWindow(window, aiPlayer, humanPlayer);
			}
		}

		// Score diagonal (top left to bottom right)
		for (int row{ 0 }; row < Config::ROWS - 3; ++row)
		{
			for (int col{ 0 }; col < Config::COLS - 3; ++col)
			{
				std::vector<char> window{};
				for (int i{ 0 }; i < 4; ++i)
					window.push_back(board[row + i][col + i]);
				score += evaluationWindow(window, aiPlayer, humanPlayer);
			}
		}
		return score;
	}

	std::vector<int> getValidMoves() const
	{
		std::vector<int> validMoves{};
		for (int col{ 0 }; col < Config::COLS; ++col)
			if (board[0][col] == Config::EMPTY)
				validMoves.push_back(col);
		return validMoves;
	}

	std::vector<int> getOrderedMoves(char aiPlayer, char humanPlayer)
	{
		std::vector<int> validMoves{ getValidMoves() };

		// Score each move for ordering (preferring centre columns)
		std::vector<std::pair<int, int>> scoredMoves{};
		for (int col : validMoves)
		{
			makeMove(col, aiPlayer);
			int moveScore{ evaluateBoard(aiPlayer, humanPlayer) };
			undoMove(col);

			// Centre moves get higher initial score
			int centreBias{ Config::COLS / 2 - abs(col - Config::COLS / 2) };
			moveScore += centreBias;
			scoredMoves.push_back({ moveScore, col });
		}

		// Sort moves by score in descending order
		std::sort(scoredMoves.begin(), scoredMoves.end(), std::greater<>());

		// Extract ordered moves
		std::vector<int> orderedMoves{};
		for (const auto& move : scoredMoves)
			orderedMoves.push_back(move.second);

		return orderedMoves;
	}

	int minimax(int depth, int alpha, int beta, bool maximisingPlayer, char aiPlayer, char humanPlayer)
	{
		// Transposition table lookup
		uint64_t hash{ Transposition::computeHash(board) };
		int score{};

		if (checkWin(humanPlayer))
		{
			score = -1000000;
			Transposition::store(hash, score, Transposition::ScoreType::Exact, depth);
			return score;
		}
		if (checkWin(aiPlayer))
		{
			score = 1000000;
			Transposition::store(hash, score, Transposition::ScoreType::Exact, depth);
			return score;
		}
		if (isBoardFull() || depth == 0)
		{
			score = evaluateBoard(aiPlayer, humanPlayer);
			Transposition::store(hash, score, Transposition::ScoreType::Exact, depth);
			return score;
		}

		if (Transposition::lookup(hash, score, alpha, beta, depth))
		{
			return score;
		}

		if (maximisingPlayer)
		{
			int maxEval = std::numeric_limits<int>::min();
			for (int col : getOrderedMoves(aiPlayer, humanPlayer))
			{
				makeMove(col, aiPlayer);
				int eval = minimax(depth - 1, alpha, beta, false, aiPlayer, humanPlayer);
				undoMove(col);
				maxEval = std::max(maxEval, eval);
				alpha = std::max(alpha, eval);
				if (beta <= alpha)
					break;
			}
			if (alpha >= beta)
				Transposition::store(hash, maxEval, Transposition::ScoreType::LowerBound, depth);
			else
				Transposition::store(hash, maxEval, Transposition::ScoreType::Exact, depth);
			return maxEval;
		}
		else
		{
			int minEval = std::numeric_limits<int>::max();
			for (int col : getOrderedMoves(aiPlayer, humanPlayer))
			{
				makeMove(col, humanPlayer);
				int eval = minimax(depth - 1, alpha, beta, true, aiPlayer, humanPlayer);
				undoMove(col);
				minEval = std::min(minEval, eval);
				beta = std::min(beta, eval);
				if (beta <= alpha)
					break;
			}
			if (alpha >= beta)
				Transposition::store(hash, minEval, Transposition::ScoreType::UpperBound, depth);
			else
				Transposition::store(hash, minEval, Transposition::ScoreType::Exact, depth);
			return minEval;
		}
	}

	int getAIMove(char aiPlayer, char humanPlayer)
	{
		int bestMove{};
		int bestScore = std::numeric_limits<int>::min();

		// Dynamic depth based on number of filled cells
		int filledCells{ 0 };
		for (const auto& row : board)
			for (char cell : row)
				if (cell != Config::EMPTY)
					++filledCells;

		int depth // Tweak values if AI too slow
		{ 
			(filledCells < 2) ? 6 :
			(filledCells < 7) ? 8 :
			(filledCells < 12) ? 10 :
			(filledCells < 16) ? 11 :
			(filledCells < 18) ? 12 :
			(filledCells < 22) ? 15 : 25
		};
		for (int col : getOrderedMoves(aiPlayer, humanPlayer))
		{
			makeMove(col, aiPlayer);
			int score = minimax(depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false, aiPlayer, humanPlayer);
			undoMove(col);

			if (score > bestScore)
			{
				bestScore = score;
				bestMove = col;
			}
		}
		std::cout << "Best score: " << bestScore<< "\n";
		std::cout << "Depth: " << depth << "\n";
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
	char aiPlayer     { isPlayerOne ? Config::PLAYER2 : Config::PLAYER1 };
	char humanPlayer  { isPlayerOne ? Config::PLAYER1 : Config::PLAYER2 };
	int move{};

	while (true)
	{
		game.displayBoard();

		if (aiEnabled && currentPlayer == aiPlayer)
		{
			// AI's turn
			move = game.getAIMove(aiPlayer, humanPlayer);
			game.makeMove(move, aiPlayer);
			std::cout << "AI plays in column " << move + 1 << "\n";
		}
		else
		{
			// Player's turn
			bool validMove{ false };
			while (!validMove)
			{
				std::cout << "Player " << currentPlayer << ", enter your move (1-7): ";
				std::cin >> move;

				if (std::cin.fail() || move < 1 || move > 7)
				{
					std::cin.clear();
					ignoreLine();
					std::cout << "Invalid input. Please try again.\n";
					continue;
				}

				ignoreLine();

				if (!game.makeMove(move - 1, currentPlayer))
					std::cout << "Column " << move << " is full. Please try again.\n";
				else
					validMove = true;
			}
		}

		if (game.checkWin(currentPlayer))
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

bool isAIEnabled()
{
	while (true)
	{
		std::cout << "Do you want to play against an AI? (y/n): ";
		char choice{};
		std::cin >> choice;

		if (std::cin.fail())
		{
			std::cin.clear();
			ignoreLine();
			std::cout << "Invalid input. Please try again.\n";
			continue;
		}

		ignoreLine();

		if (choice == 'y' || choice == 'Y')
			return true;
		else if (choice == 'n' || choice == 'N')
			return false;
		else
			std::cout << "Invalid input. Please try again.\n";
	}
}

bool isPlayerOne()
{
	while (true)
	{
		std::cout << "Do you want to be player 1? (y/n): ";
		char choice{};
		std::cin >> choice;

		if (std::cin.fail())
		{
			std::cin.clear();
			ignoreLine();
			std::cout << "Invalid input. Please try again.\n";
			continue;
		}

		ignoreLine();

		if (choice == 'y' || choice == 'Y')
			return true;
		else if (choice == 'n' || choice == 'N')
			return false;
		else
			std::cout << "Invalid input. Please try again.\n";
	}
}

int main()
{
	if (!isAIEnabled())
		playGame(); // Human vs Human
	else
	{
		if (isPlayerOne())
			playGame(true, true); // Human vs AI
		else
			playGame(true, false); // AI vs Human
	}
	return 0;
}

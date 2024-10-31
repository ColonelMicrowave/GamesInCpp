#include <iostream>
#include <vector>
#include <limits>
#include "random.h"

namespace Config
{
	const int ROWS{ 6 };
	const int COLS{ 7 };
	const char EMPTY{ '.' };
	const char PLAYER1{ 'X' };
	const char PLAYER2{ 'O' };
}

class Connect4
{
private:
	std::vector<std::vector<char>> board;

public:
	Connect4()
	{
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
				std::cout << cell << " ";
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

	int getAIMove(char aiPlayer, char opponent)
	{
		// Check for winning move
		for (int col{ 0 }; col < Config::COLS; ++col)
		{
			if (makeMove(col, aiPlayer))
			{
				if (checkWin(aiPlayer))
				{
					undoMove(col); // Undo the move after checking
					return col;
				}
				undoMove(col); // Undo the move after checking
			}
		}

		// Check for opponent winning move and block it
		for (int col{ 0 }; col < Config::COLS; ++col)
		{
			if (makeMove(col, opponent))
			{
				if (checkWin(opponent))
				{
					undoMove(col); // Undo the move after checking
					return col;
				}
				undoMove(col); // Undo the move after checking
			}
		}

		// Choose a random move if no winning move is found
		int col{};
		do
		{
			col = Random::get(0, Config::COLS - 1);
		} while (!makeMove(col, Config::EMPTY)); // Keep generating random moves until a valid move is found
		return col;
	}
};

void ignoreLine()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void playGame(bool aiEnabled = false, bool isPlayerOne = true)
{
	Connect4 game{};
	char humanPlayer  { isPlayerOne ? Config::PLAYER1 : Config::PLAYER2 };
	char aiPlayer     { isPlayerOne ? Config::PLAYER2 : Config::PLAYER1 };
	char currentPlayer{ Config::PLAYER1 };
	int move{};

	while (true)
	{
		game.displayBoard();

		if (aiEnabled && currentPlayer == aiPlayer)
		{
			// AI's turn
			move = game.getAIMove(Config::PLAYER2, Config::PLAYER1);
			game.makeMove(move, Config::PLAYER2);
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

				if (std::cin.fail())
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
			std::cout << "Player " << currentPlayer << " wins!\n";
			break;
		}

		if (game.isBoardFull())
		{
			game.displayBoard();
			std::cout << "It's a tie! The board is full.\n";
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
	bool aiEnabled{ isAIEnabled() };

	if (!aiEnabled)
		playGame();
	else
	{
		bool playerOne{ isPlayerOne() };
		playGame(aiEnabled, playerOne);
	}

	return 0;
}

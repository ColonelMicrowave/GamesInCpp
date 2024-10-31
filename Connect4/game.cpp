#include <iostream>
#include <vector>
#include <limits>

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
};

void ignoreLine()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void playGame()
{
	Connect4 game{};
	char currentPlayer{ Config::PLAYER1 };
	int move{};

	while (true)
	{
		game.displayBoard();

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

int main()
{
	playGame();

	return 0;
}

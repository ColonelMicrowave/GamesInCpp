#include <iostream>
#include <limits>

void ignoreLine()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printBoardState(char board[3][3])
{
	std::cout << "\n";

	// Print the board state
	// Iterates through each row, then each column to print the board
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			std::cout << board[i][j];
			if (j < 2) 
				std::cout << " | "; // Print column separators
		}
		std::cout << "\n";
		if (i < 2) 
			std::cout << "---------\n"; // Print row separator
	}
	std::cout << "\n";
}

int getUserMove(bool isPlayerX)
{
	while (true)
	{
		std::cout << (isPlayerX ? "Player X " : "Player O ");
		std::cout << "enter your move: ";
		int move{};
		std::cin >> move;

		if (std::cin.fail())
		{
			std::cin.clear();
			ignoreLine();
			std::cout << "Invalid input. Please try again.\n";
		}
		else if (move < 1 || move > 9)
		{
			ignoreLine();
			std::cout << "Move must be between 1 and 9. Please try again.\n";
		}
		else
		{
			ignoreLine();
			return move;
		}
	}
}

bool checkForWin(char board[3][3])
{
	// Check rows and columns
	// Iterates through each row and column to check if all elements are the same
	// As there are multiple rows and columns which can be filled with the same element, thus a loop is used
	for (int i = 0; i < 3; ++i)
	{
		if (board[i][0] == board[i][1] && board[i][1] == board[i][2])
		{
			return true;
		}
		if (board[0][i] == board[1][i] && board[1][i] == board[2][i])
		{
			return true;
		}
	}

	// Check diagonals
	// There are only two diagonals which can be filled with the same element
	// Thus no loop is needed
	if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
	{
		return true;
	}
	if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
	{
		return true;
	}

	return false; // No win
}

void gamePlay(char board[3][3])
{
	bool isPlayerX = true;

	while (true)
	{
		printBoardState(board);

		int move = getUserMove(isPlayerX);

		int row = (move - 1) / 3;
		int col = (move - 1) % 3;

		if (board[row][col] == 'X' || board[row][col] == 'O')
		{
			std::cout << "That space is already taken. Please try again.\n";
			continue;
		}
		else
		{
			board[row][col] = isPlayerX ? 'X' : 'O';
		}

		if (checkForWin(board))
		{
			printBoardState(board);
			std::cout << (isPlayerX ? "Player X wins!\n" : "Player O wins!\n");
			break;
		}

		// Assumes the board is full
		// If a space is found that is not 'X' or 'O', the board is not full
		// Else the board is full and it's a tie
		bool isBoardFull = true;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (board[i][j] != 'X' && board[i][j] != 'O')
				{
					isBoardFull = false;
					break;
				}
			}
		}

		// If the board is full, it's a tie
		if (isBoardFull)
		{
			printBoardState(board);
			std::cout << "It's a tie!\n";
			break;
		}

		// Switch players by setting isPlayerX to the opposite of what it currently is (true -> false / false -> true)
		isPlayerX = !isPlayerX;
	}
}

int main()
{
	std::cout << "Welcome to Tic Tac Toe!\n";

	char board[3][3] = { {'1', '2', '3'},
						 {'4', '5', '6'},
						 {'7', '8', '9'} };

	gamePlay(board);
	return 0;
}

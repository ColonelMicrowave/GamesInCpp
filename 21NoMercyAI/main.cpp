#include <iostream>
#include <limits>
#include "random.h"

// Helper functions
void ignoreLine()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void setColour(int colour)
{
	std::cout << "\033[" << colour << "m";
}

void resetColour()
{
	std::cout << "\033[0m";
}

// Game functions
int getPlayerChoice(int count)
{
	while (true)
	{
		std::cout << "Enter a number between 1 and 3: ";
		int choice{};
		std::cin >> choice;

		if (std::cin.fail())
		{
			std::cin.clear();
			ignoreLine();

			setColour(31);
			std::cout << "Invalid input. Please try again.\n";
			resetColour();
		}

		ignoreLine();

		if (choice < 1 || choice > 3 || choice + count > 21)
		{
			setColour(31);
			std::cout << "Invalid input. Please try again.\n";
			resetColour();
		}
		else
		{
			return choice;
		}
	}
}

int getAIChoice(int count)
{
	while (true)
	{
		int choice{ ((4 - count % 4) + 1) % 4 };

		if (choice == 0)
		{
			choice = Random::get(1, 3);
		}

		return choice;
	}
}

bool hasWon(int count, bool isPlayerTurn)
{
	if (count == 21)
	{
		return true;
	}
	return false;
}

void playGame()
{
	int count{ 0 };
	bool isPlayerTurn{ true };
	int choice{};

	while (true)
	{
		setColour(36);
		std::cout << "Current count: " << count << "\n";
		resetColour();

		if (isPlayerTurn)
		{
			choice = getPlayerChoice(count);
			count += choice;
		}
		else
		{
			choice = getAIChoice(count);
			count += choice;

			setColour(32);
			std::cout << "AI chooses: " << choice << "\n";
			resetColour();
		}

		if (hasWon(count, isPlayerTurn))
		{
			setColour(36);
			std::cout << "Current count: " << count << "\n";
			resetColour();

			if (isPlayerTurn)
			{
				setColour(32);
				std::cout << "Player wins!\n";
				resetColour();
			}
			else
			{
				setColour(31);
				std::cout << "AI wins!\n";
				resetColour();
			}
			break;
		}

		isPlayerTurn = !isPlayerTurn;
	}
}

int main()
{
	std::cout << "Welcome to 21 game!\n";

	playGame();

	return 0;
}

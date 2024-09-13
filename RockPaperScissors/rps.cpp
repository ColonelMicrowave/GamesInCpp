#include <iostream>
#include <limits>
#include "random.h"

int getComputerChoice()
{
	return Random::get(1, 3);
}

int getUserChoice()
{
	while (true)
	{
		std::cout << "1) Rock\n";
		std::cout << "2) Paper\n";
		std::cout << "3) Scissors\n";
		std::cout << "Enter your choice: ";

		int choice{ };
		std::cin >> choice;

		if (choice == 1 || choice == 2 || choice == 3)
		{
			return choice;
		}
		else
		{
			std::cout << "Invalid choice. Please try again.\n";
			std::cin.clear(); // Clear any error flags
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
		}
	}
}

int determineWinner(int userChoice, int computerChoice)
{

	int points{ 0 };

	if (userChoice == computerChoice)
	{
		std::cout << "\nTie!\n";
	}
	else if (userChoice == 1 && computerChoice == 3)
	{
		std::cout << "\nRock beats Scissors. You win!\n";
		points++;
	}
	else if (userChoice == 2 && computerChoice == 1)
	{
		std::cout << "\nPaper beats Rock. You win!\n";
		points++;
	}
	else if (userChoice == 3 && computerChoice == 2)
	{
		std::cout << "\nScissors beats Paper. You win!\n";
		points++;
	}
	else if (userChoice == 3 && computerChoice == 1)
	{
		std::cout << "\nRock beats Scissors. You lose!\n";
		points--;
	}
	else if (userChoice == 1 && computerChoice == 2)
	{
		std::cout << "\nPaper beats Rock. You lose!\n";
		points--;
	}
	else if (userChoice == 2 && computerChoice == 3)
	{
		std::cout << "\nScissors beats Paper. You lose!\n";
		points--;
	}
	else
	{
		std::cout << "\nInvalid choice. Try again!\n";
	}

	return points;
}

bool playAgain()
{
	while (true)
	{
		std::cout << "Do you want to play again? (y/n): ";
		char choice{ };
		std::cin >> choice;

		if (choice == 'y')
		{
			std::cout << "\n";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
			return true;
		}
		else if (choice == 'n')
		{
			std::cout << "\nThanks for playing!\n";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
			return false;
		}
		else
		{
			std::cout << "Invalid choice. Please try again.\n";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
		}
	}
}

int main()
{
	std::cout << "Rock, Paper, Scissors!\n";
	int playerPoints{ 0 };
	int computerPoints{ 0 };

	while (true)
	{
		int userChoice{ getUserChoice() };
		int computerChoice{ getComputerChoice() };

		int points{ determineWinner(userChoice, computerChoice) };

		// Updates points based on winner of round
		if (points > 0)
		{
			++playerPoints;
		}
		else if (points < 0)
		{
			++computerPoints;
		}
		else
		{
			;  // Terminate the statement with a semicolon (Tie)
		}

		std::cout << "Player: " << playerPoints << "\nComputer: " << computerPoints << "\n";

		// Checks for winner
		if (playerPoints == 3) // Modify to change the win condition
		{
			std::cout << "Player wins!\n";
			playerPoints = 0;
			computerPoints = 0;
			if (!playAgain())
				break;
		}
		else if (computerPoints == 3) // Modify to change the win condition
		{
			std::cout << "Computer wins!\n";
			playerPoints = 0;
			computerPoints = 0;
			if (!playAgain())
				break;
		}
	}
}

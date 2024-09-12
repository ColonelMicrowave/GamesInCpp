#include <iostream>
#include "random.h"

int getComputerChoice()
{
	return Random::get(1, 3);
}

int getUserChoice()
{
	std::cout << "\n1) Rock\n";
	std::cout << "2) Paper\n";
	std::cout << "3) Scissors\n";
	std::cout << "Enter your choice: ";

	int choice{ };
	std::cin >> choice;

	return choice;
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
	std::cout << "Do you want to play again? (y/n): ";
	char choice{ };
	std::cin >> choice;

	if (choice == 'y')
	{
		std::cout << "\n";
		return true;
	}
	else if (choice == 'n')
	{
		std::cout << "\nThanks for playing!\n";
		return false;
	}
	else
	{
		std::cout << "Invalid choice. Please try again.\n";
		playAgain();
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

		// Checks for winner
		if (playerPoints == 3)
		{
			std::cout << "Player wins!\n";
			if (!playAgain())
				break;
		}
		else if (computerPoints == 3)
		{
			std::cout << "Computer wins!\n";
			if (!playAgain())
				break;
		}

		std::cout << "Player: " << playerPoints << "\nComputer: " << computerPoints << "\n";
	}
}

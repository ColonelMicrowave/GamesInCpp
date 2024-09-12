#include <iostream>
#include "random.h"

int getRand(int min, int max)
{
	return Random::get(min, max);
}

int getMin()
{
	std::cout << "Enter the minimum value: ";
	int min{};
	std::cin >> min;

	return min;
}

int getMax()
{
	std::cout << "Enter the maximum value: ";
	int max{};
	std::cin >> max;

	return max;
}

int getMaxGuesses()
{
	std::cout << "Enter the maximum number of guesses allowed: ";
	int maxGuesses{};
	std::cin >> maxGuesses;

	return maxGuesses;
}

bool playAgain()
{
	std::cout << "Would you like to play again (y/n)? ";
	char choice{};
	std::cin >> choice;

	if ((choice != 'y') && (choice != 'Y') && (choice != 'n') && (choice != 'N'))
	{
		return playAgain();
	}

	return (choice == 'y' || choice == 'Y');
}


int main()
{
	std::cout << "Welcome to Hi-Lo!\n";

	int min{ getMin() };
	int max{ getMax() };
	int maxGuesses{ getMaxGuesses() };

	if (min > max)
	{
		int temp{ min };
		min = max;
		max = temp;

		if (maxGuesses < 1)
		{
			maxGuesses = 1;
		}
	}

	int randNum{ getRand(min, max) };

	if (maxGuesses == 1)
	{
		std::cout << "I'm thinking of a number between " << min << " and " << max << ". You have 1 try to guess it correctly.\n";
	}
	else
	{
		std::cout << "I'm thinking of a number between " << min << " and " << max << ". You have " << maxGuesses << " tries to guess it correctly.\n";
	}

	int guess{ 0 };
	
	while (maxGuesses > guess)
	{
		std::cout << "Guess #" << guess + 1 << ": ";
		int userGuess{};
		std::cin >> userGuess;

		if (userGuess == randNum)
		{
			std::cout << "Correct! You win!\n";
			break;
		}
		else if (userGuess < randNum)
		{
			std::cout << "Your guess is too low.\n";
		}
		else
		{
			std::cout << "Your guess is too high.\n";
		}

		++guess;
	}

	if (maxGuesses == guess)
	{
		std::cout << "Sorry, you lose. The correct number was " << randNum << ".\n";
	}

	if (playAgain())
	{
		main();
	}
	else
	{
		std::cout << "Thank you for playing.\n";
	}

	return 0;
}

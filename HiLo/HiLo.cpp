#include <iostream>
#include <limits>
#include "random.h"

// std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); is used to clear the input buffer.
// std::cin.clear(); is used to clear the error flag on std::cin to prevent an infinite loop.

int getRand(int min, int max)
{
	return Random::get(min, max);
}

int getMin()
{
	while (true)
	{
		std::cout << "Enter the minimum value: ";
		int min{};
		std::cin >> min;

		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return min;
		}
	}
}

int getMax()
{
	while (true)
	{
		std::cout << "Enter the maximum value: ";
		int max{};
		std::cin >> max;

		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return max;
		}
	}
}

int getMaxGuesses()
{
	while (true)
	{
		std::cout << "Enter the maximum number of guesses allowed: ";
		int maxGuesses{};
		std::cin >> maxGuesses;

		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else if (maxGuesses < 1)
		{
			maxGuesses = 1;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return maxGuesses;
		}
		else
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return maxGuesses;
		}
	}
}

bool playAgain()
{
	while (true)
	{
		std::cout << "Would you like to play again (y/n)? ";
		char choice{};
		std::cin >> choice;

		if ((choice != 'y') && (choice != 'Y') && (choice != 'n') && (choice != 'N'))
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return (choice == 'y' || choice == 'Y');
		}
	}
}

int uGuess(int guessNumber)
{
	while (true)
	{
		std::cout << "Guess: " << guessNumber + 1 << ": ";
		int guess{};
		std::cin >> guess;

		if (std::cin.fail())
		{
			std::cout << "Invalid input. Please try again.\n";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return guess;
		}
	}
}


int main()
{
	while (true)
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
			int userGuess = uGuess(guess);

			if (userGuess < min || userGuess > max)
			{
				std::cout << "Your guess is out of range.\n";
			}
			else if (userGuess == randNum)
			{
				std::cout << "Correct! You win!\n";
				break;
			}
			else if (userGuess < randNum)
			{
				std::cout << "Your guess is too low.\n";
				++guess;
			}
			else if (userGuess > randNum)
			{
				std::cout << "Your guess is too high.\n";
				++guess;
			}
		}

		if (maxGuesses == guess)
		{
			std::cout << "Sorry, you lose. The correct number was " << randNum << ".\n";
		}

		if (!playAgain())
		{
			std::cout << "Thank you for playing!\n";
			break;
		}
	}

	return 0;
}

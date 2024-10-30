#include <iostream>
#include <limits>
#include <utility>
#include <algorithm>

void ignoreLine() 
{ 
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::pair<int, int> getRange()
{
	while (true)
	{
		std::cout << "Enter the lowest number in the range: ";
		int low{};
		std::cin >> low;

		if (std::cin.fail())
		{
			std::cin.clear();
			ignoreLine();
			std::cout << "Invalid input. Please try again.\n";
			continue;
		}

		ignoreLine();

		std::cout << "Enter the highest number in the range: ";
		int high{};
		std::cin >> high;

		if (std::cin.fail())
		{
			std::cin.clear();
			ignoreLine();
			std::cout << "Invalid input. Please try again.\n";
			continue;
		}

		ignoreLine();

		if (low > high)
			std::swap(low, high);

		std::cout << "\n";
		return { low, high };
	}
}

int binarySearch(int low, int high)
{
	return (low + high) / 2;
}

int getUserResponse(int guess)
{
	while (true)
	{
		std::cout << "Is your number " << guess << "?\n";
		std::cout << "1) Yes\n";
		std::cout << "2) Too high\n";
		std::cout << "3) Too low\n";
		std::cout << "Response: ";

		int response{};
		std::cin >> response;

		if (std::cin.fail())
		{
			std::cin.clear();
			ignoreLine();
			std::cout << "Invalid input. Please try again.\n";
			continue;
		}

		ignoreLine();
		std::cout << "\n";

		if (response == 1)
			return 1;
		if (response == 2)
			return 2;
		if (response == 3)
			return 3;
		
		std::cout << "Invalid input. Please try again.\n";
	}
}

void playGame(int low, int high)
{
	int guessCount{ 1 };

	while (true)
	{
		std::cout << "Guess count: " << guessCount << "\n";

		int guess{ binarySearch(low, high) };
		int response{ getUserResponse(guess) };

		if (response == 1) // correct guess
		{
			std::cout << "I win!\n";
			return;
		}
		else if (response == 2) // guess was too high
		{
			high = guess - 1;
		}
		else if (response == 3) // guess was too low
		{
			low = guess + 1;
		}

		++guessCount;
	}
}

bool playAgain()
{
	while (true)
	{
		std::cout << "Would you like to play again? (y/n)\n";
		char response{};
		std::cin >> response;

		if (std::cin.fail())
		{
			std::cin.clear();
			ignoreLine();
			std::cout << "Invalid input. Please try again.\n";
		}

		ignoreLine();
		std::cout << "\n";

		if (response == 'y' || response == 'Y')
			return true;
		if (response == 'n' || response == 'N')
			return false;

		std::cout << "Invalid input. Please try again.\n";
	}
}

int main()
{
	std::cout << "Welcome to the Hi-Lo game!\n";
	std::cout << "In this game, I have to try and guess your number.\n";

	do
	{
		auto [low, high] { getRange() }; // Structured binding

		std::cout << "Now think of a number between " << low << " and " << high << ".\n\n";

		playGame(low, high);
	} while (playAgain());
}

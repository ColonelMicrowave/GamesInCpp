#include <iostream>
#include <string>
#include <limits>
#include "random.h"

enum class PigState
{
	sider,
	trotter,
	doubleTrotter,
	pigOut,
};

void ignoreLine() // Helper function
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

constexpr std::string_view getPigStateNames(PigState state)
{
	switch (state)
	{
	case PigState::sider:
		return "Sider";
	case PigState::trotter:
		return "Trotter";
	case PigState::doubleTrotter:
		return "Double Trotter";
	case PigState::pigOut:
		return "Pig Out";
	default:
		return "Unknown";
	}
}

int getScore(PigState state)
{
	switch (state)
	{
	case PigState::sider:         return 1;
	case PigState::trotter:       return 5;
	case PigState::doubleTrotter: return 20;
	case PigState::pigOut:        return 0;
	default:
		std::cout << "Error: Invalid PigState\n";
		return 0;
	}
}

PigState getPigStates()
{
	int pigA{ Random::get(1, 3) };
	int pigB{ Random::get(1, 3) };

	if (pigA == 1 && pigB == 1 || pigA == 2 && pigB == 2)
	{
		return PigState::sider;
	}
	else if (pigA == 3 && pigB == 3)
	{
		return PigState::doubleTrotter;
	}
	else if (pigA == 3 || pigB == 3)
	{
		return PigState::trotter;
	}
	else
	{
		return PigState::pigOut;
	}
}

bool getRoll()
{
	while (true)
	{
		std::cout << "Press enter to roll the pigs: \n";
		std::string input{};
		std::getline(std::cin, input);

		if (input.empty())
		{
			std::cout << "Rolling the pigs...\n";
			return true;
		}
		else
		{
			ignoreLine();
		}
	}
}

void playGame()
{
	int playerScore{ 0 };

	while (playerScore < 100)
	{
		if (getRoll())
		{
			PigState state{ getPigStates() };
			std::cout << "You rolled a " << getPigStateNames(state) << "!\n";

			int score{ getScore(state) };

			if (score == 0)
			{
				playerScore = 0;
				std::cout << "Your score is now 0.";
			}
			else
			{
				playerScore += score;
				std::cout << "You gained " << score << " points!";
			}

			std::cout << "\tYour score is: " << playerScore << '\n';
		}
	}
}

int main()
{
	std::cout << "Welcome to Pass the Pigs!\n";
	playGame();

	return 0;
}

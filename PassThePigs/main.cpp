#include <iostream>
#include <string>
#include <limits>
#include <array>
#include "random.h"

class PigState
{
public:
	enum state
	{
		sider,
		trotter,
		doubleTrotter,
		snouter,
		doubleSnouter,
		razorback,
		doubleRazorback,
		pigOut,
		maxStates
	};

	PigState() = default;

	constexpr std::string_view getPigStateNames(state pigState) // Helper function
	{
		switch (pigState)
		{
		case sider:           return "Sider";
		case trotter:         return "Trotter";
		case doubleTrotter:   return "Double Trotter";
		case snouter:         return "Snouter";
		case doubleSnouter:   return "Double Snouter";
		case razorback:       return "Razorback";
		case doubleRazorback: return "Double Razorback";
		case pigOut:          return "Pig Out";
		default:              return "Unknown";
		}
	}

	state getPigStates()
	{
		int pigA{ Random::get(1, 20) };
		int pigB{ Random::get(1, 20) };

		// Check for sider
		if (isInArray(m_leftSide, pigA) && isInArray(m_leftSide, pigB))
			return sider;
		else if (isInArray(m_rightSide, pigA) && isInArray(m_rightSide, pigB))
			return sider;

		// Check for double states
		else if (isInArray(m_trotter, pigA) && isInArray(m_trotter, pigB))
			return doubleTrotter;
		else if (isInArray(m_razorback, pigA) && isInArray(m_razorback, pigB))
			return doubleRazorback;
		else if (isInArray(m_snouter, pigA) && isInArray(m_snouter, pigB))
			return doubleSnouter;

		// Check for single states
		else if (isInArray(m_trotter, pigA) || isInArray(m_trotter, pigB))
			return trotter;
		else if (isInArray(m_razorback, pigA) || isInArray(m_razorback, pigB))
			return razorback;
		else if (isInArray(m_snouter, pigA) || isInArray(m_snouter, pigB))
			return snouter;
		
		else
			return pigOut;
	}

private:
	constexpr static std::array<int, 5> m_rightSide{ 1, 2, 3, 4, 5 };
	constexpr static std::array<int, 5> m_leftSide{ 1, 2, 3, 4, 5 };
	constexpr static std::array<int, 4> m_trotter{ 11, 12, 13, 14 };
	constexpr static std::array<int, 4> m_razorback{ 15, 16, 17, 18 };
	constexpr static std::array<int, 2> m_snouter{ 19, 20 };

	template <std::size_t N>
	bool isInArray(const std::array<int, N>& arr, int value) const // Helper function
	{
		return std::find(arr.begin(), arr.end(), value) != arr.end();
	}
};

void ignoreLine() // Helper function
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void setColour(int colour) // Helper function
{
	std::cout << "\033[" << colour << "m";
}

void resetColour() // Helper function
{
	std::cout << "\033[0m";
}

int getScore(PigState::state state)
{
	switch (state)
	{
	case PigState::sider:           return 1;

	case PigState::trotter:
	case PigState::snouter:
	case PigState::razorback:       return 5;

	case PigState::doubleTrotter:
	case PigState::doubleSnouter:
	case PigState::doubleRazorback: return 20;

	case PigState::pigOut:          return 0;

	default:
		std::cout << "Error: Invalid PigState\n";
		return 0;
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
	PigState pigStates{};
	int playerScore{ 0 };

	while (playerScore < 100)
	{
		if (getRoll())
		{
			PigState::state pigState{ pigStates.getPigStates() };
			std::cout << "You rolled a " << pigStates.getPigStateNames(pigState) << "!\n";

			int score{ getScore(pigState) };

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

	setColour(32);
	std::cout << "\n\tCongratulations! You won!\n";
	resetColour();
}

int main()
{
	std::cout << "Welcome to Pass the Pigs!\n";

	playGame();

	return 0;
}

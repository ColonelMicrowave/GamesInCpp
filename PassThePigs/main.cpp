#include <iostream>
#include <string>
#include <limits>
#include <array>
#include <algorithm>
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
	constexpr static std::array<int, 5> m_leftSide{ 6, 7, 8, 9, 10 };
	constexpr static std::array<int, 4> m_trotter{ 11, 12, 13, 14 };
	constexpr static std::array<int, 4> m_razorback{ 15, 16, 17, 18 };
	constexpr static std::array<int, 2> m_snouter{ 19, 20 };

	template <std::size_t N>
	bool isInArray(const std::array<int, N>& arr, int value) const
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
		std::cout << "Error: Invalid Pig State\n";
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
			std::cout << "Invalid input. Try again.";
			ignoreLine();
		}
	}
}

bool isPlayerBanking(bool isPlayerTwo, int& score, std::array<int, 2>& arr)
{
	while (true)
	{
		std::cout << "\nWould you like to bank? (y/n): ";
		char choice{};
		std::cin >> choice;

		if (std::cin.fail())
		{
			std::cin.clear();
			ignoreLine();
			std::cout << "Invalid input. Try again.\n";
		}

		ignoreLine();

		if (choice == 'y')
		{
			arr[isPlayerTwo] += score;
			score = 0;
			return true;
		}
		else if (choice == 'n')
		{
			return false;
		}
		else
		{
			std::cout << "Invalid input. Try again.\n";
		}
	}
}

bool hasPlayerWon(std::array<int, 2> playerScores)
{
	if (playerScores[0] >= 100)
	{
		setColour(32);
		std::cout << "\n\tPlayer One has won!\n";
		resetColour();
		return true;
	}
	else if (playerScores[1] >= 100)
	{
		setColour(32);
		std::cout << "\n\tPlayer Two has won!\n";
		resetColour();
		return true;
	}

	return false;
}

void playGame()
{
	PigState pigStates{};
	bool isPlayerTwo{ false };
	int currentPlayerScore{ 0 };
	std::array<int, 2> playerBankedScores{ 0, 0 };

	while (!hasPlayerWon(playerBankedScores))
	{
		setColour(35);
		std::cout << "\n\tPlayer One's Score: " << playerBankedScores[0] << '\n';
		std::cout << "\tPlayer Two's Score: "   << playerBankedScores[1] << "\n\n";
		resetColour();

		setColour(36);
		std::cout << "Player " << (isPlayerTwo ? "Two" : "One") << " Turn\n";
		resetColour();

		if (getRoll())
		{
			PigState::state pigState{ pigStates.getPigStates() };

			setColour(33);
			std::cout << "You rolled a " << pigStates.getPigStateNames(pigState) << "!\n";
			resetColour();

			int score{ getScore(pigState) };

			if (score == 0)
			{
				currentPlayerScore = 0;

				setColour(31);
				std::cout << "Your score is now 0. Your turn is over.\n\n";
				resetColour();

				isPlayerTwo = !isPlayerTwo; // Switch players
				continue;
			}
			else
			{
				currentPlayerScore += score;

				setColour(32);
				std::cout << "You gained " << score << " points!";
				resetColour();
			}

			setColour(36);
			std::cout << "\tYour total score is: " << currentPlayerScore << '\n';
			resetColour();
		}

		if (isPlayerBanking(isPlayerTwo, currentPlayerScore, playerBankedScores))
		{
			isPlayerTwo = !isPlayerTwo; // Switch players
		}
	}
}

int main()
{
	std::cout << "Welcome to Pass the Pigs!\n";

	playGame();

	return 0;
}

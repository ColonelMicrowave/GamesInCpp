#include <iostream>
#include <string>
#include <limits>
#include <array>
#include <algorithm>
#include "random.h"

// Helper functions here:
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

// Main game logic here:
class PigState
{
public:
	enum state
	{
		leftsider,
		rightsider,
		sider,
		trotter,
		doubleTrotter,
		snouter,
		doubleSnouter,
		razorback,
		doubleRazorback,
		leaningJowler,
		doubleLeaningJowler,
		pigOut,
		oinker,
		maxStates
	};

	PigState() = default;

	constexpr std::string_view getPigStateNames(state pigState) // Helper function
	{
		switch (pigState)
		{
		case sider:               return "Sider";
		case trotter:             return "Trotter";
		case doubleTrotter:       return "Double Trotter";
		case snouter:             return "Snouter";
		case doubleSnouter:       return "Double Snouter";
		case razorback:           return "Razorback";
		case doubleRazorback:     return "Double Razorback";
		case leaningJowler:       return "Leaning Jowler";
		case doubleLeaningJowler: return "Double Leaning Jowler";
		case pigOut:		      return "Pig Out";
		case oinker:              return "Oinker";
		default:                  return "Unknown";
		}
	}

	int getScore(state state)
	{
		switch (state)
		{
		case sider:               return 1;

		case trotter:       [[fallthrough]];
		case razorback:           return 5;

		case snouter:             return 10;

		case leaningJowler:       return 15;

		case doubleTrotter: [[fallthrough]];
		case doubleRazorback:     return 20;

		case doubleSnouter:       return 40;

		case doubleLeaningJowler: return 60;

		case oinker:        [[fallthrough]];
		case pigOut:              return 0;

		default:
			std::cout << "Error: Invalid Pig State\n";
			return 0;
		}
	}

	state getPigStates()
	{
		int roll{ Random::get(1, 101) };

		// Check for sider
		if (isInArray(m_leftSide, roll))
			return leftsider;
		else if (isInArray(m_rightSide, roll))
			return rightsider;

		// Check for single states
		else if (isInArray(m_trotter, roll))
			return trotter;
		else if (isInArray(m_razorback, roll))
			return razorback;
		else if (isInArray(m_snouter, roll))
			return snouter;
		else if (isInArray(m_leaningJowler, roll))
			return leaningJowler;
		else if (isInArray(m_oinker, roll))
			return oinker;
	}

	int interpretRolls(state pigA, state pigB)
	{
		int score{ 0 };
		if (pigA == pigB)
		{
			switch (pigA)
			{
			case leftsider:
				setColour(33);
				std::cout << "You rolled a " << getPigStateNames(sider) << "!\n";
				resetColour();

				score += getScore(sider);
				return score;

			case rightsider:
				setColour(33);
				std::cout << "You rolled a " << getPigStateNames(sider) << "!\n";
				resetColour();

				score += getScore(sider);
				return score;

			case trotter:
				setColour(33);
				std::cout << "You rolled a " << getPigStateNames(doubleTrotter) << "!\n";
				resetColour();

				score += getScore(doubleTrotter);
				return score;

			case razorback:
				setColour(33);
				std::cout << "You rolled a " << getPigStateNames(doubleRazorback) << "!\n";
				resetColour();

				score += getScore(doubleRazorback);
				return score;

			case snouter:
				setColour(33);
				std::cout << "You rolled a " << getPigStateNames(doubleSnouter) << "!\n";
				resetColour();

				score += getScore(doubleSnouter);
				return score;

			case leaningJowler:
				setColour(33);
				std::cout << "You rolled a " << getPigStateNames(doubleLeaningJowler) << "!\n";
				resetColour();

				score += getScore(doubleLeaningJowler);
				return score;
			}
		}
		else
		{
			if ((pigA == leftsider && pigB == rightsider) || (pigA == rightsider && pigB == leftsider))
			{
				setColour(31);
				std::cout << "You rolled a " << getPigStateNames(pigOut) << "!\n";
				resetColour();

				score += getScore(pigOut);
				return score;
			}
			else if (pigA == oinker || pigB == oinker)
			{
				setColour(31);
				std::cout << "You rolled an " << getPigStateNames(oinker) << "!\n";
				resetColour();

				score += getScore(oinker);
				return score;
			}
			else
			{
				if (pigA != leftsider && pigA != rightsider)
				{
					setColour(33);
					std::cout << "You rolled a " << getPigStateNames(pigA) << "!\n";
					resetColour();

					score += getScore(pigA);
				}
				else if (pigB != leftsider && pigB != rightsider)
				{
					setColour(33);
					std::cout << "You rolled a " << getPigStateNames(pigB) << "!\n";
					resetColour();

					score += getScore(pigB);
				}

				return score;
			}
		}
	}

private:
	constexpr static std::array<int, 29> m_rightSide{
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29
	};
	constexpr static std::array<int, 29> m_leftSide{
		30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
		45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58
	};
	constexpr static std::array<int, 15> m_razorback{
		59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73
	};
	constexpr static std::array<int, 15> m_trotter{
		74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88
	};
	constexpr static std::array<int, 7> m_snouter{
		89, 90, 91, 92, 93, 94, 95
	};
	constexpr static std::array<int, 1> m_leaningJowler{
		96
	};
	constexpr static std::array<int, 5> m_oinker{
		97, 98, 99, 100, 101
	};

	template <std::size_t N>
	bool isInArray(const std::array<int, N>& arr, int value) const
	{
		return std::find(arr.begin(), arr.end(), value) != arr.end();
	}
};

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
			std::cout << "Invalid input. Try again.\n";
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

		if (choice == 'y' || choice == 'Y')
		{
			arr[isPlayerTwo] += score;
			score = 0;
			return true;
		}
		else if (choice == 'n' || choice == 'N')
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
			PigState::state pigA{ pigStates.getPigStates() };
			PigState::state pigB{ pigStates.getPigStates() };

			int score{ pigStates.interpretRolls(pigA, pigB) };

			if (score == 0)
			{
				if ((pigA == PigState::leftsider && pigB == PigState::rightsider) || (pigA == PigState::rightsider && pigB == PigState::leftsider))
				{
					setColour(31);
					std::cout << "Your score is now 0. Your turn is over.\n\n";
					resetColour();

					currentPlayerScore = 0;
					isPlayerTwo = !isPlayerTwo; // Switch players
					continue;
				}
				else if (pigA == PigState::oinker || pigB == PigState::oinker)
				{
					setColour(31);
					std::cout << "Your banked score is now 0. Your turn is over.\n\n";
					resetColour();

					playerBankedScores[isPlayerTwo] = 0;
					currentPlayerScore = 0;
					isPlayerTwo = !isPlayerTwo; // Switch players
					continue;
				}
				else
				{
					setColour(32);
					std::cout << "You gained " << score << " points!";
					resetColour();
				}
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

	setColour(36);
	std::cout << "\n\tPlayer 1's Score: " << playerBankedScores[0];
	std::cout << "\n\tPlayer 2's Score: " << playerBankedScores[1];
	std::cout << '\n';
	resetColour();
}

int main()
{
	std::cout << "Welcome to Pass the Pigs!\n";

	playGame();

	return 0;
}

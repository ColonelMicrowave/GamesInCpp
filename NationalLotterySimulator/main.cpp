#include <iostream>
#include <limits>
#include <vector>
#include "random.h"

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

void generateLotteryNumbers(std::vector<int>& lotteryNumbers)
{
	for (int i = 0; i < 6; ++i)
	{
		int newNumber = Random::get(1, 59);

		// Check if the number is already in the vector
		while (std::find(lotteryNumbers.begin(), lotteryNumbers.end(), newNumber) != lotteryNumbers.end())
		{
			newNumber = Random::get(1, 59);  // Generate a new number if duplicate found
		}

		lotteryNumbers.at(i) = newNumber;
	}
}

int generateBonusBall(const std::vector<int>& numbers)
{
	while (true)
	{
		bool isInUse{ false };
		int bonus{ Random::get(1, 59) };
		for (int i : numbers)
		{
			if (i == bonus)
			{
				isInUse = true;
			}
		}

		if (!isInUse)
			return bonus;
	}
}

int calculateMean(const std::vector<int>& numbers)
{
	int sum{ 0 };
	for (int number : numbers)
	{
		sum += number;
	}
	return sum / std::ssize(numbers);
}

int getPlayerNumber(const std::vector<int>& previousNumbers)
{
	while (true)
	{
		std::cout << "Enter your number (1-59): ";
		int userNum{};
		std::cin >> userNum;

		if (std::cin.fail())
		{
			std::cin.clear();
			ignoreLine();
			std::cout << "Invalid input. Try again.\n";
			continue;
		}

		ignoreLine();

		if (userNum > 59 || userNum < 1)
		{
			std::cout << "Number out of bounds. Try again.\n";
			continue;
		}

		bool isDuplicate = false;
		for (int num : previousNumbers)
		{
			if (num != 0 && num == userNum)
			{
				isDuplicate = true;
				break;
			}
		}

		if (isDuplicate)
		{
			std::cout << "Duplicate number. Try again.\n";
			continue;
		}

		return userNum;
	}
}

int checkForCorrectNumber(const std::vector<int>& playerNumbers, std::vector<int> winningNumbers)
{
	int correctNumbers{ 0 };
	for (int guess : playerNumbers)
	{
		for (int& actual : winningNumbers)
		{
			if (guess == actual)
			{
				++correctNumbers;
				actual = -1;
				break;
			}
		}
	}

	return correctNumbers;
}

void simulateLottery()
{
	constexpr int sims{ 1 }; // Adjust this value to increase or decrease the number of simulations
	constexpr int initialSize{ 100'000'000 }; // Adjust this value to increase or decrease the number of tickets bought per simulation

	std::vector<int> six(sims);
	std::vector<int> fiveBonus(sims);
	std::vector<int> five(sims);
	std::vector<int> four(sims);
	std::vector<int> three(sims);
	std::vector<int> two(sims);
	std::vector<int> one(sims);
	std::vector<int> zero(sims);

	for (int i{ 0 }; i < sims; ++i)
	{

		for (int j{ 0 }; j < initialSize; ++j)
		{
			std::vector<int> winningNumbers(6, 0);
			std::vector<int> playerNumbers(6, 0);

			generateLotteryNumbers(winningNumbers);
			generateLotteryNumbers(playerNumbers);

			int bonusBall{ generateBonusBall(winningNumbers) };

			int correctNumbers{ checkForCorrectNumber(playerNumbers, winningNumbers) };
			bool isBonus{ false };

			switch (correctNumbers)
			{
			case 6:
				++six[i];
				break;
			case 5:
				for (int guess : playerNumbers)
				{
					if (guess == bonusBall)
					{
						isBonus = true;
						break;
					}
				}

				if (isBonus)
					++fiveBonus[i];
				else
					++five[i];

				break;
			case 4:
				++four[i];
				break;
			case 3:
				++three[i];
				break;
			case 2:
				++two[i];
				break;
			case 1:
				++one[i];
				break;
			case 0:
				++zero[i];
				break;
			default:
				std::cout << "Error: Invalid number of correct numbers\n";
				break;
			}
		}
	}

	std::cout << "Probability of winning the National Lottery:\n";
	std::cout << "Six numbers: " << calculateMean(six) << " in " << initialSize <<'\n';
	std::cout << "Five numbers and the bonus ball: " << calculateMean(fiveBonus) << " in " << initialSize << '\n';;
	std::cout << "Five numbers: " << calculateMean(five) << " in " << initialSize << '\n';;
	std::cout << "Four numbers: " << calculateMean(four) << " in " << initialSize << '\n';;
	std::cout << "Three numbers: " << calculateMean(three) << " in " << initialSize << '\n';;
	std::cout << "Two numbers: " << calculateMean(two) << " in " << initialSize << '\n';;
	std::cout << "One number: " << calculateMean(one) << " in " << initialSize << '\n';;
}

void playLottery()
{
	std::vector<int> winningNumbers(6);
	std::vector<int> playerNumbers(6);

	generateLotteryNumbers(winningNumbers);

	for (int& i : playerNumbers)
	{
		i = getPlayerNumber(playerNumbers);
	}

	int bonusBall{ generateBonusBall(winningNumbers) };

	int correctNumbers{ checkForCorrectNumber(playerNumbers, winningNumbers) };
	bool isBonus{ false };

	std::cout << "Winning Numbers: ";
	for (int number : winningNumbers)
	{
		std::cout << number << ' ';
	}
	std::cout << "\nBonus Ball: " << bonusBall << "\n";

	std::cout << "Your Numbers: ";
	for (int playerNumber : playerNumbers)
	{
		bool isMatch{ false };
		for (int winningNumber : winningNumbers)
		{
			if (playerNumber == winningNumber)
			{
				isMatch = true;
				break;
			}
		}

		if (isMatch)
		{
			setColour(32);
			std::cout << playerNumber << ' ';
			resetColour();
		}
		else
		{
			setColour(34);
			std::cout << playerNumber << ' ';
			resetColour();
		}
	}
	std::cout << '\n';

	switch (correctNumbers)
	{
	case 6:
		std::cout << "You won the jackpot prize: \x9C 3,000,000!\n";
		break;

	case 5:
		for (int guess : playerNumbers)
		{
			if (guess == bonusBall)
			{
				isBonus = true;
				break;
			}
		}

		if (isBonus)
			std::cout << "You won the bonus ball + 5 numbers! You earned \x9C 1,000,000!\n";
		else
			std::cout << "You won 5 numbers! You earned \x9C 1,750!\n";

		break;

	case 4:
		std::cout << "You won 4 numbers! You earned \x9C 140!\n";
		break;

	case 3:
		std::cout << "You won 3 numbers! You earned \x9C 30!\n";
		break;

	case 2:
		std::cout << "You won 2 numbers! You earned a free Lucky Dip!\n";
		break;

	case 1: [[fallthrough]];
	case 0:
		std::cout << "Unfortunately, you did not win anything. Better luck next time!\n";
		break;

	default:
		std::cout << "What have you done!\n";
		break;
	}
}

char playerChoice()
{
	while (true)
	{
		std::cout << "Would you like to play (p) or simulate (s) the National Lottery Lotto: ";
		char choice{};
		std::cin >> choice;

		if (std::cin.fail())
		{
			std::cin.clear();
			ignoreLine();
			std::cout << "Invalid input. Try again!\n";

			continue;
		}

		ignoreLine();

		if (choice == 'p' || choice == 'P')
			return 'p';
		else if (choice == 's' || choice == 'S')
			return 's';
		else
			std::cout << "Invalid input. Try again!\n";
	}
}

int main()
{
	std::cout << "Welcome to the National Lottery Simulator!\n";

	if (playerChoice() == 'p')
		playLottery();
	else
		simulateLottery();

	return 0;
}

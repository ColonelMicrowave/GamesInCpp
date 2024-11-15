#include <iostream>
#include <vector>
#include "random.h"

void generateLotteryNumbers(std::vector<int>& lotteryNumbers)
{
	for (int i{ 0 }; i < 6; ++i)
	{
		lotteryNumbers.at(i) = Random::get(1, 59);
	}
}

int generateBonusBall()
{
	return Random::get(1, 7);
}

void simulateLottery()
{
	int six{ 0 };
	int fiveBonus{ 0 };
	int five{ 0 };
	int four{ 0 };
	int three{ 0 };
	int two{ 0 };
	int one{ 0 };
	int zero{ 0 };

	for (int i{ 0 }; i < 100'000'000; i++)
	{
		std::vector<int> winningNumbers(6);
		std::vector<int> playerNumbers(6);

		int bonusBall{ generateBonusBall() };
		int playerBonusBall{ generateBonusBall() };

		generateLotteryNumbers(winningNumbers);
		generateLotteryNumbers(playerNumbers);

		int correctNumbers{ 0 };
		std::vector<int> tempWinningNumbers{ winningNumbers };
		for (int j : playerNumbers)
		{
			for (int& k : tempWinningNumbers)
			{
				if (j == k)
				{
					++correctNumbers;
					k = -1;
					break;
				}
			}
		}

		switch (correctNumbers)
		{
		case 6:
			++six;
			break;
		case 5:
			if (playerBonusBall == bonusBall)
			{
				++fiveBonus;
			}
			else
			{
				++five;
			}
			break;
		case 4:
			++four;
			break;
		case 3:
			++three;
			break;
		case 2:
			++two;
			break;
		case 1:
			++one;
			break;
		case 0:
			++zero;
			break;
		default:
			std::cout << "Error: Invalid number of correct numbers\n";
			break;
		}
	}

	std::cout << "Jackpot (6 numbers): " << six << '\n';
	std::cout << "5 numbers + bonus: " << fiveBonus << '\n';
	std::cout << "5 numbers: " << five << '\n';
	std::cout << "4 numbers: " << four << '\n';
	std::cout << "3 numbers: " << three << '\n';
	std::cout << "2 numbers: " << two << '\n';
	std::cout << "1 number: " << one << '\n';
	std::cout << "0 numbers: " << zero << '\n';
}

int main()
{
	std::cout << "Welcome to the National Lottery Simulator!\n";

	simulateLottery();

	return 0;
}

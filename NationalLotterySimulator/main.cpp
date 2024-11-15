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

long long calculateMean(const std::vector<int>& numbers)
{
	long long sum{ 0 };
	for (int number : numbers)
	{
		sum += number;
	}
	return sum / numbers.size();
}

void simulateLottery()
{
	constexpr int sims{ 5 }; // Adjust this value to increase or decrease the number of simulations
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

			int bonusBall{ generateBonusBall() };
			int playerBonusBall{ generateBonusBall() };

			generateLotteryNumbers(winningNumbers);
			generateLotteryNumbers(playerNumbers);

			int correctNumbers{ 0 };
			std::vector<int> tempWinningNumbers{ winningNumbers };
			for (int guess : playerNumbers)
			{
				for (int& actual : tempWinningNumbers)
				{
					if (guess == actual)
					{
						++correctNumbers;
						actual = -1;
						break;
					}
				}
			}

			switch (correctNumbers)
			{
			case 6:
				++six[i];
				break;
			case 5:
				if (playerBonusBall == bonusBall)
				{
					++fiveBonus[i];
				}
				else
				{
					++five[i];
				}
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

int main()
{
	std::cout << "Welcome to the National Lottery Simulator!\n";

	simulateLottery();

	return 0;
}

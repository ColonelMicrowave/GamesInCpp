#include <iostream>
#include <string>
#include <limits>
#include "random.h"
#include "wordList.h"

void ignoreLine()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

const std::string& chooseWord()
{
	using namespace WordList;

	// Get a random word from the list
	// sizeof(word) returns the size of the array in bytes (size of 1 element * number of total elements)
	// sizeof(word[0]) returns the size of one element in the array
	// Divide the total size by the size of one element to get the number of total elements and minus 1 to get the upper bound
	// Because the index starts from 0
	int upperBound{ sizeof(word) / sizeof(word[0]) };
	int wordIndex{ Random::get(0, (upperBound - 1)) };
	return word[wordIndex];
}

std::string scrambleWord(const std::string& word)
{
	std::string scrambledWord{ word };

	// Scramble the word
	// Start from the end of the string and swap each character with a random character before it
	// i > 0 because there is no need to swap the first character with itself
	for (size_t i{ scrambledWord.length() - 1 }; i > 0; --i)
	{
		// Generate a random index between 0 and i
		// Then swap the characters at i and j
		int j{ Random::get(0, static_cast<int>(i)) };
		std::swap(scrambledWord[i], scrambledWord[j]);
	}

	return scrambledWord;
}

bool playAgain()
{
	while (true)
	{
		std::cout << "Would you like to play (y/n): ";
		char response{};
		std::cin >> response;

		if (std::cin.fail())
		{
			std::cin.clear();
			ignoreLine();
			std::cout << "Invalid input. Please try again.\n";
		}
		else
		{
			ignoreLine();
			std::cout << '\n';

			if (response == 'y' || response == 'Y')
			{
				return true;
			}
			else if (response == 'n' || response == 'N')
			{
				return false;
			}
			else
			{
				std::cout << "Invalid input. Please try again.\n";
			}
		}
	}
}

void playGame()
{
	const std::string& chosenWord{ chooseWord() };
	std::string scrambledWord{ scrambleWord(chosenWord) };
	int numGuesses{ 0 };

	std::cout << "The scrambled word is: " << scrambledWord << '\n';

	while (true)
	{
		++numGuesses;

		std::cout << "Guess number " << numGuesses << ".\n";
		std::cout << "Your guess: ";
		std::string guess{};
		std::cin >> guess;

		if (std::cin.fail())
		{
			std::cin.clear();
			ignoreLine();
			std::cout << "Invalid input. Please try again.\n";
		}
		else
		{
			ignoreLine();
			if (guess == chosenWord)
			{
				std::cout << "Congratulations! You guessed the word!\n";
				std::cout << "Number of guesses: " << numGuesses << '\n';
				break;
			}
			else
			{
				std::cout << "Incorrect guess. Please try again.\n";
			}
		}
	}
}

int main()
{
	std::cout << "Welcome to Scrambled Words!\n";

	while (playAgain())
	{
		playGame();
	}

	return 0;
}

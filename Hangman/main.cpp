#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "random.h"
#include "wordList.h"

// Helper function to ignore the rest of the line after a failed input
void ignoreLine()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

namespace Settings
{
	// Modify the maximum number of wrong guesses allowed
	constexpr int maxWrongGuesses{ 6 };
}

class Session
{
private:
	std::string_view m_word     { WordList::getRandomWord() };
	std::vector<bool> m_guesses { std::vector<bool>(26) };
	int m_wrongGuessesLeft      { Settings::maxWrongGuesses };

	// Converts a letter to an index in the m_guesses vector (0-25)
	std::size_t toIndex(char c) const { return static_cast<std::size_t>((c % 32) - 1); }

public:
	std::string_view getWord() const   { return m_word; }

	int wrongGuessesLeft() const       { return m_wrongGuessesLeft; }
	void removeMaxGuess()              { --m_wrongGuessesLeft; }

	bool isLetterGuessed(char c) const { return m_guesses[toIndex(c)]; }
	void setLetterGuessed(char c)      { m_guesses[toIndex(c)] = true; }

	bool isLetterInWord(char c) const
	{
		for (auto ch : m_word)
		{
			if (ch == c)
				return true;
		}

		return false;
	}

	bool won() const
	{
		for (auto c : m_word)
		{
			if (!isLetterGuessed(c))
				return false;
		}

		return true;
	}
};

void displayGameState(const Session& s)
{
	std::cout << "\nWord: ";

	for (auto c : s.getWord())
	{
		if (s.isLetterGuessed(c))
			std::cout << c;
		else
			std::cout << "_";
	}

	std::cout << "\tWrong Guesses: ";
	for (int i{ 0 }; i < s.wrongGuessesLeft(); ++i)
		std::cout << "+";

	for (char c = 'a'; c <= 'z'; ++c)
	{
		if (s.isLetterGuessed(c) && !s.isLetterInWord(c))
			std::cout << c;
	}

	std::cout << "\n";
}

char getGuess(Session& s)
{
	while (true)
	{
		std::cout << "Enter your next guess: ";
		char c{};
		std::cin >> c;

		// Check for invalid input (non-characters)
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cout << "Invalid input. Please try again.\n";
			ignoreLine();
			continue;
		}

		ignoreLine(); // Ignore the rest of the line

		// Check for invalid input (non-letters)
		if (c < 'a' || c > 'z')
		{
			std::cout << "Invalid input. Please try again.\n";
			continue;
		}

		// Check if the letter has already been guessed
		if (s.isLetterGuessed(c))
		{
			std::cout << "You've already guessed that letter. Please try again.\n";
			continue;
		}

		// Valid input if it successfully passes all checks
		return c;
	}
}

void handleGuess(Session& s, char c)
{
	s.setLetterGuessed(c);

	if (s.isLetterInWord(c))
	{
		std::cout << "Yes, '" << c << "' is in the word!\n";
		return;
	}

	std::cout << "No, '" << c << "' is not in the word.\n";
	s.removeMaxGuess();
}

int main()
{
	std::cout << "Welcome to C++Man (a variation of Hangman)!\n";
	std::cout << "To win: guess the word. To lose: run out of guesses.\n";

	Session s{};

	while (s.wrongGuessesLeft() && !s.won())
	{
		displayGameState(s);
		char c{ getGuess(s) };
		handleGuess(s, c);
	}

	// Display the final game state
	displayGameState(s);

	// Display the result of the game (win or lose)
	if (!s.wrongGuessesLeft())
		std::cout << "You lose! The word was: " << s.getWord() << "\n";
	else
		std::cout << "Congratulations! You win!\n";

	return 0;
}

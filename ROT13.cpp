#include <iostream>
#include <string>
#include <limits>

void ignoreLine()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool tryAgain()
{
	while (true)
	{
		std::cout << "Would you like to encode or decode another message? (y/n): ";
		char choice{ };
		std::cin >> choice;

		if (std::cin.fail())
		{
			std::cin.clear();
			ignoreLine();
			std::cout << "Invalid choice\n";
		}
		else if (choice == 'y' || choice == 'Y')
		{
			ignoreLine();
			return true;
		}
		else if (choice == 'n' || choice == 'N')
		{
			ignoreLine();
			return false;
		}
		else
		{
			ignoreLine();
			std::cout << "Invalid choice\n";
		}
	}
}

int main()
{
	while (true)
	{
		std::cout << "Would you like to encode or decode a message? (e/d): ";
		char choice{ };
		std::cin >> choice;

		if (std::cin.fail() || (choice != 'e' && choice != 'd'))
		{
			std::cin.clear();
			ignoreLine();
			std::cout << "Invalid choice\n";
		}
		else if (choice == 'e' || choice == 'E')
		{
			ignoreLine();
			std::cout << "Enter a sentence to encrypt: ";
			std::string sentence{ };
			std::getline(std::cin >> std::ws, sentence);

			for (char& c : sentence)
			{
				if (c >= 'a' && c <= 'z')
				{
					c = ('a' + ((c - 'a') + 13) % 26);
				}
				else if (c >= 'A' && c <= 'Z')
				{
					c = ('A' + ((c - 'A') + 13) % 26);
				}
			}

			std::cout << "Encrypted sentence: " << sentence << '\n';
			if (!tryAgain())
				break;
		}
		else if (choice == 'd' || choice == 'D')
		{
			ignoreLine();
			std::cout << "Enter a sentence to decrypt: ";
			std::string sentence{ };
			std::getline(std::cin >> std::ws, sentence);

			for (char& c : sentence)
			{
				if (c >= 'a' && c <= 'z')
				{
					c = ('a' + ((c - 'a') + 13) % 26);
				}
				else if (c >= 'A' && c <= 'Z')
				{
					c = ('A' + ((c - 'A') + 13) % 26);
				}
			}

			std::cout << "Decrypted sentence: " << sentence << '\n';
			if (!tryAgain())
				break;
		}
	}

	return 0;
}

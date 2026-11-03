#ifndef CONFIG_H
#define CONFIG_H

namespace Config
{
	inline constexpr int ROWS{ 6 };
	inline constexpr int COLS{ 7 };
	inline constexpr int CELLS{ ROWS * COLS };
	inline constexpr char EMPTY{ '.' };
	inline constexpr char PLAYER1{ 'X' };
	inline constexpr char PLAYER2{ 'O' };
}

#endif

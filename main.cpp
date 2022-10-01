#include <iostream>
#include <clocale>  
#include <sstream> // для stringstream
enum TCell : char
{
	PLAYER = 'X',
	EMPTY = '_',
};

struct TCoord
{
	size_t y{ 0U };
	size_t x{ 0U };
};


enum class TKey
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	ESC,
	UNDEFINED,
	EMPTY
};

std::string CoordToString(TCoord& coord)
{
	std::stringstream stream;
	std::string result;
	stream << coord.y;
	stream << ":";
	stream << coord.x;
	stream >> result;
	return result;
}

class IllegalCommand
{
private:
	std::string m_error;
public:
	IllegalCommand() : m_error("НЕИЗВЕСТНАЯ КОМАНДА") { }
	const char* getError()
	{
		return m_error.c_str();
	}
};
class OffTheField
{
private:
	std::string m_error;
	TCoord coord;
public:
	OffTheField(std::string stringCoord) : m_error("Не могу переместиться в координату " + stringCoord) { }
	const char* getError()
	{
		return m_error.c_str();
	}
};

#pragma pack(push, 1)
struct TGame
{
	TCell** ppField{ nullptr };
	size_t SIZE;
};
#pragma pack(pop)

void initGame(TGame& g)
{
	g.SIZE = 10;
	g.ppField = new TCell * [g.SIZE];
	for (size_t i = 0; i < g.SIZE; i++)
	{
		g.ppField[i] = new TCell[g.SIZE];
	}

	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			g.ppField[y][x] = EMPTY;
		}
	}
	g.ppField[3][5] = TCell::PLAYER;
}

void printGame(TGame& g)
{
	std::cout << "Управление - WSDA клавиши (в любом регистре и любой раскладке), выход - Q" << std::endl;
	std::cout << "Ввод направления подтверждать через ENTER" << std::endl;
	std::cout << "     ";
	for (size_t x = 0; x < g.SIZE; x++)
	{
		std::cout << x + 1 << "   ";
	}
	std::cout << std::endl;

	for (size_t y = 0; y < g.SIZE; y++)
	{
		std::string space;
		if ((y + 1) > 9)
		{
			space = " ";
		}
		else
		{
			space = "  ";
		}
		std::cout << " " << y + 1 << space << "| ";
		for (size_t x = 0; x < g.SIZE; x++)
		{
			std::cout << g.ppField[y][x] << " | ";
		}
		std::cout << std::endl;
	}
}

TKey CheckKey(char ch)
{
	int charCode;
	charCode = static_cast<int>(ch);

	if (charCode == 97 || charCode == 65 || charCode == -28 || charCode == -108)
	{
		return TKey::LEFT;
	}
	else if (charCode == 100 || charCode == 68 || charCode == -94 || charCode == -126)
	{
		return TKey::RIGHT;
	}
	else if (charCode == 87 || charCode == 119 || charCode == -26 || charCode == -106)
	{
		return TKey::UP;
	}
	else if (charCode == 83 || charCode == 115 || charCode == -21 || charCode == -101)
	{
		return TKey::DOWN;
	}
	else if (charCode == 113 || charCode == 81 || charCode == -87 || charCode == -119)
	{
		return TKey::ESC;
	}
	else
	{
		throw IllegalCommand();
		return TKey::UNDEFINED;
	}
}

TCoord FindPlayer(TGame& g)
{
	TCoord coord;
	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			if (g.ppField[y][x] == TCell::PLAYER)
			{
				coord.y = y;
				coord.x = x;
				return coord;
			};
		}
	}
}

void InitStep(TGame& g, TKey key)
{
	TCoord coord = FindPlayer(g);
	TCoord newCoord = coord;
	std::string err;
	if (key == TKey::LEFT)
	{
		newCoord.x--;
	}
	else if (key == TKey::RIGHT)
	{
		newCoord.x++;
	}
	else if (key == TKey::UP)
	{
		newCoord.y--;
	}
	else if (key == TKey::DOWN)
	{
		newCoord.y++;
	}
	if (newCoord.y > 9 || newCoord.y < 0 || newCoord.x > 9 || newCoord.x < 0)
	{
		throw OffTheField(CoordToString(newCoord));
	}
	else
	{
		g.ppField[coord.y][coord.x] = TCell::EMPTY;
		g.ppField[newCoord.y][newCoord.x] = TCell::PLAYER;
	}
	

}


//CONTROLED BY 'WSDA' KEYS INPUT
//Q KEY - EXIT GAME
int main()
{
	setlocale(LC_ALL, "Russian");
	TGame game;
	initGame(game);
	char keyInput;
	TKey keyCommand = TKey::EMPTY;
	while (keyCommand != TKey::ESC)
	{	
		printGame(game);
		std::cin >> keyInput;
		system("cls");
		try
		{
			keyCommand = CheckKey(keyInput);
			InitStep(game, keyCommand);
		}
		catch (IllegalCommand& exception)
		{
			std::cerr << "Error: " << exception.getError() << std::endl;
		}
		catch (OffTheField& exception)
		{
			std::cerr << "Error: " << exception.getError() << std::endl;
		}
	}
}
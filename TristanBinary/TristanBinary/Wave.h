#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <array>

struct Node
{
	int possibilities;
};

struct Tile
{
	enum Type
	{
		BRICK = 1 << 0,
		GRASS = 1 << 1,
		WATER = 1 << 2,
		COUNT = 1 << 3
	};

	Tile()
	{
		for (int i = COUNT >> 1; i > 0; i >>= 1)
		{
			entropy |= i;
		}
	}

	int entropy{ 0 };
};

class Wave
{
public:
	Wave();
	~Wave();

	template <typename T>
	uint8_t nbBits(T _nb)
	{
		int size{ sizeof(T) };
		uint8_t count{ 0 };
		for (int i = 0; i < size; i++)
		{
			if (_nb & 1 << i)
				count++;
		}
		return count;
	}

	template <typename T>
	void randBit(T& _data)
	{
		size_t size{ sizeof(T) };
		std::vector<T> tab;
		tab.reserve(size);
		
		size_t count{ 0 };
		for (size_t i = 0; i < size * 8; i++)
		{
			if (_data & 1 << i)
			{
				tab.push_back(1 << i);
				count++;
			}
		}

		_data = tab[std::rand() % count];
	}


private:
	std::map<Tile::Type, std::array<Node, 4>> m_Rules{};
	std::vector<std::vector<Tile*>> m_Map{};

};

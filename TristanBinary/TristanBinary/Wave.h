#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <array>

struct Node
{
	uint8_t id{ 0 };
	uint8_t possibilities{ 0b1111 };
	uint8_t maxPossibilities{ 4 };
	uint8_t collapsedValue{ 255 };
	uint8_t entropy{ 4 };

	Node() = default;
	Node(uint8_t _id) : id(_id) {}

	inline uint8_t getEntropy() const
	{
		uint8_t entropy = 0;
		for (size_t i = 0; i < maxPossibilities; i++)
		{
			if (i << 1)
			{
				entropy++;
			}
		}
		return entropy;
	}
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
		for (int i = Type::COUNT >> 1; i > 0; i >>= 1)
		{
			entropy |= i;
		}
	}

	int entropy{ 0 };
};

struct Vector2
{
	inline Vector2() = default;
	inline Vector2(size_t _x, size_t _y) : x(_x), y(_y) {}
	size_t x = 0;
	size_t y = 0;
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
		for (int i = 0; i < size * 8; i++)
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
		tab.reserve(size * 8);
		
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

	void generate();

	Vector2 getLowestEntropyTile();
	bool isInMap(Vector2 _i);
	bool updateNeighbors(Vector2 _i);

	void cout();


	Vector2 getLowestEntropyNode();
	bool hasStillEntropy();
	void collapse(Vector2 _i);
	void updateNodeNeighbors(Vector2 _i);
	void updateNodePossibilities(Vector2 _i);
	bool isInNodeMap(Vector2 _i);

	void coutEntropy();
	void coutCollapsed();

private:
	std::map<Tile::Type, std::array<Node, 4>> m_Rules{};
	std::vector<std::vector<Tile*>> m_Map{};

	std::vector<std::vector<Node>> m_map;
	
	std::array<std::array<uint8_t, 4>, 4> m_rules{ { {0b0000, 0b0001, 0b0001, 0b0001},
													 {0b0000, 0b0001, 0b0001, 0b0001},
													 {0b0000, 0b0001, 0b0001, 0b0001},
													 {0b0000, 0b0001, 0b0001, 0b0001} } };

};

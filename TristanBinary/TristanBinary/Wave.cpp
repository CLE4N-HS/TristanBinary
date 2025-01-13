#include "Wave.h"

Wave::Wave()
{
	size_t sizeY = 10;
	size_t sizeX = 10;

	m_Map.reserve(sizeY);
	for (size_t y = 0; y < sizeY; y++)
	{
		m_Map.push_back(std::vector<Tile*>());
		m_Map[y].reserve(sizeX);
		for (size_t x = 0; x < sizeX; x++)
		{
			Tile* tile = new Tile();
			tile->entropy = 1;
			m_Map[y].push_back(tile);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		m_Rules[Tile::Type::BRICK].data()[i].possibilities = 0b11111111;
		m_Rules[Tile::Type::GRASS].data()[i].possibilities = 0b11111111;
		m_Rules[Tile::Type::WATER].data()[i].possibilities = 0b11111111;
		this->randBit(m_Rules[Tile::Type::BRICK].data()[i].possibilities);
		this->randBit(m_Rules[Tile::Type::GRASS].data()[i].possibilities);
		this->randBit(m_Rules[Tile::Type::WATER].data()[i].possibilities);
	}

	m_Rules[Tile::Type::BRICK].data()[0].possibilities = 0b11111111;
	m_Rules[Tile::Type::BRICK].data()[1].possibilities = 0b11111111;
	m_Rules[Tile::Type::BRICK].data()[2].possibilities = 0b11111111;
	m_Rules[Tile::Type::BRICK].data()[3].possibilities = 0b11111111;
	m_Rules[Tile::Type::GRASS].data()[0].possibilities = 0b11111111;
	m_Rules[Tile::Type::GRASS].data()[1].possibilities = 0b11111111;
	m_Rules[Tile::Type::GRASS].data()[2].possibilities = 0b11111111;
	m_Rules[Tile::Type::GRASS].data()[3].possibilities = 0b11111111;
	m_Rules[Tile::Type::WATER].data()[0].possibilities = 0b11111111;
	m_Rules[Tile::Type::WATER].data()[1].possibilities = 0b11111111;
	m_Rules[Tile::Type::WATER].data()[2].possibilities = 0b11111111;
	m_Rules[Tile::Type::WATER].data()[3].possibilities = 0b11111111;
}

Wave::~Wave()
{
}

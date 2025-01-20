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
			tile->entropy = Tile::COUNT;
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

	this->cout();
	generate();
	this->cout();
}

Wave::~Wave()
{
}

void Wave::generate()
{
	Vector2 i = this->getLowestEntropyTile();

	//debug
	i = Vector2(5, 5);

	// choose a random bit
	m_Map[i.y][i.x]->entropy = rand() % this->nbBits(m_Map[i.y][i.x]->entropy);

	updateNeighbors(i);
}

Vector2 Wave::getLowestEntropyTile()
{
	Vector2 index(0, 0);
	int lowestEntropy = m_Map[index.y][index.x]->entropy;

	for (size_t y = 0; y < m_Map.size(); y++)
	{
		for (size_t x = 0; x < m_Map[y].size(); x++)
		{
			int tmpEntropy = m_Map[y][x]->entropy;
			if (tmpEntropy < lowestEntropy)
			{
				lowestEntropy = tmpEntropy;
				index.x = x;
				index.y = y;
			}
		}
	}

	return index;
}

bool Wave::isInMap(Vector2 _i)
{
	return !(_i.x < 0 && _i.y < 0 && _i.y >= m_Map.size() && _i.x >= m_Map[0].size());
}

bool Wave::updateNeighbors(Vector2 _i)
{
	std::vector<Vector2> n;
	n.reserve(8);
	
	auto addN = [&](Vector2 _v)
		{
			if (this->isInMap(_v) && m_Map[_v.y][_v.x]->entropy >= Tile::Type::COUNT)
				n.push_back(_v);
		};

	addN(Vector2(_i.x - 1, _i.y - 1));
	addN(Vector2(_i.x + 0, _i.y - 1));
	addN(Vector2(_i.x + 1, _i.y - 1));
	addN(Vector2(_i.x + 1, _i.y + 0));
	addN(Vector2(_i.x + 1, _i.y + 1));
	addN(Vector2(_i.x + 0, _i.y + 1));
	addN(Vector2(_i.x - 1, _i.y + 1));
	addN(Vector2(_i.x - 1, _i.y + 0));

	bool hasNeighbors = (n.size() > 0);

	if (hasNeighbors)
	{
		for (size_t i = 0; i < n.size(); i++)
		{
			m_Map[n[i].y][n[i].x]->entropy = rand() % Tile::Type::COUNT;
			this->updateNeighbors(n[i]);
			return false;
		}
	}

	return hasNeighbors;
}

void Wave::cout()
{
	for (size_t y = 0; y < m_Map.size(); y++)
	{
		for (size_t x = 0; x < m_Map[y].size(); x++)
		{
			std::cout << m_Map[y][x]->entropy << " ";
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}

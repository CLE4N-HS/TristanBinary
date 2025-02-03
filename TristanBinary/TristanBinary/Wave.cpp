#include "Wave.h"
#include <fstream>

Wave::Wave()
{
	size_t sizeY = 10;
	size_t sizeX = 10;

	m_map.reserve(sizeY);
	for (size_t y = 0; y < sizeY; y++)
	{
		m_map.push_back(std::vector<Node>());
		m_map[y].reserve(sizeX);
		for (size_t x = 0; x < sizeX; x++)
		{
			m_map[y].push_back(Node());
		}
	}

	this->load();
	this->coutCollapsed();

	return;

	if (1) // autoRules
	{
		for (size_t j = 0; j < m_rules.size(); j++)
		{
			for (size_t i = 0; i < m_rules[j].size(); i++)
			{
				m_rules[j][i] = 0;
			}
		}

		for (size_t y = 0; y < m_map.size(); y++)
		{
			for (size_t x = 0; x < m_map[y].size(); x++)
			{
				Vector2 i;
				if (this->isInMap(Vector2(x, y)))
				{

				}
			}
		}


	}

	while (this->hasStillEntropy())
	{
		Vector2 current = this->getLowestEntropyNode();
		this->collapse(current);
		//this->updateNodeNeighbors(current);

		//this->coutEntropy();
		this->coutCollapsed();
	}

	this->coutCollapsed();
	this->save();

	return;

#pragma region initBefore

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

#pragma endregion

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

#pragma region before

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
	return (_i.x >= 0 && _i.y >= 0 && _i.y < m_Map.size() && _i.x < m_Map[0].size());
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

#pragma endregion

void Wave::cout()
{
	std::cout << std::endl;
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

Vector2 Wave::getLowestEntropyNode()
{
	Vector2 index(0, 0);
	int lowestEntropy = m_map[index.y][index.x].entropy;
	lowestEntropy = 256;

	std::vector<Vector2> allIndex{};

	for (size_t y = 0; y < m_map.size(); y++)
	{
		for (size_t x = 0; x < m_map[y].size(); x++)
		{
			if (m_map[y][x].entropy < lowestEntropy && m_map[y][x].collapsedValue >= 255)
			{
				lowestEntropy = m_map[y][x].entropy;
				index.x = x;
				index.y = y;
				//allIndex.clear();
				allIndex.push_back(index);
			}
			//else if (m_map[y][x].entropy == lowestEntropy && m_map[y][x].collapsedValue >= 255)
			//{
			//	index.x = x;
			//	index.y = y;
			//	allIndex.push_back(index);
			//}
			else
			{
				allIndex.clear();
			}
		}
	}

	if (allIndex.empty())
	{
		return index;
	}

	return (allIndex[rand() % allIndex.size()]);
}

bool Wave::hasStillEntropy()
{
	for (size_t y = 0; y < m_map.size(); y++)
	{
		for (size_t x = 0; x < m_map[y].size(); x++)
		{
			if (m_map[y][x].entropy > 0)
				return true;
		}
	}

	return false;
}

void Wave::collapse(Vector2 _i)
{
	if (this->isInNodeMap(_i))
	{
		Node& node = m_map[_i.y][_i.x];

		std::vector<uint8_t> validOptions;
		for (uint8_t i = 0; i < 4; i++)
		{
			if (node.possibilities & (1 << i))
				validOptions.push_back(i);
		}

		if (validOptions.empty())
		{
			return;
		}

		node.collapsedValue = validOptions[rand() % validOptions.size()];
		node.entropy = 0;

		updateNodeNeighbors(_i);
	}
}

void Wave::updateNodeNeighbors(Vector2 _i)
{
	Vector2 up   (_i.x,     _i.y - 1);
	Vector2 right(_i.x + 1, _i.y    );
	Vector2 down (_i.x,     _i.y + 1);
	Vector2 left (_i.x - 1, _i.y    );

	if (this->isInNodeMap(up))
	{
		updateNodePossibilities(up);
	}
	if (this->isInNodeMap(right))
	{
		updateNodePossibilities(right);
	}
	if (this->isInNodeMap(down))
	{
		updateNodePossibilities(down);
	}
	if (this->isInNodeMap(left))
	{
		updateNodePossibilities(left);
	}
}

void Wave::updateNodePossibilities(Vector2 _i)
{
	Node& currentNode = m_map[_i.y][_i.x];

	if (currentNode.entropy == 0)
	{
		uint8_t collapsedType = currentNode.collapsedValue;

		Vector2 neighbors[4] = {
			Vector2(_i.x, _i.y - 1),
			Vector2(_i.x + 1, _i.y),
			Vector2(_i.x, _i.y + 1),
			Vector2(_i.x - 1, _i.y) 
		};

		for (int dir = 0; dir < 4; dir++)
		{
			Vector2 neighborPos = neighbors[dir];
			if (!isInNodeMap(neighborPos)) continue;

			Node& neighborNode = m_map[neighborPos.y][neighborPos.x];

			if (neighborNode.entropy == 0)
			{
				// TODO error violation
			}
			else
			{
				uint8_t validPossibilities = 0;

				for (uint8_t i = 0; i < 4; i++)
				{
					if (isCompatible(collapsedType, i, dir))
					{
						validPossibilities |= (1 << i);
					}
				}

				neighborNode.possibilities &= validPossibilities;
				neighborNode.entropy = nbBits(neighborNode.possibilities);
			}
		}
	}
}

bool Wave::isInNodeMap(Vector2 _i)
{
	return (_i.x >= 0 && _i.y >= 0 && _i.y < m_map.size() && _i.x < m_map[0].size());
}

bool Wave::isCompatible(uint8_t tileType, uint8_t neighborTileType, int direction)
{
	uint8_t allowed = m_rules[direction][tileType];

	return (allowed & (1 << neighborTileType)) != 0;
}

void Wave::coutEntropy()
{
	std::cout << std::endl;
	for (size_t y = 0; y < m_map.size(); y++)
	{
		for (size_t x = 0; x < m_map[y].size(); x++)
		{
			std::cout << static_cast<int>(m_map[y][x].entropy) << " ";
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}

void Wave::coutCollapsed()
{
	std::cout << std::endl;
	for (size_t y = 0; y < m_map.size(); y++)
	{
		for (size_t x = 0; x < m_map[y].size(); x++)
		{
			std::cout << static_cast<int>(m_map[y][x].collapsedValue) << " ";
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}

void Wave::save()
{
	std::ofstream file("../Resources/map.txt");

	for (size_t y = 0; y < m_map.size(); y++)
	{
		for (size_t x = 0; x < m_map[y].size(); x++)
		{
			file << static_cast<int>(m_map[y][x].collapsedValue) << " ";
		}
		file << "\n";
	}

	file.close();
}

void Wave::load()
{
	std::ifstream file("../Resources/map.txt");
	std::string a;

	for (size_t y = 0; y < m_map.size(); y++)
	{
		for (size_t x = 0; x < m_map[y].size(); x++)
		{
			int b;
			file >> b;
			m_map[y][x].collapsedValue = b;
		}
		file >> a;
	}

	file.close();
}

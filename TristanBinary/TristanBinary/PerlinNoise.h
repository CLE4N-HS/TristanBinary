#pragma once
#include "Gradient.h"
#include <fstream>
#include <vector>

class PerlinNoise
{
public:
	PerlinNoise();
	~PerlinNoise();

private:
	std::vector<std::vector<float>> noise;

};

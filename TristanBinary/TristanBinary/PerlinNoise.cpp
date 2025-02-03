#include "PerlinNoise.h"
#include <iostream>

void WriteBMP(const char* filename, int width, int height, const std::vector<unsigned char>& data) {
    std::ofstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Erreur lors de l'ouverture du fichier pour écrire l'image." << std::endl;
        return;
    }

    int filesize = 54 + 3 * width * height;
    unsigned char bmpfileheader[14] = { 'B', 'M', filesize, filesize >> 8, filesize >> 16, filesize >> 24,
                                       0, 0, 0, 0, 54, 0, 0, 0 };
    unsigned char bmpinfoheader[40] = { 40, 0, 0, 0, width, width >> 8, width >> 16, width >> 24,
                                       height, height >> 8, height >> 16, height >> 24,
                                       1, 0, 24, 0 };

    file.write(reinterpret_cast<char*>(bmpfileheader), 14);
    file.write(reinterpret_cast<char*>(bmpinfoheader), 40);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());

    file.close();
}

PerlinNoise::PerlinNoise()
{
    size_t sizeY{ 1080 * 1};
    size_t sizeX{ 1080 * 1};

    noise.resize(sizeY);
    for (size_t y = 0; y < sizeY; y++)
    {
        noise[y].resize(sizeX);
    }

    const float scale = 15.f;

    float seedX = 0.88f;
    float seedY = 0.22f;

    float lacunarity = 2.f;
    float persistence = 0.5f;

    float frequency = 1.f;
    float amplitude = 1.f;

    std::vector<unsigned char> imageData(3 * sizeX * sizeY);

    float noiseValue = 0.f;

    const int octave = 1028;

    for (int i = 0; i < octave; i++)
    {
        for (int y = 0; y < sizeY; y++) {
            for (int x = 0; x < sizeX; x++) {
                float u = static_cast<float>(x) / sizeX + seedX;
                float v = static_cast<float>(y) / sizeY + seedY;

                noise[x][y] += GradientNoise::clamp(-1.f, 1.f, GradientNoise::GradNoise(Vector2f(u, v), scale * frequency) * amplitude * 1.f);
            }
        }
        frequency *= lacunarity;
        amplitude *= persistence;
    }
    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {

            unsigned char color = static_cast<unsigned char>((noise[x][y] * 0.5f + 0.5f) * 255.0f);

            // BMP format: BGR
            int index = (x + y * sizeX) * 3;
            float noiseValue = GradientNoise::clamp(0.f, 1.f, noise[x][y] * 0.5f + 0.5f);
            if (noiseValue < 0.1f)
            {
                imageData[index] = 50;      // Blue
                imageData[index + 1] = 0;  // Green
                imageData[index + 2] = 0;  // Red
            }
            else if (noiseValue < 0.5f)
            {
                imageData[index] = 150;      // Blue
                imageData[index + 1] = 0;  // Green
                imageData[index + 2] = 0;  // Red
            }
            else if (noiseValue < 0.6f)
            {
                imageData[index] = 0;      // Blue
                imageData[index + 1] = 150;  // Green
                imageData[index + 2] = 0;  // Red
            }
            else if (noiseValue < 0.9f)
            {
                imageData[index] = 0;      // Blue
                imageData[index + 1] = 64;  // Green
                imageData[index + 2] = 128;  // Red
            }
            else
            {
                imageData[index] = 255;      // Blue
                imageData[index + 1] = 255;  // Green
                imageData[index + 2] = 255;  // Red
            }
        }
    }

    WriteBMP("../perlin_noise.bmp", sizeX, sizeY, imageData);
    std::cout << "Image générée : perlin_noise.bmp" << std::endl;
}

PerlinNoise::~PerlinNoise()
{
}

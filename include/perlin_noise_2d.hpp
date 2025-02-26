#ifndef PERLIN_NOISE_2D_HPP
#define PERLIN_NOISE_2D_HPP

#include <vector>
#include <cmath>
#include <numeric>
#include <random>
#include <algorithm>

class PerlinNoise2D {
public:
    PerlinNoise2D() = default;
    PerlinNoise2D(unsigned int seed = 0, int tableSize = 2048, int octaves = 12, float persistence = 0.2f, float lacunarity = 0.5f)
        : tableSize(tableSize), octaves(octaves), persistence(persistence), lacunarity(lacunarity) {
        // Initialize the permutation table
        p.resize(tableSize);
        std::iota(p.begin(), p.end(), 0);

        // Shuffle with the given seed
        std::default_random_engine engine(seed);
        std::shuffle(p.begin(), p.end(), engine);

        // Duplicate for seamless wrapping
        p.insert(p.end(), p.begin(), p.end());
    }

    // Public method to generate noise (height map)
    float noise(float x, float y) const {
        return fractalNoise(x, y);
    }

private:
    int tableSize;
    int octaves;           // Number of octaves for fractal noise
    float persistence;     // Amplitude scaling factor for each octave
    float lacunarity;      // Frequency scaling factor for each octave
    std::vector<int> p;    // Permutation table

    // Fractal Noise (combines multiple layers of noise)
    float fractalNoise(float x, float y) const {
        float amplitude = 1.0f;
        float frequency = 1.0f;
        float total = 0.0f;
        float maxAmplitude = 0.0f;

        for (int i = 0; i < octaves; ++i) {
            // Apply noise with frequency and amplitude scaling
            total += warpedNoise(x * frequency, y * frequency) * amplitude;
            maxAmplitude += amplitude;

            // Adjust amplitude and frequency for the next octave
            amplitude *= persistence;
            frequency *= lacunarity;
        }

        // Normalize result to [0, 1] and amplify variation by adjusting the scale factor
        return total / maxAmplitude * 1.5f; // Adjust for larger variation
    }

    // Warped Noise (distorts input coordinates to reduce repetition)
    float warpedNoise(float x, float y) const {
        float warpX = baseNoise(x * 0.5f, y * 0.5f) * 20.0f;      // Secondary noise for x
        float warpY = baseNoise(x * 0.5f + 100, y * 0.5f + 100) * 20.0f; // Secondary noise for y
        return baseNoise(x + warpX, y + warpY);
    }

    // Base Perlin Noise implementation
    float baseNoise(float x, float y) const {
        // Find unit grid cell
        int X = static_cast<int>(std::floor(x)) & (tableSize - 1);
        int Y = static_cast<int>(std::floor(y)) & (tableSize - 1);

        // Relative x, y coordinates within the grid cell
        float relX = x - std::floor(x);
        float relY = y - std::floor(y);

        // Compute fade curves
        float u = fade(relX);
        float v = fade(relY);

        // Hash corners of the grid cell
        int aa = hash(X, Y);
        int ab = hash(X, Y + 1);
        int ba = hash(X + 1, Y);
        int bb = hash(X + 1, Y + 1);

        // Blend results using gradients and linear interpolation
        float blendX1 = lerp(u, grad(aa, relX, relY), grad(ba, relX - 1, relY));
        float blendX2 = lerp(u, grad(ab, relX, relY - 1), grad(bb, relX - 1, relY - 1));
        float result = lerp(v, blendX1, blendX2);

        // Scale to [0, 1]
        return (result + 1.0f) / 2.0f;
    }

    // Hash function to get a gradient index
    int hash(int x, int y) const {
        return p[p[x] + y];
    }

    // Fade function to smooth transitions
    static float fade(float t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    // Linear interpolation
    static float lerp(float t, float a, float b) {
        return a + t * (b - a);
    }

    // Gradient function to determine the influence of a grid point
    static float grad(int hash, float x, float y) {
        static const std::vector<std::pair<float, float>> gradients = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {0.707f, 0.707f}, {-0.707f, 0.707f},
            {0.707f, -0.707f}, {-0.707f, -0.707f}
        };
        const auto& g = gradients[hash % gradients.size()];
        return g.first * x + g.second * y;
    }
};

#endif // PERLIN_NOISE_2D_HPP
#include "Misc.h"
#include <random>

float RandomFloat(float min, float max) {
    static std::random_device rd;      // inicializa uma vez só
    static std::mt19937 gen(rd());     // gerador de números
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}
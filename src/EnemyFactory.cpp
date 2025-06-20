#include "EnemyFactory.h"
#include <random>
#include "GameExceptions.h"
#include "Fish.h"
#include "Squid.h"
#include "Frog.h"
#include "Pufferfish.h"

std::unique_ptr<Enemy> EnemyFactory::create(const int difficulty, const int frog_counter) {
    switch (difficulty) {
        case 1: return createEasyEnemy();
        case 2: return createMediumEnemy(frog_counter);
        case 3: return createHardEnemy(frog_counter);
        default: throw InvalidMinigameDifficulty{difficulty};
    }
}

std::unique_ptr<Enemy> EnemyFactory::createEasyEnemy() {
    std::uniform_int_distribution<int> d(0, 3);
    static std::mt19937 gen{std::random_device{}()};
    const int type = d(gen);
    if (type == 0) {
        return std::make_unique<Fish>();
    }
    return std::make_unique<Squid>();
}

std::unique_ptr<Enemy> EnemyFactory::createMediumEnemy(const int frog_counter) {
    std::uniform_int_distribution<int> d(0, 3);
    static std::mt19937 gen{std::random_device{}()};
    const int type = d(gen);
    if ((type == 1 || type == 2) && frog_counter < 4)
        return std::make_unique<Frog>();
    return std::make_unique<Fish>();
}

std::unique_ptr<Enemy> EnemyFactory::createHardEnemy(const int frog_counter) {
    std::uniform_int_distribution<int> d(0, 3);
    static std::mt19937 gen{std::random_device{}()};
    const int type = d(gen);
    if (type == 2)
        return std::make_unique<Pufferfish>();
    if (type == 1 && frog_counter < 4)
        return std::make_unique<Frog>();
    return std::make_unique<Fish>();
}

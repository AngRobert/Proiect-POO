#ifndef ENEMYFACTORY_H
#define ENEMYFACTORY_H

#include <memory>
#include "Enemy.h"

class EnemyFactory {
private:
    static std::unique_ptr<Enemy> createEasyEnemy();
    static std::unique_ptr<Enemy> createMediumEnemy(int frog_counter);
    static std::unique_ptr<Enemy> createHardEnemy(int frog_counter);\

public:
    static std::unique_ptr<Enemy> create(int difficulty, int frog_counter);
};



#endif //ENEMYFACTORY_H

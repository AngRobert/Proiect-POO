#ifndef FISH_H
#define FISH_H

#include "Enemy.h"

class Fish : public Enemy {
private:

    void generateEnemyPosition() override;
    void print(std::ostream& os) const override;

public:

    void moveEnemy(float deltaTime) override;
    explicit Fish();
    Enemy* clone() const override;
};



#endif //FISH_H

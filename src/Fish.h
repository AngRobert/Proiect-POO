#ifndef FISH_H
#define FISH_H

#include "Enemy.h"

class Fish : public Enemy {
private:
    sf::Vector2i fish_y_range;
    std::string fish_pos;

    void generateEnemyPosition() override;

public:

    void moveEnemy(float deltaTime) override;
    explicit Fish();
    Enemy* clone() const override;
};



#endif //FISH_H
